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

#include "ParticleEventHandlers/ParticleUniverseDoPlacementParticleEventHandler.h"
#include "ParticleUniversePhysicsActor.h"

namespace ParticleUniverse
{
	// Constants
	const unsigned int DoPlacementParticleEventHandler::DEFAULT_NUMBER_OF_PARTICLES = 1;

	//-----------------------------------------------------------------------
	DoPlacementParticleEventHandler::DoPlacementParticleEventHandler(void) : 
		ParticleEventHandler(),
		TechniqueListener(),
		mForceEmitterName(BLANK_STRING),
		mNumberOfParticles(DEFAULT_NUMBER_OF_PARTICLES),
		mFound(false),
		mAlwaysUsePosition(true),
		mEmitter(0),
		mTechnique(0),
		mInheritPosition(true),
		mInheritDirection(false),
		mInheritOrientation(false),
		mInheritTimeToLive(false),
		mInheritMass(false),
		mInheritTextureCoordinate(false),
		mInheritColour(false),
		mInheritParticleWidth(false),
		mInheritParticleHeight(false),
		mInheritParticleDepth(false),
		mBaseParticle(0)
	{
	}
	//-----------------------------------------------------------------------
	DoPlacementParticleEventHandler::~DoPlacementParticleEventHandler(void)
	{
		// We cannot remove this listener from mTechnique, because it is undetermined whether the ParticleTechnique 
		// still exist.
	}
	//-----------------------------------------------------------------------
	void DoPlacementParticleEventHandler::_handle (ParticleTechnique* particleTechnique, Particle* particle, Real timeElapsed)
	{
		if (!particle)
			return;

		if (!mFound)
		{
			ParticleTechnique* technique = particleTechnique;
			ParticleEmitter* emitter = particleTechnique->getEmitter(mForceEmitterName);
			if (!emitter)
			{
				// Search all techniques in this ParticleSystem for an emitter with the correct name
				ParticleSystem* system = particleTechnique->getParentSystem();
				size_t size = system->getNumTechniques();
				for(size_t i = 0; i < size; ++i)
				{
					technique = system->getTechnique(i);
					emitter = technique->getEmitter(mForceEmitterName);
					if (emitter)
						break;
				}
			}

			if (emitter)
			{
				mTechnique = technique;
				mEmitter = emitter;
				if (mTechnique)
				{
					mTechnique->addTechniqueListener(this);
				}
				mFound = true;
			}
			else
			{
				return;
			}
		}

		// Emit 1 or more particles
		if (mTechnique)
		{
			mBaseParticle = particle;
			mTechnique->forceEmission(mEmitter, mNumberOfParticles);
		}

		mBaseParticle = 0;
	}
	//-----------------------------------------------------------------------
	void DoPlacementParticleEventHandler::particleEmitted(ParticleTechnique* particleTechnique, Particle* particle)
	{
		if (!mBaseParticle)
			return;

		if (particle && mEmitter == particle->parentEmitter)
		{
			if (mInheritPosition)
			{
#ifdef PU_PHYSICS
				// Do not assume that the contact point is always used if a physics engine is used.
				if (!mAlwaysUsePosition && particle->physicsActor)
				{
					particle->position = mBaseParticle->physicsActor->contactPoint; // Store the contact point to spawn new particles on that position.
				}
				else
				{
					particle->position = mBaseParticle->position; // Store the particles' position to spawn new particles on that position.
				}
#else
				particle->position = mBaseParticle->position; // Store the particles' position to spawn new particles on that position.
#endif // PU_PHYSICS
				particle->originalPosition = particle->position;
			}
			if (mInheritDirection)
			{
				particle->direction = mBaseParticle->direction;
				particle->originalDirection = particle->direction;
				particle->originalDirectionLength = mBaseParticle->originalDirectionLength;
				particle->originalScaledDirectionLength = mBaseParticle->originalScaledDirectionLength;
				particle->originalVelocity = mBaseParticle->originalVelocity;
			}
			if (mInheritOrientation)
			{
				if (mBaseParticle->particleType == Particle::PT_VISUAL && particle->particleType == Particle::PT_VISUAL)
				{
					VisualParticle* visualBaseParticle = static_cast<VisualParticle*>(mBaseParticle);
					VisualParticle* visualParticle = static_cast<VisualParticle*>(particle);
					visualParticle->orientation = visualBaseParticle->orientation;
					visualParticle->originalOrientation = visualBaseParticle->originalOrientation;
				}
			}
			if (mInheritTimeToLive)
			{
				particle->timeToLive = mBaseParticle->timeToLive;
				particle->totalTimeToLive = mBaseParticle->totalTimeToLive;
				particle->timeFraction = mBaseParticle->timeFraction;
			}
			if (mInheritMass)
			{
				particle->mass = mBaseParticle->mass;
			}
			if (mInheritTextureCoordinate)
			{
				if (mBaseParticle->particleType == Particle::PT_VISUAL && particle->particleType == Particle::PT_VISUAL)
				{
					VisualParticle* visualBaseParticle = static_cast<VisualParticle*>(mBaseParticle);
					VisualParticle* visualParticle = static_cast<VisualParticle*>(particle);
					visualParticle->textureAnimationTimeStep = visualBaseParticle->textureAnimationTimeStep;
					visualParticle->textureAnimationTimeStepCount = visualBaseParticle->textureAnimationTimeStepCount;
					visualParticle->textureCoordsCurrent = visualBaseParticle->textureCoordsCurrent;
					visualParticle->textureAnimationDirectionUp = visualBaseParticle->textureAnimationDirectionUp;
				}
			}
			if (mInheritColour)
			{
				if (mBaseParticle->particleType == Particle::PT_VISUAL && particle->particleType == Particle::PT_VISUAL)
				{
					VisualParticle* visualBaseParticle = static_cast<VisualParticle*>(mBaseParticle);
					VisualParticle* visualParticle = static_cast<VisualParticle*>(particle);
					visualParticle->colour = visualBaseParticle->colour;
					visualParticle->originalColour = visualBaseParticle->originalColour;
				}
			}
			if (mInheritParticleWidth)
			{
				if (mBaseParticle->particleType == Particle::PT_VISUAL && particle->particleType == Particle::PT_VISUAL)
				{
					VisualParticle* visualBaseParticle = static_cast<VisualParticle*>(mBaseParticle);
					VisualParticle* visualParticle = static_cast<VisualParticle*>(particle);
					visualParticle->setOwnDimensions(visualBaseParticle->width, visualParticle->height, visualParticle->depth);
				}
			}
			if (mInheritParticleHeight)
			{
				if (mBaseParticle->particleType == Particle::PT_VISUAL && particle->particleType == Particle::PT_VISUAL)
				{
					VisualParticle* visualBaseParticle = static_cast<VisualParticle*>(mBaseParticle);
					VisualParticle* visualParticle = static_cast<VisualParticle*>(particle);
					visualParticle->setOwnDimensions(visualParticle->width, visualBaseParticle->height, visualParticle->depth);
				}
			}
			if (mInheritParticleDepth)
			{
				if (mBaseParticle->particleType == Particle::PT_VISUAL && particle->particleType == Particle::PT_VISUAL)
				{
					VisualParticle* visualBaseParticle = static_cast<VisualParticle*>(mBaseParticle);
					VisualParticle* visualParticle = static_cast<VisualParticle*>(particle);
					visualParticle->setOwnDimensions(visualParticle->width, visualParticle->height, visualBaseParticle->depth);
				}
			}
		}
	}
	//-----------------------------------------------------------------------
	void DoPlacementParticleEventHandler::setForceEmitterName(const String& forceEmitterName)
	{
		mForceEmitterName = forceEmitterName;
	}
	//-----------------------------------------------------------------------
	ParticleEmitter* DoPlacementParticleEventHandler::getForceEmitter(void) const
	{
		return mEmitter;
	}
	//-----------------------------------------------------------------------
	void DoPlacementParticleEventHandler::removeAsListener(void)
	{
		// Reset some values and remove this as a listener from the old technique.
		if (mTechnique)
		{
			mTechnique->removeTechniqueListener(this);
		}
		mFound = false;
		mEmitter = 0;
		mTechnique = 0;
	}
	//-----------------------------------------------------------------------
	void DoPlacementParticleEventHandler::copyAttributesTo (ParticleEventHandler* eventHandler)
	{
		ParticleEventHandler::copyAttributesTo(eventHandler);
		DoPlacementParticleEventHandler* doPlacementParticleEventHandler = static_cast<DoPlacementParticleEventHandler*>(eventHandler);
		doPlacementParticleEventHandler->setForceEmitterName(mForceEmitterName);
		doPlacementParticleEventHandler->setNumberOfParticles(mNumberOfParticles);
		doPlacementParticleEventHandler->mAlwaysUsePosition = mAlwaysUsePosition;
		doPlacementParticleEventHandler->mInheritPosition = mInheritPosition;
		doPlacementParticleEventHandler->mInheritDirection = mInheritDirection;
		doPlacementParticleEventHandler->mInheritOrientation = mInheritOrientation;
		doPlacementParticleEventHandler->mInheritTimeToLive = mInheritTimeToLive;
		doPlacementParticleEventHandler->mInheritMass = mInheritMass;
		doPlacementParticleEventHandler->mInheritTextureCoordinate = mInheritTextureCoordinate;
		doPlacementParticleEventHandler->mInheritColour = mInheritColour;
		doPlacementParticleEventHandler->mInheritParticleWidth = mInheritParticleWidth;
		doPlacementParticleEventHandler->mInheritParticleHeight = mInheritParticleHeight;
		doPlacementParticleEventHandler->mInheritParticleDepth = mInheritParticleDepth;
	}

}
