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

#include "i6engine/api/EngineController.h"
#include "i6engine/api/configs/GUIConfig.h"
#include "i6engine/api/facades/GUIFacade.h"
#include "i6engine/api/facades/ScriptingFacade.h"

#include "boost/python.hpp"

namespace i6e {
namespace python {
namespace gui {

	void startGUI(const std::string & strScheme, const std::string & strFont, const std::string & strDefaultFont, const std::string & strDefaultMouseImageSet, const std::string & strDefaultMouseImageName) {
		i6eGUIFacade->startGUI(strScheme, strFont, strDefaultFont, strDefaultMouseImageSet, strDefaultMouseImageName);
	}

	void addToRootWindow(const std::string & child_window) {
		i6eGUIFacade->addToRootWindow(child_window);
	}

	void setMouseVisibility(const bool visibility) {
		i6eGUIFacade->setMouseVisibility(visibility);
	}

	void clearAllWindows() {
		i6eGUIFacade->clearAllWindows();
	}

	ISIXE_DEPRECATED void subscribeEventString(const std::string & windowname, const std::string &, const std::string & func) {
		i6eGUIFacade->subscribeEvent(windowname, i6e::api::gui::SubscribeEvent::Clicked, [func]() {
			i6eScriptingFacade->callFunction<void>(func);
		});
	}

	void subscribeEvent(const std::string & windowname, i6e::api::gui::SubscribeEvent eventType, const std::string & func) {
		i6eGUIFacade->subscribeEvent(windowname, eventType, [func]() {
			i6eScriptingFacade->callFunction<void>(func);
		});
	}

	void changeEvent(const std::string & windowname, bool b) {
		i6eGUIFacade->changeEvent(windowname, b);
	}

	void setMouseCursorImage(const std::string & image) {
		i6eGUIFacade->setMouseCursorImage(image);
	}

	void setMouseCursorImage(const std::vector<std::string> & sequence, double fps, bool looping) {
		i6eGUIFacade->setMouseCursorImage(sequence, fps, looping);
	}

	void addImageset(const std::string & imageset) {
		i6eGUIFacade->addImageset(imageset);
	}

	void loadCanvas(const std::string & name, const std::string & file) {
		i6eGUIFacade->loadCanvas(name, file);
	}

	void addPrint(const std::string & name, const std::string & type, const double x, const double y, const std::string & message, api::gui::Alignment alignment, const int64_t lifetime) {
		i6eGUIFacade->addPrint(name, type, x, y, message, alignment, lifetime);
	}

	void addProgressBar(const std::string & name, const std::string & type, const double x, const double y, const double w, const double h) {
		i6eGUIFacade->addProgressBar(name, type, x, y, w, h);
	}

	void addEditbox(const std::string & name, const std::string & type, double x, double y, double w, double h, const std::string & defaultText, const std::string & func) {
		i6eGUIFacade->addEditbox(name, type, x, y, w, h, defaultText, [func](std::string s) {
			i6eScriptingFacade->callFunction<void>(func, s);
		});
	}

	void addTextButton(const std::string & name, const std::string & type, double x, double y, double w, double h, const std::string & text) {
		i6eGUIFacade->addTextButton(name, type, x, y, w, h, text);
	}

	void addImageButton(const std::string & name, const std::string & type, double x, double y, double w, double h, const std::string & normalImage, const std::string & hoverImage, const std::string & pushedImage) {
		i6eGUIFacade->addImageButton(name, type, x, y, w, h, normalImage, hoverImage, pushedImage);
	}

	void addToggleButton(const std::string & name, const std::string & type, double x, double y, double w, double h, bool selected, const std::string & func) {
		i6eGUIFacade->addToggleButton(name, type, x, y, w, h, selected, [func](bool b) {
			i6eScriptingFacade->callFunction<void>(func, b);
		});
	}

	void addStatusList(const std::string & name, const std::string & type, const double x, const double y, const int64_t lifetime) {
		i6eGUIFacade->addStatusList(name, type, x, y, lifetime);
	}

