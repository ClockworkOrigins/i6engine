#!/bin/bash

# i6engine
# Copyright (2016) Daniel Bonrath, Michael Baer, All rights reserved.
#
# This file is part of i6engine; i6engine is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

set -e

# make sure this doesn't get called directly
if [ "`basename "${0}"`" == "build-common.sh" ]; then
	echo "This script should not be called directly. Please execute build-dependencies.sh instead" >&2
	exit 1
fi

# shared variables
DEP_DIR="${PWD}"
DEP_DIR_OUT="${DEP_DIR}/x86"
BUILD_ROOT="/tmp/`whoami`/i6engine"
CPU_CORES=`grep -c processor /proc/cpuinfo`

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
