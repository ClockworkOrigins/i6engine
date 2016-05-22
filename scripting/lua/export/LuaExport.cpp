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

#include "LuaAPIExport.h"
#include "LuaAudioExport.h"
#include "LuaCoreExport.h"
#include "LuaGraphicsExport.h"
#include "LuaGUIExport.h"
#include "LuaInputExport.h"
#include "LuaMathExport.h"
#include "LuaMessagingExport.h"
#include "LuaObjectExport.h"
#include "LuaPhysicsExport.h"
#include "LuaUtilsExport.h"

#ifdef ISIXE_WITH_NETWORK
	#include "LuaNetworkExport.h"
#endif

#ifdef ISIXE_WITH_RPG
	#include "LuaRPGExport.h"
#endif

extern "C" ISIXE_LUA_API int init(lua_State * L) {
	using namespace luabind;

	open(L);

	module(L)
		[
			registerAPI(),
			registerAudio(),
			registerCore(),
			registerGraphics(),
			registerGUI(),
			registerInput(),
			registerMath(),
			registerMessaging(),
			registerObject(),
			registerPhysics(),
			registerUtils()
#ifdef ISIXE_WITH_NETWORK
			,
			registerNetwork()
#endif
#ifdef ISIXE_WITH_RPG
			,
			registerRPG()
#endif
		];

	return 0;
}