	void addImage(const std::string & name, const std::string & type, const std::string & imageSetName, const std::string & imageName, const double x, const double y, const double w, const double h) {
		i6eGUIFacade->addImage(name, type, imageSetName, imageName, x, y, w, h);
	}

	void addMultiColumnList(const std::string & name, const std::string & type, const double x, const double y, const double w, const double h) {
		i6eGUIFacade->addMultiColumnList(name, type, x, y, w, h);
	}

	void addComboBox(const std::string & name, const std::string & type, const double x, const double y, const double w, const double h, const std::string & func) {
		i6eGUIFacade->addComboBox(name, type, x, y, w, h, [func](std::string s) {
			i6eScriptingFacade->callFunction<void>(func, s);
		});
	}

	void addChat(const std::string & name, const std::string & type, const double x, const double y, const double w, const double h, const std::string & func) {
		i6eGUIFacade->addChat(name, type, x, y, w, h, [func](std::string s) {
			return i6eScriptingFacade->callFunction<std::string>(func, s)->get();
		});
	}

	void setText(const std::string & name, const std::string & text) {
		i6eGUIFacade->setText(name, text);
	}

	void setPosition(const std::string & name, const double x, const double y) {
		i6eGUIFacade->setPosition(name, x, y);
	}

	void setSize(const std::string & name, const double w, const double h) {
		i6eGUIFacade->setSize(name, w, h);
	}

	void setVisibility(const std::string & windowname, const bool visibility) {
		i6eGUIFacade->setVisibility(windowname, visibility);
	}

	void setAmount(const std::string & name, const uint32_t amount) {
		i6eGUIFacade->setAmount(name, amount);
	}

	void setColour(const std::string & name, double alpha, double red, double green, double blue) {
		i6eGUIFacade->setColour(name, alpha, red, green, blue);
	}

	void setFont(const std::string & name, const std::string & font) {
		i6eGUIFacade->setFont(name, font);
	}

	void setLifetime(const std::string & name, const int64_t time) {
		i6eGUIFacade->setLifetime(name, time);
	}

	void setAlignment(const std::string & name, i6e::api::gui::Alignment alignment) {
		i6eGUIFacade->setAlignment(name, alignment);
	}

	void enableDropTarget(const std::string & name, bool target, const std::string & func) {
		i6eGUIFacade->enableDropTarget(name, target, [func](std::string s) {
			return i6eScriptingFacade->callFunction<bool>(func, s)->get();
		});
	}

	void setDragable(const std::string & name, bool enabled) {
		i6eGUIFacade->setDragable(name, enabled);
	}

	void setDropCallback(const std::string & name, const std::string & func) {
		i6eGUIFacade->setDropCallback(name, [func](const std::string & s1, const std::string & s2) {
			i6eScriptingFacade->callFunction<void>(func, s1, s2);
		});
	}

	void setProgress(const std::string & name, const double progress) {
		i6eGUIFacade->setProgress(name, progress);
	}

	void addRowToList(const std::string & name) {
		i6eGUIFacade->addRowToList(name);
	}

	void addRowEntry(const std::string & name, const uint32_t row, const uint32_t column, const std::string & entry) {
		i6eGUIFacade->addRowEntry(name, row, column, entry);
	}

	void addColumn(const std::string & name, const std::string & columnHeader, uint32_t column, double width) {
		i6eGUIFacade->addColumn(name, columnHeader, column, width);
	}

	void clearWidget(const std::string & name) {
		i6eGUIFacade->clearWidget(name);
	}

	void setImage(const std::string & name, const std::string & imageSetName, const std::string & imageSet) {
		i6eGUIFacade->setImage(name, imageSetName, imageSet);
	}

	void setImage(const std::string & name, const std::vector<std::pair<std::string, std::string>> & sequence, double fps, bool looping) {
		i6eGUIFacade->setImage(name, sequence, fps, looping);
	}

	void setEnterTextCallback(const std::string & name, const std::string & func) {
		i6eGUIFacade->setEnterTextCallback(name, [func](std::string s) {
			i6eScriptingFacade->callFunction<void>(func, s);
		});
	}

