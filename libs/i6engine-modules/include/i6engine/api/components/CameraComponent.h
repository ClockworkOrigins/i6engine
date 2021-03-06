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
 * \addtogroup Components
 * @{
 */

#ifndef __I6ENGINE_API_CAMERACOMPONENT_H__
#define __I6ENGINE_API_CAMERACOMPONENT_H__

#include "i6engine/api/components/Component.h"
#include "i6engine/api/configs/GraphicsConfig.h"

namespace i6e {
namespace api {

	/**
	 * \class CameraComponent
	 * \brief Attaches a camera to an object. The Camera will follow the object
	 * For creating a CameraComponent, these keys are possible:
	 * | Name | Required | Type | Description | Public |
	 * |------|----------|------| ----------- | ------------ |
	 * | pos | yes | Vec3 | relative position of camera to SceneNode | yes |
	 * | lookAt | yes | Vec3 | absolute look at position in the world | yes |
	 * | nearclip | yes | double | near clipping distance | yes |
	 * | aspect | no | double | aspect ration of the camera | yes |
	 * | viewport | no | bool | defines whether this camera has a viewport or not | yes |
	 * | zOrder | *) | int | relative position on the screen depending on other viewports (has to be unique), *) required when viewport true | yes |
	 * | vp_left | *) | double | left startpoint of the viewport, value between 0 and 1, *) required when viewport true | yes |
	 * | vp_top | *) | double | uppper startpoint of the viewport, value between 0 and 1, *) required when viewport true | yes |
	 * | vp_width | *) | double | width of the viewport, value between 0 and 1, *) required when viewport true | yes |
	 * | vp_height | *) | double | height of the viewport, value between 0 and 1, *) required when viewport true | yes |
	 * | vp_red | *) | double | red part of the viewports background colour, *) required when viewport true | yes |
	 * | vp_green | *) | double | green part of the viewports background colour, *) required when viewport true | yes |
	 * | vp_blue | *) | double | blue part of the viewports background colour, *) required when viewport true | yes |
	 * | vp_alpha | *) | double | alpha part of the viewports background colour, *) required when viewport true | yes |
	 */
	class ISIXE_MODULES_API CameraComponent : public Component {
	public:
		/**
		 * \brief Constructor with attribute map
		 */
		CameraComponent(const int64_t id, const attributeMap & params);

		/**
		 * \brief Destructor
		 */
		virtual ~CameraComponent();

		/**
		 * \brief Sets position of the camera
		 * Sets position of the component relative to its parent GameObject
		 * \param[in] relativePosition New Position of the camera
		 */
		void setPosition(const Vec3 & relativePosition);

		/**
		 * \brief Sets lookAt position of the camera
		 * Sets position of the component. Just like in the \ref Init() function,
		 * this point should be in absolute world coordinates, but the viewing vector will update itself on moving the GameObject
		 * \param[in] lookAt Position of the camera
		 */
		void setLookAt(const Vec3 & lookAt);

		/**
		 * \brief Sets nearClipDistance of the camera
		 * Sets nearClipDistance of the camera. Objects closer to the camera than this distance will not be rendered
		 * \param[in] new nearClipDistance
		 */
		void setNearClip(const int32_t n);

		/**
		 * \brief Sets aspect Ratio
		 * Sets aspect Ratio. This ratio specifies the ratio between width and height of the camera
		 * \param[in] new ratio
		 */
		void setAspectRatio(const double ratio);

		/**
		 * \brief Sets the vertical Field of View
		 * This parameter specifies the opening angle of the viewing frustrum along the y-axis.
		 * \param[in] fov new Field of View
		 * \caution in difference to all other angles, this is in degrees, not radians
		 */
		void setFOVy(const double fov);

		/**
		 * \brief Sets the extends of the Frustum
		 */
		void setFrustumExtends(const double left, const double right, const double top, const double bottom);

		/**
		 * \brief sets the dimensions for the viewport used to display this camera
		 * \notice all values are relative and between 0 and 1
		 * e.g. to use only the upper right quarter of the render window, use values (0.5, 0, 0.5, 0.5)
		 */
		void setViewportDimension(const double left, const double top, const double width, const double height);

		/**
		 * \brief sets the backgroundcolor of the viewport
		 */
		void setViewportBackground(const double red, const double green, const double blue, const double alpha);

		/**
		 * \brief returns the relative position of the camera on the GameObject
		 */
		Vec3 getPosition() const {
			return _position;
		}

		/**
		 * \brief returns the position of the lookAt in world space!
		 */
		Vec3 getLookAt() const {
			return _lookAt;
		}

		/**
		 * \brief synchronizes the Components state
		 */
		attributeMap synchronize() const override;

		std::string getTemplateName() const override {
			return "Camera";
		}

		std::vector<componentOptions> getComponentOptions() override;

		/**
		 * \brief enables or disables a compositor
		 */
		void enableCompositor(const std::string & compositor, bool enabled);

		virtual std::pair<AddStrategy, int64_t> howToAdd(const ComPtr & comp) const override;

	protected:
		/**
		 * \brief Position of the camera
		 */
		Vec3 _position;

		/**
		 * \brief Point the Camera looks at
		 */
		Vec3 _lookAt;

		/**
		 * \brief nearClippingDistance
		 */
		int32_t _nearClip;

		/**
		 * aspect Ration, Ogre-default: 4 : 3
		 * Width / Height
		 */
		double _aspect;

		int _zOrder;

		bool _viewport;

		double _left;
		double _top;
		double _width;
		double _height;
		double _red;
		double _green;
		double _blue;
		double _alpha;
		double _fov;
		double _frustumLeft;
		double _frustumRight;
		double _frustumTop;
		double _frustumBottom;

		/**
		 * \brief initializes the component
		 */
		void Init() override;

		/**
		 * \brief Sends message to MessagingController
		 *
		 * Sends message to MessagingController containing ObjectID, component and position
		 */
		void sendCameraUpdateMessage();

		/**
		 * \brief sends an update message containing the new viewport values
		 */
		void sendViewportUpdateMessage();

		/**
		 * \brief Sends a message to update the frustum extends
		 */
		void sendFrustumUpdateMessage();
	};

} /* namespace api */
} /* namespace i6e */

#endif /* __I6ENGINE_API_CAMERACOMPONENT_H__ */

/**
 * @}
 */
