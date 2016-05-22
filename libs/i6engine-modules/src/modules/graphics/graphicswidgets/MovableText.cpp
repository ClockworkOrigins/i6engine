/**
 * File: MovableText.cpp
 *
 * description: This create create a billboarding object that display a text.
 *
 * @author	2003 by cTh see gavocanov@rambler.ru
 * @update	2006 by barraq see nospam@barraquand.com
 * @update	2012 to work with newer versions of OGRE by MindCalamity <mindcalamity@gmail.com>
 *	- See "Notes" on: http://www.ogre3d.org/tikiwiki/tiki-editpage.php?page=MovableText
 */

#include "i6engine/modules/graphics/graphicswidgets/MovableText.h"

#include "OGRE/Overlay/OgreOverlayManager.h"

#include "OGRE/Overlay/OgreFontManager.h"

namespace i6e {
namespace modules {

#define POS_TEX_BINDING    0
#define COLOUR_BINDING     1

	MovableText::MovableText(const Ogre::String & name, const Ogre::String & caption, const Ogre::String & fontName, Ogre::Real charHeight, const Ogre::ColourValue & color): _camera(nullptr), _renderWindow(nullptr), _font(nullptr), _name(name), _caption(caption), _fontName(fontName), _charHeight(charHeight), _color(color), _type("MovableText"), _timeUntilNextToggle(0), _spaceWidth(0), _updateColours(true), _onTop(false), _horizontalAlignment(H_CENTER), _verticalAlignment(V_ABOVE), _globalTranslation(0.0), _localTranslation(0.0) {
		assert(name != "" && "Trying to create MovableText without name");
		assert(caption != "" && "Trying to create MovableText without caption");

		_renderOp.vertexData = nullptr;
		setFontName(_fontName);
		_setupGeometry();
	}

	MovableText::~MovableText() {
		if (_renderOp.vertexData) {
			delete _renderOp.vertexData;
		}
		// May cause crashing... check this and comment if it does
		if (!_material.isNull()) {
			Ogre::MaterialManager::getSingletonPtr()->remove(_material->getName());
		}
	}

	void MovableText::setFontName(const Ogre::String & fontName) {
		if (Ogre::MaterialManager::getSingletonPtr()->resourceExists(_name + "Material")) {
			Ogre::MaterialManager::getSingleton().remove(_name + "Material");
		}

		if (_fontName != fontName || _material.isNull() || !_font) {
			_fontName = fontName;

			_font = Ogre::FontManager::getSingleton().getByName(_fontName).getPointer();
			if (!_font) {
				throw Ogre::Exception(Ogre::Exception::ERR_ITEM_NOT_FOUND, "Could not find font " + fontName, "MovableText::setFontName");
			}

			_font->load();
			if (!_material.isNull()) {
				Ogre::MaterialManager::getSingletonPtr()->remove(_material->getName());
				_material.setNull();
			}

			_material = _font->getMaterial()->clone(_name + "Material");
			if (!_material->isLoaded()) {
				_material->load();
			}

			_material->setDepthCheckEnabled(!_onTop);
			_material->setDepthBias(1.0, 1.0);
			_material->setDepthWriteEnabled(_onTop);
			_material->setLightingEnabled(false);
			_needUpdate = true;
		}
	}

	void MovableText::setCaption(const Ogre::String & caption) {
		if (caption != _caption) {
			_caption = caption;
			_needUpdate = true;
		}
	}

	void MovableText::setColor(const Ogre::ColourValue & color) {
		if (color != _color) {
			_color = color;
			_updateColours = true;
		}
	}

	void MovableText::setCharacterHeight(Ogre::Real height) {
		if (height != _charHeight) {
			_charHeight = height;
			_needUpdate = true;
		}
	}

	void MovableText::setSpaceWidth(Ogre::Real width) {
		if (width != _spaceWidth) {
			_spaceWidth = width;
			_needUpdate = true;
		}
	}

	void MovableText::setTextAlignment(const HorizontalAlignment & horizontalAlignment, const VerticalAlignment & verticalAlignment) {
		if (_horizontalAlignment != horizontalAlignment) {
			_horizontalAlignment = horizontalAlignment;
			_needUpdate = true;
		}
		if (_verticalAlignment != verticalAlignment) {
			_verticalAlignment = verticalAlignment;
			_needUpdate = true;
		}
	}

	void MovableText::setGlobalTranslation(Ogre::Vector3 trans) {
		_globalTranslation = trans;
	}

	void MovableText::setLocalTranslation(Ogre::Vector3 trans) {
		_localTranslation = trans;
	}

