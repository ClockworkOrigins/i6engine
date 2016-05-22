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

add_subdirectory(common)

add_subdirectory(canvas)
add_subdirectory(chain)

IF(ISIXE_WITH_NETWORK)
	add_subdirectory(chat)
ENDIF(ISIXE_WITH_NETWORK)

add_subdirectory(compositor)
add_subdirectory(dragdrop)
add_subdirectory(floating)
add_subdirectory(jenga)
add_subdirectory(multilanguage)
add_subdirectory(physicspower)

IF(ISIXE_WITH_RPG)
	add_subdirectory(rpg)
ENDIF(ISIXE_WITH_RPG)

IF(SCRIPTING_LUA OR SCRIPTING_PYTHON)
	add_subdirectory(scripting)
ENDIF(SCRIPTING_LUA OR SCRIPTING_PYTHON)

add_subdirectory(shadow)
add_subdirectory(terraforming)
add_subdirectory(terrain)
add_subdirectory(tooltip)

IF(ISIXE_WITH_RPG)
	add_subdirectory(waynet)
ENDIF(ISIXE_WITH_RPG)
