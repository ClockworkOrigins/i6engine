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

. ./build-common.sh ${1}

ARCHIVE="bullet3-2.83.7.tar.gz"
BUILD_DIR="${BUILD_ROOT}/bullet3-2.83.7"
PREFIX="${DEP_OUT_DIR}/bullet/"

if [ -d ${PREFIX} ]; then
	exit 0
fi

title "Compile Bullet"

status "Extracting Bullet"

downloadAndUnpack ${ARCHIVE}

status "Configuring Bullet"

cd "${BUILD_DIR}"
cmake \
	-DINSTALL_LIBS=ON \
	-DBUILD_SHARED_LIBS=ON \
	-DUSE_DOUBLE_PRECISION=ON \
	-DUSE_GRAPHICAL_BENCHMARK=OFF \
	-DUSE_GLUT=OFF \
	-DBUILD_EXTRAS=ON \
	-DBUILD_CPU_DEMOS=OFF \
	-DBUILD_BULLET2_DEMOS=OFF \
	-DBUILD_BULLET3=OFF \
	-DBUILD_OPENGL3_DEMOS=OFF \
	-DBUILD_UNIT_TESTS=OFF \
	-DINSTALL_EXTRA_LIBS=ON \
	-DCMAKE_INSTALL_PREFIX="${PREFIX}" \
	-DCMAKE_BUILD_TYPE=Release \
	-DCMAKE_C_COMPILER=${C_COMPILER} \
	-DCMAKE_CXX_COMPILER=${CXX_COMPILER} \
.

status "Building Bullet"

make -j ${CPU_CORES}

status "Installing Bullet"

make install

status "Cleaning up"

cd "${DEP_DIR}"
rm -rf "${BUILD_ROOT}"

