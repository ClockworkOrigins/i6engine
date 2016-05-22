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

#ifndef __PU_PARTICLE_SCRIPT_SERIALIZER_H__
#define __PU_PARTICLE_SCRIPT_SERIALIZER_H__

#include "ParticleUniverseContext.h"
#include "ParticleUniverseSystemTokens.h"

namespace ParticleUniverse
{
	/** The ParticleScriptSerializer class is responsible for writing objects and attributes to a particle system script.
	*/
	class _ParticleUniverseExport ParticleScriptSerializer : public SerializerAlloc
	{
		public:
			Context context;
			ParticleScriptSerializer(void);
			virtual ~ParticleScriptSerializer(void) {}

			/** Writes a ParticleSystem to a file in the script format.
			*/
			void writeScript(const ParticleSystem* particleSystem, const String& fileName);

			/** If no filename has been given, serialisation is done to a string.
			*/
			const String& writeScript(const ParticleSystem* particleSystem);

			/** Writes a line to a script file.
			*/
			void writeLine(
				const String& s0, 
				const String& s1, 
				const String& s2,
				const String& s3,
				const String& s4,
				short indentation0 = -1,
				short indentation1 = -1,
				short indentation2 = -1,
				short indentation3 = -1,
				short indentation4 = -1);

			/** Writes a line to a script file.
			*/
			void writeLine(
				const String& s0, 
				const String& s1, 
				const String& s2,
				const String& s3,
				short indentation0 = -1,
				short indentation1 = -1,
				short indentation2 = -1,
				short indentation3 = -1);

			/** Writes a line to a script file.
			*/
			void writeLine(
				const String& s0, 
				const String& s1, 
				const String& s2,
				short indentation0 = -1,
				short indentation1 = -1,
				short indentation2 = -1);

			/** Writes a line to a script file.
			*/
			void writeLine(
				const String& s0, 
				const String& s1,
				short indentation0 = -1,
				short indentation1 = -1);

			/** Writes a line to a script file.
			*/
			void writeLine(
				const String& s0,
				short indentation0 = -1);

			/** Set the default tab values. If a writeline doesn't contain any indentation value, the default tab values are 
				used.
			*/
			void setDefaultTabs (
				short tab0 = 0, 
				short tab1 = 48, 
				short tab2 = 52, 
				short tab3 = 56,
				short tab4 = 60);

			/** Set the directory to which files are serialized.
			*/
			void setPath (const String& path);

			/** Creates a string from a vector with Reals. This function contains an additional argument. If set to true, the
				square root is applied to all real values (don't ask!).
			*/
			static String toString(vector<Real> vector, bool applySqrt = false);

			/** Get/set the indentation. To be used in cases where it is unkown what the indentation is.
			*/
			short getIndentation(void) const;
			void setIndentation(const short indentation);

			/** Get/set the keyword. To be used in cases where it is unkown what the keyword is or in case serialisation is spread across
				multiple objects.
			*/
			const String& getKeyword(void) const;
			void setKeyword(const String& keyword);

		protected:
			ParticleSystemWriter mParticleSystemWriter;
			String mPath;
			std::ofstream mScriptFile;
			StringStream mScript;
			String mScriptString;
			short mTab[5];
			short mIndentation;
			String mKeyword;
			bool mStreamToFile;

			/** Concatenate base string and string 's', with the proper indentation.
			*/
			void _appendTabbedString(
				size_t index, 
				String& base, 
				const String& s, 
				short indentation);

			/** Writes to a file or to a string
			*/
			void _stream (const String& string);
	};

}
#endif
