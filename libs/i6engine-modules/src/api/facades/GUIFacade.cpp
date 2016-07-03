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

#include "i6engine/api/facades/GUIFacade.h"

#include "i6engine/utils/Exceptions.h"

#include "i6engine/core/configs/SubsystemConfig.h"

#include "i6engine/api/FrontendMessageTypes.h"
#include "i6engine/api/configs/GUIConfig.h"
#include "i6engine/api/facades/MessagingFacade.h"

namespace i6e {
namespace api {

	GUIFacade::GUIFacade() : _onWindow(), _captured() {
	}

	void GUIFacade::startGUI(const std::string & strScheme, const std::string & strFont, const std::string & strDefaultFont, const std::string & strDefaultMouseImageSet, const std::string & strDefaultMouseImageName) const {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::GUIMessageType, gui::GuiAdd, core::Method::Create, new gui::GUI_Add_Create(strScheme, strFont, strDefaultFont, strDefaultMouseImageSet, strDefaultMouseImageName), core::Subsystem::Unknown);

		i6eMessagingFacade->deliverMessage(msg);
	}

	void GUIFacade::addToRootWindow(const std::string & child_window) const {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::GUIMessageType, gui::GuiAddToRoot, core::Method::Update, new gui::GUI_AddToRoot_Update(child_window), core::Subsystem::Unknown);

