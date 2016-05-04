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

#include "i6engine/api/EngineController.h"
#include "i6engine/api/configs/GUIConfig.h"
#include "i6engine/api/facades/GUIFacade.h"
#include "i6engine/api/facades/ScriptingFacade.h"

#include "boost/python.hpp"

namespace i6e {
namespace python {
namespace gui {

	void startGUI(const std::string & strScheme, const std::string & strFont, const std::string & strDefaultFont, const std::string & strDefaultMouseImageSet, const std::string & strDefaultMouseImageName) {
		api::EngineController::GetSingletonPtr()->getGUIFacade()->startGUI(strScheme, strFont, strDefaultFont, strDefaultMouseImageSet, strDefaultMouseImageName);
	}

	void addToRootWindow(const std::string & child_window) {
		api::EngineController::GetSingletonPtr()->getGUIFacade()->addToRootWindow(child_window);
	}

	void setMouseVisibility(const bool visibility) {
		api::EngineController::GetSingletonPtr()->getGUIFacade()->setMouseVisibility(visibility);
	}

	void clearAllWindows() {
		api::EngineController::GetSingletonPtr()->getGUIFacade()->clearAllWindows();
	}

	void subscribeEvent(const std::string & windowname, const std::string & eventType, const std::string & func) {
		api::EngineController::GetSingletonPtr()->getGUIFacade()->subscribeEvent(windowname, eventType, [func]() {
			api::EngineController::GetSingletonPtr()->getScriptingFacade()->callFunction<void>(func);
		});
	}

	void changeEvent(const std::string & windowname, bool b) {
		api::EngineController::GetSingletonPtr()->getGUIFacade()->changeEvent(windowname, b);
	}

	void setMouseCursorImage(const std::string & image) {
		api::EngineController::GetSingletonPtr()->getGUIFacade()->setMouseCursorImage(image);
	}

	void addImageset(const std::string & imageset) {
		api::EngineController::GetSingletonPtr()->getGUIFacade()->addImageset(imageset);
	}

	void loadCanvas(const std::string & name, const std::string & file) {
		api::EngineController::GetSingletonPtr()->getGUIFacade()->loadCanvas(name, file);
	}

	void addPrint(const std::string & name, const std::string & type, const double x, const double y, const std::string & message, api::gui::Alignment alignment, const int64_t lifetime) {
		api::EngineController::GetSingletonPtr()->getGUIFacade()->addPrint(name, type, x, y, message, alignment, lifetime);
	}

	void addProgressBar(const std::string & name, const std::string & type, const double x, const double y, const double w, const double h) {
		api::EngineController::GetSingletonPtr()->getGUIFacade()->addProgressBar(name, type, x, y, w, h);
	}

	void addEditbox(const std::string & name, const std::string & type, double x, double y, double w, double h, const std::string & defaultText, const std::string & func) {
		api::EngineController::GetSingletonPtr()->getGUIFacade()->addEditbox(name, type, x, y, w, h, defaultText, [func](std::string s) {
			api::EngineController::GetSingletonPtr()->getScriptingFacade()->callFunction<void>(func, s);
		});
	}

	void addTextButton(const std::string & name, const std::string & type, double x, double y, double w, double h, const std::string & text) {
		api::EngineController::GetSingletonPtr()->getGUIFacade()->addTextButton(name, type, x, y, w, h, text);
	}

	void addImageButton(const std::string & name, const std::string & type, double x, double y, double w, double h, const std::string & normalImage, const std::string & hoverImage, const std::string & pushedImage) {
		api::EngineController::GetSingletonPtr()->getGUIFacade()->addImageButton(name, type, x, y, w, h, normalImage, hoverImage, pushedImage);
	}

	void addToggleButton(const std::string & name, const std::string & type, double x, double y, double w, double h, bool selected, const std::string & func) {
		api::EngineController::GetSingletonPtr()->getGUIFacade()->addToggleButton(name, type, x, y, w, h, selected, [func](bool b) {
			api::EngineController::GetSingletonPtr()->getScriptingFacade()->callFunction<void>(func, b);
		});
	}

