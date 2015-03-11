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

# CEGUI
ARCHIVE="cegui-0.8.4.tar.bz2"
BUILD_DIR="${BUILD_ROOT}/cegui-0.8.4"

if [ -d ${BUILD_DIR} ]; then
	rm -rf ${BUILD_DIR}
fi

PREFIX="${PWD}/cegui/"

PARALLEL_FLAG=""
if [ ! -z "${BUILD_PARALLEL}" ]; then
	PARALLEL_FLAG="-j ${BUILD_PARALLEL}"
fi

DEBUG_FLAG="Debug"
RELEASE_FLAG="Release"
if [ -z "${DEBUG}" ]; then
	BUILD_TYPE="${RELEASE_FLAG}"
else
	BUILD_TYPE="${DEBUG_FLAG}"
fi

if [ ! -z "${CLEAN}" ]; then
	status "Cleaning CEGUI"
	rm -rf "${PREFIX}"
	exit 0
fi

title "Compile CEGUI"

if ! uptodate "${EX_DIR}/${ARCHIVE}" "${PREFIX}"; then
	status "CEGUI seems to be up to date, skipping build"
	exit 0
fi

./download-dependency.sh ${ARCHIVE}

status "Cleaning CEGUI"
rm -rf "${DEST_DIR}" >/dev/null

status "Extracting CEGUI"
cd "${BUILD_ROOT}"
tar xf "${EX_DIR}/${ARCHIVE}" >/dev/null

cd ${BUILD_DIR}

status "Configuring CEGUI"
cmake -DCMAKE_INSTALL_PREFIX="${PREFIX}"\
	-DCEGUI_BUILD_XMLPARSER_XERCES=OFF\
	-DCEGUI_BUILD_XMLPARSER_LIBXML2=OFF\
	-DCEGUI_BUILD_XMLPARSER_RAPIDXML=OFF\
	-DCEGUI_BUILD_XMLPARSER_TINYXML=OFF\
	-DCEGUI_BUILD_XMLPARSER_EXPAT=ON\
	-DCEGUI_BUILD_IMAGECODEC_SILLY=OFF\
	-DCEGUI_BUILD_IMAGECODEC_DEVIL=OFF\
	-DCEGUI_BUILD_IMAGECODEC_FREEIMAGE=ON\
	-DCEGUI_BUILD_IMAGECODEC_CORONA=OFF\
	-DCEGUI_BUILD_IMAGECODEC_STB=OFF\
	-DCEGUI_BUILD_IMAGECODEC_TGA=ON\
	-DCEGUI_BUILD_IMAGECODEC_PVR=OFF\
	-DCEGUI_BUILD_RENDERER_OGRE=ON\
	-DCEGUI_BUILD_RENDERER_OPENGL=OFF\
	-DCEGUI_BUILD_RENDERER_OPENGL3=OFF\
	-DCEGUI_BUILD_RENDERER_IRRLICHT=OFF\
	-DCEGUI_BUILD_RENDERER_NULL=OFF\
	-DCEGUI_BUILD_RENDERER_OPENGLES=OFF\
	-DCEGUI_BUILD_LUA_MODULE=OFF\
	-DCEGUI_BUILD_LUA_GENERATOR=OFF\
	-DCEGUI_BUILD_PYTHON_MODULES=OFF\
	-DCEGUI_SAMPLES_ENABLED=OFF\
	-DCEGUI_BUILD_TESTS=OFF\
	-DCMAKE_BUILD_TYPE=${BUILD_TYPE}\
	-DBOOST_ROOT=${DEP_DIR}/boost/\
	-DOGRE_H_PATH=${DEP_DIR}/ogre/include/OGRE/\
	-DOGRE_H_BUILD_SETTINGS_PATH=${DEP_DIR}/ogre/include/OGRE/\
	-DOGRE_LIB=${DEP_DIR}/ogre/lib/libOgreMain.so\
	-DOIS_H_PATH=${DEP_DIR}/ois/include\
	-DOIS_LIB=${DEP_DIR}/ois/lib64/libOIS.so

status "Building CEGUI"
make ${PARALLEL_FLAG} >/dev/null

status "Installing CEGUI"
make ${PARALLEL_FLAG} install >/dev/null

# cp instead of mv is important. otherwise dynamically loaded libs aren't found somehow...
cp -r ${PREFIX}lib/cegui-0.8/* ${PREFIX}lib/
cp -r ${PREFIX}include/cegui-0/* ${PREFIX}include/

status "Cleaning up"
cd "${DEP_DIR}"
rm -r "${BUILD_DIR}" >/dev/null
rm -rf "${EX_DIR}/.."

touch "${PREFIX}"
