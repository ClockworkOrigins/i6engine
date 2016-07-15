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

cd "$(readlink -f "$(dirname "${0}")")"

. ./build-common.sh

# boost
ARCHIVE="boost_1_58_0.tar.bz2"
BUILD_DIR="${BUILD_ROOT}/boost_1_58_0"

if [ -d ${BUILD_DIR} ]; then
	rm -rf ${BUILD_DIR}
fi

PREFIX="${DEP_DIR}/boost/"
DEBUG_FLAG="variant=debug"
RELEASE_FLAG="variant=release"
PARALLEL_FLAG=""

if [ -d ${PREFIX} ]; then
	exit 0
fi

if [ ! -z "${BUILD_PARALLEL}" ]; then
	PARALLEL_FLAG="-j ${BUILD_PARALLEL}"
fi
if [ -z "${DEBUG}" ]; then
	BUILD_TYPE="${RELEASE_FLAG}"
else
	BUILD_TYPE="${DEBUG_FLAG}"
fi
if [ ! -z "${CLEAN}" ]; then
	status "Cleaning Boost"
	rm -rf "${PREFIX}"
	exit 0
fi

title "Compile Boost"

./download-dependency.sh ${ARCHIVE}

status "Cleaning Boost"
rm -rf "${PREFIX}" >/dev/null

status "Extracting Boost"
cd "${BUILD_ROOT}"
tar xfj "${ARCHIVE}" >/dev/null

status "Bootstrapping Boost"
cd "${BUILD_DIR}"

./bootstrap.sh --prefix="${PREFIX}" --with-libraries=atomic,filesystem,thread,date_time,python,system,serialization,log --with-python=python2.7

status "Building & Installing Boost"
./bjam -d2 \
	${PARALLEL_FLAG} \
	${BUILD_TYPE} \
	link=shared \
	--layout=system \
	threading=multi \
	install >/dev/null

if [ 0 -ne $? ]; then
	status "Failed installing boost. Buildfiles not deleted"
	exit 1
fi

status "Cleaning up"
cd "${DEP_DIR}"
rm -rf "${BUILD_ROOT}" >/dev/null

touch "${PREFIX}"
