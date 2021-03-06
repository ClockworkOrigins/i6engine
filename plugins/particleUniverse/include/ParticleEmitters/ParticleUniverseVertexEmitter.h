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

#ifndef __PU_VERTEX_EMITTER_H__
#define __PU_VERTEX_EMITTER_H__

#include "ParticleUniverseEmitter.h"

namespace ParticleUniverse {

	/** The VertexEmitter gradually generates spawn points, based on the edges of a mesh. 
		This means that in every ParticleTechnique-update, a few points are generated, 
		until all vertices of the mesh have been processed.
	@remarks
		Beware, there is no intermediate validation whether the mesh still exist (performance reasons), 
		so destroying the mesh before all vertices are processed will result in an exception.

		Todo:
		- Add normals to the mSpawnPositionList
    */
	class _ParticleUniverseExport VertexEmitter : public ParticleEmitter {
	public:
		using Particle::copyAttributesTo;

		// Constants
		static const ushort DEFAULT_STEP;
		static const ushort DEFAULT_SEGMENTS;
		static const ushort DEFAULT_ITERATIONS;

		VertexEmitter();
	    virtual ~VertexEmitter() {}

		/** 
	    */
		ushort getIterations() const;

		/** 
	    */
		void setIterations(ushort iterations);
			
		/** 
	    */
		ushort getSegments() const;

		/** 
	    */
		void setSegments(ushort segments);

		/** 
	    */
		ushort getStep() const;

		/** 
	    */
		void setStep(ushort step);

		/** 
	    */
		const String & getMeshName() const;

		/** 
	    */
		virtual void _notifyStart();
			
		/** 
	    */
		void setMeshName(const String & meshName);

		/** 
	    */
		virtual void _initParticlePosition(Particle * particle);

		/** 
	    */
		ushort _calculateRequestedParticles(Real timeElapsed);

		/** 
	    */
		virtual void copyAttributesTo(ParticleEmitter * emitter);

	protected:
		list<Vector3> mSpawnPositionList;
		const Ogre::VertexElement * mPositionElement;
		Ogre::VertexData * mVertexData;
		Ogre::HardwareVertexBufferSharedPtr mBuffer;
		unsigned char * mVertexBuffer;
		Ogre::Mesh * mMesh;
		Ogre::SubMesh * mSubMesh;
		bool mMeshProcessed;
		size_t mVertexSize;
		size_t mMaxVertexCount;
		size_t mVertexCount;
		ushort mSubMeshNumber;
		bool mAllVerticesProcessed;

		/** Determines the size of the steps that are used to traverse through the vertices.
		*/
		ushort mStep;

		/** Determines in how many segments an edge is divided. This results in spawn points.
		*/
		ushort mSegments;

		/** Defines the speed of generating spawnpoints. In each Particle Technique update
		'mIterations' vertices are traversed.
		@remarks
		Setting this attribute to a higher value is needed if the emssion rate of the emitter
		is high. On slower computers, emitting the particles may exceed generating the
		spawnpoints (because this is not done at once, but per Particle Tehcnique update).
		*/
		ushort mIterations;

		/** Determines the name of the mesh of which the vertices are used.
		*/
		String mMeshName;

		/** The _preProcessParticles() function is used to generate some spawn points in every update.
		*/
		virtual void _preProcessParticles(ParticleTechnique * technique, Real timeElapsed);

		/**
		*/
		void _setDefaults();

		/**
		*/
		void _loadMesh();

		/** Generate spawn points from a mesh. The points will not be generated all at once, but
		gradually. In each call, the points on an edge are generated, until all vertices
		have been processed. In that case, mAllVerticesProcessed is set to true and this
		function stops further processing.
		*/
		void _generatePoints();

		/**
		*/
		void _generatePointsFromMesh();

		/**
		*/
		void _generatePointsFromSubMesh();

		/**
		*/
		void _fillVertexBuffer();

		/**
		*/
		void _generatePointsFromVertexBuffer();

		/**
		*/
		void _generatePoints(const Vector3 & startVector, const Vector3 & endVector, Real lengthIncrement = 0.0);
	};

} /* namespace ParticleUniverse */

#endif /* __PU_VERTEX_EMITTER_H__ */
