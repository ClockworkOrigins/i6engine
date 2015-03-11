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
./build-ois.sh
./build-ogre.sh		# depends on boost and ois
./build-cegui.sh	# depends on ois, ogre and boost
./build-gperftools.sh
./build-gmock.sh
./build-tinyxml.sh
./build-clockUtils.sh
./build-openalsoft.sh
#./build-m2etis.sh	# depends on boost; not usable until m2etis is published as well

