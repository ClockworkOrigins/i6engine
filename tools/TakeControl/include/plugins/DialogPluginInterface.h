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

#ifndef __TAKECONTROL_PLUGINS_DIALOGPLUGININTERFACE_H__
#define __TAKECONTROL_PLUGINS_DIALOGPLUGININTERFACE_H__

#include "i6engine/rpg/dialog/Dialog.h"
#include "i6engine/rpg/npc/NPC.h"

#include <qplugin.h>

namespace i6e {
namespace takeControl {
namespace plugins {

	/**
	 * \brief interface for plugins used to handle different dialog languages
	 */
	class DialogPluginInterface {
	public:
		virtual ~DialogPluginInterface() {
		}

		/**
		 * \brief returns all dialogs this plugin provides
		 * the vector returned is the list, while the tuple contains the Dialog itself and a vector containing all sentences of this dialog
		 * the sentences are split into a tuple consisting of the speaking NPC and an identifier for the sentence itself
		 */
		virtual std::vector<std::tuple<rpg::dialog::Dialog *, std::vector<std::tuple<std::string, std::string>>>> getDialogs() = 0;

		/**
		 * \brief returns all NPCs this plugin provides
		 */
		virtual std::vector<rpg::npc::NPC *> getNPCs() = 0;

		/**
		 * \brief returns the preview text for the given subtitle identifier
		 */
		virtual QString getSubtitleText(QString identifier) const = 0;
	};

} /* namespace plugins */
} /* namespace takeControl */
} /* namespace i6e */

Q_DECLARE_INTERFACE(i6e::takeControl::plugins::DialogPluginInterface, "i6e.takeControl.script.DialogPluginInterface")

#endif /* __TAKECONTROL_PLUGINS_DIALOGPLUGININTERFACE_H__ */
