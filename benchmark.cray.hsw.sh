#!/bin/bash

source benchmark.sh
source benchmark_config.sh

benchmark_config_hsw

compiler=cray.hsw

benchmark "${curdir}" "${n}" "${compiler}" "${simdwidths}" "${funcs}" "${threads}" "${executables}" "${versions}"

