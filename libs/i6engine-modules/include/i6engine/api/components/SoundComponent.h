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

#ifndef __I6ENGINE_API_SOUNDCOMPONENT_H__
#define __I6ENGINE_API_SOUNDCOMPONENT_H__

#include "i6engine/math/i6eVector.h"

#include "i6engine/api/components/Component.h"

namespace i6e {
namespace api {

	class PhysicalStateComponent;

	/**
	 * \class SoundComponent
	 * \brief Attaches a sound node to an object. The Sound will follow the object
	 * For creating a SoundComponent, these keys are possible:
	 * | Name | Required | Type | Description | Public |
	 * |------|----------|------| ----------- | ------------ |
	 * | offset | yes | Vec3 | offset to the position of the PhysicalStateComponent this Component is connected to | yes |
	 * | direction | yes | Vec3 | direction of the sound | yes |
	 * | file | yes | std::string | sound file for this sound, currently only wav is supported | yes |
	 * | looping | yes | bool | is this sound looping or only played once | yes |
	 * | maxDist | yes | double | maxmimum distance this sound should be hearable, volume is interpolated | yes |
	 * | cache | yes | bool | if set to true, this sound is cached in memory and mustn't be loaded from harddisk every time it is played | yes |
	 * | category | no | std::string | specifies the category this sound belongs to, e.g. music, effect, dialog, default value is empty string | yes |
	 */
	class ISIXE_MODULES_API SoundComponent : public Component {
	public:
		/**
		 * \brief Constructor with attribute map
		 */
		SoundComponent(const int64_t id, const attributeMap & params);

		/**
		 * \brief Destructor
		 */
		~SoundComponent();

		/**
		 * \brief synchronizes the Components state
		 */
		attributeMap synchronize() const override;

		std::string getTemplateName() const override {
			return "Sound";
		}

		std::vector<componentOptions> getComponentOptions() override;

	private:
		/**
		 * \brief Position of the parent
		 */
		Vec3 _position;

		/**
		 * \brief Offset of the sound of the GameObject itself
		 */
		Vec3 _offset;

		/**
		 * \brief direction of the sound
		 */
		Vec3 _direction;

		/**
		 * \brief Filename of the Sound
		 */
		std::string _file;

		/**
		 * \brief is the sound looping or not?
		 */
		bool _looping;

		/**
		 * \brief distance where the sound disappears
		 */
		double _maxDist;

		/**
		 * \brief weakPtr on PhysicalStateComponent for faster access
		 */
		utils::weakPtr<PhysicalStateComponent, Component> _psc;

		/**
		 * \brief will this sound file be cached
		 */
		bool _cacheable;

		/**
		 * \brief category of this sound (e.g. music, effect, dialog)
		 */
		std::string _category;

		/**
		 * \brief initializes the component
		 */
		void Init() override;

		void Finalize() override;

		void Tick() override;

		virtual std::pair<AddStrategy, int64_t> howToAdd(const ComPtr & comp) const override;
	};

} /* namespace api */
} /* namespace i6e */

#endif /* __I6ENGINE_API_SOUNDCOMPONENT_H__ */

/**
 * @}
 */
