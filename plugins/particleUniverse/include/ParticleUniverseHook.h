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

#ifndef __PU_HOOK_H__
#define __PU_HOOK_H__

#include "ParticleUniversePrerequisites.h"

namespace ParticleUniverse {

	/** The Hook class is a template helper class that is used to call the hook functions of objects (Affectors, 
		Externs, ...) registered at the ParticleSystem. This prevents that the same code is duplicated 
		too much.
    */
	template<typename T>
	class _ParticleUniverseExport Hook {
	public:
		typedef vector<T *> HookList;

		/** Templated version to prepare objects.
		*/
		static inline void _prepare(HookList * list, ParticleTechnique * technique) {
			if (list->empty()) {
				return;
			}

			for (typename HookList::iterator it = list->begin(); it != list->end(); ++it) {
				(*it)->_prepare(technique);
			}
		}

		/** Templated version to unprepare objects.
		*/
		static inline void _unprepare(HookList * list, ParticleTechnique * technique) {
			if (list->empty()) {
				return;
			}

			for (typename HookList::iterator it = list->begin(); it != list->end(); ++it) {
				(*it)->_unprepare(technique);
			}
		}

		/** Templated version to notify objects that the ParticleSystem starts
		*/
		static inline void _notifyStart(HookList * list) {
			if (list->empty()) {
				return;
			}

			for (typename HookList::iterator it = list->begin(); it != list->end(); ++it) {
				(*it)->_notifyStart();
			}
		}

		/** Templated version to notify objects that the ParticleSystem pauses
		*/
		static inline void _notifyPause(HookList * list) {
			if (list->empty()) {
				return;
			}

			for (typename HookList::iterator it = list->begin(); it != list->end(); ++it) {
				(*it)->_notifyPause();
			}
		}

		/** Templated version to notify objects that the ParticleSystem resumes
		*/
		static inline void _notifyResume(HookList * list) {
			if (list->empty()) {
				return;
			}

			for (typename HookList::iterator it = list->begin(); it != list->end(); ++it) {
				(*it)->_notifyResume();
			}
		}

		/** Templated version to notify objects that the ParticleSystem stops
		*/
		static inline void _notifyStop(HookList * list) {
			if (list->empty()) {
				return;
			}

			for (typename HookList::iterator it = list->begin(); it != list->end(); ++it) {
				(*it)->_notifyStop();
			}
		}

		/** Templated version of _preProcessParticles.
		@remarks
			_preProcessParticles allows actions before the individual particles are processed.
		*/
		static inline void _preProcessParticles(HookList * list, ParticleTechnique * technique, Real timeElapsed) {
			if (list->empty()) {
				return;
			}

			for (typename HookList::iterator it = list->begin(); it != list->end(); ++it) {
				(*it)->_preProcessParticles(technique, timeElapsed);
			}
		}

		/** Templated version to process a particle.
		*/
		static inline void _processParticle(HookList * list, ParticleTechnique * particleTechnique, Particle * particle, Real timeElapsed, bool firstParticle) {
			if (list->empty()) {
				return;
			}

			for (typename HookList::iterator it = list->begin(); it != list->end(); ++it) {
				(*it)->_processParticle(particleTechnique, particle, timeElapsed, firstParticle);
			}
		}

		/** Templated version of _postProcessParticles.
		@remarks
			_postProcessParticles allows actions after the individual particles are processed.
		*/
		static inline void _postProcessParticles(HookList * list, ParticleTechnique * technique, Real timeElapsed) {
			if (list->empty()) {
				return;
			}

			for (typename HookList::iterator it = list->begin(); it != list->end(); ++it) {
				(*it)->_postProcessParticles(technique, timeElapsed);
			}
		}
	};

} /* namespace ParticleUniverse */

#endif /* __PU_HOOK_H__ */
