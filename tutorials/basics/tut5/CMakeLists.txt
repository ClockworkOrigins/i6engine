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

configure_file(tut5.tex ${CMAKE_CURRENT_BINARY_DIR}/tut5.tex COPYONLY)
configure_file(listings/BasicMove.h ${CMAKE_CURRENT_BINARY_DIR}/listings/BasicMove.h COPYONLY)
configure_file(listings/FullMove.h ${CMAKE_CURRENT_BINARY_DIR}/listings/FullMove.h COPYONLY)

add_custom_target(BasicTutorial05 COMMAND ${PDFLATEX_COMPILER} ${CMAKE_CURRENT_BINARY_DIR}/tut5.tex WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
add_custom_command(TARGET BasicTutorial05 POST_BUILD COMMAND ${PDFLATEX_COMPILER} ${CMAKE_CURRENT_BINARY_DIR}/tut5.tex WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
