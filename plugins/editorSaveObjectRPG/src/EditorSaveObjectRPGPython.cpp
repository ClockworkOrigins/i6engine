/*
 * i6engine
 * Copyright (2016) Daniel Bonrath, Michael Baer, All rights reserved.
 *
 * This file is part of i6engine; i6engine is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "EditorSaveObjectRPG.h"

namespace i6e {
namespace plugins {

	void EditorSaveObjectRPG::insertMethodDeclaration() {
		_file << "# This method is called during first loading of the level in RPG" << std::endl;
		_file << "def Startup_" << _level.toStdString() << "():" << std::endl;
	}

	void EditorSaveObjectRPG::saveNPC(const std::string & identifier, const Vec3 & pos, const Quaternion & rot) {
		_file << "\tinsertNPCAtPosition(" << identifier << ", Vec3(" << pos.getX() << ", " << pos.getY() << ", " << pos.getZ() << "), Quaternion(" << rot.getW() << ", " << rot.getX() << ", " << rot.getY() << ", " << rot.getZ() << "))" << std::endl;
	}

	void EditorSaveObjectRPG::saveItem(const std::string & identifier, const Vec3 & pos, const Quaternion & rot) {
		_file << "\tinsertItemAtPosition(" << identifier << ", Vec3(" << pos.getX() << ", " << pos.getY() << ", " << pos.getZ() << "), Quaternion(" << rot.getW() << ", " << rot.getX() << ", " << rot.getY() << ", " << rot.getZ() << "))" << std::endl;
	}

	void EditorSaveObjectRPG::insertMethodEnd() {
		_file << "\treturn" << std::endl;
	}

} /* namespace plugins */
} /* namespace i6e */