	void setProperty(const std::string & name, const std::string & prop, const std::string & value) {
		i6eGUIFacade->setProperty(name, prop, value);
	}

	void setSelected(const std::string & name, bool selected) {
		i6eGUIFacade->setSelected(name, selected);
	}

	void setSelectedCallback(const std::string & name, const std::string & func) {
		i6eGUIFacade->setSelectedCallback(name, [func](bool b) {
			i6eScriptingFacade->callFunction<void>(func, b);
		});
	}

	void setAcceptedTextCallback(const std::string & name, const std::string & func) {
		i6eGUIFacade->setAcceptedTextCallback(name, [func](std::string s) {
			return i6eScriptingFacade->callFunction<std::string>(func, s)->get();
		});
	}

	void setTooltip(const std::string & name, const std::string & tooltip) {
		i6eGUIFacade->setTooltip(name, tooltip);
	}

	void addTextToWidget(const std::string & name, const std::string & text) {
		i6eGUIFacade->addTextToWidget(name, text);
	}

	void setSelectedStringCallback(const std::string & name, const std::string & func) {
		i6eGUIFacade->setSelectedStringCallback(name, [func](std::string s) {
			i6eScriptingFacade->callFunction<void>(func, s);
		});
	}

	void playAnimation(const std::string & name, const std::string & animation) {
		i6eGUIFacade->playAnimation(name, animation);
	}

	void stopAnimation(const std::string & name, const std::string & animation) {
		i6eGUIFacade->stopAnimation(name, animation);
	}

	void pauseAnimation(const std::string & name, const std::string & animation) {
		i6eGUIFacade->pauseAnimation(name, animation);
	}

	void unpauseAnimation(const std::string & name, const std::string & animation) {
		i6eGUIFacade->unpauseAnimation(name, animation);
	}

	void createWidget(const std::string & name, const std::string & type, const std::string & style) {
		i6eGUIFacade->createWidget(name, type, style);
	}

	void deleteWidget(const std::string & name) {
		i6eGUIFacade->deleteWidget(name);
	}

	void resetGUISubSystem() {
		i6eGUIFacade->resetSubSystem();
	}

	bool getInputCaptured() {
		return i6eGUIFacade->getInputCaptured();
	}

	bool getOnWindow() {
		return i6eGUIFacade->getOnWindow();
	}

	void setTransparencyCheckEnabled(const std::string & name, bool enabled) {
		i6eGUIFacade->setTransparencyCheckEnabled(name, enabled);
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

	enum_<i6e::api::gui::SubscribeEvent>("SubscribeEvent")
		.value("Clicked", i6e::api::gui::SubscribeEvent::Clicked)
		.value("MouseEnter", i6e::api::gui::SubscribeEvent::MouseEnter)
		.value("MouseLeave", i6e::api::gui::SubscribeEvent::MouseLeave)
		.export_values();

	def("startGUI", &i6e::python::gui::startGUI);
	def("addToRootWindow", &i6e::python::gui::addToRootWindow);
	def("setMouseVisibility", &i6e::python::gui::setMouseVisibility);
	def("clearAllWindows", &i6e::python::gui::clearAllWindows);
	def("subscribeEvent", &i6e::python::gui::subscribeEventString);
	def("subscribeEvent", &i6e::python::gui::subscribeEvent);
	def("changeEvent", &i6e::python::gui::changeEvent);
	def("setMouseCursorImage", (void(*)(const std::string &)) &i6e::python::gui::setMouseCursorImage);
	def("setMouseCursorImage", (void(*)(const std::vector<std::string> &, double, bool)) &i6e::python::gui::setMouseCursorImage);
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
	def("setImage", (void(*)(const std::string &, const std::string &, const std::string &)) &i6e::python::gui::setImage);
	def("setImage", (void(*)(const std::string &, const std::vector<std::pair<std::string, std::string>> &, double, bool)) &i6e::python::gui::setImage);
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
	def("setTransparencyCheckEnabled", &i6e::python::gui::setTransparencyCheckEnabled);
}
