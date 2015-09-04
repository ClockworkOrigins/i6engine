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

configure_file(tut5.tex ${CMAKE_CURRENT_BINARY_DIR}/tut5.tex COPYONLY)
configure_file(listings/BasicMove.h ${CMAKE_CURRENT_BINARY_DIR}/listings/BasicMove.h COPYONLY)
configure_file(listings/FullMove.h ${CMAKE_CURRENT_BINARY_DIR}/listings/FullMove.h COPYONLY)

add_custom_target(BasicTutorial05 COMMAND ${PDFLATEX_COMPILER} ${CMAKE_CURRENT_BINARY_DIR}/tut5.tex WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
add_custom_command(TARGET BasicTutorial05 POST_BUILD COMMAND ${PDFLATEX_COMPILER} ${CMAKE_CURRENT_BINARY_DIR}/tut5.tex WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
