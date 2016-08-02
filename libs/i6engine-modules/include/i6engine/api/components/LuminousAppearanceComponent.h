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

#ifndef __I6ENGINE_API_LUMINOUSAPPEARANCECOMPONENT_H_
#define __I6ENGINE_API_LUMINOUSAPPEARANCECOMPONENT_H_

#include "i6engine/math/i6eVector.h"
#include "i6engine/math/i6eVector4.h"

#include "i6engine/api/components/Component.h"

namespace i6e {
namespace api {

	/**
	 * \class LuminousAppearanceComponent
	 * \brief Tells the engine that this GameObject is emitting light.
	 * For creating a LuminousAppearanceComponent, these keys are possible:
	 * | Name | Required | Type | Description | Public |
	 * |------|----------|------| ----------- | ------------ |
	 * | lightType | yes | int | one of the LightTypes | yes |
	 * | diffuseColor | yes | Vec3 | diffuse colour of the light, in range [0.0; 1.0] | yes |
	 * | specularColor | yes | Vec3 | specular colour of the light, in range [0.0; 1.0] | yes |
	 * | attenuation | yes | Vec4 | attenuation of the light, W = range (range in world units), X = constant (1.0 never attenuate, 0.0 complete attenuation), Y = linear (linear attenuation depending on distance), Z = quadratic factor | yes |
	 * | pos | no | Vec3 | relative position to SceneNode | yes |
	 * | direction | *) | Vec3 | direction of the light shaft, *) required if lightType is DIRECTIONAL or SPOT | yes |
	 * | spotLightRangeInner | *) | double | range of the inner cone of a spot light in degree, *) required if lightType is SPOT | yes |
	 * | spotLightRangeOuter | *) | double | range of the outer cone of a spot light in degree, *) required if lightType is SPOT | yes |
	 */
	class ISIXE_MODULES_API LuminousAppearanceComponent : public Component {
	public:
		/**
		 * \brief type of the light emitted
		 */
		enum class LightType {
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
		 * \brief sets inner spot light angle
		 */
		void setSpotLightInnerRange(double angle);

		/**
		 * \brief sets outer spot light angle
		 */
		void setSpotLightOuterRange(double angle);

		/**
		 * \brief synchronizes the Components state
		 */
		virtual attributeMap synchronize() const override;

		virtual std::string getTemplateName() const override {
			return "LuminousAppearance";
		}

		virtual std::vector<componentOptions> getComponentOptions() override;

	protected:
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

		virtual void Init() override;
		virtual void Finalize() override;

	private:
		std::pair<AddStrategy, int64_t> howToAdd(const ComPtr & comp) const override;

		/**
		 * \brief Sends update message to the MessagingController
		 *
		 * Sends update message to the MessagingController containing ObjectID, component and the light attributes
		 */
		void sendUpdateMessage();
	};

} /* namespace api */
} /* namespace i6e */

#endif /* __I6ENGINE_API_LUMINOUSAPPEARANCECOMPONENT_H_ */

/**
 * @}
 */
