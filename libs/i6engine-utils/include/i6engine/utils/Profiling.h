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
 * \addtogroup utils
 * @{
 */

#ifndef __I6ENGINE_UTILS_PROFILING_H__
#define __I6ENGINE_UTILS_PROFILING_H__

#include <string>

#include "i6engine/utils/i6eSystemParameters.h"

namespace i6e {
namespace utils {
namespace profiling {

	extern ISIXE_UTILS_API bool packetDump;		// = false;
	extern ISIXE_UTILS_API bool fps;			// = false;
	extern ISIXE_UTILS_API bool fpsCrit;		// = false;
	extern ISIXE_UTILS_API double fpsRed;		// = 0.9
	extern ISIXE_UTILS_API bool waitingMsg;		// = false;
	extern ISIXE_UTILS_API bool countMsg;		// = false;
	extern ISIXE_UTILS_API bool timeStamp;		// = false;
	extern ISIXE_UTILS_API bool numberObjects;	// = false;
	extern ISIXE_UTILS_API bool numberMessages;	// = false;
	extern ISIXE_UTILS_API bool profileCPU;		// = false;
	extern ISIXE_UTILS_API bool profileMemory;	// = false;

	/**
	 * \brief parses the given file into the variables
	 */
	ISIXE_UTILS_API void parse(const std::string & file);

	/**
	 * \brief this methods calculates the current memory usage
	 */
	ISIXE_UTILS_API void calculateMemoryUsage();

	/**
	 * \brief this methods calculates the current cpu usage
	 */
	ISIXE_UTILS_API void calculateCPUUsage();

} /* namespace profiling */
} /* namespace utils */
} /* namespace i6e */

#endif /* __I6ENGINE_UTILS_PROFILING_H__ */

/**
 * @}
 */
