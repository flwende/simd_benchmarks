#!/bin/bash

source benchmark.sh
source benchmark_config.sh

benchmark_config_knl

compiler=clang.knl

benchmark "${curdir}" "${n}" "${compiler}" "${simdwidths}" "${funcs}" "${threads}" "${executables}" "${versions}"