	void MovableText::showOnTop(bool show) {
		if (_onTop != show && !_material.isNull()) {
			_onTop = show;
			_material->setDepthBias(1.0, 1.0);
			_material->setDepthCheckEnabled(!_onTop);
			_material->setDepthWriteEnabled(_onTop);
		}
	}

	void MovableText::_setupGeometry() {
		assert(_font);
		assert(!_material.isNull());

		unsigned int vertexCount = static_cast<unsigned int>(_caption.size() * 6);

		if (_renderOp.vertexData) {
			// Removed this test as it causes problems when replacing a caption
			// of the same size: replacing "Hello" with "hello"
			// as well as when changing the text alignment
			//if (_renderOp.vertexData->vertexCount != vertexCount)
			{
				delete _renderOp.vertexData;
				_renderOp.vertexData = nullptr;
				_updateColours = true;
			}
		}

		if (!_renderOp.vertexData) {
			_renderOp.vertexData = new Ogre::VertexData();
		}

		_renderOp.indexData = 0;
		_renderOp.vertexData->vertexStart = 0;
		_renderOp.vertexData->vertexCount = vertexCount;
		_renderOp.operationType = Ogre::RenderOperation::OT_TRIANGLE_LIST;
		_renderOp.useIndexes = false;

		Ogre::VertexDeclaration * decl = _renderOp.vertexData->vertexDeclaration;
		Ogre::VertexBufferBinding * bind = _renderOp.vertexData->vertexBufferBinding;
		size_t offset = 0;

		// create/bind positions/tex.ccord. buffer
		if (!decl->findElementBySemantic(Ogre::VES_POSITION)) {
			decl->addElement(POS_TEX_BINDING, offset, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
		}

		offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);

		if (!decl->findElementBySemantic(Ogre::VES_TEXTURE_COORDINATES)) {
			decl->addElement(POS_TEX_BINDING, offset, Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES, 0);
		}

		Ogre::HardwareVertexBufferSharedPtr ptbuf = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(decl->getVertexSize(POS_TEX_BINDING), _renderOp.vertexData->vertexCount, Ogre::HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY);
		bind->setBinding(POS_TEX_BINDING, ptbuf);

		// Colours - store these in a separate buffer because they change less often
		if (!decl->findElementBySemantic(Ogre::VES_DIFFUSE)) {
			decl->addElement(COLOUR_BINDING, 0, Ogre::VET_COLOUR, Ogre::VES_DIFFUSE);
		}

		Ogre::HardwareVertexBufferSharedPtr cbuf = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(decl->getVertexSize(COLOUR_BINDING), _renderOp.vertexData->vertexCount, Ogre::HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY);
		bind->setBinding(COLOUR_BINDING, cbuf);

		float * pPCBuff = static_cast<float *>(ptbuf->lock(Ogre::HardwareBuffer::HBL_DISCARD));

		float largestWidth = 0.0f;
		float left = 0 * 2.0f - 1.0f;
		float top = -((0 * 2.0f) - 1.0f);

		Ogre::Real spaceWidth = _spaceWidth;
		// Derive space width from a capital A
		if (spaceWidth == 0) {
			spaceWidth = _font->getGlyphAspectRatio('A') * _charHeight * 2.0;
		}

		// for calculation of AABB
		Ogre::Vector3 min, max, currPos;
		Ogre::Real maxSquaredRadius;
		bool first = true;

		// Use iterator
		Ogre::String::iterator i, iend;
		iend = _caption.end();
		bool newLine = true;
		Ogre::Real len = 0.0;

		Ogre::Real verticalOffset = 0;
		switch (_verticalAlignment) {
		case MovableText::V_ABOVE:
			verticalOffset = _charHeight;
			break;
		case MovableText::V_CENTER:
			verticalOffset = 0.5*_charHeight;
			break;
		case MovableText::V_BELOW:
			verticalOffset = 0;
			break;
		}
		// Raise the first line of the caption
		top += verticalOffset;
		for (i = _caption.begin(); i != iend; ++i) {
			if (*i == '\n')
				top += verticalOffset * 2.0;
		}

		for (i = _caption.begin(); i != iend; ++i) {
			if (newLine) {
				len = 0.0f;
				for (Ogre::String::iterator j = i; j != iend && *j != '\n'; j++) {
					if (*j == ' ')
						len += spaceWidth;
					else
						len += _font->getGlyphAspectRatio((unsigned char) *j) * _charHeight * 2.0;
				}
				newLine = false;
			}

			if (*i == '\n') {
				left = 0 * 2.0 - 1.0;
				top -= _charHeight * 2.0;
				newLine = true;

				// Bugfix by Wladimir Lukutin - thanks :)
				// Also reduce tri count
				_renderOp.vertexData->vertexCount -= 6;
				// Bugfix end.

				continue;
			}

			if (*i == ' ') {
				// Just leave a gap, no tris
				left += spaceWidth;
				// Also reduce tri count
				_renderOp.vertexData->vertexCount -= 6;
				continue;
			}

			Ogre::Real horiz_height = _font->getGlyphAspectRatio((unsigned char) *i);
			Ogre::Real u1, u2, v1, v2;
			Ogre::Font::UVRect utmp;
			utmp = _font->getGlyphTexCoords((unsigned char) *i);
			u1 = utmp.left;
			u2 = utmp.right;
			v1 = utmp.top;
			v2 = utmp.bottom;

			// each vert is (x, y, z, u, v)
			//-------------------------------------------------------------------------------------
			// First tri
			//
			// Upper left
			if (_horizontalAlignment == MovableText::H_LEFT) {
				*pPCBuff++ = left;
			} else {
				*pPCBuff++ = left - (len / 2);
			}
			*pPCBuff++ = top;
			*pPCBuff++ = -1.0;
			*pPCBuff++ = u1;
			*pPCBuff++ = v1;

			// Deal with bounds
			if (_horizontalAlignment == MovableText::H_LEFT) {
				currPos = Ogre::Vector3(left, top, -1.0);
			} else {
				currPos = Ogre::Vector3(left - (len / 2), top, -1.0);
			}
			if (first) {
				min = max = currPos;
				maxSquaredRadius = currPos.squaredLength();
				first = false;
			} else {
				min.makeFloor(currPos);
				max.makeCeil(currPos);
				maxSquaredRadius = std::max(maxSquaredRadius, currPos.squaredLength());
			}

			top -= _charHeight * 2.0;

			// Bottom left
			if (_horizontalAlignment == MovableText::H_LEFT) {
				*pPCBuff++ = left;
			} else {
				*pPCBuff++ = left - (len / 2);
			}
			*pPCBuff++ = top;
			*pPCBuff++ = -1.0;
			*pPCBuff++ = u1;
			*pPCBuff++ = v2;

			// Deal with bounds
			if (_horizontalAlignment == MovableText::H_LEFT) {
				currPos = Ogre::Vector3(left, top, -1.0);
			} else {
				currPos = Ogre::Vector3(left - (len / 2), top, -1.0);
			}
			min.makeFloor(currPos);
			max.makeCeil(currPos);
			maxSquaredRadius = std::max(maxSquaredRadius, currPos.squaredLength());

			top += _charHeight * 2.0;
			left += horiz_height * _charHeight * 2.0;

			// Top right
			if (_horizontalAlignment == MovableText::H_LEFT) {
				*pPCBuff++ = left;
			} else {
				*pPCBuff++ = left - (len / 2);
			}
			*pPCBuff++ = top;
			*pPCBuff++ = -1.0;
			*pPCBuff++ = u2;
			*pPCBuff++ = v1;
			//-------------------------------------------------------------------------------------

			// Deal with bounds
			if (_horizontalAlignment == MovableText::H_LEFT) {
				currPos = Ogre::Vector3(left, top, -1.0);
			} else {
				currPos = Ogre::Vector3(left - (len / 2), top, -1.0);
			}
			min.makeFloor(currPos);
			max.makeCeil(currPos);
			maxSquaredRadius = std::max(maxSquaredRadius, currPos.squaredLength());

			//-------------------------------------------------------------------------------------
			// Second tri
			//
			// Top right (again)
			if (_horizontalAlignment == MovableText::H_LEFT) {
				*pPCBuff++ = left;
			} else {
				*pPCBuff++ = left - (len / 2);
			}
			*pPCBuff++ = top;
			*pPCBuff++ = -1.0;
			*pPCBuff++ = u2;
			*pPCBuff++ = v1;

			currPos = Ogre::Vector3(left, top, -1.0);
			min.makeFloor(currPos);
			max.makeCeil(currPos);
			maxSquaredRadius = std::max(maxSquaredRadius, currPos.squaredLength());

			top -= _charHeight * 2.0;
			left -= horiz_height  * _charHeight * 2.0;

			// Bottom left (again)
			if (_horizontalAlignment == MovableText::H_LEFT) {
				*pPCBuff++ = left;
			} else {
				*pPCBuff++ = left - (len / 2);
			}
			*pPCBuff++ = top;
			*pPCBuff++ = -1.0;
			*pPCBuff++ = u1;
			*pPCBuff++ = v2;

			currPos = Ogre::Vector3(left, top, -1.0);
			min.makeFloor(currPos);
			max.makeCeil(currPos);
			maxSquaredRadius = std::max(maxSquaredRadius, currPos.squaredLength());

			left += horiz_height  * _charHeight * 2.0;

			// Bottom right
			if (_horizontalAlignment == MovableText::H_LEFT) {
				*pPCBuff++ = left;
			} else {
				*pPCBuff++ = left - (len / 2);
			}
			*pPCBuff++ = top;
			*pPCBuff++ = -1.0;
			*pPCBuff++ = u2;
			*pPCBuff++ = v2;
			//-------------------------------------------------------------------------------------

			currPos = Ogre::Vector3(left, top, -1.0);
			min.makeFloor(currPos);
			max.makeCeil(currPos);
			maxSquaredRadius = std::max(maxSquaredRadius, currPos.squaredLength());

			// Go back up with top
			top += _charHeight * 2.0;

			float currentWidth = (left + 1) / 2 - 0;
			if (currentWidth > largestWidth) {
				largestWidth = currentWidth;
			}
		}

		// Unlock vertex buffer
		ptbuf->unlock();

		// update AABB/Sphere radius
		_AABB = Ogre::AxisAlignedBox(min, max);
		_radius = Ogre::Math::Sqrt(maxSquaredRadius);

		if (_updateColours) {
			_updateColors();
		}

		_needUpdate = false;
	}

