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

#ifndef __PU_PHYSX_LOGGING_H__
#define __PU_PHYSX_LOGGING_H__

#include "ParticleUniversePrerequisites.h"
#ifdef PU_PHYSICS_PHYSX

	namespace ParticleUniverse
	{
		class PhysXLogging : public NxUserOutputStream
		{
			public:
				/** 
				*/
				void reportError(NxErrorCode e, const char* message, const char* file, int line)
				{
					LogManager::getSingleton().logMessage("ParticleUniverse PhysX: " + 
						String(file) + " " + 
						StringConverter::toString(line));
					switch (e)
					{
						case NXE_INVALID_PARAMETER:
						LogManager::getSingleton().logMessage("Invalid parameter");
						break;

						case NXE_INVALID_OPERATION:
						LogManager::getSingleton().logMessage("Invalid operation");
						break;

						case NXE_OUT_OF_MEMORY:
						LogManager::getSingleton().logMessage("Out of memory");
						break;

						case NXE_DB_INFO:
						LogManager::getSingleton().logMessage("Info");
						break;

						case NXE_DB_WARNING:
						LogManager::getSingleton().logMessage("Warning");
						break;

						default:
						LogManager::getSingleton().logMessage("Unknown error");
					}

					LogManager::getSingleton().logMessage(String(message));
				}

				/** 
				*/
				NxAssertResponse reportAssertViolation(const char* message, const char* file, int line)
				{
					LogManager::getSingleton().logMessage("Particle Universe PhysX - Access Violation: " + 
						String(message) + " " + 
						String(file) + " " + 
						StringConverter::toString(line));
	#ifdef WIN32
					switch (MessageBox(0, message, "AssertViolation, see console for details.", MB_ABORTRETRYIGNORE))
					{
						case IDRETRY:
							return NX_AR_CONTINUE;
						case IDIGNORE:
							return NX_AR_IGNORE;
						case IDABORT:
						default:
						return NX_AR_BREAKPOINT;
					}
	#elif LINUX
					assert(0);
	#elif _XBOX
					return NX_AR_BREAKPOINT;
	#elif __CELLOS_LV2__
					return NX_AR_BREAKPOINT;
	#endif
				}

				/** 
				*/
				void print(const char* message)
				{
					LogManager::getSingleton().logMessage(String(message));
				}
		};

	}
#endif //PU_PHYSICS_PHYSX
#endif