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

#ifndef __I6ENGINE_API_LUMINOUSAPPEARANCECOMPONENT_H_
#define __I6ENGINE_API_LUMINOUSAPPEARANCECOMPONENT_H_

#include "i6engine/math/i6eVector.h"
#include "i6engine/math/i6eVector4.h"

#include "i6engine/api/components/Component.h"

namespace i6engine {
namespace api {

	/**
	 * \class LuminousAppearanceComponent
	 * \brief Tells the engine that this GameObject is emitting light.
	 */
	class ISIXE_MODULES_API LuminousAppearanceComponent : public Component {
	public:
		/**
		 * \brief type of the light emitted
		 */
		enum class LightType : uint8_t {
			POINT = 0,		//!< Point light (e.g. a light bulb or torch)
			DIRECTIONAL = 1,	//!< Directional Light (e.g. the sun)
			SPOT = 2		//!< Spot light (e.g. a flashlight)
		};

		/**
		 * \brief Constructor sets ID and component
		 */
		LuminousAppearanceComponent(const int64_t id, const attributeMap & params);

		virtual ~LuminousAppearanceComponent();

		/**
		 * \brief creates the Component with given attributeMap
		 */
		static ComPtr createC(const int64_t id, const attributeMap & params);

		void Init() override;

		/**
		 * \brief Sets light type
		 *
		 * Sets the light type of the component and sends update message
		 *
		 * \param[in] lightType New light type
		 */
		void setLightType(const LightType lightType);

		/**
		 * \brief Gets component's light type
		 *
		 * Gets component's light type
		 *
		 * \return Light type of the component
		 *
		 */
		inline LightType getLightType() const { return _lightType; }

		/**
		 * \brief Sets diffuse colour
		 *
		 * Sets diffuse colour and sends update message
		 *
		 * \param[in] diffuseColor New diffuse colour of the light
		 */
		void setDiffuseColor(const Vec3 & diffuseColor);

		/**
		 * \brief Gets diffuse colour
		 *
		 * Gets diffuse colour
		 *
		 * \return Diffuse colour of the component
		 */
		inline Vec3 getDiffuseColor() const { return _diffuseColor; }

		/**
		 * \brief Sets specular colour
		 *
		 * Sets specular colour and sends update message
		 *
		 * \param[in] specular New specular colour of the component
		 */
		void setSpecularColor(const Vec3 & specular);

		/**
		 * \brief Gets specular colour
		 *
		 * Gets specular colour
		 *
		 * \return Specular colour of the component
		 */
		inline Vec3 getSpecularColor() const { return _specularColor; }

		/**
		 * \brief Sets attenuation
		 *
		 * Sets attenuation and sends update message
		 *
		 * \param[in] attenuation New attenuation of the component
		 */
		void setAttenuation(const Vec4 & attenuation);

		/**
		 * \brief Gets attenuation
		 *
		 * Gets attenuation
		 *
		 * \return Attenuation of the component
		 */
		inline Vec4 getAttenuation() const { return _attenuation; }

		/**
		 * \brief Sets direction
		 *
		 * Sets direction and sends update message
		 *
		 * \param[in] direction New direction of the component
		 */
		void setDirection(const Vec3 & direction);

		/**
		 * \brief Gets direction
		 *
		 * Gets direction
		 *
		 * \return Direction of the component
		 */
		inline Vec3 getDirection() const { return _direction; }

		/**
		 * \brief synchronizes the Components state
		 */
		attributeMap synchronize() override;

		virtual std::pair<AddStrategy, int64_t> howToAdd(const ComPtr & comp) const override;

		std::string getTemplateName() const override {
			return "LuminousAppearance";
		}

		std::vector<componentOptions> getComponentOptions() override;

	private:
		/**
		 * Light type of the component
		 */
		LightType _lightType;

		/**
		 * Diffuse colour of the component
		 */
		Vec3 _diffuseColor;

		/**
		 * Specular colour of the component
		 */
		Vec3 _specularColor;

		/**
		 * Attenuation of the component
		 * W = range (range in world units)
		 * X = constant (1.0 never attenuate, 0.0 complete attenuation)
		 * Y = linear (linear attenuation depending on distance)
		 * Z = quadratic
		 */
		Vec4 _attenuation;

		/**
		 * Direction of the component
		 */
		Vec3 _direction;

		/**
		 * Relative position of this component
		 */
		Vec3 _position;

		/**
		 * Range of the inner cone of a spotlight
		 * in degree, whole angle
		 */
		double _spotlightRangeInner;

		/**
		 * Range of the outer cone of a spotlight
		 * in degree, whole angle
		 */
		double _spotlightRangeOuter;

		/**
		 * \brief Sends update message to the MessagingController
		 *
		 * Sends update message to the MessagingController containing ObjectID, component and the light attributes
		 */
		void sendUpdateMessage();
	};

} /* namespace api */
} /* namespace i6engine */

#endif /* __I6ENGINE_API_LUMINOUSAPPEARANCECOMPONENT_H_ */

/**
 * @}
 */
