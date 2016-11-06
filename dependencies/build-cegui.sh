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

ARCHIVE="cegui-0.8.4.tar.bz2"
BUILD_DIR="${BUILD_ROOT}/cegui-0.8.4"
PREFIX="${DEP_OUT_DIR}/cegui/"

if [ -d ${PREFIX} ]; then
	exit 0
fi

title "Compile CEGUI"

. ./download-dependency.sh ${ARCHIVE}

status "Extracting CEGUI"

cd "${BUILD_ROOT}"
tar xf "${ARCHIVE}"

status "Configuring CEGUI"

cd "${BUILD_DIR}"
cmake \
	-DBOOST_NO_SYSTEM_PATHS=ON \
	-DCEGUI_BUILD_XMLPARSER_XERCES=OFF \
	-DCEGUI_BUILD_XMLPARSER_LIBXML2=OFF \
	-DCEGUI_BUILD_XMLPARSER_RAPIDXML=OFF \
	-DCEGUI_BUILD_XMLPARSER_TINYXML=OFF \
	-DCEGUI_BUILD_XMLPARSER_EXPAT=ON \
	-DCEGUI_BUILD_IMAGECODEC_SILLY=OFF \
	-DCEGUI_BUILD_IMAGECODEC_DEVIL=OFF \
	-DCEGUI_BUILD_IMAGECODEC_FREEIMAGE=ON \
	-DCEGUI_BUILD_IMAGECODEC_CORONA=OFF \
	-DCEGUI_BUILD_IMAGECODEC_STB=OFF \
	-DCEGUI_BUILD_IMAGECODEC_TGA=ON \
	-DCEGUI_BUILD_IMAGECODEC_PVR=OFF \
	-DCEGUI_BUILD_RENDERER_OGRE=ON \
	-DCEGUI_BUILD_RENDERER_OPENGL=OFF \
	-DCEGUI_BUILD_RENDERER_OPENGL3=OFF \
	-DCEGUI_BUILD_RENDERER_IRRLICHT=OFF \
	-DCEGUI_BUILD_RENDERER_NULL=OFF \
	-DCEGUI_BUILD_RENDERER_OPENGLES=OFF \
	-DCEGUI_BUILD_LUA_MODULE=OFF \
	-DCEGUI_BUILD_LUA_GENERATOR=OFF \
	-DCEGUI_BUILD_PYTHON_MODULES=OFF \
	-DCEGUI_SAMPLES_ENABLED=OFF \
	-DCEGUI_BUILD_TESTS=OFF \
	-DBOOST_ROOT=${DEP_OUT_DIR}/boost/ \
	-DOGRE_H_PATH=${DEP_OUT_DIR}/ogre/include/OGRE/ \
	-DOGRE_H_BUILD_SETTINGS_PATH=${DEP_OUT_DIR}/ogre/include/OGRE/ \
	-DOGRE_LIB=${DEP_OUT_DIR}/ogre/lib/libOgreMain.so \
	-DOIS_H_PATH=${DEP_OUT_DIR}/ois/include \
	-DOIS_LIB=${DEP_OUT_DIR}/ois/lib64/libOIS.so \
	-DCMAKE_SKIP_BUILD_RPATH=FALSE \
	-DCMAKE_BUILD_WITH_INSTALL_RPATH=FALSE \
	-DCMAKE_INSTALL_RPATH="${DEP_OUT_DIR}/boost/lib" \
	-DCMAKE_INSTALL_RPATH_USE_LINK_PATH=TRUE \
	-DCMAKE_INSTALL_PREFIX="${PREFIX}" \
	-DCMAKE_BUILD_TYPE=Release \
	-DCMAKE_C_COMPILER=${C_COMPILER} \
	-DCMAKE_CXX_COMPILER=${CXX_COMPILER} \
.

status "Building CEGUI"

make -j ${CPU_CORES}

status "Installing CEGUI"

make install

# cp instead of mv is important. otherwise dynamically loaded libs aren't found somehow...
cp -r ${PREFIX}lib/cegui-0.8/* ${PREFIX}lib/
cp -r ${PREFIX}include/cegui-0/* ${PREFIX}include/

status "Cleaning up"

cd "${DEP_DIR}"
rm -rf "${BUILD_ROOT}"

