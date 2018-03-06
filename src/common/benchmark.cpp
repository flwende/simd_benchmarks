// Copyright (c) 2016 Florian Wende (flwende@gmail.com)
//
// Distributed under the BSD 2-clause Software License
// (See accompanying file LICENSE)

#include <iostream>
#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <unistd.h>
#include <omp.h>
#include "common/kernel.hpp"

// length of the arrays to be processed: default = 1024^2
#define N_DEFAULT (1 * 1024 * 1024)

// range of values stored in arrays x_1 and x_2: default = [-1.0,+1.0] 
#define X_UPPER_DEFAULT +1.0
#define X_LOWER_DEFAULT -1.0

// functions should be called conditionally from within the loop?
#if !defined(CONDITIONAL_CALL) && !defined(UNCONDITIONAL_CALL)
	#define CONDITIONAL_CALL
#endif

// threshold probability for calling functions conditionally.
#if defined(CONDITIONAL_CALL) && !defined(THRESHOLD)
	#define THRESHOLD 0.5
#endif

// number of outer loop iterations to be skipped before taking measurements.
#if !defined(SKIP)
	#define SKIP 4
#endif

// total number of outer loop iterations.
#if !defined(RUNS)
	#define RUNS 10
#endif

#if defined(SIMD_CLASS_VC) || defined(SIMD_CLASS_VCL) || defined(SIMD_CLASS_UMESIMD)
	#define SIMD_CLASS
#endif

