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

set -e

#
# Automatic build script to compile system dependencies
#

# all paths are relative to this file's directory
cd "$(readlink -f "$(dirname "${0}")")"

. build-common.sh

if [ ! -z "${CLEAN}" ]; then
	title "Cleaning all dependencies..."
else
	title "Compiling all dependencies..."
fi

./build-boost.sh
./build-bullet.sh
./build-ogre.sh		# depends on boost and ois
./build-cegui.sh	# depends on ois, ogre and boost
./build-gperftools.sh
./build-gmock.sh
./build-tinyxml.sh
./build-clockUtils.sh
./build-openalsoft.sh
#./build-m2etis.sh	# depends on boost; not usable until m2etis is published as well

