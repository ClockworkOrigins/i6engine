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

namespace i6e {
namespace modules {

	class MovableText : public Ogre::MovableObject, public Ogre::Renderable {
	public:
		enum HorizontalAlignment {
			H_LEFT, H_CENTER
		};
		enum VerticalAlignment {
			V_BELOW, V_ABOVE, V_CENTER
		};

	public:
		MovableText(const Ogre::String & name, const Ogre::String & caption, const Ogre::String & fontName, Ogre::Real charHeight, const Ogre::ColourValue & color);
		~MovableText();

		// Add to build on Shoggoth:
		void visitRenderables(Ogre::Renderable::Visitor * visitor, bool debugRenderables = false);

		// Set settings
		void setFontName(const Ogre::String & fontName);
		void setCaption(const Ogre::String & caption);
		void setColor(const Ogre::ColourValue &color);
		void setCharacterHeight(Ogre::Real height);
		void setSpaceWidth(Ogre::Real width);
		void setTextAlignment(const HorizontalAlignment & horizontalAlignment, const VerticalAlignment & verticalAlignment);
		void setGlobalTranslation(Ogre::Vector3 trans);
		void setLocalTranslation(Ogre::Vector3 trans);
		void showOnTop(bool show = true);

		// Get settings
		const Ogre::String &  getFontName() const {
			return _fontName;
		}

		const Ogre::String &  getCaption() const {
			return _caption;
		}

		const Ogre::ColourValue & getColor() const {
			return _color;
		}

		Ogre::Real getCharacterHeight() const {
			return _charHeight;
		}

		Ogre::Real getSpaceWidth() const {
			return _spaceWidth;
		}

		Ogre::Vector3 getGlobalTranslation() const {
			return _globalTranslation;
		}

		Ogre::Vector3 getLocalTranslation() const {
			return _localTranslation;
		}

		bool getShowOnTop() const {
			return _onTop;
		}

		Ogre::AxisAlignedBox GetAABB() {
			return _AABB;
		}

	private:
		Ogre::String _fontName;
		Ogre::String _type;
		Ogre::String _name;
		Ogre::String _caption;
		HorizontalAlignment _horizontalAlignment;
		VerticalAlignment _verticalAlignment;

		Ogre::ColourValue _color;
		Ogre::RenderOperation _renderOp;
		Ogre::AxisAlignedBox _AABB;
		Ogre::LightList _lightList;

		Ogre::Real _charHeight;
		Ogre::Real _spaceWidth;

		bool _needUpdate;
		bool _updateColours;
		bool _onTop;

		Ogre::Real _timeUntilNextToggle;
		Ogre::Real _radius;

		Ogre::Vector3 _globalTranslation;
		Ogre::Vector3 _localTranslation;

		Ogre::Camera * _camera;
		Ogre::RenderWindow * _renderWindow;
		Ogre::Font * _font;
		Ogre::MaterialPtr _material;
		Ogre::MaterialPtr _backgroundMaterial;

		// from MovableText, create the object
		void _setupGeometry();
		void _updateColors();

		// from MovableObject
		void getWorldTransforms(Ogre::Matrix4 * xform) const;

		Ogre::Real getBoundingRadius() const {
			return _radius;
		}

		Ogre::Real getSquaredViewDepth(const Ogre::Camera * cam) const {
			return 0.0;
		}

		const Ogre::Quaternion & getWorldOrientation() const;

		const Ogre::Vector3 & getWorldPosition() const;

		const Ogre::AxisAlignedBox & getBoundingBox() const {
			return _AABB;
		}

		const Ogre::String & getName() const {
			return _name;
		}

		const Ogre::String & getMovableType() const {
			static Ogre::String movType = "MovableText";
			return movType;
		}

		void _notifyCurrentCamera(Ogre::Camera * cam);
		void _updateRenderQueue(Ogre::RenderQueue * queue);

		// from renderable
		void getRenderOperation(Ogre::RenderOperation & op);

		const Ogre::MaterialPtr & getMaterial() const {
			assert(!_material.isNull());
			return _material;
		}

		const Ogre::LightList & getLights() const {
			return _lightList;
		}
	};

} /* namespace modules */
} /* namespace i6e */

#endif /* __I6ENGINE_MODULES_GRAPHICS_MOVABLETEXT_H__ */

/**
 * @}
 */
