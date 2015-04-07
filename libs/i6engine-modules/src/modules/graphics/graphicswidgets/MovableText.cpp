#include "i6engine/modules/graphics/graphicswidgets/MovableText.h"

namespace i6engine {
namespace modules {

	Ogre::Overlay * MovableText::overlay = nullptr;

	MovableText::MovableText(Ogre::MovableObject * p, Ogre::SceneManager * s, const std::string & font) : _object(p), _sceneManager(s), _enabled(false), _pText(), _text() {
		// create an overlay that we can use for later
		if (overlay == nullptr) {
			overlay = Ogre::OverlayManager::getSingleton().create("floatingTextOverlay");
			overlay->show();
		}

		_container = dynamic_cast<Ogre::OverlayContainer *>(Ogre::OverlayManager::getSingleton().createOverlayElement("Panel", "c_" + p->getName()));

		overlay->add2D(_container);

		_pText = Ogre::OverlayManager::getSingleton().createOverlayElement("TextArea", "ct_" + p->getName());
		_pText->setDimensions(1.0, 1.0);
		_pText->setMetricsMode(Ogre::GMM_PIXELS);
		_pText->setPosition(0.0, 0.0);

		_pText->setParameter("font_name", font);
		_pText->setParameter("char_height", "16");
		_pText->setParameter("horz_align", "center");
		_pText->setColour(Ogre::ColourValue(1.0, 1.0, 1.0));

		_container->addChild(_pText);
		_container->setEnabled(_enabled);
	}

	MovableText::~MovableText() {
		// overlay cleanup -- Ogre would clean this up at app exit but if your app 
		// tends to create and delete these objects often it's a good idea to do it here.
		Ogre::OverlayManager * overlayManager = Ogre::OverlayManager::getSingletonPtr();
		_container->removeChild("ct_" + _object->getName());
		overlay->remove2D(_container);
		overlayManager->destroyOverlayElement(_pText);
		overlayManager->destroyOverlayElement(_container);
	}

	void MovableText::update() {
		if (!_enabled) {
			return;
		}
		if (!_sceneManager->getCurrentViewport()) {
			return;
		}
		const Ogre::AxisAlignedBox & bbox = _object->getWorldBoundingBox(true);
		Ogre::Matrix4 mat = _sceneManager->getCurrentViewport()->getCamera()->getViewMatrix();

		// We want to put the text point in the center of the top of the AABB Box.
		Ogre::Vector3 topcenter = bbox.getCenter();
		// Y is up.
		topcenter.y += bbox.getHalfSize().y;
		topcenter = mat * topcenter;
		// We are now in screen pixel coords and depth is +Z away from the viewer.
		if (topcenter.z > 0.0) {
			// Don't show text for objects behind the camera.
			_container->setPosition(-1000, -1000);
		} else {
			// Not in pixel coordinates, in screen coordinates as described above.
			// We convert to screen relative coord by knowing the window size.
			// Top left screen corner is 0, 0 and the bottom right is 1,1
			// The 0.45's here offset alittle up and right for better "text above head" positioning.
			// The 2.2 and 1.7 compensate for some strangeness in Ogre projection?
			// Tested in wide screen and normal aspect ratio.
			_container->setPosition(0.45 - topcenter.x / topcenter.z, topcenter.y / (topcenter.z) + 0.45);
			// Sizse is relative to screen size being 1.0 by 1.0 (not pixels size)
			_container->setDimensions(0.1, 0.1);
		}
	}

} /* namespace modules */
} /* namespace i6engine */
