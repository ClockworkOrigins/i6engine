/*
-----------------------------------------------------------------------------------------------
Copyright (C) 2013 Henry van Merode. All rights reserved.

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
-----------------------------------------------------------------------------------------------
*/

#include "ParticleUniversePCH.h"

#ifndef PARTICLE_UNIVERSE_EXPORTS
#define PARTICLE_UNIVERSE_EXPORTS
#endif

#include "ParticleEmitters/ParticleUniverseSphereSurfaceEmitter.h"
#include "ParticleEmitters/ParticleUniverseSphereSurfaceEmitterTokens.h"

namespace ParticleUniverse
{
	//-----------------------------------------------------------------------
	bool SphereSurfaceEmitterTranslator::translateChildProperty(ScriptCompiler* compiler, const AbstractNodePtr &node)
	{
		PropertyAbstractNode* prop = reinterpret_cast<PropertyAbstractNode*>(node.get());
		ParticleEmitter* em = any_cast<ParticleEmitter*>(prop->parent->context);
		SphereSurfaceEmitter* emitter = static_cast<SphereSurfaceEmitter*>(em);

		if (prop->name == token[TOKEN_RADIUS])
		{
			// Property: radius
			if (passValidateProperty(compiler, prop, token[TOKEN_RADIUS], VAL_REAL))
			{
				Real val = 0.0f;
				if(getReal(prop->values.front(), &val))
				{
					emitter->setRadius(val);
					return true;
				}
			}
		}
		else if (prop->name == token[TOKEN_SPHERE_RADIUS])
		{
			// Property: sphere_surface_em_radius (deprecated and replaced by 'radius')
			if (passValidateProperty(compiler, prop, token[TOKEN_SPHERE_RADIUS], VAL_REAL))
			{
				Real val = 0.0f;
				if(getReal(prop->values.front(), &val))
				{
					emitter->setRadius(val);
					return true;
				}
			}
		}

		return false;
	}
	//-----------------------------------------------------------------------
	bool SphereSurfaceEmitterTranslator::translateChildObject(ScriptCompiler* compiler, const AbstractNodePtr &node)
	{
		// No objects
		return false;
	}
	//-----------------------------------------------------------------------
	//-----------------------------------------------------------------------
	//-----------------------------------------------------------------------
	void SphereSurfaceEmitterWriter::write(ParticleScriptSerializer* serializer, const IElement* element)
	{
		// Cast the element to a SphereSurfaceEmitter
		const SphereSurfaceEmitter* emitter = static_cast<const SphereSurfaceEmitter*>(element);

		// Write the header of the SphereSurfaceEmitter
		serializer->writeLine(token[TOKEN_EMITTER], emitter->getEmitterType(), emitter->getName(), 8);
		serializer->writeLine("{", 8);

		// Write base attributes
		ParticleEmitterWriter::write(serializer, element);

		// Write own attributes
		if (emitter->getRadius() != SphereSurfaceEmitter::DEFAULT_RADIUS) serializer->writeLine(
			token[TOKEN_RADIUS], StringConverter::toString(emitter->getRadius()), 12);

		// Write the close bracket
		serializer->writeLine("}", 8);
	}

}
