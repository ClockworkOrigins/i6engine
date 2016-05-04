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

/**
 * \addtogroup RPG
 * @{
 */

#ifndef __I6ENGINE_RPG_DIALOG_DIALOGMANAGER_H__
#define __I6ENGINE_RPG_DIALOG_DIALOGMANAGER_H__

#include <set>

#include "i6engine/utils/DoubleBufferQueue.h"
#include "i6engine/utils/Future.h"
#include "i6engine/utils/i6eSystemParameters.h"
#include "i6engine/utils/Singleton.h"

#include "i6engine/math/i6eVector2.h"

#include "i6engine/api/GameMessage.h"
#include "i6engine/api/facades/MessageSubscriberFacade.h"

#include "i6engine/rpg/dialog/DialogParser.h"

namespace i6e {
namespace rpg {
namespace npc {
	class ExitDialogJob;
} /* namespace npc */
namespace dialog {

	struct Dialog;

	/**
	 * \brief handles all dialogs
	 * checks if dialogs can be used, manages jobs etc.
	 */
	class ISIXE_RPG_API DialogManager : public utils::Singleton<DialogManager>, public api::MessageSubscriberFacade {
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
		 * \brief call this to change state of subtitles (on/off)
		 */
		void setDialogNumbersEnabled(bool b) {
			_dialogNumbers = b;
		}

		/**
		 * \brief returns true if a dialog is running
		 */
		bool isDialogRunning() const {
			return _dialogActive;
		}

		/**
		 * \brief sets position of subtitle box
		 */
		void setSubtitlePosition(const Vec2 & pos);

		/**
		 * \brief sets size of subtitle box
		 */
		void setSubtitleSize(const Vec2 & size);

		/**
		 * \brief sets font of subtitle box
		 */
		void setSubtitleFont(const std::string & font);

		/**
		 * \brief gets all dialogs
		 */
		std::map<std::string, Dialog *> getDialogs() const {
			return _parser._dialogs;
		}

		/**
		 * \brief gets all dialogs with their npcs
		 */
		std::map<std::string, std::vector<Dialog *>> getNpcDialogs() const {
			return _npcDialogs;
		}

		/**
		 * \brief resets DialogManager
		 */
		void reset();

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

		bool _running;
		uint64_t _jobID;

		bool _guiInitialized;

		std::vector<std::string> _activeNPCs;
		std::map<std::string, std::string> _dialogMapping;
		std::vector<std::string> _dialogNumberVector;
		int8_t _showDialogCalls;

		bool _subtitles;
		Vec2 _subtitlePosition;
		Vec2 _subtitleSize;
		std::string _subtitleFont;
		bool _dialogNumbers;

		DialogManager();

		bool checkDialogsLoop();
		bool runDialog(const std::string & npc, const std::string & dia);
		void showDialog(const std::string & npc, const std::string & dia);
		void stopDialog() {
			_showDialogCalls--;
		}

		void News(const api::GameMessage::Ptr & msg);
	};

} /* namespace dialog */
} /* namespace rpg */
} /* namespace i6e */

#endif /* __I6ENGINE_RPG_DIALOG_DIALOGMANAGER_H__ */

/**
 * @}
 */
