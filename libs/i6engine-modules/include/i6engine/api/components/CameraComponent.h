/**
 * Copyright 2012 FAU (Friedrich Alexander University of Erlangen-Nuremberg)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * \addtogroup Components
 * @{
 */

#ifndef __I6ENGINE_API_CAMERACOMPONENT_H__
#define __I6ENGINE_API_CAMERACOMPONENT_H__

#include "i6engine/api/components/Component.h"
#include "i6engine/api/configs/GraphicsConfig.h"

namespace i6engine {
namespace api {

	/**
	 * \class CameraComponent
	 * \brief Attaches a camera to an object. The Camera will follow the object
	 */
	class ISIXE_MODULES_API CameraComponent : public Component {
	public:
		/**
		 * \brief Constructor with attribute map
		 */
		explicit CameraComponent(const int64_t id, const attributeMap & params);

		/**
		 * \brief Destructor
		 */
		virtual ~CameraComponent();

		static ComPtr createC(int64_t id, const attributeMap & params);

		/**
		 * \brief initializes the component
		 * \info don't use this method at the moment, because all properties will be random
		 */
		void Init() override;

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

		virtual std::pair<AddStrategy, int64_t> howToAdd(const ComPtr & comp) const override;

		std::string getTemplateName() const override {
			return "Camera";
		}

		std::vector<componentOptions> getComponentOptions() override;

		void enableCompositor(const std::string & compositor, bool enabled);

	private:
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
} /* namespace i6engine */

#endif /* __I6ENGINE_API_CAMERACOMPONENT_H__ */

/**
 * @}
 */