	void MovableText::_updateColors() {
		assert(_font);
		assert(!_material.isNull());

		// Convert to system-specific
		Ogre::RGBA color;
		Ogre::Root::getSingleton().convertColourValue(_color, &color);
		Ogre::HardwareVertexBufferSharedPtr vbuf = _renderOp.vertexData->vertexBufferBinding->getBuffer(COLOUR_BINDING);
		Ogre::RGBA * pDest = static_cast<Ogre::RGBA *>(vbuf->lock(Ogre::HardwareBuffer::HBL_DISCARD));
		for (int i = 0; i < (int) _renderOp.vertexData->vertexCount; ++i) {
			*pDest++ = color;
		}
		vbuf->unlock();
		_updateColours = false;
	}

	const Ogre::Quaternion & MovableText::getWorldOrientation() const {
		assert(_camera);
		return const_cast<Ogre::Quaternion &>(_camera->getDerivedOrientation());
	}

	// Add to build on Shoggoth:
	void MovableText::visitRenderables(Ogre::Renderable::Visitor * visitor, bool debugRenderables) {
	}

	const Ogre::Vector3 & MovableText::getWorldPosition() const {
		assert(mParentNode);
		return mParentNode->_getDerivedPosition();
	}

	void MovableText::getWorldTransforms(Ogre::Matrix4 * xform) const {
		if (isVisible() && _camera) {
			Ogre::Matrix3 rot3x3, scale3x3 = Ogre::Matrix3::IDENTITY;

			// store rotation in a matrix
			_camera->getDerivedOrientation().ToRotationMatrix(rot3x3);

			// parent node position
			Ogre::Vector3 ppos = mParentNode->_getDerivedPosition() + Ogre::Vector3::UNIT_Y * _globalTranslation;
			ppos += rot3x3 * _localTranslation;

			// apply scale
			scale3x3[0][0] = mParentNode->_getDerivedScale().x / 2;
			scale3x3[1][1] = mParentNode->_getDerivedScale().y / 2;
			scale3x3[2][2] = mParentNode->_getDerivedScale().z / 2;

			// apply all transforms to xform       
			*xform = (rot3x3 * scale3x3);
			xform->setTrans(ppos);
		}
	}

	void MovableText::getRenderOperation(Ogre::RenderOperation & op) {
		if (isVisible()) {
			if (_needUpdate) {
				_setupGeometry();
			}
			if (_updateColours) {
				_updateColors();
			}
			op = _renderOp;
		}
	}

	void MovableText::_notifyCurrentCamera(Ogre::Camera * cam) {
		_camera = cam;
	}

	void MovableText::_updateRenderQueue(Ogre::RenderQueue * queue) {
		if (isVisible()) {
			if (_needUpdate) {
				_setupGeometry();
			}
			if (_updateColours) {
				_updateColors();
			}

			queue->addRenderable(this, mRenderQueueID, OGRE_RENDERABLE_DEFAULT_PRIORITY);
			//queue->addRenderable(this, mRenderQueueID, RENDER_QUEUE_SKIES_LATE);
		}
	}

} /* namespace modules */
} /* namespace i6e */
