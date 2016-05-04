#include "EditorRPGRunGame.h"

#include "i6engine/configs/FrameTimes.h"

#include "i6engine/api/EngineController.h"

#ifdef ISIXE_WITH_AUDIO
	#include "i6engine/modules/audio/AudioController.h"
#endif

#include "i6engine/modules/graphics/GraphicsController.h"
#include "i6engine/modules/object/ObjectController.h"
#include "i6engine/modules/physics/PhysicsController.h"

#if ISIXE_SCRIPTING == SCRIPTING_LUA
	#include "i6engine/modules/luascripting/LuaScriptingController.h"
#elif ISIXE_SCRIPTING == SCRIPTING_PYTHON
	#include "i6engine/modules/pythonscripting/PythonScriptingController.h"
#endif

#include "i6engine/rpg/dialog/DialogManager.h"

namespace i6e {
namespace plugins {

	void EditorRPGRunGame::Initialize() {
		RPGApplication::Initialize();

		rpg::dialog::DialogManager::GetSingleton().reset();
	}

	void EditorRPGRunGame::initializeSubSystems(HWND hWnd) {
		api::EngineController::GetSingleton().registerSubSystem("Graphics", new modules::GraphicsController(hWnd), { core::Subsystem::Object });
		api::EngineController::GetSingleton().registerSubSystem("Object", new modules::ObjectController(), { core::Subsystem::Physic });
		api::EngineController::GetSingleton().registerSubSystem("Physics", new modules::PhysicsController(), LNG_PHYSICS_FRAME_TIME);
#if ISIXE_SCRIPTING == SCRIPTING_LUA
		api::EngineController::GetSingleton().registerSubSystem("Scripting", new modules::LuaScriptingController(), LNG_SCRIPTING_FRAME_TIME);
#elif ISIXE_SCRIPTING == SCRIPTING_PYTHON
		api::EngineController::GetSingleton().registerSubSystem("Scripting", new modules::PythonScriptingController(), LNG_SCRIPTING_FRAME_TIME);
#endif
#ifdef ISIXE_WITH_AUDIO
		api::EngineController::GetSingleton().registerSubSystem("Audio", new modules::AudioController(), LNG_AUDIO_FRAME_TIME);
#endif
	}

} /* namespace plugins */
} /* namespace i6e */

Q_PLUGIN_METADATA(IID "i6e.editor.runGame.RunGamePluginInterface")
