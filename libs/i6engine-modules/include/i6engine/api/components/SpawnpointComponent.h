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

#ifndef __I6ENGINE_API_SPAWNPOINTCOMPONENT_H__
#define __I6ENGINE_API_SPAWNPOINTCOMPONENT_H__

#include <vector>

#include "i6engine/api/components/Component.h"

namespace i6e {
namespace api {

	/**
	* \brief Only defines this object as a spawnpoint
	 * For creating a SpawnpointComponent, these keys are possible:
	 * | Name | Required | Type | Description | Public |
	 * |------|----------|------| ----------- | ------------ |
	 * | spawntypes | yes | std::string | GameObject types being spawnable at this Spawnpoint, separated by delimitter ; | yes |
	*/
	class ISIXE_MODULES_API SpawnpointComponent : public Component {
	public:
		/**
		 * \brief Constructor
		 */
		SpawnpointComponent(const int64_t id, const attributeMap & params);
		~SpawnpointComponent();

		/**
		 * \brief Adds an object type to spawn on this component
		 *
		 * \param[in] type Type of the object being able to be spawned on the component
		 */
		inline void addSpawntype(const std::string & type) { _spawntypes.push_back(type); }

		/**
		 * \brief Adds object types to spawn on this component
		 *
		 * \param[in] types Types of the objects being able to be spawned on the component (type1;type2;type3;)
		 */
		void addSpawntypes(const std::string & types);

		/**
		 * \brief Removes the given type from the spawn list
		 *
		 * \param[in] type Type of the object being removed from the list
		 */
		void removeSpawntype(const std::string & type);

		/**
		 * \brief Return wether the given type is contained in the list (true) or not (false)
		 *
		 * \return True if the type can be respawned, otherwise else
		 */
		bool containsSpawntype(const std::string & type) const;

		/**
		 * \brief synchronizes the Components state
		 */
		attributeMap synchronize() const override;

		/**
		 * \brief returns true, if this spawnpoint currently can be used
		 */
		bool available() const { return _state; }

		/**
		 * \brief sets the state of the Spawnpoint
		 */
		void setState(bool b);

		std::string getTemplateName() const override {
			return "Spawnpoint";
		}

		std::vector<componentOptions> getComponentOptions() override;

	private:
		std::vector<std::string> _spawntypes;
		bool _state;

		void Init() override;

		/**
		 * \brief Receives messages for this component
		 */
		void News(const GameMessage::Ptr & msg) override;
	};

} /* namespace api */
} /* namespace i6e */

#endif /* __I6ENGINE_API_SPAWNPOINTCOMPONENT_H__ */

/**
 * @}
 */
