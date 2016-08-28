cd "$(readlink -f "$(dirname "${0}")")"

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

. ./build-common.sh

ARCHIVE="lua-5.3.3.tar.gz"
BUILD_DIR="${BUILD_ROOT}/lua-5.3.3"

PREFIX="${DEP_DIR}/lua"
DEBUG_FLAG="DEBUG"
RELEASE_FLAG="RELEASE"
PARALLEL_FLAG=""
BUILD_TYPE="Release"

if [ -d ${PREFIX} ]; then
	exit 0
fi

if [ ! -z "${BUILD_PARALLEL}" ]; then
	PARALLEL_FLAG="-j ${BUILD_PARALLEL}"
fi

if [ ! -z "${CLEAN}" ]; then
	rm -rf "${PREFIX}"
	exit 0
fi

rm -rf "${PREFIX}"

./download-dependency.sh ${ARCHIVE}


cd "${BUILD_ROOT}"
tar xfz "${ARCHIVE}" > /dev/null

cd "${BUILD_DIR}"

if [ -e "CMakeCache.txt" ]; then
	rm "CMakeCache.txt"
	make clean
fi

cp "${DEP_DIR}/../../ext/lua/CMakeLists.txt" "${BUILD_DIR}/CMakeLists.txt"

cmake -G 'Unix Makefiles'\
 -DCMAKE_BUILD_TYPE="${BUILD_TYPE}"\
 -DCMAKE_INSTALL_PREFIX="${PREFIX}"\
 -DCMAKE_CXX_COMPILER=g++\
 .

make -j 8 > /dev/null

make install > /dev/null

cd "${DEP_DIR}"
rm -rf "${BUILD_ROOT}" > /dev/null

touch "${PREFIX}"
