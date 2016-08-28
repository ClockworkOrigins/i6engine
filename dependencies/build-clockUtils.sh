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

ARCHIVE="clockutils-1.0.0-src.zip"
BUILD_DIR="${BUILD_ROOT}/clockutils-1.0.0-src"

PREFIX="${DEP_DIR_OUT}/clockUtils"

if [ -d ${PREFIX} ]; then
	exit 0
fi

. ./download-dependency.sh ${ARCHIVE} http://clockwork-origins.de/clockUtils/downloads/

cd "${BUILD_ROOT}"

unzip "${ARCHIVE}" >/dev/null

cd "${BUILD_DIR}"

cmake -G 'Unix Makefiles'\
 -DCMAKE_BUILD_TYPE=Release\
 -DCMAKE_INSTALL_PREFIX="${PREFIX}"\
 -DWITH_TESTING=OFF\
 -DWITH_LIBRARY_COMPRESSION=ON\
 -DWITH_LIBRARY_SOCKETS=ON\
 -DWITH_LIBRARY_INIPARSER=ON\
 -DWITH_LIBRARY_ARGPARSER=ON\
 -DCMAKE_CXX_COMPILER=g++\
 .

make -j ${CPU_CORES}

make -j ${CPU_CORES} install

cd "${DEP_DIR}"
rm -rf "${BUILD_ROOT}"

touch "${PREFIX}"