	void addStatusList(const std::string & name, const std::string & type, const double x, const double y, const int64_t lifetime) {
		api::EngineController::GetSingletonPtr()->getGUIFacade()->addStatusList(name, type, x, y, lifetime);
	}

	void addImage(const std::string & name, const std::string & type, const std::string & imageSetName, const std::string & imageName, const double x, const double y, const double w, const double h) {
		api::EngineController::GetSingletonPtr()->getGUIFacade()->addImage(name, type, imageSetName, imageName, x, y, w, h);
	}

	void addMultiColumnList(const std::string & name, const std::string & type, const double x, const double y, const double w, const double h) {
		api::EngineController::GetSingletonPtr()->getGUIFacade()->addMultiColumnList(name, type, x, y, w, h);
	}

	void addComboBox(const std::string & name, const std::string & type, const double x, const double y, const double w, const double h, const std::string & func) {
		api::EngineController::GetSingletonPtr()->getGUIFacade()->addComboBox(name, type, x, y, w, h, [func](std::string s) {
			api::EngineController::GetSingletonPtr()->getScriptingFacade()->callFunction<void>(func, s);
		});
	}

	void addChat(const std::string & name, const std::string & type, const double x, const double y, const double w, const double h, const std::string & func) {
		api::EngineController::GetSingletonPtr()->getGUIFacade()->addChat(name, type, x, y, w, h, [func](std::string s) {
			return api::EngineController::GetSingletonPtr()->getScriptingFacade()->callFunction<std::string>(func, s)->get();
		});
	}

	void setText(const std::string & name, const std::string & text) {
		api::EngineController::GetSingletonPtr()->getGUIFacade()->setText(name, text);
	}

	void setPosition(const std::string & name, const double x, const double y) {
		api::EngineController::GetSingletonPtr()->getGUIFacade()->setPosition(name, x, y);
	}

	void setSize(const std::string & name, const double w, const double h) {
		api::EngineController::GetSingletonPtr()->getGUIFacade()->setSize(name, w, h);
	}

	void setVisibility(const std::string & windowname, const bool visibility) {
		api::EngineController::GetSingletonPtr()->getGUIFacade()->setVisibility(windowname, visibility);
	}

	void setAmount(const std::string & name, const uint32_t amount) {
		api::EngineController::GetSingletonPtr()->getGUIFacade()->setAmount(name, amount);
	}

	void setColour(const std::string & name, double alpha, double red, double green, double blue) {
		api::EngineController::GetSingletonPtr()->getGUIFacade()->setColour(name, alpha, red, green, blue);
	}

	void setFont(const std::string & name, const std::string & font) {
		api::EngineController::GetSingletonPtr()->getGUIFacade()->setFont(name, font);
	}

	void setLifetime(const std::string & name, const int64_t time) {
		api::EngineController::GetSingletonPtr()->getGUIFacade()->setLifetime(name, time);
	}

	void setAlignment(const std::string & name, i6e::api::gui::Alignment alignment) {
		api::EngineController::GetSingletonPtr()->getGUIFacade()->setAlignment(name, alignment);
	}

	void enableDropTarget(const std::string & name, bool target, const std::string & func) {
		api::EngineController::GetSingletonPtr()->getGUIFacade()->enableDropTarget(name, target, [func](std::string s) {
			return api::EngineController::GetSingletonPtr()->getScriptingFacade()->callFunction<bool>(func, s)->get();
		});
	}

	void setDragable(const std::string & name, bool enabled) {
		api::EngineController::GetSingletonPtr()->getGUIFacade()->setDragable(name, enabled);
	}

	void setDropCallback(const std::string & name, const std::string & func) {
		api::EngineController::GetSingletonPtr()->getGUIFacade()->setDropCallback(name, [func](const std::string & s1, const std::string & s2) {
			api::EngineController::GetSingletonPtr()->getScriptingFacade()->callFunction<void>(func, s1, s2);
		});
	}

	void setProgress(const std::string & name, const double progress) {
		api::EngineController::GetSingletonPtr()->getGUIFacade()->setProgress(name, progress);
	}

	void addRowToList(const std::string & name) {
		api::EngineController::GetSingletonPtr()->getGUIFacade()->addRowToList(name);
	}

