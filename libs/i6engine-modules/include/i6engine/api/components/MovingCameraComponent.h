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

#ifndef __I6ENGINE_API_MOVINGCAMERACOMPONENT_H__
#define __I6ENGINE_API_MOVINGCAMERACOMPONENT_H__

#include "i6engine/math/i6eVector.h"

#include "i6engine/api/components/Component.h"

namespace i6engine {
namespace api {

	/**
	 * \class MovingCameraComponent
	 * \brief Attaches a moving camera to an object.
	 * This camera will follow the object in a more sophisticated way.
	 */
	class ISIXE_MODULES_API MovingCameraComponent : public Component {
	public:
		MovingCameraComponent(const int64_t id, const attributeMap & params);
		virtual ~MovingCameraComponent();

		static ComPtr createC(const int64_t id, const attributeMap & params);

		void Init() override;

		/**
		 * \brief Sets position of the component
		 *
		 * Sets position of the component and sends message to the MessagingController
		 *
		 * \param[in] relativePosition New Position of the camera
		 */
		void setPosition(const Vec3 & pos);

		/**
		 * \brief Sets lookAt position of the component
		 *
		 * Sets position of the component and sends message to the MessagingController
		 *
		 * \param[in] lookAt Position of the camera
		 */
		void setLookAt(const Vec3 & lookAt);

		/**
		 * \brief Sets nearClipDistance of the component
		 *
		 * Sets nearClipDistance of the component and sends message to the MessagingController
		 *
		 * \param[in] new nearClipDistance
		 */
		void setNearClip(const int32_t n);

		/**
		 * \brief Sets aspect Ratio
		 *
		 * Sets aspect Ratio and sends message to the MessagingController
		 *
		 * \param[in] new ratio
		 */
		void setAspectRatio(const double ratio);

		/**
		 * \brief Sets the vertical Field of View
		 *
		 * Also sends an update message.
		 */
		void setFOVy(double fov);

		/**
		 * \brief Sets the extends of the Frustum.
		 *
		 * Also sends an update message.
		 */
		void setFrustumExtends(double left, double right, double top, double bottom);

		/**
		 * \brief Gets component's position
		 *
		 * Gets the component's position
		 *
		 * \return The position of the component
		 *
		 */
		Vec3 getRelativePosition() const {
			return _position;
		}

		void setViewportDimension(const double left, const double top, const double width, const double height);

		void setViewportBackground(const double red, const double green, const double blue, const double alpha);

		void Tick() override;

		/**
		 * \brief synchronizes the Components state
		 */
		attributeMap synchronize() override;

		std::pair<AddStrategy, int64_t> howToAdd(const ComPtr & comp) const override;

		std::string getTemplateName() const override {
			return "MovingCamera";
		}

		std::vector<componentOptions> getComponentOptions() override;

	private:
		/**
		 * \brief Position of the component
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

		void sendViewportUpdateMessage();

		/**
		 * \brief Sends a message to update the frustum extends
		 */
		void sendFrustumUpdateMessage();

		Vec3 _oldPos;
	};

} /* namespace api */
} /* namespace i6engine */

#endif /* __I6ENGINE_API_MOVINGCAMERACOMPONENT_H__ */

/**
 * @}
 */
