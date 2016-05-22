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

#ifndef __PU_DO_ENABLE_COMPONENT_EVENT_HANDLER_H__
#define __PU_DO_ENABLE_COMPONENT_EVENT_HANDLER_H__

#include "ParticleUniverseEventHandler.h"
#include "ParticleUniverseSystem.h"

namespace ParticleUniverse {

	/** This class makes it possible to enable or disable a Component. This component is an 
		named emitter, affector or technique.
    */
	class _ParticleUniverseExport DoEnableComponentEventHandler : public ParticleEventHandler {
	public:
		DoEnableComponentEventHandler();
	    virtual ~DoEnableComponentEventHandler() {}

		/** Get the name of the component that must be enabled or disabled.
	    */
		const String & getComponentName() const { return mComponentName; }

		/** Set the name of the component that must be enabled or disables.
	    */
		void setComponentName(const String & componentName) { mComponentName = componentName; }

		/** Get the value that identifies whether the component must be enabled or disabled.
	    */
		bool isComponentEnabled() const { return mComponentEnabled; }

		/** Set the value that identifies whether the component must be enabled or disabled.
	    */
		void setComponentEnabled(bool enabled) { mComponentEnabled = enabled; }

		/** Get the value that identifies whether the component must be enabled or disabled.
	    */
		ComponentType getComponentType() const { return mComponentType; }

		/** Set the value that identifies whether the component must be enabled or disabled.
	    */
		void setComponentType(ComponentType componentType) { mComponentType = componentType; }

		/** If the _handle() function of this class is invoked (by an Observer), it searches the 
			ParticleEmitter, ParticleAffector or ParticleTechnique defined by the its name. 
			The ParticleEmitter/Affector is either part of the ParticleTechnique in which the 
			DoEnableComponentEventHandler is defined, or if the ParticleEmitter/Affector is not 
			found, other ParticleTechniques are searched.
	    */
		virtual void _handle(ParticleTechnique * particleTechnique, Particle * particle, Real timeElapsed);

		/** Copy attributes to another event handler.
	    */
		virtual void copyAttributesTo(ParticleEventHandler * eventHandler);

	protected:
		// Identifies the name of component
		String mComponentName;

		// Identifies the type of component
		ComponentType mComponentType;

		/** Determines whether the Component must be enabled or disabled.
		*/
		bool mComponentEnabled;
	};

} /* namespace ParticleUniverse */

#endif /* __PU_DO_ENABLE_COMPONENT_EVENT_HANDLER_H__ */
