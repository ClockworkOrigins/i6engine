#ifndef __I6ENGINE_PLUGINS_EDITORRPGRUNGAME_H__
#define __I6ENGINE_PLUGINS_EDITORRPGRUNGAME_H__

#include "i6engine/rpg/RPGApplication.h"

#include "i6engine/editor/plugins/RunGamePluginInterface.h"

#include <QObject>

namespace i6engine {
namespace plugins {

	class EditorRPGRunGame : public QObject, public rpg::RPGApplication, public editor::plugins::RunGamePluginInterface {
		Q_OBJECT
		Q_PLUGIN_METADATA(IID "i6engine.editor.runGame.RunGamePluginInterface")
		Q_INTERFACES(i6engine::editor::plugins::RunGamePluginInterface)

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
} /* namespace i6engine */

#endif /* __I6ENGINE_PLUGINS_EDITORRPGRUNGAME_H__ */
