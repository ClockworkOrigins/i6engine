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

#ifndef __I6ENGINE_RPG_DIALOG_DIALOGMANAGER_H__
#define __I6ENGINE_RPG_DIALOG_DIALOGMANAGER_H__

#include <set>
#include <thread>

#include "i6engine/utils/DoubleBufferQueue.h"
#include "i6engine/utils/Future.h"
#include "i6engine/utils/i6eSystemParameters.h"
#include "i6engine/utils/Singleton.h"

#include "i6engine/rpg/dialog/DialogParser.h"

namespace i6engine {
namespace rpg {
namespace npc {
	class ExitDialogJob;
} /* namespace npc */
namespace dialog {

	struct Dialog;

	class ISIXE_RPG_API DialogManager : public utils::Singleton<DialogManager> {
		friend class utils::Singleton<DialogManager>;
		friend class npc::ExitDialogJob;

	public:
		~DialogManager();

		/**
		 * \brief loads all dialog files in directory and subdirectories
		 */
		void loadDialogs(const std::string & directory);

		/**
		 * \brief checks all important dialogs of given npc
		 * starts dialog, if an important dialog was found
		 */
		void checkImportantDialogs(const std::string & identifier);

		/**
		 * \brief checks all dialogs of the given npcs and shows updated dialog box
		 */
		void checkDialogs(const std::string & identifier);

		/**
		 * \brief returns true if this dialog was already heard
		 */
		bool wasHeard(const std::string & identifier) const;

		/**
		 * \brief exits a dialog
		 */
		void exitDialog();

		/**
		 * \brief returns all NPCs being part of active dialog
		 */
		std::vector<std::string> getActiveNPCs() const {
			return _activeNPCs;
		}

		/**
		 * \brief call this to change state of subtitles (on/off)
		 */
		void setSubtitlesEnabled(bool b) {
			_subtitles = b;
		}

		/**
		 * \brief returns whether subtitles are activated or not
		 */
		bool getSubtitlesEnabled() const {
			return _subtitles;
		}

		/**
		 * \brief returns true if a dialog is running
		 */
		bool isDialogRunning() const {
			return _dialogActive;
		}

	private:
		DialogParser _parser;
		std::map<std::string, std::vector<Dialog *>> _npcDialogs;

		enum DialogCheck {
			NPCIdentifier,
			DialogIdentifier,
			Result
		};

		utils::DoubleBufferQueue<std::tuple<std::string, std::string, std::shared_ptr<utils::Future<bool>>>, true, false> _importantChecks;
		utils::DoubleBufferQueue<std::tuple<std::string, std::string, std::shared_ptr<utils::Future<bool>>>, true, false> _showDialogboxChecks;
		std::atomic<bool> _dialogActive;

		mutable std::mutex _lock;

		std::set<Dialog *> _heardDialogs;

		// TODO: (Daniel) exchange this with scheduler job implementing ticket ISIXE-1457
		bool _running;
		std::thread _worker;

		bool _guiInitialized;

		std::vector<std::string> _activeNPCs;
		std::map<std::string, std::string> _dialogMapping;
		int8_t _showDialogCalls;

		bool _subtitles;

		DialogManager();

		bool checkDialogsLoop();
		bool runDialog(const std::string & npc, const std::string & dia);
		void showDialog(const std::string & npc, const std::string & dia);
		void stopDialog() {
			_showDialogCalls--;
		}
	};

} /* namespace dialog */
} /* namespace rpg */
} /* namespace i6engine */

#endif /* __I6ENGINE_RPG_DIALOG_DIALOGMANAGER_H__ */
