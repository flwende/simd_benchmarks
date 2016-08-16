CXX = g++
LD = g++

ifdef GLIBC_ROOT
    CXXFLAGS = -O3 -fopenmp -std=c++11 -ftree-vectorize -ffast-math -fopt-info-vec-optimized -I$(CURDIR)/include -I$(GLIBC_ROOT)/include
    LDFLAGS = -fopenmp -Wl,-rpath,$(GLIBC_ROOT)/lib -Wl,-dynamic-linker,$(GLIBC_ROOT)/lib/ld-linux-x86-64.so.2 -L$(GLIBC_ROOT)/lib -lm
else
    CXXFLAGS = -O3 -fopenmp -std=c++11 -ftree-vectorize -ffast-math -fopt-info-vec-optimized -I$(CURDIR)/include
    LDFLAGS = -fopenmp -lm
endif

FUNC = SQRT_SQRT
SIMDWIDTH = 4

CXXFLAGS += -mavx2 -mfma -D__AVX2__
CXXFLAGS += -DBENCHMARK_FUNC_$(FUNC)
CXXFLAGS += -DSIMD_WIDTH_LOGICAL_REAL64=$(SIMDWIDTH)
CXXFLAGS += -DDEBUG

BIN_DIR=$(CURDIR)/bin.gnu

# export all variables for subsequent make calls
export

all : 
	make -C src -f makefile_global

clean :
	make -C src -f makefile_global clean
