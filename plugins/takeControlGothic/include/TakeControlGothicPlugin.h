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

#ifndef __I6ENGINE_PLUGINS_TAKECONTROLGOTHICPLUGIN_H__
#define __I6ENGINE_PLUGINS_TAKECONTROLGOTHICPLUGIN_H__

#include "plugins/DialogPluginInterface.h"

namespace i6e {
namespace plugins {

	class TakeControlGothicPlugin : public QObject, public takeControl::plugins::DialogPluginInterface {
		Q_OBJECT
		Q_PLUGIN_METADATA(IID "i6e.takeControl.script.DialogPluginInterface")
		Q_INTERFACES(i6e::takeControl::plugins::DialogPluginInterface)

	public:
		TakeControlGothicPlugin();

	signals:
		void updatedData() override;
		void updateParseProgress(int);

	public slots:
		void gothicPathChanged(QString path);

	private:
		std::vector<rpg::npc::NPC *> _npcList;
		std::vector<std::tuple<rpg::dialog::Dialog *, std::vector<std::tuple<std::string, std::string>>>> _dialogList;
		std::map<QString, QString> _subtitleToTextMap;
		QString _lastPath;

		QString getName() const override {
			return "Gothic";
		}

		std::vector<std::tuple<rpg::dialog::Dialog *, std::vector<std::tuple<std::string, std::string>>>> getDialogs() override;

		std::vector<rpg::npc::NPC *> getNPCs() override;

		QString getSubtitleText(QString identifier) const override;

		takeControl::widgets::SettingsWidget * getSettingsWidget(clockUtils::iniParser::IniParser * iniParser, QWidget * par) override;
	};

} /* namespace plugins */
} /* namespace i6e */

#endif /* __I6ENGINE_PLUGINS_TAKECONTROLGOTHICPLUGIN_H__ */
