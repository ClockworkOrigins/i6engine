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

set -e

. ./build-boost.sh ${1}
. ./build-bullet.sh ${1}
. ./build-clockUtils.sh ${1}
. ./build-gmock.sh ${1}
. ./build-lua.sh ${1}
. ./build-m2etis.sh ${1}
. ./build-ogre.sh ${1}
. ./build-openalsoft.sh ${1}
. ./build-tinyxml.sh ${1}
. ./build-cegui.sh ${1}
