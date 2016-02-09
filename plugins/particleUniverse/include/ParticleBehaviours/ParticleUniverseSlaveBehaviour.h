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

#ifndef __PU_SLAVE_BEHAVIOUR_H__
#define __PU_SLAVE_BEHAVIOUR_H__

#include "ParticleUniverseBehaviour.h"

namespace ParticleUniverse {

	/** The SlaveBehaviour makes the particle act as a slave, so it follows another particle to which it is related.
    */
	class _ParticleUniverseExport SlaveBehaviour : public ParticleBehaviour {
	public:
		Particle * masterParticle;

		SlaveBehaviour();
	    virtual ~SlaveBehaviour() {}

		/** See ParticleBehaviour.
	    */
		virtual void _processParticle(ParticleTechnique * technique, Particle * particle, Real timeElapsed);

		/** 
	    */
		virtual void copyAttributesTo(ParticleBehaviour * behaviour);
	};

} /* namespace ParticleUniverse */

#endif /* __PU_SLAVE_BEHAVIOUR_H__ */
