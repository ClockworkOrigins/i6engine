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

. ./build-common.sh

# OIS
ARCHIVE="wxWidgets-2.8.12.zip"
BUILD_DIR="${BUILD_ROOT}/wxWidgets-2.8.12"

if [ -d ${BUILD_DIR} ]; then
	rm -rf ${BUILD_DIR}
fi

PREFIX="${DEP_DIR}/wxWidgets/"

if [ ! -z "${CLEAN}" ]; then
	status "Cleaning wxWidgets"
	rm -rf "${PREFIX}"
	exit 0
fi

title "Compile wxWidgets"

./download-dependency.sh ${ARCHIVE}

status "Cleaning wxWidgets"
rm -rf "${PREFIX}"

status "Extracting wxWidgets"

cd "${BUILD_ROOT}"
unzip "${ARCHIVE}" >/dev/null

status "Configuring wxWidgets"
cd "${BUILD_DIR}"
chmod 700 ./*
./configure \
	--prefix="${PREFIX}"\
	--disable-shared\
	>/dev/null

status "Building wxWidgets"
make -j 8 #>/dev/null

status "Installing wxWidgets"
make -j 8 install #>/dev/null

status "Cleaning up"
cd "${DEP_DIR}"
rm -rf "${BUILD_ROOT}" >/dev/null

touch "${PREFIX}"

