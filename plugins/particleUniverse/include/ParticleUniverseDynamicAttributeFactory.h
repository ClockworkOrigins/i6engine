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

#ifndef __PU_DYNAMIC_ATTRIBUTE_FACTORY_H__
#define __PU_DYNAMIC_ATTRIBUTE_FACTORY_H__

#include "ParticleUniverseDynamicAttribute.h"
#include "ParticleUniverseDynamicAttributeTokens.h"

namespace ParticleUniverse {

	/** This factory class is responsible for creating a DynamicAttribute object.
    */
	class _ParticleUniverseExport DynamicAttributeFactory : public ScriptWriter, public FactoryAlloc {
	public:
		DynamicAttributeFactory() {}
	    virtual ~DynamicAttributeFactory() {}

		/** 
	    */
		template <class T>
		T * createDynamicAttribute() {
			return PU_NEW_T(T, MEMCATEGORY_SCENE_OBJECTS)();
		}

		/** 
	    */
		DynamicAttribute * cloneDynamicAttribute(DynamicAttribute * dynamicAttribute) {
			if (dynamicAttribute) {
				switch (dynamicAttribute->getType()) {
				case DynamicAttribute::DAT_FIXED: {
					DynamicAttributeFixed * dynAttr = createDynamicAttribute<DynamicAttributeFixed>();
					dynamicAttribute->copyAttributesTo(dynAttr);
					return dynAttr;	
				}

				case DynamicAttribute::DAT_RANDOM: {
					DynamicAttributeRandom * dynAttr = createDynamicAttribute<DynamicAttributeRandom>();
					dynamicAttribute->copyAttributesTo(dynAttr);
					return dynAttr;
				}

				case DynamicAttribute::DAT_CURVED: {
					DynamicAttributeCurved* dynAttr = createDynamicAttribute<DynamicAttributeCurved>();
					dynamicAttribute->copyAttributesTo(dynAttr);
					return dynAttr;
				}

				case DynamicAttribute::DAT_OSCILLATE: {
					DynamicAttributeOscillate* dynAttr = createDynamicAttribute<DynamicAttributeOscillate>();
					dynamicAttribute->copyAttributesTo(dynAttr);
					return dynAttr;
				}
				default: {
					break;
				}
				}
			}

			return nullptr;
		}
			
		/** Delete a DynamicAttribute
	    */
		void destroyDynamicAttribute(DynamicAttribute * dynamicAttribute) {
			PU_DELETE_T(dynamicAttribute, DynamicAttribute, MEMCATEGORY_SCENE_OBJECTS);
		}

		/** See ScriptWriter */
		virtual void write(ParticleScriptSerializer * serializer, const IElement * element) {
			// Delegate to mDynamicAttributeWriter
			mDynamicAttributeWriter.write(serializer, element);
		}

		/** Returns the default value of a DynamicAttribute. Not for general use. */
		Real _getDefaultValue(const DynamicAttribute * dynAttribute) {
			if (!dynAttribute) {
				return 0;
			}

			switch (dynAttribute->getType()) {
			case DynamicAttribute::DAT_FIXED: {
				const DynamicAttributeFixed * dynFixed = static_cast<const DynamicAttributeFixed *>(dynAttribute);
				DynamicAttributeFixed d(*dynFixed); // Needed to get rid of the const, because the getValue function is not a const!
				return d.getValue();
			}

			case DynamicAttribute::DAT_RANDOM: {
				const DynamicAttributeRandom * dynRandom = static_cast<const DynamicAttributeRandom *>(dynAttribute);
				DynamicAttributeRandom d(*dynRandom); // Needed to get rid of the const
				return d.getValue();
			}

			case DynamicAttribute::DAT_CURVED: {
				const DynamicAttributeCurved * dynCurved = static_cast<const DynamicAttributeCurved *>(dynAttribute);
				DynamicAttributeCurved d(*dynCurved); // Needed to get rid of the const
				return d.getValue();
			}

			case DynamicAttribute::DAT_OSCILLATE: {
				const DynamicAttributeOscillate * dynOscillate = static_cast<const DynamicAttributeOscillate *>(dynAttribute);
				DynamicAttributeOscillate d(*dynOscillate); // Needed to get rid of the const
				return d.getValue();
			}
			default: {
				break;
			}
			}

			return 0; // Take 0 as the default
		}

	protected:
		DynamicAttributeWriter mDynamicAttributeWriter;
	};

} /* namespace ParticleUniverse */

#endif /* __PU_DYNAMIC_ATTRIBUTE_FACTORY_H__ */
