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

# Ogre
ARCHIVE="sinbad-ogre-dd30349ea667.tar.bz2"
DEP_ARCHIVE="cabalistic-ogredeps-0e96ef9d3475.zip"
BUILD_DIR="${BUILD_ROOT}/sinbad-ogre-dd30349ea667"
BUILD_DIR_DEPS="${BUILD_ROOT}/cabalistic-ogredeps-0e96ef9d3475"

if [ -d ${BUILD_DIR} ]; then
	rm -rf ${BUILD_DIR}
fi

PREFIX="${DEP_DIR}/ogre/"
PREFIX_DEPS="${DEP_DIR}/misc/"
DEBUG_FLAG="-DCMAKE_BUILD_TYPE=Debug"
RELEASE_FLAG="-DCMAKE_BUILD_TYPE=Release"
PARALLEL_FLAG=""

if [ -d ${PREFIX} ]; then
	exit 0
fi

if [ ! -z "${BUILD_PARALLEL}" ]; then
	PARALLEL_FLAG="-j ${BUILD_PARALLEL}"
fi

if [ ! -z "${CLEAN}" ]; then
	status "Cleaning Ogre"
	rm -rf "${PREFIX}"
	exit 0
fi

title "Compile Ogre"

./download-dependency.sh ${ARCHIVE}
./download-dependency.sh ${DEP_ARCHIVE}
./download-dependency.sh 1.9.0-03_move_stowed_template_func.patch

status "Cleaning Ogre"
rm -rf "${PREFIX}"

status "Extracting Ogre"
cd "${BUILD_ROOT}"
tar xfj "${ARCHIVE}"
unzip "${DEP_ARCHIVE}"

cd ${BUILD_DIR_DEPS}

cmake -DCMAKE_INSTALL_PREFIX:PATH=${PREFIX_DEPS} -DOGREDEPS_BUILD_OIS=ON -DCMAKE_BUILD_TYPE=Release .

make
make install

# ../cmake_files/FindBoost.cmake tries to find the libs here
export BOOST_ROOT="${DEP_DIR}/boost"
export BOOST_INCLUDEDIR="${DEP_DIR}/boost/include"
export BOOST_LIBRARYDIR="${DEP_DIR}/boost/lib"

export OIS_HOME="${DEP_DIR}/ois"

cd "${BUILD_DIR}"

patch -p0 -i "1.9.0-03_move_stowed_template_func.patch"

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
	-DOGRE_INSTALL_SAMPLES:BOOL=ON\
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
	-DOGRE_DEPENDENCIES_DIR=${PREFIX_DEPS}\
	${RELEASE_FLAG} .

status "Building release version of Ogre"
# I'll leave the build output enabled here, because it's cmake output and because it takes fairly long
make ${PARALLEL_FLAG}

status "Installing release version of Ogre"
make ${PARALLEL_FLAG} install

status "Cleaning up"
cd "${DEP_DIR}"
rm -rf "${BUILD_ROOT}" &>/dev/null

touch "${PREFIX}"