	void addRowEntry(const std::string & name, const uint32_t row, const uint32_t column, const std::string & entry) {
		api::EngineController::GetSingletonPtr()->getGUIFacade()->addRowEntry(name, row, column, entry);
	}

	void addColumn(const std::string & name, const std::string & columnHeader, uint32_t column, double width) {
		api::EngineController::GetSingletonPtr()->getGUIFacade()->addColumn(name, columnHeader, column, width);
	}

	void clearWidget(const std::string & name) {
		api::EngineController::GetSingletonPtr()->getGUIFacade()->clearWidget(name);
	}

	void setImage(const std::string & name, const std::string & imageSetName, const std::string & imageSet) {
		api::EngineController::GetSingletonPtr()->getGUIFacade()->setImage(name, imageSetName, imageSet);
	}

	void setEnterTextCallback(const std::string & name, const std::string & func) {
		api::EngineController::GetSingletonPtr()->getGUIFacade()->setEnterTextCallback(name, [func](std::string s) {
			api::EngineController::GetSingletonPtr()->getScriptingFacade()->callFunction<void>(func, s);
		});
	}

	void setProperty(const std::string & name, const std::string & prop, const std::string & value) {
		api::EngineController::GetSingletonPtr()->getGUIFacade()->setProperty(name, prop, value);
	}

	void setSelected(const std::string & name, bool selected) {
		api::EngineController::GetSingletonPtr()->getGUIFacade()->setSelected(name, selected);
	}

	void setSelectedCallback(const std::string & name, const std::string & func) {
		api::EngineController::GetSingletonPtr()->getGUIFacade()->setSelectedCallback(name, [func](bool b) {
			api::EngineController::GetSingletonPtr()->getScriptingFacade()->callFunction<void>(func, b);
		});
	}

	void setAcceptedTextCallback(const std::string & name, const std::string & func) {
		api::EngineController::GetSingletonPtr()->getGUIFacade()->setAcceptedTextCallback(name, [func](std::string s) {
			return api::EngineController::GetSingletonPtr()->getScriptingFacade()->callFunction<std::string>(func, s)->get();
		});
	}

	void setTooltip(const std::string & name, const std::string & tooltip) {
		api::EngineController::GetSingletonPtr()->getGUIFacade()->setTooltip(name, tooltip);
	}

	void addTextToWidget(const std::string & name, const std::string & text) {
		api::EngineController::GetSingletonPtr()->getGUIFacade()->addTextToWidget(name, text);
	}

	void setSelectedStringCallback(const std::string & name, const std::string & func) {
		api::EngineController::GetSingletonPtr()->getGUIFacade()->setSelectedStringCallback(name, [func](std::string s) {
			api::EngineController::GetSingletonPtr()->getScriptingFacade()->callFunction<void>(func, s);
		});
	}

	void playAnimation(const std::string & name, const std::string & animation) {
		api::EngineController::GetSingletonPtr()->getGUIFacade()->playAnimation(name, animation);
	}

	void stopAnimation(const std::string & name, const std::string & animation) {
		api::EngineController::GetSingletonPtr()->getGUIFacade()->stopAnimation(name, animation);
	}

	void pauseAnimation(const std::string & name, const std::string & animation) {
		api::EngineController::GetSingletonPtr()->getGUIFacade()->pauseAnimation(name, animation);
	}

	void unpauseAnimation(const std::string & name, const std::string & animation) {
		api::EngineController::GetSingletonPtr()->getGUIFacade()->unpauseAnimation(name, animation);
	}

	void createWidget(const std::string & name, const std::string & type, const std::string & style) {
		api::EngineController::GetSingletonPtr()->getGUIFacade()->createWidget(name, type, style);
	}

	void deleteWidget(const std::string & name) {
		api::EngineController::GetSingletonPtr()->getGUIFacade()->deleteWidget(name);
	}

	void resetGUISubSystem() {
		api::EngineController::GetSingletonPtr()->getGUIFacade()->resetSubSystem();
	}

	bool getInputCaptured() {
		return api::EngineController::GetSingletonPtr()->getGUIFacade()->getInputCaptured();
	}

	bool getOnWindow() {
		return api::EngineController::GetSingletonPtr()->getGUIFacade()->getOnWindow();
	}

} /* namespace gui */
} /* namespace python */
} /* namespace i6e */