		i6eMessagingFacade->deliverMessage(msg);
	}

	void GUIFacade::setMouseVisibility(const bool visibility) const {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::GUIMessageType, gui::GuiMouseVisible, core::Method::Update, new gui::GUI_MouseVisible_Update(visibility), core::Subsystem::Unknown);

		i6eMessagingFacade->deliverMessage(msg);
	}

	void GUIFacade::clearAllWindows() const {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::GUIMessageType, gui::GuiCleanUp, core::Method::Delete, new gui::GUI_CleanUp_Delete(), core::Subsystem::Unknown);

		i6eMessagingFacade->deliverMessage(msg);
	}

	void GUIFacade::subscribeEvent(const std::string & windowname, const std::string & eventType, const boost::function<void(void)> & ptrEventMethod) const {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::GUIMessageType, gui::GuiSubscribeEvent, core::Method::Update, new gui::GUI_SubscribeEvent_Update(windowname, eventType, ptrEventMethod), core::Subsystem::Unknown);

		i6eMessagingFacade->deliverMessage(msg);
	}

	void GUIFacade::changeEvent(const std::string & windowname, const bool b) const {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::GUIMessageType, gui::GuiEvent, core::Method::Update, new gui::GUI_Event_Update(windowname, b), core::Subsystem::Unknown);

		i6eMessagingFacade->deliverMessage(msg);
	}

	void GUIFacade::setMouseCursorImage(const std::string & image) const {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::GUIMessageType, gui::GuiMouseCursorImage, core::Method::Update, new gui::GUI_MouseCursorImage_Update(image), core::Subsystem::Unknown);

		i6eMessagingFacade->deliverMessage(msg);
	}

	void GUIFacade::addImageset(const std::string & imageset) const {
		i6eMessagingFacade->deliverMessage(boost::make_shared<GameMessage>(messages::GUIMessageType, gui::GuiAddImageset, core::Method::Create, new gui::GUI_AddImageset_Create(imageset), core::Subsystem::Unknown));
	}

	void GUIFacade::loadCanvas(const std::string & name, const std::string & file) const {
		i6eMessagingFacade->deliverMessage(boost::make_shared<GameMessage>(messages::GUIMessageType, gui::GuiLoadCanvas, core::Method::Create, new gui::GUI_LoadCanvas_Create(name, file), core::Subsystem::Unknown));
	}

	void GUIFacade::registerWidgetTemplate(const std::string & name, const boost::function<api::GUIWidget * (const std::string & name, const std::string & style)> & createFunc) const {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::GUIMessageType, gui::GuiWidgetTemplate, core::Method::Create, new gui::GUI_WidgetTemplate_Create(name, createFunc), core::Subsystem::Unknown);

		i6eMessagingFacade->deliverMessage(msg);
	}

	void GUIFacade::createWidget(const std::string & name, const std::string & type, const std::string & style) const {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::GUIMessageType, gui::GuiAddWidget, core::Method::Create, new gui::GUI_Widget_Create(name, type, style), core::Subsystem::Unknown);
		i6eMessagingFacade->deliverMessage(msg);
	}

	void GUIFacade::deleteWidget(const std::string & name) const {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::GUIMessageType, gui::GuiDeleteWidget, core::Method::Delete, new gui::GUIUpdateMessageStruct(name), core::Subsystem::Unknown);
		i6eMessagingFacade->deliverMessage(msg);
	}

	void GUIFacade::addPrint(const std::string & name, const std::string & type, const double x, const double y, const std::string & message, gui::Alignment alignment, const int64_t lifetime /*=-1*/) const {
		createWidget(name, "GUIPrint", type);
		setPosition(name, x, y);
		setSize(name, 0.15, 0.04); // Default size for Prints
		setText(name, message);
		setAlignment(name, alignment);

		if (lifetime != -1) {
			setLifetime(name, lifetime);
		}
	}

	void GUIFacade::addStatusList(const std::string & name, const std::string & type, const double x, const double y, const int64_t lifetime) const {
		createWidget(name, "GUIStatusList", type);
		setPosition(name, x, y);
		setLifetime(name, lifetime);
	}

	void GUIFacade::addProgressBar(const std::string & name, const std::string & type, const double x, const double y, const double w, const double h) const {
		createWidget(name, "GUIBar", type);
		setPosition(name, x, y);
		setSize(name, w, h);
	}

	void GUIFacade::addEditbox(const std::string & name, const std::string & type, double x, double y, double w, double h, const std::string & defaultText, const std::function<void(std::string)> & enterCallback) const {
		createWidget(name, "GUIEditbox", type);
		setPosition(name, x, y);
		setSize(name, w, h);
		setText(name, defaultText);
		setEnterTextCallback(name, enterCallback);
	}

	void GUIFacade::addTextButton(const std::string & name, const std::string & type, double x, double y, double w, double h, const std::string & text) const {
		createWidget(name, "GUITextButton", type);
		setPosition(name, x, y);
		setSize(name, w, h);
		setText(name, text);
	}

	void GUIFacade::addImageButton(const std::string & name, const std::string & type, double x, double y, double w, double h, const std::string & normalImage, const std::string & hoverImage, const std::string & pushedImage) const {
		createWidget(name, "GUIImageButton", type);
		setPosition(name, x, y);
		setSize(name, w, h);
		setProperty(name, "NormalImage", normalImage);
		setProperty(name, "HoverImage", hoverImage);
		setProperty(name, "PushedImage", pushedImage);
	}

	void GUIFacade::addToggleButton(const std::string & name, const std::string & type, double x, double y, double w, double h, bool selected, const std::function<void(bool)> & selectedCallback) const {
		createWidget(name, "GUIToggleButton", type);
		setPosition(name, x, y);
		setSize(name, w, h);
		setSelected(name, selected);
		setSelectedCallback(name, selectedCallback);
	}

	void GUIFacade::addImage(const std::string & name, const std::string & type, const std::string & imageSetName, const std::string & imageName, const double x, const double y, const double w, const double h) const {
		createWidget(name, "GUIImage", type);
		setPosition(name, x, y);
		setSize(name, w, h);
		setImage(name, imageSetName, imageName);
	}

	void GUIFacade::addMultiColumnList(const std::string & name, const std::string & type, const double x, const double y, const double w, const double h) const {
		createWidget(name, "GUIMultiColumnList", type);
		setPosition(name, x, y);
		setSize(name, w, h);
	}

	void GUIFacade::addComboBox(const std::string & name, const std::string & type, double x, double y, double w, double h, const std::function<void(std::string)> & selectedCallback) const {
		createWidget(name, "GUIComboBox", type);
		setPosition(name, x, y);
		setSize(name, w, h);
		setSelectedStringCallback(name, selectedCallback);
	}

	void GUIFacade::addChat(const std::string & name, const std::string & type, double x, double y, double w, double h, const std::function<std::string(std::string)> & acceptedCallback) const {
		createWidget(name, "GUIChat", type);
		setPosition(name, x, y);
		setSize(name, w, h);
		setAcceptedTextCallback(name, acceptedCallback);
	}

	void GUIFacade::setLifetime(const std::string & name, const int64_t time) const {
		GameMessage::Ptr tim = boost::make_shared<GameMessage>(messages::GUIMessageType, gui::GuiSetLifetime, core::Method::Update, new gui::GUI_Lifetime(name, time), core::Subsystem::Unknown);
		i6eMessagingFacade->deliverMessage(tim);
	}

	void GUIFacade::setText(const std::string & name, const std::string & text) const {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::GUIMessageType, gui::GuiSetText, core::Method::Update, new gui::GUI_Text(name, text), core::Subsystem::Unknown);
		i6eMessagingFacade->deliverMessage(msg);
	}

	void GUIFacade::setPosition(const std::string & name, const double x, const double y) const {
		GameMessage::Ptr pos = boost::make_shared<GameMessage>(messages::GUIMessageType, gui::GuiSetPosition, core::Method::Update, new gui::GUI_Position(name, x, y), core::Subsystem::Unknown);

		i6eMessagingFacade->deliverMessage(pos);
	}

	void GUIFacade::setAmount(const std::string & name, const uint32_t amount) const {
		GameMessage::Ptr pos = boost::make_shared<GameMessage>(messages::GUIMessageType, gui::GuiSetAmount, core::Method::Update, new gui::GUI_Amount(name, amount), core::Subsystem::Unknown);

		i6eMessagingFacade->deliverMessage(pos);
	}

	void GUIFacade::setImage(const std::string & name, const std::string & imageSetName, const std::string & imageName) const {
		GameMessage::Ptr pos = boost::make_shared<GameMessage>(messages::GUIMessageType, gui::GuiSetImage, core::Method::Update, new gui::GUI_Image(name, imageSetName, imageName), core::Subsystem::Unknown);

		i6eMessagingFacade->deliverMessage(pos);
	}

	void GUIFacade::setEnterTextCallback(const std::string & name, const std::function<void(std::string)> & enterCallback) const {
		i6eMessagingFacade->deliverMessage(boost::make_shared<GameMessage>(messages::GUIMessageType, gui::GuiSetEnterCallback, core::Method::Update, new gui::GUI_SetEnterTextCallback(name, enterCallback), core::Subsystem::Unknown));
	}

	void GUIFacade::setProperty(const std::string & name, const std::string & prop, const std::string & value) const {
		i6eMessagingFacade->deliverMessage(boost::make_shared<GameMessage>(messages::GUIMessageType, gui::GuiSetProperty, core::Method::Update, new gui::GUI_SetProperty(name, prop, value), core::Subsystem::Unknown));
	}

	void GUIFacade::setSelected(const std::string & name, bool selected) const {
		i6eMessagingFacade->deliverMessage(boost::make_shared<GameMessage>(messages::GUIMessageType, gui::GuiSetSelected, core::Method::Update, new gui::GUI_SetSelected(name, selected), core::Subsystem::Unknown));
	}

	void GUIFacade::setSelectedCallback(const std::string & name, const std::function<void(bool)> & selectedCallback) const {
		i6eMessagingFacade->deliverMessage(boost::make_shared<GameMessage>(messages::GUIMessageType, gui::GuiSetSelectedCallback, core::Method::Update, new gui::GUI_SetSelectedCallback(name, selectedCallback), core::Subsystem::Unknown));
	}

	void GUIFacade::setSize(const std::string & name, const double w, const double h) const {
		GameMessage::Ptr pos = boost::make_shared<GameMessage>(messages::GUIMessageType, gui::GuiSetSize, core::Method::Update, new gui::GUI_Size(name, w, h), core::Subsystem::Unknown);

		i6eMessagingFacade->deliverMessage(pos);
	}

	void GUIFacade::setColour(const std::string & name, double alpha, double red, double green, double blue) const {
		GameMessage::Ptr col = boost::make_shared<GameMessage>(messages::GUIMessageType, gui::GuiSetColour, core::Method::Update, new gui::GUI_Colour(name, alpha, red, green, blue), core::Subsystem::Unknown);

		i6eMessagingFacade->deliverMessage(col);
	}

	void GUIFacade::setFont(const std::string & name, const std::string & font) const {
		GameMessage::Ptr fon = boost::make_shared<GameMessage>(messages::GUIMessageType, gui::GuiSetFont, core::Method::Update, new gui::GUI_Text(name, font), core::Subsystem::Unknown);
		i6eMessagingFacade->deliverMessage(fon);
	}

	void GUIFacade::addTextToWidget(const std::string & name, const std::string & text) const {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::GUIMessageType, gui::GuiAddText, core::Method::Update, new gui::GUI_Text(name, text), core::Subsystem::Unknown);

		i6eMessagingFacade->deliverMessage(msg);
	}

	void GUIFacade::setProgress(const std::string & name, const double progress) const {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::GUIMessageType, gui::GuiSetProgress, core::Method::Update, new gui::GUI_Progress(name, progress), core::Subsystem::Unknown);
		i6eMessagingFacade->deliverMessage(msg);
	}

	void GUIFacade::addRowToList(const std::string & name) const {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::GUIMessageType, gui::GuiAddRow, core::Method::Update, new gui::GUIUpdateMessageStruct(name), core::Subsystem::Unknown);
		i6eMessagingFacade->deliverMessage(msg);
	}

	void GUIFacade::addRowEntry(const std::string & name, const uint32_t row, const uint32_t column, const std::string & entry) const {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::GUIMessageType, gui::GuiSetRowEntry, core::Method::Update, new gui::GUI_Add_Row_Entry(name, row, column, entry), core::Subsystem::Unknown);
		i6eMessagingFacade->deliverMessage(msg);
	}

	void GUIFacade::addColumn(const std::string & name, const std::string & columnHeader, uint32_t column, double width) const {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::GUIMessageType, gui::GuiAddColumn, core::Method::Update, new gui::GUI_AddColumn(name, columnHeader, column, width), core::Subsystem::Unknown);
		i6eMessagingFacade->deliverMessage(msg);
	}

	void GUIFacade::clearWidget(const std::string & name) const {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::GUIMessageType, gui::GuiClearWidget, core::Method::Update, new gui::GUIUpdateMessageStruct(name), core::Subsystem::Unknown);
		i6eMessagingFacade->deliverMessage(msg);
	}

	void GUIFacade::setVisibility(const std::string & windowname, const bool visibility) const {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::GUIMessageType, gui::GuiSetVisible, core::Method::Update, new gui::GUI_Visibility(windowname, visibility), core::Subsystem::Unknown);
		i6eMessagingFacade->deliverMessage(msg);
	}

	void GUIFacade::setAlignment(const std::string & windowname, gui::Alignment alignment) const {
		i6eMessagingFacade->deliverMessage(boost::make_shared<GameMessage>(messages::GUIMessageType, gui::GuiSetAlignment, core::Method::Update, new gui::GUI_Alignment(windowname, alignment), core::Subsystem::Unknown));
	}

	void GUIFacade::enableDropTarget(const std::string & windowname, bool target, const std::function<bool(std::string)> & func) const {
		i6eMessagingFacade->deliverMessage(boost::make_shared<GameMessage>(messages::GUIMessageType, gui::GuiSetDropTarget, core::Method::Update, new gui::GUI_SetDropTarget(windowname, target, func), core::Subsystem::Unknown));
	}

	void GUIFacade::setDragable(const std::string & windowname, bool enabled) const {
		i6eMessagingFacade->deliverMessage(boost::make_shared<GameMessage>(messages::GUIMessageType, gui::GuiSetDragable, core::Method::Update, new gui::GUI_SetDragable(windowname, enabled), core::Subsystem::Unknown));
	}

	void GUIFacade::setDropCallback(const std::string & windowname, const std::function<void(const std::string &, const std::string &)> & callback) const {
		i6eMessagingFacade->deliverMessage(boost::make_shared<GameMessage>(messages::GUIMessageType, gui::GuiSetDropCallback, core::Method::Update, new gui::GUI_SetDropCallback(windowname, callback), core::Subsystem::Unknown));
	}

	void GUIFacade::setTooltip(const std::string & windowname, const std::string & tooltip) const {
		i6eMessagingFacade->deliverMessage(boost::make_shared<GameMessage>(messages::GUIMessageType, gui::GuiSetTooltip, core::Method::Update, new gui::GUI_SetTooltip(windowname, tooltip), core::Subsystem::Unknown));
	}

	void GUIFacade::setSelectedStringCallback(const std::string & name, const std::function<void(std::string)> & selectedCallback) const {
		i6eMessagingFacade->deliverMessage(boost::make_shared<GameMessage>(messages::GUIMessageType, gui::GuiSetSelectedStringCallback, core::Method::Update, new gui::GUI_SetSelectedStringCallback(name, selectedCallback), core::Subsystem::Unknown));
	}

	void GUIFacade::setAcceptedTextCallback(const std::string & name, const std::function<std::string(std::string)> & acceptedCallback) const {
		i6eMessagingFacade->deliverMessage(boost::make_shared<GameMessage>(messages::GUIMessageType, gui::GuiSetTextAcceptCallback, core::Method::Update, new gui::GUI_SetAcceptTextCallback(name, acceptedCallback), core::Subsystem::Unknown));
	}

	void GUIFacade::playAnimation(const std::string & name, const std::string & animation) const {
		i6eMessagingFacade->deliverMessage(boost::make_shared<GameMessage>(messages::GUIMessageType, gui::GuiPlayAnimation, core::Method::Update, new gui::GUI_PlayAnimation(name, animation), core::Subsystem::Unknown));
	}

	void GUIFacade::stopAnimation(const std::string & name, const std::string & animation) const {
		i6eMessagingFacade->deliverMessage(boost::make_shared<GameMessage>(messages::GUIMessageType, gui::GuiStopAnimation, core::Method::Update, new gui::GUI_StopAnimation(name, animation), core::Subsystem::Unknown));
	}

	void GUIFacade::pauseAnimation(const std::string & name, const std::string & animation) const {
		i6eMessagingFacade->deliverMessage(boost::make_shared<GameMessage>(messages::GUIMessageType, gui::GuiPauseAnimation, core::Method::Update, new gui::GUI_PauseAnimation(name, animation), core::Subsystem::Unknown));
	}

	void GUIFacade::unpauseAnimation(const std::string & name, const std::string & animation) const {
		i6eMessagingFacade->deliverMessage(boost::make_shared<GameMessage>(messages::GUIMessageType, gui::GuiUnpauseAnimation, core::Method::Update, new gui::GUI_UnpauseAnimation(name, animation), core::Subsystem::Unknown));
	}

	void GUIFacade::setAutoLineBreak(const std::string & name, bool enabled) const {
		i6eMessagingFacade->deliverMessage(boost::make_shared<GameMessage>(messages::GUIMessageType, gui::GuiSetAutoLineBreak, core::Method::Update, new gui::GUI_SetAutoLineBreak(name, enabled), core::Subsystem::Unknown));
	}

	void GUIFacade::addTicker(GUIWidget * widget) {
		_addTickerCallback(widget);
	}

	void GUIFacade::removeTicker(GUIWidget * widget) {
		_removeTickerCallback(widget);
	}

	void GUIFacade::registerAddTickerCallback(const boost::function<void(GUIWidget *)> & func) {
		_addTickerCallback = func;
	}

	void GUIFacade::registerRemoveTickerCallback(const boost::function<void(GUIWidget *)> & func) {
		_removeTickerCallback = func;
	}

	void GUIFacade::resetSubSystem() {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::GUIMessageType, gui::GuiReset, core::Method::Delete, new GameMessageStruct(), core::Subsystem::Unknown);

		i6eMessagingFacade->deliverMessage(msg);
	}

	void GUIFacade::setDefaultFont(const std::string & font) const {
		i6eMessagingFacade->deliverMessage(boost::make_shared<GameMessage>(messages::GUIMessageType, gui::GuiDefaultFont, core::Method::Update, new gui::GUI_SetDefaultFont(font), core::Subsystem::Unknown));
	}

	void GUIFacade::addWidgetAsChildTo(const std::string & child, const std::string & parent) const {
		i6eMessagingFacade->deliverMessage(boost::make_shared<GameMessage>(messages::GUIMessageType, gui::GuiSetParent, core::Method::Update, new gui::GUI_SetParent(child, parent), core::Subsystem::Unknown));
	}

	void GUIFacade::setTransparencyCheckEnabled(const std::string & name, bool enabled) const {
		i6eMessagingFacade->deliverMessage(boost::make_shared<GameMessage>(messages::GUIMessageType, gui::GuiSetTransparencyCheck, core::Method::Update, new gui::GUI_SetTransparencyCheck(name, enabled), core::Subsystem::Unknown));
	}

} /* namespace api */
} /* namespace i6e */
