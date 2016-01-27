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

#ifndef __I6ENGINE_EDITOR_EDITOR_H__
#define __I6ENGINE_EDITOR_EDITOR_H__

#include "i6engine/api/Application.h"
#include "i6engine/api/configs/ObjectConfig.h"

#include "clockUtils/iniParser/iniParser.h"

namespace i6engine {
namespace editor {

	class Editor : public i6engine::api::Application {
	public:
		/**
		 * \brief creates Editor with given window name
		 */
		Editor();

		/**
		 * \brief destructor
		 */
		virtual ~Editor();

		api::GOPtr getCamera() const {
			return _camera;
		}

		/**
		 * \brief saves current state in given file
		 */
		void saveLevel(const std::string & file);

	protected:
		/**
		 * \brief called before subsystems are started
		 */
		void Initialize() override;

		/**
		 * \brief called after all subsystems are started
		 * call this always from derived classes!
		 */
		virtual void AfterInitialize() override;

		/**
		 * \brief can the Editor shut down or not?
		 */
		bool ShutdownRequest() override;

		/**
		 * \brief called during shutdown
		 */
		void Finalize() override;

		/**
		 * \brief called during each tick of the editor
		 */
		void Tick() override;

		/**
		 * \brief loads given level file
		 * removes old level if some is loaded
		 */
		void loadLevel(const std::string & file);

		/**
		 * \brief returns the base path where the file dialog shall look for level files
		 */
		virtual std::string getBasePath() const = 0;

		/**
		 * \brief returns a vector containing all available level filter flags
		 */
		virtual std::vector<std::string> getLevelFlags() const = 0;

	protected:
		clockUtils::iniParser::IniParser _iniParser;

		double _movementSpeed;
		double _rotationSpeed;

		virtual void setProgressValue(int value) = 0;
		virtual void setProgressMaximum(int value) = 0;
		virtual void finishedProgress() = 0;

	private:
		api::GOPtr _camera;

		/**
		 * \brief In this map all registered buttons and their respective actions are stored.
		 * The boolean indicates if the button is still pressed.
		 */
		std::map<std::string, std::pair<boost::function<void(void)>, bool> > _eventMap;

		/**
		 * \brief stores state if a level was loaded or not
		 */
		bool _inLevel;

		/**
		 * \brief stores the id of the currently selected GameObject or -1
		 */
		int64_t _selectedObjectID;

		/**
		 * \brief stores the state of free fly mode
		 */
		bool _freeFlyMode;

		/**
		 * \brief stores whether movement mode is for camera or object
		 */
		bool _moveObject;

		/**
		 * \brief last mouse positions
		 */
		int32_t _lastX;
		int32_t _lastY;

		std::set<utils::weakPtr<api::GameObject, api::GameObject>> _lastNearWaypoints;

		bool _removeBox;

		/**
		 * \brief stores camera object used for navigation through level
		 */
		inline void setCamera(const api::GOPtr & go) {
			_camera = go;
			updateWaypointNames();
		}

		/**
		 * \brief processes input messages
		 */
		void InputMailbox(const api::GameMessage::Ptr & msg);

		/**
		 * \brief removes all entries from ObjectList and adds current list
		 */
		virtual void updateObjectList() = 0;

		/**
		 * \brief called when an object from the object list is selected
		 */
		virtual void selectObject(int64_t id) = 0;

		virtual void removeObject() = 0;

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
		void ScaleUp();
		void ScaleDown();
		void ScaleXUp();
		void ScaleXDown();
		void ScaleYUp();
		void ScaleYDown();
		void ScaleZUp();
		void ScaleZDown();

		void updateWaypointNames();
	};

} /* namespace editor */
} /* namespace i6engine */

#endif /* __I6ENGINE_EDITOR_EDITOR_H__ */
