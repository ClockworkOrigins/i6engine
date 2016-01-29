#ifndef __I6ENGINE_PLUGINS_I6ARCHIVEFACTORY_H__
#define __I6ENGINE_PLUGINS_I6ARCHIVEFACTORY_H__

#include "OGRE/OgreArchiveFactory.h"

namespace i6engine {
namespace plugins {

	/**
	 * \class i6ArchiveFactory factory to produce i6Archives
	 */
	class i6ArchiveFactory : public Ogre::ArchiveFactory {
	public:
		i6ArchiveFactory() : ArchiveFactory(), _type("i6arch") {
		}

	private:
		const Ogre::String _type;
        /**
		 * \brief Creates a new object.
         * \param name Name of the object to create
         * \return An object created by the factory. The type of the object depends on the factory. In this case it's an i6Archive
         */
		Ogre::Archive * createInstance(const Ogre::String & name, bool readOnly) override;

		/**
		 * \brief returns the type this factory supports
		 * here it is i6arch
		 */
		const Ogre::String & getType() const override {
			return _type;
		}

		void destroyInstance(Ogre::Archive * archive) override;
	};

} /* namespace plugins */
} /* namespace i6engine */

#endif /* __I6ENGINE_PLUGINS_I6ARCHIVEFACTORY_H__ */
