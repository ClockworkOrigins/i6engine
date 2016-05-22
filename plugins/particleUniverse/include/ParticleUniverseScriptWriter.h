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

#ifndef __PU_SCRIPT_WRITER_H__
#define __PU_SCRIPT_WRITER_H__

#include "ParticleUniversePrerequisites.h"

namespace ParticleUniverse {

	class IElement;
	class ParticleScriptSerializer;

	/*	A class that implements the ScriptWriter, is responsible for writing (serialization) to a particle script.
	*/
	class _ParticleUniverseExport ScriptWriter {
	public:
		ScriptWriter() {}
		virtual ~ScriptWriter() {}

		/** Child classes must implement this pure virtual function, which must be used to write an object or attribute to
			a particle script.
		**/
		virtual void write(ParticleScriptSerializer * serializer, const IElement * element) = 0;
	};

} /* namespace ParticleUniverse */

#endif /* __PU_SCRIPT_WRITER_H__ */
