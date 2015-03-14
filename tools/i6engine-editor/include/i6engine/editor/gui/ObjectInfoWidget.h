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

#pragma once

#include <map>

#include "i6engine/api/gui/GUIWidget.h"

namespace CEGUI {
	class Editbox;
	class EventArgs;
} /* namespace CEGUI */

namespace i6engine {
namespace editor {
namespace gui {

	enum CallbackTuple {
		WRITEABLE,
		GETTER,
		SETTER
	};

	class ObjectInfoWidget : public api::GUIWidget {
	public:
		ObjectInfoWidget(const std::string & name, const std::string & type);

		~ObjectInfoWidget();

		void update(uint16_t type, i6engine::api::gui::GUIUpdateMessageStruct * data) override;

	private:
		std::map<CEGUI::Editbox *, std::tuple<bool, boost::function<std::string(void)>, boost::function<void(std::string)>>> _callbacks;
		size_t _count;
		CEGUI::Window * _lastWindow;

		void addComponent(const std::string & component, const std::string & identifier);
		void addComponentOption(bool writeable, const std::string & optionName, const boost::function<std::string(void)> & getter, const boost::function<bool(std::string)> & setter);

		bool applyChanges(const CEGUI::EventArgs & e);
	};

} /* namespace gui */
} /* namespace editor */
} /* namespace i6engine */
