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
#define I6E_DEBUG	1
#define I6E_RELEASE 2

#if defined(NDEBUG)
	#define ISIXE_MODE I6E_RELEASE
#else
	#define ISIXE_MODE I6E_DEBUG
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
#ifndef ISIXE_EDITOR_API
	#if ISIXE_MPLATFORM == ISIXE_MPLATFORM_WIN32
		#ifdef ISIXE_BUILD_SHARED
			#ifdef i6engine_editor_EXPORTS
				#define ISIXE_EDITOR_API __declspec(dllexport)
			#else
				#define ISIXE_EDITOR_API __declspec(dllimport)
			#endif
		#else
			#define ISIXE_EDITOR_API
		#endif
	#elif ISIXE_MPLATFORM == ISIXE_MPLATFORM_LINUX
		#define ISIXE_EDITOR_API
	#else
		#define ISIXE_EDITOR_API
	#endif
#endif

#ifndef ISIXE_LUA_API
	#if ISIXE_MPLATFORM == ISIXE_MPLATFORM_WIN32
		#define ISIXE_LUA_API __declspec(dllexport)
	#else
		#define ISIXE_LUA_API
	#endif
#endif

#ifdef _MSC_VER
	#pragma warning(disable : 4251) // dll interface to be used by clients...
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
