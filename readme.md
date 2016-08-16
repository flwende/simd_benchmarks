OpenMP 4.x SIMD Benchmarks
==========================

This repository contains a set of benchmark kernels to be called from within a SIMD context (e.g. a loop or another SIMD kernel).
The general idea is to assess compiler (SIMD) vectorization capabilities using simple reproducer kernels that were extracted from real-world scientific codes.
The kernels `k` implement
- wrapped math function calls (sqrt, log, exp): `k=simple`,
- conditional math function calls: `k=conditional_math`,
- early return from functions: `k=early_return`,
- nested branching with conditional math function calls: `k=nested_branching`,
- while loop on SIMD lanes with varying trip count not known at compile time: `k=while_loop`.

The different implementations `i` of the kernels include a(n)
- sequential reference version: `i=reference`,
- OpenMP 4.x version using the `omp declare simd` feature: `i=explicit_vectorization`,
- OpenMP 4.x version using `omp simd` loops to process user-defined vector data types: `i=enhanced_explicit_vectorization`,
- SIMD intrinsics version: `i=manual_vectorization`,
- C++ SIMD class version (using the Vc library): `i=simd_class_vectorization`.

Structure
---------
This repository has the following structure:
```
   build.<cxx=gnu,clang,intel>
   makefile.<cxx=gnu,clang,intel>[.<xphi=knc,knl>]
   readme.md
   LICENSE                // BSD-2 license file.
   
src/
   makefile_global
   makefile_local

src/common
   benchmark.cpp          // contains the main() function.
   simd_data_types.cpp

include/common
   kernel.hpp
   simd_data_types.hpp

include/<k>               // one sub-directory for each kernel k.
   kernel_<i>.cpp         // contains the implementations i.
```

Build
=====

Use the `build.<cxx>[.<xphi>]` scripts to generate the executables `bin.<cxx>[.<xphi>]/.<k>_<i>.x` for the different platforms and compilers.

The build scripts take as optional command line arguments
- `FUNC=<SQRT_SQRT,SQRT_LOG,SQRT_EXP,LOG_LOG,LOG_EXP,EXP_EXP>` the math function(s) to be used throughout the kernel execution,
- `SIMDWIDTH=<2,4,8,...>` the "logical" SIMD width to be used by OpenMP 4.x SIMD. The default is "logical=native" SIMD width (e.g. 4 on an AVX(2) system with 64 bit words). In case of `i=<reference,manual_vectorization,simd_class_vectorization>` the `SIMDWIDTH` paramter has no effect---these implementations always use "logical=native".

Example: build all kerneles with the Intel compiler for Xeon Phi KNL using log as math call and SIMD width 8
```
$> build.intel.knl FUNC=LOG_LOG SIMDWIDTH=8
```

Vc library
----------
If the Vc library is available and should be used, set up the environment variables `VC_AVAILABLE=<yes,[no=default]>` and `VC_ROOT=<Vc install directory>` in `build.*` appropriately.

Non-system glibc library
------------------------
In case of a non-system glibc should be used (e.g. to make use of `libmvec`, which comes with glibc-2.22 and later), set up the environment variable `GLIBC_ROOT=<glibc install directory>` in `build.*` appropriately.

Execute
=======

The executables `bin.<cxx>[.<xphi>]/<k>_<i>.x` take as optional command line arguments
- `-n <some integer value>` the number of array entries to be processed by the SIMD loop,
- `-l <some double value>` a lower bound for the random numbers to be assigned to the arrays,
- `-u <some double value>` an upper bound for the random numbers to be assigned to the arrays.

Default values are `n=8192`, `l=-1.0`, `u=+1.0`.

The executables are senstive to the `OMP_NUM_THREADS` environment variable.
You maybe need to explicitly manage the thread placement, e.g., via `KMP_AFFINITY` for Intel. 

Output
------
All executables output
- the number of OpenMP threads used for the execution (weak scaling experiment),
- the mean/min/max loop execution times in seconds and the statistical error for 6 benchmark iterations,
- the first 8 entries of the result array (per thread),
- the maximum absolute relative error when comparing the output arrays generated by the sequential and the SIMD execution of the loop (per thread).

An example run (and output) with 12 OpenMP threads could look like this:
```
$> export OMP_NUM_THREADS=12
$> export KMP_AFFINITY=scatter
$> bin.intel.knl/conditional_math_call_explicit_vectorization.x -n 8192 -l -1.0 -u 1.0
# number of OMP threads : 12
# timings per loop iteration:
# mean	      err	min		max
9.72674e-09   5.65089e-11		8.41101e-09	1.07393e-08
# output (thread 0): 6.37885 10 9 10 8.18427 10 8.38018 8.2433
# output (thread 3): 6.37885 10 9 10 8.18427 10 8.38018 8.2433
# max. abs. rel. error (thread 8): 2.31621e-13
# max. abs. rel. error (thread 9): 2.31621e-13
# max. abs. rel. error (thread 10): 2.31621e-13
# max. abs. rel. error (thread 11): 2.31621e-13
# output (thread 2): 6.37885 10 9 10 8.18427 10 8.38018 8.2433
# output (thread 1): 6.37885 10 9 10 8.18427 10 8.38018 8.2433
# max. abs. rel. error (thread 0): 2.31621e-13
# output (thread 4): 6.37885 10 9 10 8.18427 10 8.38018 8.2433
# output (thread 5): 6.37885 10 9 10 8.18427 10 8.38018 8.2433
# max. abs. rel. error (thread 3): 2.31621e-13
# output (thread 6): 6.37885 10 9 10 8.18427 10 8.38018 8.2433
# output (thread 7): 6.37885 10 9 10 8.18427 10 8.38018 8.2433
# max. abs. rel. error (thread 2): 2.31621e-13
# max. abs. rel. error (thread 1): 2.31621e-13
# max. abs. rel. error (thread 4): 2.31621e-13
# max. abs. rel. error (thread 5): 2.31621e-13
# max. abs. rel. error (thread 6): 2.31621e-13
# max. abs. rel. error (thread 7): 2.31621e-13
```

Tested setups
-------------
- GNU 5.3.0 + glibc-2.22 on Intel Haswell platform
- GNU 6.1.0 + glibc-2.23 on Intel Haswell platform
- Intel 16.0.[1,2,3] on Intel Haswell, KNC, KNL platform
- Clang 3.8, 3.9(pre) on Intel Haswell platform

References
==========
*Portable SIMD Performance with OpenMP 4.x Compiler Directives*, Florian Wende, Matthias Noack, Thomas Steinke, Michael Klemm, Chris J. Newburn, and Georg Zitzlsberger, Euro-Par 2016, Grenoble, France, 2016