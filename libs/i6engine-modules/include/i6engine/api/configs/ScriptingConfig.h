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

/**
 * \addtogroup Scripting
 * @{
 */

#ifndef __I6ENGINE_API_SCRIPTINGCONFIG_H__
#define __I6ENGINE_API_SCRIPTINGCONFIG_H__

#include "i6engine/api/GameMessageStruct.h"
#include "i6engine/api/configs/PhysicsConfig.h"

namespace i6e {
namespace api {
namespace scripting {
	enum ScriptingMessageTypes {
		ScrReset = 0,
		ScrRayResult,
		ScrLoadAllScripts
	};

	/**
	 * \brief struct for message executing given method in given script file using raytest result as parameter
	 */
	typedef struct ISIXE_MODULES_API Scripting_RayResult_Update : physics::Physics_RayTestNotify_Update {
		std::string file;
		std::string func;
		int64_t rayID;

		Scripting_RayResult_Update(const std::string & fi, const std::string & fu, const int64_t & ri);
		Scripting_RayResult_Update * copy() {
			return new Scripting_RayResult_Update(*this);
		}
	} Scripting_RayResult_Update;

} /* namespace scripting */
} /* namespace api */
} /* namespace i6e */

#endif /* __I6ENGINE_API_SCRIPTINGCONFIG_H__ */

/**
 * @}
 */
