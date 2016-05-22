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

#ifndef __I6ENGINE_PLUGINS_EDITORRPGRUNGAME_H__
#define __I6ENGINE_PLUGINS_EDITORRPGRUNGAME_H__

#include "i6engine/rpg/RPGApplication.h"

#include "i6engine/editor/plugins/RunGamePluginInterface.h"

#include <QObject>

namespace i6e {
namespace plugins {

	class EditorRPGRunGame : public QObject, public rpg::RPGApplication, public editor::plugins::RunGamePluginInterface {
		Q_OBJECT
		Q_PLUGIN_METADATA(IID "i6e.editor.runGame.RunGamePluginInterface")
		Q_INTERFACES(i6e::editor::plugins::RunGamePluginInterface)

	public:
		QString getMenuEntry() const override {
			return "RPG";
		}

		void initializeSubSystems(HWND hWnd) override;

		void Initialize() override;

		void AfterInitialize() override {
			RPGApplication::AfterInitialize();

			loadLevel(_level);
		}

		void Tick() override {
			RPGApplication::Tick();
		}

		bool ShutdownRequest() override {
			return RPGApplication::ShutdownRequest();
		}

		void Finalize() override {
			RPGApplication::Finalize();

			_callback();
		}
	};

} /* namespace plugins */
} /* namespace i6e */

#endif /* __I6ENGINE_PLUGINS_EDITORRPGRUNGAME_H__ */
