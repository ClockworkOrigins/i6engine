#include "i6Archive.h"

namespace i6engine {
namespace plugins {

	void i6Archive::load() {
		std::cout << "load" << std::endl;
		// TODO
	}

	void i6Archive::unload() {
		std::cout << "unload" << std::endl;
		// TODO
	}

	Ogre::DataStreamPtr i6Archive::open(const Ogre::String & filename, bool readOnly) const {
		std::cout << "open" << std::endl;
		// TODO
		return Ogre::DataStreamPtr();
	}

	Ogre::StringVectorPtr i6Archive::list(bool recursive, bool dirs) {
		std::cout << "list" << std::endl;
		Ogre::StringVectorPtr ret(OGRE_NEW_T(Ogre::StringVector, Ogre::MEMCATEGORY_GENERAL)(), Ogre::SPFM_DELETE_T);
		// TODO
		return Ogre::StringVectorPtr();
	}

	Ogre::FileInfoListPtr i6Archive::listFileInfo(bool recursive, bool dirs) {
		std::cout << "listFileInfo" << std::endl;
		Ogre::FileInfoListPtr ret(OGRE_NEW_T(Ogre::FileInfoList, Ogre::MEMCATEGORY_GENERAL)(), Ogre::SPFM_DELETE_T);
		// TODO
		return ret;
	}

	Ogre::StringVectorPtr i6Archive::find(const Ogre::String & pattern, bool recursive, bool dirs) {
		std::cout << "find" << std::endl;
		Ogre::StringVectorPtr ret(OGRE_NEW_T(Ogre::StringVector, Ogre::MEMCATEGORY_GENERAL)(), Ogre::SPFM_DELETE_T);
		// TODO
		return ret;
	}

	bool i6Archive::exists(const Ogre::String & filename) {
		std::cout << "exists" << std::endl;
		// TODO
		return false;
	}

	Ogre::FileInfoListPtr i6Archive::findFileInfo(const Ogre::String & pattern, bool recursive, bool dirs) const {
		std::cout << "findFileInfo" << std::endl;
		Ogre::FileInfoListPtr ret(OGRE_NEW_T(Ogre::FileInfoList, Ogre::MEMCATEGORY_GENERAL)(), Ogre::SPFM_DELETE_T);
		// TODO
		return ret;
	}

} /* namespace plugins */
} /* namespace i6engine */
