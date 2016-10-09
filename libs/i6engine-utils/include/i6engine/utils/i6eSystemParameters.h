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

#ifndef __I6ENGINE_UTILS_I6ESYSTEMPARAMETERS_H__
#define __I6ENGINE_UTILS_I6ESYSTEMPARAMETERS_H__

#include "i6engine/i6engineBuildSettings.h"

// Global platform specific defines
#define ISIXE_MPLATFORM_WIN32 1
#define ISIXE_MPLATFORM_LINUX 2
#define BOOST_ALL_DYN_LINK
#define WIN32_LEAN_AND_MEAN
// #define _WIN32_WINNT 0x0501
// Determine the current platform
#if defined(WIN32) || defined(__WIN32__) || defined(_WIN32)
	#define ISIXE_MPLATFORM ISIXE_MPLATFORM_WIN32
	#define WIN32_LEAN_AND_MEAN
#else
	#define ISIXE_MPLATFORM ISIXE_MPLATFORM_LINUX
#endif

// Debug/Release modes
#define ISIXE_DEBUG	1
#define ISIXE_RELEASE 2

#if defined(NDEBUG)
	#define ISIXE_MODE ISIXE_RELEASE
#else
	#define ISIXE_MODE ISIXE_DEBUG
#endif

// Dynamic library import/export macro
#ifndef ISIXE_UTILS_API
	#if ISIXE_MPLATFORM == ISIXE_MPLATFORM_WIN32
		#ifdef ISIXE_BUILD_SHARED
			#ifdef i6engine_utils_EXPORTS
				#define ISIXE_UTILS_API __declspec(dllexport)
			#else
				#define ISIXE_UTILS_API __declspec(dllimport)
			#endif
		#else
			#define ISIXE_UTILS_API
		#endif
	#elif ISIXE_MPLATFORM == ISIXE_MPLATFORM_LINUX
		#define ISIXE_UTILS_API
	#else
		#define ISIXE_UTILS_API
	#endif
#endif
#ifndef ISIXE_MATH_API
	#if ISIXE_MPLATFORM == ISIXE_MPLATFORM_WIN32
		#ifdef ISIXE_BUILD_SHARED
			#ifdef i6engine_math_EXPORTS
				#define ISIXE_MATH_API __declspec(dllexport)
			#else
				#define ISIXE_MATH_API __declspec(dllimport)
			#endif
		#else
			#define ISIXE_MATH_API
		#endif
	#elif ISIXE_MPLATFORM == ISIXE_MPLATFORM_LINUX
		#define ISIXE_MATH_API
	#else
		#define ISIXE_MATH_API
	#endif
#endif
#ifndef ISIXE_CORE_API
	#if ISIXE_MPLATFORM == ISIXE_MPLATFORM_WIN32
		#ifdef ISIXE_BUILD_SHARED
			#ifdef i6engine_core_EXPORTS
				#define ISIXE_CORE_API __declspec(dllexport)
			#else
				#define ISIXE_CORE_API __declspec(dllimport)
			#endif
		#else
			#define ISIXE_CORE_API
		#endif
	#elif ISIXE_MPLATFORM == ISIXE_MPLATFORM_LINUX
		#define ISIXE_CORE_API
	#else
		#define ISIXE_CORE_API
	#endif
#endif
#ifndef ISIXE_MODULES_API
	#if ISIXE_MPLATFORM == ISIXE_MPLATFORM_WIN32
		#ifdef ISIXE_BUILD_SHARED
			#ifdef i6engine_modules_EXPORTS
				#define ISIXE_MODULES_API __declspec(dllexport)
			#else
				#define ISIXE_MODULES_API __declspec(dllimport)
			#endif
		#else
			#define ISIXE_MODULES_API
		#endif
	#elif ISIXE_MPLATFORM == ISIXE_MPLATFORM_LINUX
		#define ISIXE_MODULES_API
	#else
		#define ISIXE_MODULES_API
	#endif
#endif
#ifndef ISIXE_RPG_API
	#if ISIXE_MPLATFORM == ISIXE_MPLATFORM_WIN32
		#ifdef ISIXE_BUILD_SHARED
			#ifdef i6engine_rpg_EXPORTS
				#define ISIXE_RPG_API __declspec(dllexport)
			#else
				#define ISIXE_RPG_API __declspec(dllimport)
			#endif
		#else
			#define ISIXE_RPG_API
		#endif
	#elif ISIXE_MPLATFORM == ISIXE_MPLATFORM_LINUX
		#define ISIXE_RPG_API
	#else
		#define ISIXE_RPG_API
	#endif