int main(int argc, char **argv)
{
	// set parameters to default vaules
	std::int32_t n = N_DEFAULT;
	std::int32_t n_padded = N_DEFAULT;
	double x_lower = X_LOWER_DEFAULT;
	double x_upper = X_UPPER_DEFAULT;
	
	// process command line arguments: -n <val_1> -l <val_2> -u <val_3>
	std::int32_t c;
	while ((c = getopt(argc, argv, "l:n:u:")) != -1)
	{
		switch(c)
		{
			case 'n': 
				n = atoi(optarg);
				n_padded = ((n + SIMD_WIDTH_LOGICAL_REAL64 - 1) / SIMD_WIDTH_LOGICAL_REAL64) * SIMD_WIDTH_LOGICAL_REAL64; 
				break;
			case 'l':
				x_lower = atof(optarg);
				break;
			case 'u':
				x_upper = atof(optarg);
				break;
			case '?':
				if (optopt == 'n' || optopt == 'l' || optopt == 'u')
					std::cerr << "n|l|u require arguments" << std::endl;
				return 1;
			default:
				return 1;
		}
	}

	// determine the maximum number of threads.
	std::int32_t threads = omp_get_max_threads();
	
	// input arrays for the benchmark.
	double* x_1[RUNS];
	double* x_2[RUNS];

	// if (simd) functions are called conditionally, we need mask arrays.
	#if defined(CONDITIONAL_CALL)
		#if defined(INTRINSICS)
			MASK_REAL64* m_vec[RUNS];
		#elif defined(SIMD_CLASS)
			bool* m_vec[RUNS];
		#endif
			BOOL* m[RUNS];
	#endif

	for (std::int32_t r = 0; r < RUNS; ++r)
	{
		// memory allocation.
		x_1[r] = reinterpret_cast<double*>(_mm_malloc(n_padded * sizeof(double), ALIGNMENT));
		x_2[r] = reinterpret_cast<double*>(_mm_malloc(n_padded * sizeof(double), ALIGNMENT));
		
		srand48(1 + r);
		// assign random numbers to x_1 and x_2 taken from [x_lower, x_upper].
		for (std::int32_t i=0; i<n; i++)
		{
			x_1[r][i] = (x_upper - x_lower) * drand48() + x_lower;
			x_2[r][i] = (x_upper - x_lower) * drand48() + x_lower;
		}

		#if defined(CONDITIONAL_CALL)
			// memory allocation.
			m[r] = reinterpret_cast<BOOL*>(_mm_malloc(n_padded * sizeof(BOOL), ALIGNMENT));
			// for each loop iteration draw a random number r over [0,1] and set the mask to TRUE
			// if i < n and r > THRESHOLD. otherwise, set the mask to FALSE.
			for (std::int32_t i = 0; i < n_padded; ++i)
				m[r][i] = (i < n && drand48() > THRESHOLD ? TRUE : FALSE);

			// map the simd masks m[][] to the respective internal representations of the different simd ISAs.
			#if defined(INTRINSICS) && defined(SIMD_512)
				m_vec[r] = reinterpret_cast<MASK_REAL64*>(_mm_malloc((n_padded / SIMD_WIDTH_NATIVE_REAL64) * sizeof(MASK_REAL64), ALIGNMENT));
				for (std::int32_t i = 0, j = 0; i < n_padded; i += SIMD_WIDTH_NATIVE_REAL64, ++j)
					{
						// with MIC simd ISA and AVX512, masks internally are represented by bitsets.
						m_vec[r][j] = 0x0;
						for (std::int32_t ii = 0; ii < SIMD_WIDTH_NATIVE_REAL64; ++ii)
							m_vec[r][j] |= (m[r][i + ii] == TRUE ? (SIMD_TRUE << ii) : SIMD_FALSE);
					}
			#elif defined(INTRINSICS) && defined(SIMD_256)
				m_vec[r] = reinterpret_cast<MASK_REAL64*>(_mm_malloc((n_padded / SIMD_WIDTH_NATIVE_REAL64) * sizeof(MASK_REAL64), ALIGNMENT));
				std::uint64_t* ptr_m_vec = reinterpret_cast<std::uint64_t*>(m_vec[r]);
				for (std::int32_t i = 0; i < n_padded; ++i)
					ptr_m_vec[i] = (m[r][i] == TRUE ? SIMD_TRUE : SIMD_FALSE);
			#elif defined(SIMD_CLASS)
				m_vec[r] = reinterpret_cast<bool*>(_mm_malloc(n_padded * sizeof(bool), ALIGNMENT));
				for (std::int32_t i = 0; i < n_padded; ++i)
					m_vec[r][i] = (m[r][i] == TRUE ? true : false);
			#endif
		#endif
	}

	// output arrays for the benchmark. each thread uses two buffers:
	// buffer [][0] holds the final results which are the accumulated values over multiple runs.
	// biffer [][1] holds the results of the current runs. these values are accumulated into buffer [][0] after each run.
	double* y[threads][2];
	#pragma omp parallel
	{
		// create threads and make thread 0 say hello.
		std::int32_t thread_id = omp_get_thread_num();
		if (thread_id == 0)
			std::cout << "# number of OMP threads : " << threads << std::endl;
		
		y[thread_id][0] = reinterpret_cast<double*>(_mm_malloc(n_padded * sizeof(double), ALIGNMENT));
		y[thread_id][1] = reinterpret_cast<double*>(_mm_malloc(n_padded * sizeof(double), ALIGNMENT));
		for (std::int32_t i = 0; i < n_padded; ++i)
		{
			y[thread_id][0][i] = 0.0;
			y[thread_id][1][i] = 0.0;
		}
	}

	// perform a reference computation in case of debug mode is enabled.
	#if defined(DEBUG)
		// memory allocation.
		double* y_reference[2];
		y_reference[0] = reinterpret_cast<double*>(_mm_malloc(n * sizeof(double), ALIGNMENT));
		y_reference[1] = reinterpret_cast<double*>(_mm_malloc(n * sizeof(double), ALIGNMENT));
		for (std::int32_t i = 0; i < n; ++i)
		{
			y_reference[0][i] = 0.0;
			y_reference[1][i] = 0.0;
		}

		// run the benchmark.
		for (std::int32_t r = 0; r < RUNS; ++r)
		{
			for (std::int32_t i = 0; i < n; ++i)
				{
					#if defined(CONDITIONAL_CALL)
						if (m[r][i] != FALSE)
							kernel_reference(x_1[r][i], x_2[r][i], y_reference[1][i]);
					#else
						kernel_reference(x_1[r][i], x_2[r][i], y_reference[1][i]);
					#endif
				}

			// accumulate results.
			for (std::int32_t i = 0; i < n; ++i)
				y_reference[0][i] += y_reference[1][i];
		}
	#endif

	// perform the actual benchmark.
	double time[threads][RUNS];
	#pragma omp parallel
	{
		std::int32_t thread_id = omp_get_thread_num();
		
		#pragma omp barrier
		#pragma omp barrier
		
		for (std::int32_t r = 0; r < RUNS; ++r)
		{
			// start timer.
			time[thread_id][r] = omp_get_wtime();

			// explicit vectorization using OpenMP 4.x.
			#if defined(EXPLICIT_VECTORIZATION)
				const double* ptr_x_1 = x_1[r];
				const double* ptr_x_2 = x_2[r];
				double* ptr_y = y[thread_id][1];
				#pragma omp simd SIMDLEN(SIMD_WIDTH_LOGICAL_REAL64)
				for (std::int32_t i = 0; i < n; ++i)
					#if defined(CONDITIONAL_CALL)
						if (m[r][i] != FALSE)
							#pragma noinline
							kernel_explicit_vectorization(&ptr_x_1[i], &ptr_x_2[i], &ptr_y[i]);
					#else
						#pragma noinline
						kernel_explicit_vectorization(&ptr_x_1[i], &ptr_x_2[i], &ptr_y[i]);
					#endif
			// explicit vectorization with user defined vector data types.
			#elif defined(ENHANCED_EXPLICIT_VECTORIZATION)
				#if defined(CONDITIONAL_CALL)
					for (std::int32_t i = 0; i < n; i += SIMD_WIDTH_LOGICAL_REAL64)
						#pragma noinline
						kernel_enhanced_explicit_vectorization(reinterpret_cast<vec_real64_t&>(x_1[r][i]), reinterpret_cast<vec_real64_t&>(x_2[r][i]), reinterpret_cast<vec_real64_t&>(y[thread_id][1][i]), reinterpret_cast<mask_real64_t&>(m[r][i]));
				#else
					// in the unconditional case the mask is allways TRUE except for the loop remainder case.
					mask_real64_t m;
					for (std::int32_t ii = 0; ii < SIMD_WIDTH_LOGICAL_REAL64; ++ii)
						m.x[ii] = TRUE;
					// perform all loop iterations with i < 'n_floor = largest multiple of SIMD_WIDTH_LOGICAL_REAL64 lower or equal to n'.
					std::int32_t n_floor = (n / SIMD_WIDTH_LOGICAL_REAL64) * SIMD_WIDTH_LOGICAL_REAL64; 
					for (std::int32_t i = 0; i < n_floor; i += SIMD_WIDTH_LOGICAL_REAL64)
						#pragma noinline
						kernel_enhanced_explicit_vectorization(reinterpret_cast<vec_real64_t&>(x_1[r][i]), reinterpret_cast<vec_real64_t&>(x_2[r][i]), reinterpret_cast<vec_real64_t&>(y[thread_id][1][i]), m);
					// now perform the remainder loop.
					for (std::int32_t i = n_floor; i < n; i += SIMD_WIDTH_LOGICAL_REAL64)
					{
						// create vector mask.
						for (std::int32_t ii = 0; ii < SIMD_WIDTH_LOGICAL_REAL64; ++ii)
							m.x[ii] = ((i + ii) < n ? TRUE : FALSE);
						// call the kernel using that mask.
						#pragma noinline
						kernel_enhanced_explicit_vectorization(reinterpret_cast<vec_real64_t&>(x_1[r][i]), reinterpret_cast<vec_real64_t&>(x_2[r][i]), reinterpret_cast<vec_real64_t&>(y[thread_id][1][i]), m);
					}
				#endif
			// manual vectorization with simd intrinsics.
			#elif defined(INTRINSICS)
				#if defined(CONDITIONAL_CALL)
					for (std::int32_t i = 0, j = 0; i < n; i += SIMD_WIDTH_NATIVE_REAL64, ++j)
						#pragma noinline
						kernel_intrinsics(reinterpret_cast<VEC_REAL64&>(x_1[r][i]), reinterpret_cast<VEC_REAL64&>(x_2[r][i]), reinterpret_cast<VEC_REAL64&>(y[thread_id][1][i]), m_vec[r][j]);
				#else
					// in the unconditional case the mask is allways TRUE except for the loop remainder case.
					MASK_REAL64 m = SIMD_ALL_LANES_ACTIVE_MASK_REAL64;
					std::int32_t n_floor = (n / SIMD_WIDTH_NATIVE_REAL64) * SIMD_WIDTH_NATIVE_REAL64;
					// perform all loop iterations with i < 'n_floor = largest multiple of SIMD_WIDTH_LOGICAL_REAL64 lower or equal to n'.
					for (std::int32_t i = 0; i < n_floor; i += SIMD_WIDTH_NATIVE_REAL64)
						#pragma noinline
						kernel_intrinsics(reinterpret_cast<VEC_REAL64&>(x_1[r][i]), reinterpret_cast<VEC_REAL64&>(x_2[r][i]), reinterpret_cast<VEC_REAL64&>(y[thread_id][1][i]), m);
					// now perform the remainder loop.
					#if defined(SIMD_512)
						std::int32_t m_int = 0x0;
						for (std::int32_t i = n_floor, ii = 0; i < n_padded; ++i, ++ii)
							m_int |= (i < n ? (SIMD_TRUE << ii) : SIMD_FALSE);
						m = SIMD_SET_MASK_REAL64(m_int);
					#elif defined(SIMD_256)
						std::uint64_t* m_int = reinterpret_cast<std::uint64_t*>(&m);
						for (std::int32_t i = n_floor, ii = 0; i < n_padded; ++i, ++ii)
							m_int[ii] = (i < n ? SIMD_TRUE : SIMD_FALSE);
					#endif
					for (std::int32_t i = n_floor; i < n; i += SIMD_WIDTH_NATIVE_REAL64)
						#pragma noinline
						kernel_intrinsics(reinterpret_cast<VEC_REAL64&>(x_1[r][i]), reinterpret_cast<VEC_REAL64&>(x_2[r][i]), reinterpret_cast<VEC_REAL64&>(y[thread_id][1][i]), m);
					#endif
			// manual vectorization with Vc C++ simd class.
			#elif defined(SIMD_CLASS_VC)
				#if defined(CONDITIONAL_CALL)
					Vc::double_m mask;
					for (std::int32_t i = 0; i < n; i += SIMD_WIDTH_NATIVE_REAL64)
					{
						mask.load(&m_vec[r][i]);
						#pragma noinline
						kernel_simd_class_vc(reinterpret_cast<Vc::double_v&>(x_1[r][i]), reinterpret_cast<Vc::double_v&>(x_2[r][i]), reinterpret_cast<Vc::double_v&>(y[thread_id][1][i]), mask);
					}
				#else
					// in the unconditional case the mask is allways TRUE except for the loop remainder case.
					Vc::double_m m(Vc::One);
					std::int32_t n_floor = (n / SIMD_WIDTH_NATIVE_REAL64) * SIMD_WIDTH_NATIVE_REAL64;
					// perform all loop iterations with i < 'n_floor = largest multiple of SIMD_WIDTH_LOGICAL_REAL64 lower or equal to n'.					
					for (std::int32_t i = 0; i < n_floor; i += SIMD_WIDTH_NATIVE_REAL64)
						#pragma noinline
						kernel_simd_class_vc(reinterpret_cast<Vc::double_v&>(x_1[r][i]), reinterpret_cast<Vc::double_v&>(x_2[r][i]), reinterpret_cast<Vc::double_v&>(y[thread_id][1][i]), m);
					// now perform the remainder loop.					
					bool mask_array[SIMD_WIDTH_NATIVE_REAL64];
					for (std::int32_t i = n_floor, ii = 0; i < n_padded; ++i, ++ii)
						mask_array[ii] = (i < n ? true : false);
					m.load(mask_array);
					for (std::int32_t i = n_floor; i < n; i += SIMD_WIDTH_NATIVE_REAL64)
						#pragma noinline
						kernel_simd_class_vc(reinterpret_cast<Vc::double_v&>(x_1[r][i]), reinterpret_cast<Vc::double_v&>(x_2[r][i]), reinterpret_cast<Vc::double_v&>(y[thread_id][1][i]), m);
				#endif
// TODO: call VCL kernels
			#elif defined(SIMD_CLASS_VCL)
				#if defined(CONDITIONAL_CALL)
					Vc::double_m mask;
					for (std::int32_t i = 0; i < n; i += SIMD_WIDTH_NATIVE_REAL64)
					{
						mask.load(&m_vec[r][i]);
						#pragma noinline
						kernel_simd_class_vcl(reinterpret_cast<Vc::double_v&>(x_1[r][i]), reinterpret_cast<Vc::double_v&>(x_2[r][i]), reinterpret_cast<Vc::double_v&>(y[thread_id][1][i]), mask);
					}
				#else
					// in the unconditional case the mask is allways TRUE except for the loop remainder case.
					Vc::double_m m(Vc::One);
					std::int32_t n_floor = (n / SIMD_WIDTH_NATIVE_REAL64) * SIMD_WIDTH_NATIVE_REAL64;
					// perform all loop iterations with i < 'n_floor = largest multiple of SIMD_WIDTH_LOGICAL_REAL64 lower or equal to n'.					
					for (std::int32_t i = 0; i < n_floor; i += SIMD_WIDTH_NATIVE_REAL64)
						#pragma noinline
						kernel_simd_class_vcl(reinterpret_cast<Vc::double_v&>(x_1[r][i]), reinterpret_cast<Vc::double_v&>(x_2[r][i]), reinterpret_cast<Vc::double_v&>(y[thread_id][1][i]), m);
					// now perform the remainder loop.					
					bool mask_array[SIMD_WIDTH_NATIVE_REAL64];
					for (std::int32_t i = n_floor, ii = 0; i < n_padded; ++i, ++ii)
						mask_array[ii] = (i < n ? true : false);
					m.load(mask_array);
					for (std::int32_t i = n_floor; i < n; i += SIMD_WIDTH_NATIVE_REAL64)
						#pragma noinline
						kernel_simd_class_vcl(reinterpret_cast<Vc::double_v&>(x_1[r][i]), reinterpret_cast<Vc::double_v&>(x_2[r][i]), reinterpret_cast<Vc::double_v&>(y[thread_id][1][i]), m);
				#endif
// TODO: call UME-SIMD kernels
			#elif defined(SIMD_CLASS_UMESIMD)
				#if defined(CONDITIONAL_CALL)
					Vc::double_m mask;
					for (std::int32_t i = 0; i < n; i += SIMD_WIDTH_NATIVE_REAL64)
					{
						mask.load(&m_vec[r][i]);
						#pragma noinline
						kernel_simd_class_vcl(reinterpret_cast<Vc::double_v&>(x_1[r][i]), reinterpret_cast<Vc::double_v&>(x_2[r][i]), reinterpret_cast<Vc::double_v&>(y[thread_id][1][i]), mask);
					}
				#else
					// in the unconditional case the mask is allways TRUE except for the loop remainder case.
					Vc::double_m m(Vc::One);
					std::int32_t n_floor = (n / SIMD_WIDTH_NATIVE_REAL64) * SIMD_WIDTH_NATIVE_REAL64;
					// perform all loop iterations with i < 'n_floor = largest multiple of SIMD_WIDTH_LOGICAL_REAL64 lower or equal to n'.					
					for (std::int32_t i = 0; i < n_floor; i += SIMD_WIDTH_NATIVE_REAL64)
						#pragma noinline
						kernel_simd_class_vcl(reinterpret_cast<Vc::double_v&>(x_1[r][i]), reinterpret_cast<Vc::double_v&>(x_2[r][i]), reinterpret_cast<Vc::double_v&>(y[thread_id][1][i]), m);
					// now perform the remainder loop.					
					bool mask_array[SIMD_WIDTH_NATIVE_REAL64];
					for (std::int32_t i = n_floor, ii = 0; i < n_padded; ++i, ++ii)
						mask_array[ii] = (i < n ? true : false);
					m.load(mask_array);
					for (std::int32_t i = n_floor; i < n; i += SIMD_WIDTH_NATIVE_REAL64)
						#pragma noinline
						kernel_simd_class_vcl(reinterpret_cast<Vc::double_v&>(x_1[r][i]), reinterpret_cast<Vc::double_v&>(x_2[r][i]), reinterpret_cast<Vc::double_v&>(y[thread_id][1][i]), m);
				#endif

			#else
				for (std::int32_t i = 0; i < n; ++i)
					#if defined(CONDITIONAL_CALL)
						if (m[r][i] != FALSE)
							#pragma noinline
							kernel_reference(x_1[r][i], x_2[r][i], y[thread_id][1][i]);
					#else
						#pragma noinline
						kernel_reference(x_1[r][i], x_2[r][i], y[thread_id][1][i]);
					#endif
			#endif

			// stop timer.
			time[thread_id][r] = (omp_get_wtime() - time[thread_id][r]) / n;

			// accumulate results.			
			for (std::int32_t i = 0; i < n; ++i)
				y[thread_id][0][i] += y[thread_id][1][i];
		}
		
		#pragma omp barrier
		
		if (thread_id == 0)
		{
			// compute mean execution time / err / min / max.
			double time_mean = 0.0;
			double time_err = 0.0;
			double time_min = 1.0E100;
			double time_max = 0.0;
			if (RUNS > 1 || threads > 1)
			{
				for (std::int32_t r = SKIP; r < RUNS; ++r)
				{
					for (std::int32_t i = 0; i < threads; ++i)
					{
						time_mean += time[i][r];
						time_min = std::min(time_min, time[i][r]);
						time_max = std::max(time_max, time[i][r]);
					}
				}
				time_mean /= ((RUNS - SKIP) * threads);
				for (std::int32_t r = SKIP; r < RUNS; ++r)
					for (std::int32_t i = 0; i < threads; ++i)
						time_err += (time_mean - time[i][r]) * (time_mean - time[i][r]);
				time_err = std::sqrt(time_err / ((RUNS - SKIP) * threads * ((RUNS - SKIP) * threads - 1)));
			}
			else
			{
				time_mean = time_min = time_max = time[0][0];
			}
			// print timings.
			std::cout << "# timings per loop iteration:" << std::endl;
			std::cout << "# mean\t\terr\t\tmin\t\tmax" << std::endl;
			std::cout << time_mean << "\t" << time_err << "\t" << time_min << "\t" << time_max << std::endl;
		}
		
		#pragma omp barrier
		
		// print some output (only for the first 8 threads).
		if (thread_id < 8)
		{
			#pragma omp critical
			{
				std::cout << "# output (thread " << thread_id << "): ";
				for (std::int32_t i = 0; i < 8; ++i)
					std::cout << y[thread_id][0][i] << " ";
				std::cout << std::endl;
			}
		}

		// compute deviation between reference computation and vectorized computation.
		#if defined(DEBUG)
			double deviation = 0.0;
			for (std::int32_t i = 0; i < n; ++i)
				deviation = std::max(deviation, std::abs((y[thread_id][0][i] - y_reference[0][i]) / y_reference[0][i]));
			#pragma omp critical
			{
				std::cout << "# max. abs. rel. error (thread " << thread_id << "): " << deviation << std::endl;
			}
		#endif
	}
	
	// release memory
	for (std::int32_t r = 0; r < RUNS; ++r)
	{
		_mm_free(x_1[r]);
		_mm_free(x_2[r]);
		
		#if defined(CONDITIONAL_CALL)
			_mm_free(m[r]);
			#if defined(INTRINSICS) || defined(SIMD_CLASS)
				_mm_free(m_vec[r]);
			#endif
		#endif
	}
	
	for (std::int32_t i = 0; i < threads; ++i)
	{
		_mm_free(y[i][0]);
		_mm_free(y[i][1]);
	}
	
	#if defined(DEBUG)
		_mm_free(y_reference[0]);
		_mm_free(y_reference[1]);
	#endif
		
	return 0;
}
