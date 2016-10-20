#!/bin/bash

function build_config_knl {
	#export VC_ROOT=${HOME}/Software/Vc
	#export VC_AVAILABLE=yes
	export VC_AVAILABLE=no

	export UMESIMD_ROOT=${HOME}/Software/umesimd
	export UMESIMD_AVAILABLE=yes
	#export UMESIMD_AVAILABLE=no

	export PLATFORM=x86
}

function build_config {
	#export VC_ROOT=${HOME}/Software/Vc
	#export VC_AVAILABLE=yes
	export VC_AVAILABLE=no

	export UMESIMD_ROOT=${HOME}/Software/umesimd
	export UMESIMD_AVAILABLE=yes
	#export UMESIMD_AVAILABLE=no

	export PLATFORM=x86
}
