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
		i6eEngineController->registerSubSystem("Graphics", new modules::GraphicsController(hWnd), { core::Subsystem::Object });
		i6eEngineController->registerSubSystem("Object", new modules::ObjectController(), { core::Subsystem::Physic });
		i6eEngineController->registerSubSystem("Physics", new modules::PhysicsController(), LNG_PHYSICS_FRAME_TIME);
#if ISIXE_SCRIPTING == SCRIPTING_LUA
		i6eEngineController->registerSubSystem("Scripting", new modules::LuaScriptingController(), LNG_SCRIPTING_FRAME_TIME);
#elif ISIXE_SCRIPTING == SCRIPTING_PYTHON
		i6eEngineController->registerSubSystem("Scripting", new modules::PythonScriptingController(), LNG_SCRIPTING_FRAME_TIME);
#endif
#ifdef ISIXE_WITH_AUDIO
		i6eEngineController->registerSubSystem("Audio", new modules::AudioController(), LNG_AUDIO_FRAME_TIME);
#endif
	}

} /* namespace plugins */
} /* namespace i6e */

Q_PLUGIN_METADATA(IID "i6e.editor.runGame.RunGamePluginInterface")