BOOST_PYTHON_MODULE(ScriptingGUIPython) {
	using namespace boost::python;

	enum_<i6e::api::gui::Alignment>("Alignment")
		.value("Left", i6e::api::gui::Alignment::Left)
		.value("Center", i6e::api::gui::Alignment::Center)
		.value("Right", i6e::api::gui::Alignment::Right)
		.export_values();


	def("startGUI", &i6e::python::gui::startGUI);
	def("addToRootWindow", &i6e::python::gui::addToRootWindow);
	def("setMouseVisibility", &i6e::python::gui::setMouseVisibility);
	def("clearAllWindows", &i6e::python::gui::clearAllWindows);
	def("subscribeEvent", &i6e::python::gui::subscribeEvent);
	def("changeEvent", &i6e::python::gui::changeEvent);
	def("setMouseCursorImage", &i6e::python::gui::setMouseCursorImage);
	def("addImageset", &i6e::python::gui::addImageset);
	def("loadCanvas", &i6e::python::gui::loadCanvas);
	def("addPrint", &i6e::python::gui::addPrint);
	def("addProgressBar", &i6e::python::gui::addProgressBar);
	def("addEditbox", &i6e::python::gui::addEditbox);
	def("addTextButton", &i6e::python::gui::addTextButton);
	def("addImageButton", &i6e::python::gui::addImageButton);
	def("addToggleButton", &i6e::python::gui::addToggleButton);
	def("addStatusList", &i6e::python::gui::addStatusList);
	def("addImage", &i6e::python::gui::addImage);
	def("addMultiColumnList", &i6e::python::gui::addMultiColumnList);
	def("addComboBox", &i6e::python::gui::addComboBox);
	def("addChat", &i6e::python::gui::addChat);
	def("setText", &i6e::python::gui::setText);
	def("setPosition", &i6e::python::gui::setPosition);
	def("setSize", &i6e::python::gui::setSize);
	def("setVisibility", &i6e::python::gui::setVisibility);
	def("setAmount", &i6e::python::gui::setAmount);
	def("setColour", &i6e::python::gui::setColour);
	def("setFont", &i6e::python::gui::setFont);
	def("setLifetime", &i6e::python::gui::setLifetime);
	def("setAlignment", &i6e::python::gui::setAlignment);
	def("enableDropTarget", &i6e::python::gui::enableDropTarget);
	def("setDragable", &i6e::python::gui::setDragable);
	def("setDropCallback", &i6e::python::gui::setDropCallback);
	def("setProgress", &i6e::python::gui::setProgress);
	def("addRowToList", &i6e::python::gui::addRowToList);
	def("addRowEntry", &i6e::python::gui::addRowEntry);
	def("addColumn", &i6e::python::gui::addColumn);
	def("clearWidget", &i6e::python::gui::clearWidget);
	def("setImage", &i6e::python::gui::setImage);
	def("setEnterTextCallback", &i6e::python::gui::setEnterTextCallback);
	def("setProperty", &i6e::python::gui::setProperty);
	def("setSelected", &i6e::python::gui::setSelected);
	def("setSelectedCallback", &i6e::python::gui::setSelectedCallback);
	def("setAcceptedTextCallback", &i6e::python::gui::setAcceptedTextCallback);
	def("setTooltip", &i6e::python::gui::setTooltip);
	def("addTextToWidget", &i6e::python::gui::addTextToWidget);
	def("setSelectedStringCallback", &i6e::python::gui::setSelectedStringCallback);
	def("playAnimation", &i6e::python::gui::playAnimation);
	def("stopAnimation", &i6e::python::gui::stopAnimation);
	def("pauseAnimation", &i6e::python::gui::pauseAnimation);
	def("unpauseAnimation", &i6e::python::gui::unpauseAnimation);
	def("createWidget", &i6e::python::gui::createWidget);
	def("deleteWidget", &i6e::python::gui::deleteWidget);
	def("resetGUISubSystem", &i6e::python::gui::resetGUISubSystem);
	def("getInputCaptured", &i6e::python::gui::getInputCaptured);
	def("getOnWindow", &i6e::python::gui::getOnWindow);
}
