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

#ifndef __I6ENGINE_SAMPLES_PHYSICSPOWERAPPLICATION_H__
#define __I6ENGINE_SAMPLES_PHYSICSPOWERAPPLICATION_H__

#include "i6engine/api/Application.h"
#include "i6engine/api/configs/ObjectConfig.h"

namespace sample {

	class PhysicsPowerApplication : public i6engine::api::Application {
	public:
		PhysicsPowerApplication();

		~PhysicsPowerApplication();

		void Initialize();

		void AfterInitialize();

		void Tick();

		bool ShutdownRequest();

		void Finalize();
		
		void ShutDown();

	private:
		bool _showFPS;
		i6engine::api::GOPtr _camera;

		/**
		 * \brief In this map all registered buttons and their respective actions are stored.
		 * The boolean indicates if the button is still pressed.
		 */
		std::map<std::string, std::pair<boost::function<void(void)>, bool> > _eventMap;

		/**
		 * \brief processes input messages
		 */
		void InputMailbox(const i6engine::api::GameMessage::Ptr & msg);

		void Forward();
		void Backward();
		void Left();
		void Right();
		void Down();
		void Up();
		void RotateLeft();
		void RotateRight();
		void RotateUp();
		void RotateDown();
		void LeanLeft();
		void LeanRight();

		void resetScene();
	};

} /* namespace sample */

#endif /* __I6ENGINE_SAMPLES_PHYSICSPOWERAPPLICATION_H__ */
