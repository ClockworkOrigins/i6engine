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

#include "ParticleAffectors/ParticleUniversePlaneCollider.h"
#include "ParticleUniverseAxisAlignedBox.h"

namespace ParticleUniverse
{
	// Constants
	const Vector3 PlaneCollider::DEFAULT_NORMAL(0, 0, 0);

	//-----------------------------------------------------------------------
	PlaneCollider::PlaneCollider(void) : 
		BaseCollider(),
		mPredictedPosition(Vector3::ZERO),
		mNormal(DEFAULT_NORMAL)
	{
	}
	//-----------------------------------------------------------------------
	const Vector3 PlaneCollider::getNormal(void) const
	{
		return mNormal;
	}
	//-----------------------------------------------------------------------
	void PlaneCollider::setNormal(const Vector3& normal)
	{
		mNormal = normal;
		mPlane.redefine(mNormal, getDerivedPosition()); // Changed in 1.3.1
	}
	//-----------------------------------------------------------------------
	void PlaneCollider::_notifyRescaled(const Vector3& scale)
	{
		// Function added in 1.3.1
		ParticleAffector::_notifyRescaled(scale);
		mPlane.redefine(mNormal, getDerivedPosition());
	}
	//-----------------------------------------------------------------------
	void PlaneCollider::_affect(ParticleTechnique* particleTechnique, Particle* particle, Real timeElapsed)
	{
		mPredictedPosition = particle->position + mVelocityScale * particle->direction;
		bool collision = false;

		switch(mIntersectionType)
		{
			case BaseCollider::IT_POINT:
			{
				// Validate for a point-plane intersection (on the plane or the back side)
				// First determine whether it is now colliding (some affector made the particle move), else
				// determine whether it WILL be colliding
				if (mPlane.getDistance(particle->position) <= 0.0f)
				{
					// Collision detected (re-position the particle)
					particle->position -= mVelocityScale * particle->direction;
					collision = true;
				}
				else if (mPlane.getDistance(mPredictedPosition) <= 0.0f)
				{
					// Collision detected
					collision = true;
				}
			}
			break;

			case BaseCollider::IT_BOX:
			{
				// Validate for a box-plane intersection
				if (particle->particleType != Particle::PT_VISUAL)
					break;

				VisualParticle* visualParticle = static_cast<VisualParticle*>(particle);
				AxisAlignedBox box;
				populateAlignedBox(box,
					visualParticle->position, 
					visualParticle->width, 
					visualParticle->height,
					visualParticle->depth);
				if (box.intersects(mPlane))
				{
					// Collision detected (re-position the particle)
					particle->position -= mVelocityScale * particle->direction;
					collision = true;
				}
				else 
				{
					populateAlignedBox(box,
						mPredictedPosition, 
						visualParticle->width, 
						visualParticle->height,
						visualParticle->depth);
					if (box.intersects(mPlane))
					{
						// Collision detected
						collision = true;
					}
				}
			}
			break;
		}

		if (collision)
		{
			calculateDirectionAfterCollision(particle, timeElapsed);
			calculateRotationSpeedAfterCollision(particle);
			particle->addEventFlags(Particle::PEF_COLLIDED);
		}
	}
	//-----------------------------------------------------------------------
	void PlaneCollider::calculateDirectionAfterCollision(Particle* particle, Real timeElapsed)
	{
		Real directionLength = particle->direction.length();
		switch (mCollisionType)
		{
			case BaseCollider::CT_BOUNCE:
			{
				/** If the particle is on the plane or at the back of the plane, bounce it.
					Make use of the same formula as the sphere collider.
				*/
				particle->direction.normalise();
				particle->direction = 2 * (-particle->direction.dotProduct(-mNormal)) * -mNormal + particle->direction;

				// Adjust to original speed
				particle->direction *= directionLength;

				// Accelerate/slow down, using the bounce value
				particle->direction *= mBouncyness;
			}
			break;
			case BaseCollider::CT_FLOW:
			{
				/** Reset the position (just in front of the plane), but keep the direction.
				@remarks
					This is not really the correct way, because the particle 'jumps'. Maybe it is better to change 
					the direction parallel to the plane.
				*/
				particle->position += timeElapsed * directionLength * mNormal;
			}
			break;
		}
	}
	//-----------------------------------------------------------------------
	void PlaneCollider::copyAttributesTo (ParticleAffector* affector)
	{
		BaseCollider::copyAttributesTo(affector);

		PlaneCollider* planeCollider = static_cast<PlaneCollider*>(affector);
		planeCollider->setNormal(mNormal);
	}
}
