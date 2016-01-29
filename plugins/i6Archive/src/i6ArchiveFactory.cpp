#include "i6ArchiveFactory.h"

#include "i6Archive.h"

namespace i6engine {
namespace plugins {

	Ogre::Archive * i6ArchiveFactory::createInstance(const Ogre::String & name, bool readOnly) {
		return OGRE_NEW i6Archive(name, "i6arch");
	}

	void i6ArchiveFactory::destroyInstance(Ogre::Archive * archive) {
		OGRE_DELETE archive;
	}

} /* namespace plugins */
} /* namespace i6engine */
