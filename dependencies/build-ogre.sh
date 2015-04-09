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

# Ogre
ARCHIVE="sinbad-ogre-dd30349ea667.tar.bz2"
BUILD_DIR="${BUILD_ROOT}/sinbad-ogre-dd30349ea667"

if [ -d ${BUILD_DIR} ]; then
	rm -rf ${BUILD_DIR}
fi

PREFIX="${PWD}/ogre/"
DEBUG_FLAG="-DCMAKE_BUILD_TYPE=Debug"
RELEASE_FLAG="-DCMAKE_BUILD_TYPE=Release"
PARALLEL_FLAG=""

if [ ! -z "${BUILD_PARALLEL}" ]; then
	PARALLEL_FLAG="-j ${BUILD_PARALLEL}"
fi

if [ ! -z "${CLEAN}" ]; then
	status "Cleaning Ogre"
	rm -rf "${PREFIX}"
	exit 0
fi

title "Compile Ogre"

if ! uptodate "${EX_DIR}/${ARCHIVE}" "${PREFIX}"; then
	status "Ogre seems to be up to date, skipping build"
	exit 0
fi

./download-dependency.sh ${ARCHIVE}
#./download-dependency.sh ogre-freetype.patch

#mkdir -p ${PATCH_DIR}

#mv ${EX_DIR}/ogre-freetype.patch ${PATCH_DIR}/

status "Cleaning Ogre"
rm -rf "${DEST_DIR}"

status "Extracting Ogre"
cd "${BUILD_ROOT}"
tar xfj "${EX_DIR}/${ARCHIVE}"

# applying freetype patch
#patch -p0 -i "${PATCH_DIR}ogre-freetype.patch"

# ../cmake_files/FindBoost.cmake tries to find the libs here
export BOOST_ROOT="${DEP_DIR}/boost"
export BOOST_INCLUDEDIR="${DEP_DIR}/boost/include"
export BOOST_LIBRARYDIR="${DEP_DIR}/boost/lib"

export OIS_HOME="${DEP_DIR}/ois"


cd "${BUILD_DIR}"

status "Configurig release version of Ogre"
cmake -G 'Unix Makefiles'\
	-DBoost_NO_SYSTEM_PATHS=ON\
	-DOGRE_BUILD_COMPONENT_PAGING:BOOL=ON\
	-DOGRE_BUILD_COMPONENT_PROPERTY:BOOL=ON\
	-DOGRE_BUILD_COMPONENT_RTSHADERSYSTEM:BOOL=ON\
	-DOGRE_BUILD_COMPONENT_TERRAIN:BOOL=ON\
	-DOGRE_BUILD_PLUGIN_BSP:BOOL=ON\
	-DOGRE_BUILD_PLUGIN_CG:BOOL=ON\
	-DOGRE_BUILD_PLUGIN_OCTREE:BOOL=ON\
	-DOGRE_BUILD_PLUGIN_PCZ:BOOL=ON\
	-DOGRE_BUILD_PLUGIN_PFX:BOOL=ON\
	-DOGRE_BUILD_RENDERSYSTEM_GL:BOOL=ON\
	-DOGRE_BUILD_RTSHADERSYSTEM_CORE_SHADERS:BOOL=ON\
	-DOGRE_BUILD_RTSHADERSYSTEM_EXT_SHADERS:BOOL=ON\
	-DOGRE_BUILD_TESTS:BOOL=OFF\
	-DOGRE_BUILD_SAMPLES:BOOL=OFF\
	-DOGRE_BUILD_TOOLS:BOOL=ON\
	-DOGRE_CONFIG_ALLOCATOR:STRING=1\
	-DOGRE_CONFIG_CONTAINERS_USE_CUSTOM_ALLOCATOR:BOOL=ON\
	-DOGRE_CONFIG_ENABLE_DDS:BOOL=ON\
	-DOGRE_CONFIG_ENABLE_FREEIMAGE:BOOL=ON\
	-DOGRE_CONFIG_ENABLE_PVRTC:BOOL=OFF\
	-DOGRE_CONFIG_ENABLE_VIEWPORT_ORIENTATIONMODE:BOOL=OFF\
	-DOGRE_CONFIG_ENABLE_ZIP:BOOL=ON\
	-DOGRE_CONFIG_MEMTRACK_RELEASE:BOOL=OFF\
	-DOGRE_CONFIG_STRING_USE_CUSTOM_ALLOCATOR:BOOL=OFF\
	-DOGRE_CONFIG_THREADS:STRING=2\
	-DOGRE_CONFIG_THREAD_PROVIDER:STRING=boost\
	-DOGRE_FULL_RPATH:BOOL=OFF\
	-DOGRE_INSTALL_DOCS:BOOL=OFF\
	-DOGRE_INSTALL_SAMPLES:BOOL=OFF\
	-DOGRE_INSTALL_SAMPLES_SOURCE:BOOL=OFF\
	-DOGRE_INSTALL_TOOLS:BOOL=ON\
	-DOGRE_LIB_DIRECTORY:STRING=lib\
	-DOGRE_PROFILING:BOOL=OFF\
	-DOGRE_STATIC:BOOL=OFF\
	-DCMAKE_INSTALL_PREFIX:PATH="${PREFIX}"\
	-DOGRE_USE_BOOST:BOOL=ON\
	-DOGRE_CONFIG_DOUBLE=TRUE\
	-DBoost_DIR="${DEP_DIR}/boost"\
	-DBoost_INCLUDE_DIR="${DEP_DIR}/boost/include"\
	-DCMAKE_SKIP_BUILD_RPATH=FALSE\
	-DCMAKE_BUILD_WITH_INSTALL_RPATH=FALSE\
	-DCMAKE_INSTALL_RPATH="${DEP_DIR}/boost/lib"\
	-DCMAKE_INSTALL_RPATH_USE_LINK_PATH=TRUE\
	${RELEASE_FLAG} . &> /dev/null

status "Building release version of Ogre"
# I'll leave the build output enabled here, because it's cmake output and because it takes fairly long
make ${PARALLEL_FLAG} &>/dev/null

status "Installing release version of Ogre"
make ${PARALLEL_FLAG} install &>/dev/null

status "Cleaning up"
cd "${DEP_DIR}"
rm -r "${BUILD_DIR}" &>/dev/null
#rm -rf "${DEP_DIR}/../externals"

touch "${PREFIX}"
