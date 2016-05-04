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
