/**
 * \addtogroup Graphics
 * @{
 */

#ifndef __I6ENGINE_MODULES_GRAPHICS_MOVABLETEXT_H__
#define __I6ENGINE_MODULES_GRAPHICS_MOVABLETEXT_H__

#include "i6engine/math/i6eVector.h"

#include "OGRE/Ogre.h"
#include "OGRE/Overlay/OgreOverlay.h"
#include "OGRE/Overlay/OgreOverlayContainer.h"

namespace i6engine {
namespace modules {

	class MovableText {
	public:
		static Ogre::Overlay * overlay;

		/**
		 * \brief constructor
		 */
		MovableText(Ogre::MovableObject * p, Ogre::SceneManager * s, const std::string & font);

		/**
		 * \brief destructor
		 */
		~MovableText();

		/**
		 * \brief enables or disables movable text
		 */
		void enable(bool b) {
			_enabled = b;
			if (b) {
				_container->show();
			} else {
				_container->hide();
			}
		}

		/**
		 * \brief sets text
		 */
		void setText(const Ogre::String & text) {
			_text = text;
			_pText->setCaption(_text);
		}

		/**
		 * \brief sets size
		 */
		void setSize(uint16_t size) {
			_pText->setParameter("char_height", std::to_string(size));
		}

		/**
		 * \brief sets colour
		 */
		void setColour(const Vec3 & colour) {
			_pText->setColour(Ogre::ColourValue(colour.getX(), colour.getY(), colour.getZ()));
		}

		/**
		 * \brief updates position and visibility of this movable text
		 * call this on every update of the graphics subsystem
		 */
		void update();

	protected:
		Ogre::MovableObject * _object;
		Ogre::SceneManager * _sceneManager;
		bool _enabled;
		Ogre::OverlayElement * _pText;
		Ogre::String _text;
		Ogre::OverlayContainer* _container;
	};

} /* namespace modules */
} /* namespace i6engine */

#endif /* __I6ENGINE_MODULES_GRAPHICS_MOVABLETEXT_H__ */

/**
 * @}
 */
