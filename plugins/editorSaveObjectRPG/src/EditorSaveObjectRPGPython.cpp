#include "EditorSaveObjectRPG.h"

namespace i6engine {
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
} /* namespace i6engine */