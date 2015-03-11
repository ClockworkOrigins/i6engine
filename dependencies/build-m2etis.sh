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

cd "$(readlink -f "$(dirname "${0}")")"

COPY_DIR=${1}
BUILD_FLAG=${2}

. ./build-common.sh

# M2etis
ARCHIVE="m2etis-0.3-rev20.zip"
BUILD_DIR="${BUILD_ROOT}/m2etis-0.3-rev20"

PREFIX="${PWD}/m2etis"
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

./download-dependency.sh ${ARCHIVE}

status "Cleaning M2etis"
rm -rf "${PREFIX}"


status "Extracting M2etis"
cd "${BUILD_ROOT}"
unzip "${EX_DIR}/${ARCHIVE}" >/dev/null

cd "${BUILD_DIR}"

status "Configuring M2etis"

cd "library"

if [ -e "CMakeCache.txt" ]; then
	rm "CMakeCache.txt"
	make clean
fi

cp "${DEP_DIR}/../config/m2etis/GeneratedChannelConfigurationExample.h" "include/m2etis/config/examples/"
cp "${DEP_DIR}/../config/m2etis/GeneratedChannelNameExample.h" "include/m2etis/config/examples/"
cp "${DEP_DIR}/../config/m2etis/GeneratedEventTypesExample.h" "include/m2etis/config/examples/"
cp "${DEP_DIR}/../config/m2etis/GeneratedMessageConfigurationExample.h" "include/m2etis/config/examples/"
mkdir -p "extern/i6engine/api"
mkdir -p "extern/i6engine/core/messaging"
mkdir -p "extern/i6engine/utils"
cp "${DEP_DIR}/../i6engine-core/include/i6engine/core/messaging/IPKey.h" "extern/i6engine/core/messaging"
cp "${DEP_DIR}/../i6engine-core/include/i6engine/core/messaging/MessageStruct.h" "extern/i6engine/core/messaging"
cp "${DEP_DIR}/../i6engine-core/include/i6engine/core/messaging/Message.h" "extern/i6engine/core/messaging"
cp "${DEP_DIR}/../i6engine-modules/include/i6engine/api/GameMessageStruct.h" "extern/i6engine/api"
cp "${DEP_DIR}/../i6engine-modules/include/i6engine/api/GameMessage.h" "extern/i6engine/api"
cp "${DEP_DIR}/../i6engine-utils/include/i6engine/utils/i6eSystemParameters.h" "extern/i6engine/utils"
if [ ! -z ${COPY_DIR} ]; then
	status "Copying '${COPY_DIR}'"
	cp ${COPY_DIR}/* "extern/i6engine"
else
	status "not Copying '${COPY_DIR}'"
fi

cmake -G 'Unix Makefiles'\
 -DCMAKE_BUILD_TYPE="${BUILD_TYPE}"\
 -DCMAKE_INSTALL_PREFIX="${PREFIX}"\
 -DDEP_DIR="${DEP_DIR}/"\
 -DADDITIONAL_INCLUDE_DIRECTORIES="${BUILD_DIR}/library/extern"\
 -DWITH_TESTING=OFF\
 -DWITH_SIM=OFF\
 -DWITH_LOGGING=OFF\
 -DCMAKE_CXX_COMPILER=clang++\
 .

status "Building M2etis"
make -j 8

status "Installing M2etis"
make install

status "Cleaning up"
cd "${DEP_DIR}"
rm -r "${BUILD_DIR}" >/dev/null
rm -rf "${EX_DIR}/.."

touch "${PREFIX}"
