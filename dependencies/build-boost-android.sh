#!/bin/bash

##
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
##

cd "$(readlink "$(dirname "${0}")")"

. ./build-common.sh android

ARCHIVE="boost_1_58_0.tar.bz2"
BUILD_DIR="${BUILD_ROOT}/boost_1_58_0"
PREFIX="${DEP_OUT_DIR}/boost/"

if [ -d ${PREFIX} ]; then
	exit 0
fi

title "Compile Boost"

. ./download-dependency.sh ${ARCHIVE}

status "Extracting Boost"

cd "${BUILD_ROOT}"
tar xfj "${ARCHIVE}"

status "Configuring Boost"

cd "${BUILD_DIR}"
cp "${PATCH_DIR}/boost/user-config-AndroidLinux.jam" "${BUILD_DIR}/tools/build/src/user-config.jam"
./bootstrap.sh  --prefix="${PREFIX}" --with-libraries=atomic,date_time,filesystem,log,python,regex,serialization,system,thread --with-python=python2.7

status "Building & Installing Boost"

./bjam -d2 \
	cxxflags=-fPIC \
	-j ${CPU_CORES} \
	variant=release \
	--layout=system \
	threading=multi \
	link=shared \
	toolset=gcc-android \
	install >/dev/null

status "Cleaning up"

cd "${DEP_DIR}"
rm -rf "${BUILD_ROOT}"
