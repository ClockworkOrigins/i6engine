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

COPY_DIR=${1}
BUILD_FLAG=${2}
ROOT_DIR=${3}

. ./build-common.sh

# M2etis
ARCHIVE="m2etis-0.4.0-src.zip"
BUILD_DIR="${BUILD_ROOT}/m2etis-0.4.0-src"

PREFIX="${ROOT_DIR}/m2etis"
DEBUG_FLAG="DEBUG"
RELEASE_FLAG="RELEASE"
PARALLEL_FLAG=""

if [ ! -z "${BUILD_PARALLEL}" ]; then
	PARALLEL_FLAG="-j ${BUILD_PARALLEL}"
fi

if [[ "${BUILD_FLAG}" == "Release" || "${BUILD_FLAG}" == "RelWithDebInfo" || "${BUILD_FLAG}" == "MinSizeRel" ]]; then
	BUILD_TYPE="${RELEASE_FLAG}"
else
	BUILD_TYPE="${DEBUG_FLAG}"
fi

if [ ! -z "${CLEAN}" ]; then
	status "Cleaning M2etis"
	rm -rf "${PREFIX}"
	exit 0
fi

title "Compile M2etis"

if [ -d ${PREFIX} ]; then
	status "Up to date. Skipping ..."
	exit 0
fi

./download-dependency.sh ${ARCHIVE} http://clockwork-origins.de/m2etis/downloads/

status "Cleaning M2etis"
rm -rf "${PREFIX}"


status "Extracting M2etis"
cd "${BUILD_ROOT}"
unzip "${ARCHIVE}" >/dev/null

cd "${BUILD_DIR}"

status "Configuring M2etis"

cp "${DEP_DIR}/../config/m2etis/GeneratedChannelConfiguration.h" "library/generated/"
cp "${DEP_DIR}/../config/m2etis/GeneratedChannelName.h" "library/generated/"
cp "${DEP_DIR}/../config/m2etis/GeneratedEventTypes.h" "library/generated/"
cp "${DEP_DIR}/../config/m2etis/GeneratedMessageConfiguration.h" "library/generated/"
mkdir -p "library/extern/i6engine/api"
mkdir -p "library/extern/i6engine/core/messaging"
mkdir -p "library/extern/i6engine/utils"
cp "${DEP_DIR}/../libs/i6engine-core/include/i6engine/core/messaging/IPKey.h" "library/extern/i6engine/core/messaging"
cp "${DEP_DIR}/../libs/i6engine-core/include/i6engine/core/messaging/MessageStruct.h" "library/extern/i6engine/core/messaging"
cp "${DEP_DIR}/../libs/i6engine-core/include/i6engine/core/messaging/Message.h" "library/extern/i6engine/core/messaging"
cp "${DEP_DIR}/../libs/i6engine-modules/include/i6engine/api/GameMessageStruct.h" "library/extern/i6engine/api"
cp "${DEP_DIR}/../libs/i6engine-modules/include/i6engine/api/GameMessage.h" "library/extern/i6engine/api"
cp "${DEP_DIR}/../libs/i6engine-utils/include/i6engine/utils/i6eSystemParameters.h" "library/extern/i6engine/utils"
if [ ! -z ${COPY_DIR} ]; then
	status "Copying '${COPY_DIR}'"
	cp ${COPY_DIR}/* "library/extern/i6engine"
else
	status "not Copying '${COPY_DIR}'"
fi

cmake -G 'Unix Makefiles'\
 -DCMAKE_BUILD_TYPE="${BUILD_TYPE}"\
 -DCMAKE_INSTALL_PREFIX="${PREFIX}"\
 -DDEP_DIR="${ROOT_DIR}/"\
 -DADDITIONAL_INCLUDE_DIRECTORIES="${BUILD_DIR}/library/extern"\
 -DWITH_TESTING=OFF\
 -DWITH_SIM=OFF\
 -DWITH_LOGGING=OFF\
 -DCMAKE_CXX_COMPILER=g++\
 .

status "Building M2etis"
make -j 8

status "Installing M2etis"
make install

status "Cleaning up"
cd "${DEP_DIR}"
rm -rf "${BUILD_ROOT}" >/dev/null

touch "${PREFIX}"
