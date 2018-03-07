#!/bin/bash

# DEFAULT configurations

function build_config_hsw {
	export VC_ROOT=${HOME}/Software/Vc
#	export VC_AVAILABLE=yes
	export VC_AVAILABLE=no

	export VCL_ROOT=${HOME}/Software/vcl
#	export VCL_AVAILABLE=yes
	export VCL_AVAILABLE=no

	export UMESIMD_ROOT=${HOME}/Software/umesimd
	export UMESIMD_AVAILABLE=yes
#	export UMESIMD_AVAILABLE=no

	export PLATFORM=x86
}

function build_config_skl {
	export VC_ROOT=${HOME}/Software/Vc
#	export VC_AVAILABLE=yes
	export VC_AVAILABLE=no

	export VCL_ROOT=${HOME}/Software/vcl
#	export VCL_AVAILABLE=yes
	export VCL_AVAILABLE=no

	export UMESIMD_ROOT=${HOME}/Software/umesimd
	export UMESIMD_AVAILABLE=yes
#	export UMESIMD_AVAILABLE=no

	export PLATFORM=x86
}

function build_config_knl {
	export VC_ROOT=${HOME}/Software/Vc
#	export VC_AVAILABLE=yes
	export VC_AVAILABLE=no

	export VCL_ROOT=${HOME}/Software/vcl
#	export VCL_AVAILABLE=yes
	export VCL_AVAILABLE=no

	export UMESIMD_ROOT=${HOME}/Software/umesimd
	export UMESIMD_AVAILABLE=yes
#	export UMESIMD_AVAILABLE=no

	export PLATFORM=x86
}

