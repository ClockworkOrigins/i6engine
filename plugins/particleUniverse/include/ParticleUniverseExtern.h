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

#ifndef __PU_EXTERN_H__
#define __PU_EXTERN_H__

#include "ParticleUniverseIAlias.h"
#include "ParticleUniverseIElement.h"
#include "ParticleUniverseTypes.h"

namespace ParticleUniverse {

	/** The Extern class is the abstract class for all extern components. Each subclass of Extern wraps a
		particular external component and forms a bridge between the external component and the
		ParticleUniverse (or to be more specific, the ParticleTechnique).
	@remarks
		External components can be used to add functionality to the ParticleUniverse plugin. An example is
		the addition of a physics library. Settings up the library is typically done within a dedicated
		subclass of Extern.
    */
	class _ParticleUniverseExport Extern : public IAlias, public IElement, public FXAlloc {
	public:
		Extern() : IElement(), mExternType(BLANK_STRING), mParentTechnique(nullptr), _mExternScale(Vector3::UNIT_SCALE) {}
		virtual ~Extern(){}

		/** Todo
	    */
		const String & getName() const { return mName; }
		void setName(const String & name) { mName = name; }

		/** Todo
	    */
		const String & getExternType() const { return mExternType; }
		void setExternType(const String & externType) { mExternType = externType; }

		/** Todo
	    */
		ParticleTechnique * getParentTechnique() const { return mParentTechnique; }
		void setParentTechnique(ParticleTechnique * parentTechnique) { mParentTechnique = parentTechnique; }

		/** Notify that the Particle System is rescaled.
	    */
		virtual void _notifyRescaled(const Vector3 & scale);

		/** Copy attributes to another extern object.
	    */
		virtual void copyAttributesTo(Extern * externObject);

		/** Copy parent attributes to another extern object.
	    */
		virtual void copyParentAttributesTo(Extern * externObject);

		/** Perform initialisation actions.
		@remarks
			The _prepare() function is automatically called during initialisation activities of a ParticleTechnique.
			Each subclass should implement this function to perform initialisation actions needed for the external
			component.
		@par
			This is a pure virtual function, to be sure that developers of an extern component don't forget to
			override this functions and perform setup/initialisation of the component. This is to prevent that 
			unexplainable errors occur because initialisation tasks where forgotten.
	    */
		virtual void _prepare(ParticleTechnique * technique) = 0;

		/** Reverse the actions from the _prepare.
	    */
		virtual void _unprepare(ParticleTechnique * particleTechnique) { /* No implementation */ }

		/** Perform activities when an Extern is started.
		*/
		virtual void _notifyStart() { /* Do nothing */ }

		/** Perform activities when an Extern is paused.
		*/
		virtual void _notifyPause() { /* Do nothing */ }

		/** Perform activities when an Extern is resumed.
		*/
		virtual void _notifyResume() { /* Do nothing */ }

		/** Perform activities when an Extern is stopped.
		*/
		virtual void _notifyStop() { /* Do nothing */ }

		/** Perform activities before the individual particles are processed.
		@remarks
			This function is called before the ParticleTechnique update-loop where all particles are traversed.
			the preProcess is typically used to perform calculations where the result must be used in 
			processing each individual particle.
	    */
		virtual void _preProcessParticles(ParticleTechnique * technique, Real timeElapsed) { /* Do nothing */ }

		/** Initialise a newly emitted particle.
		@param
			particle Pointer to a Particle to initialise.
		*/
		virtual void _initParticleForEmission(Particle * particle) { /* by default do nothing */ }

		/** Perform actions if a particle gets expired.
		*/
		virtual void _initParticleForExpiration(Particle * particle) { /* by default do nothing */ }

		/** Perform precalculations if the first Particle in the update-loop is processed.
		*/
		virtual void _firstParticle(ParticleTechnique * particleTechnique, Particle * particle, Real timeElapsed) { /* by default do nothing */ }

		/** Processes a particle.
		@remarks
			Some processing on a particle can be performed. This function is automatically called in the 
			ParticleTechnique update-loop where all particles are traversed.
	    */
		virtual void _processParticle(ParticleTechnique * technique, Particle * particle, Real timeElapsed, bool firstParticle);

		/** Actually processes a particle.
	    */
		virtual void _interface(ParticleTechnique * technique, Particle * particle, Real timeElapsed) = 0;

		/** Perform activities after the individual particles are processed.
		@remarks
			This function is called after the ParticleTechnique update-loop where all particles are traversed.
	    */
		virtual void _postProcessParticles(ParticleTechnique * technique, Real timeElapsed) { /* Do nothing */ }

	protected:
		// Type of extern
		String mExternType;

		// Name of the extern (optional)
		String mName;

		// Parent
		ParticleTechnique * mParentTechnique;

		/** Although the scale is on a Particle System level, the extern can also be scaled.
		*/
		Vector3 _mExternScale;
	};

} /* namespace ParticleUniverse */

#endif /* __PU_EXTERN_H__ */
