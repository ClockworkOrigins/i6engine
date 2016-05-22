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
