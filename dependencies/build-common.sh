#!/bin/bash

##
# Copyright 2012 FAU (Friedrich Alexander University of Erlangen-Nuremberg)
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
##

set -e

# make sure this doesn't get called directly
if [ "`basename "${0}"`" == "build-common.sh" ]; then
	echo "This script should not be called directly. Please execute build-dependencies.sh instead" >&2
	exit 1
fi

# parse arguments

# usage
usage() {
	echo "Usage: ${0} [OPTIONS]" >&2
	echo >&2
	echo "Possible options are:" >&2
	echo "  -h,   --help          displays this help message" >&2
	echo "  -j n, --parallel=n    enables parallel building with n processes" >&2
	echo "  -r,   --release       disables building the debug versions of libs" >&2
	echo "  -c,   --clean         removes all files generated by these scripts" >&2
}

# only parse arguments the first time this script is called
if [ -z "${ARGS_PARSED}" ]; then
	# default parameters
	export BUILD_PARALLEL="$(grep processor -c /proc/cpuinfo)"
	export CLEAN=""

	# option strings
	LONG_OPTS="help,parallel:,release,clean"
	SHORT_OPTS="hj:rc"
	ARGS=$( getopt -n "`basename ${0}`" -l "${LONG_OPTS}" -o "${SHORT_OPTS}" -- "$@" )
	if [[ $? != 0 ]]; then
		usage
		exit 1
	fi
	eval set -- "${ARGS}"
	for i; do
		case "${i}" in
			-h|--help)
				usage;
				exit 1;
				;;
			-j|--parallel)
				shift;
				BUILD_PARALLEL=$(( $1 + 0 ));
				shift;
				;;
			-r|--release)
				shift;
				DEBUG=""
				M2ETIS_DEBUG="ON"
				;;
			-c|--clean)
				shift;
				CLEAN="1"
				;;
		esac
	done

	export ARGS_PARSED=1
fi

# shared variables
DEP_DIR="${PWD}"
BUILD_ROOT="/tmp/`whoami`/i6engine"

mkdir -p "${BUILD_ROOT}"

# print titles
title() {
	text="$1"
	echo
	echo
	echo "${text}"
	echo
	echo
}

# print status text
status() {
	text="${1}"
	echo "	${text}"
}

# check whether dependencies are up to date
uptodate() {
	reference="${1}"
	target="${2}"
	if [ ! -d "${target}" ]; then
		# hasn't even been built at all yet
		return 0;
	fi

	mtime_reference=`stat -c '%Y' "${reference}"`
	mtime_target=`stat -c '%Y' "${target}"`
	#echo "Comparing ${reference} and ${target}"
	if [ "${mtime_reference}" -gt "${mtime_target}" ]; then
		#echo "outdated"
		return 0;
	else
		#echo "up to date"
		return 1;
	fi
}