#endif

#ifndef ISIXE_LUA_API
	#if ISIXE_MPLATFORM == ISIXE_MPLATFORM_WIN32
		#define ISIXE_LUA_API __declspec(dllexport)
	#else
		#define ISIXE_LUA_API
	#endif
#endif

#ifndef ISIXE_DEPRECATED
	#ifdef __GNUC__
		#define ISIXE_DEPRECATED __attribute__((deprecated))
	#elif defined(_MSC_VER)
		#define ISIXE_DEPRECATED __declspec(deprecated)
	#elif defined(_MSC_VER)
		#define ISIXE_DEPRECATED __attribute__((deprecated))
	#else
		#pragma message("WARNING: You need to implement ISIXE_DEPRECATED for this compiler")
		#define ISIXE_DEPRECATED
	#endif
#endif

#ifdef _MSC_VER
	#pragma warning(disable : 4251) // dll interface to be used by clients...
#endif

#if ISIXE_MPLATFORM == ISIXE_MPLATFORM_LINUX
	/**
	 * necessary to use same API for using window handles
	 */
	typedef void * HWND;
#endif

/**
 * @}
 */

#endif /* __I6ENGINE_UTILS_I6ESYSTEMPARAMETERS_H__ */

/**
 * \addtogroup Macros
 * @{
 */

/*
 * Documentation for all important macros. As doxygen doesn't offer an overview on all defined macros,
 * a new html page is manually created, that will appear in the documentation. So please document
 * your global macros here. At best in alphabetic order :)
 */
/**
* \page macrodoc Documentation for macros
* These lists contain all important macros.
*
* \section objLike Object-like macros
*
* <table>
* 	<tr>
* <td> <b> Name </b> </td> <td> <b> Defined in file </b> </td> <td> <b> Brief description </b> </td>
* </tr>
* <tr>
* <td> ISIXE_API </td> <td> i6eSystemParameters.h </td>
* <td> This macro contains a compiler directive. It should be written after each class declaration if this class should be
* 	 exported later.
* </tr>
* <tr>
* <td> ISIXE_MODE </td> <td> i6eSystemParameters.h </td>
* <td> Is defined either as '1' when running in Debug mode or as '2' when running in Release mode. </td>
* </tr>
* <tr>
* <td> ISIXE_MPLATFORM </td> <td> i6eSystemParameters.h </td>
* <td> Is defined either as '1' when running in a MS Windows environment or as '2' when running in a Linux environment. </td>
* </tr>
* </table>
*
* \section funcLike Function-like macros
*
* <table>
* 	<tr>
* <td> <b> Name </b> </td> <td> <b> Defined in file </b> </td> <td> <b> Brief description </b> </td>
* </tr>
* <tr>
* <td> ISIXE_LOG_<X>("Module", "Message") </td> <td> Logger.h </td>
* <td> This macro is used for the controlled output of error / info / warning / debug messages. X stands
*    for the logging level and can be one of: 'DEBUG', 'INFO', 'WARN', 'ERROR', 'FATAL'.
*    For example: ISIXE_LOG_WARN("PhysicsController", "Erratic Gravity!") </td>
* </tr>
* <tr>
* <td> ISIXE_LOG_SETLEVEL(level) </td> <td> Logger.h </td>
* <td> Sets the new LogLevel (i6utils::Logger::LogLevel). Everything at least as important as 'level' is logged.
*    For example: ISIXE_LOG_SETLEVEL(i6utils::Logger::INFO) </td>
* </tr>
* 	<tr>
*		<td> ISIXE_REGISTERMESSAGETYPE(strMessageTypeName, ptrMessageMethod) </td> <td> MessagingController.h </td>
*		<td> This macro should be used to register message types at the MessagingController.
*		   Have a look at its class description to get more information. </td>
* </tr>
* <tr>
* 	<td> ISIXE_UNREGISTERMESSAGETYPE(strMessageTypeName) </td> <td> MessagingController.h </td>
* 	<td> Deletes a registration of a message type.
* 	   Have a look at the MessagingController class description to get more information. </td>
* <tr>
* </table>
*
*/

/**
 * @}
 */
