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

#ifndef __I6ENGINE_UTILS_I6ETHREADSAFETY_H__
#define __I6ENGINE_UTILS_I6ETHREADSAFETY_H__

#include "i6engine/utils/i6eSystemParameters.h"

#if ISIXE_MPLATFORM == ISIXE_MPLATFORM_WIN32
	#include <Windows.h>
#else
	#include <sys/syscall.h>
#endif

#if ISIXE_MODE == I6E_RELEASE
	#define ASSERT_THREAD_SAFETY_HEADER
	#define ASSERT_THREAD_SAFETY_CONSTRUCTOR
	#define ASSERT_THREAD_SAFETY_FUNCTION
#else
	#define ASSERT_THREAD_SAFETY_HEADER int _threadID;
	#if ISIXE_MPLATFORM == ISIXE_MPLATFORM_WIN32
		#define ASSERT_THREAD_SAFETY_CONSTRUCTOR _threadID = GetCurrentThreadId();
		#define ASSERT_THREAD_SAFETY_FUNCTION assert(_threadID == GetCurrentThreadId());
	#else
		#define ASSERT_THREAD_SAFETY_CONSTRUCTOR _threadID = syscall(SYS_gettid);
		#define ASSERT_THREAD_SAFETY_FUNCTION assert(_threadID == syscall(SYS_gettid));
	#endif
#endif

#endif /* __I6ENGINE_UTILS_I6ETHREADSAFETY_H__ */

/**
 * @}
 */
