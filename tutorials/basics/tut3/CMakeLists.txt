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

configure_file(tut3.tex ${CMAKE_CURRENT_BINARY_DIR}/tut3.tex COPYONLY)
configure_file(listings/level.xml ${CMAKE_CURRENT_BINARY_DIR}/listings/level.xml COPYONLY)

add_custom_target(BasicTutorial03 COMMAND ${PDFLATEX_COMPILER} ${CMAKE_CURRENT_BINARY_DIR}/tut3.tex WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
add_custom_command(TARGET BasicTutorial03 POST_BUILD COMMAND ${PDFLATEX_COMPILER} ${CMAKE_CURRENT_BINARY_DIR}/tut3.tex WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
