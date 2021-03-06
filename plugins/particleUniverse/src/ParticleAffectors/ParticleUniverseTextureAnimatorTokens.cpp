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

#include "ParticleAffectors/ParticleUniverseTextureAnimatorTokens.h"

#include "ParticleUniverseScriptSerializer.h"

#include "ParticleAffectors/ParticleUniverseTextureAnimator.h"

namespace ParticleUniverse {
	
	bool TextureAnimatorTranslator::translateChildProperty(ScriptCompiler * compiler, const AbstractNodePtr & node) {
		PropertyAbstractNode * prop = reinterpret_cast<PropertyAbstractNode *>(node.get());
		ParticleAffector * af = any_cast<ParticleAffector *>(prop->parent->context);
		TextureAnimator * affector = static_cast<TextureAnimator *>(af);

		if (prop->name == token[TOKEN_TIME_STEP]) {
			// Property: time_step
			if (passValidateProperty(compiler, prop, token[TOKEN_TIME_STEP], VAL_REAL)) {
				Real val = 0.0;
				if (getReal(prop->values.front(), &val)) {
					affector->setAnimationTimeStep(val);
					return true;
				}
			}
		} else if (prop->name == token[TOKEN_TEXANIM_TIME_STEP]) {
			// Property: time_step_animation (deprecated and replaced by 'time_step')
			if (passValidateProperty(compiler, prop, token[TOKEN_TEXANIM_TIME_STEP], VAL_REAL)) {
				Real val = 0.0;
				if (getReal(prop->values.front(), &val)) {
					affector->setAnimationTimeStep(val);
					return true;
				}
			}
		} else if (prop->name == token[TOKEN_START_TEXANIM_TEXCOORDS_RANGE]) {
			// Property: start_texture_coords_range
			if (passValidateProperty(compiler, prop, token[TOKEN_START_TEXANIM_TEXCOORDS_RANGE], VAL_UINT)) {
				uint val = 0;
				if (getUInt(prop->values.front(), &val)) {
					affector->setTextureCoordsStart(uint16(val));
					return true;
				}
			}
		} else if (prop->name == token[TOKEN_TEXANIM_TEXCOORDS_START]) {
			// Property: texture_coords_start (deprecated and replaced by start_texture_coords_range)
			if (passValidateProperty(compiler, prop, token[TOKEN_TEXANIM_TEXCOORDS_START], VAL_UINT)) {
				uint val = 0;
				if (getUInt(prop->values.front(), &val)) {
					affector->setTextureCoordsStart(uint16(val));
					return true;
				}
			}
		} else if (prop->name == token[TOKEN_END_TEXANIM_TEXCOORDS_RANGE]) {
			// Property: texture_coords_end
			if (passValidateProperty(compiler, prop, token[TOKEN_END_TEXANIM_TEXCOORDS_RANGE], VAL_UINT)) {
				uint val = 0;
				if (getUInt(prop->values.front(), &val)) {
					affector->setTextureCoordsEnd(uint16(val));
					return true;
				}
			}
		} else if (prop->name == token[TOKEN_TEXANIM_TEXCOORDS_END]) {
			// Property: texture_coords_end (deprecated and replaced by end_texture_coords_range)
			if (passValidateProperty(compiler, prop, token[TOKEN_TEXANIM_TEXCOORDS_END], VAL_UINT)) {
				uint val = 0;
				if (getUInt(prop->values.front(), &val)) {
					affector->setTextureCoordsEnd(uint16(val));
					return true;
				}
			}
		} else if (prop->name == token[TOKEN_TEXANIM_ANIMATION_TYPE]) {
			// Property: texture_animation_type
			if (passValidateProperty(compiler, prop, token[TOKEN_TEXANIM_ANIMATION_TYPE], VAL_STRING)) {
				String val;
				if (getString(prop->values.front(), &val)) {
					if (val == token[TOKEN_TEXANIM_LOOP]) {
						affector->setTextureAnimationType(TextureAnimator::TAT_LOOP);
						return true;
					} else if (val == token[TOKEN_TEXANIM_UP_DOWN]) {
						affector->setTextureAnimationType(TextureAnimator::TAT_UP_DOWN);
						return true;
					} else if (val == token[TOKEN_TEXANIM_RANDOM]) {
						affector->setTextureAnimationType(TextureAnimator::TAT_RANDOM);
						return true;
					}
				}
			}
		} else if (prop->name == token[TOKEN_TEXANIM_START_RANDOM]) {
			// Property: texture_start_random
			if (passValidateProperty(compiler, prop, token[TOKEN_TEXANIM_START_RANDOM], VAL_BOOL)) {
				bool val;
				if (getBoolean(prop->values.front(), &val)) {
					affector->setStartRandom(val);
					return true;
				}
			}
		}

		return false;
	}
	
	bool TextureAnimatorTranslator::translateChildObject(ScriptCompiler * compiler, const AbstractNodePtr & node) {
		// No objects
		return false;
	}
	
	void TextureAnimatorWriter::write(ParticleScriptSerializer * serializer, const IElement * element) {
		// Cast the element to a TextureAnimator
		const TextureAnimator * affector = static_cast<const TextureAnimator *>(element);

		// Write the header of the TextureAnimator
		serializer->writeLine(token[TOKEN_AFFECTOR], affector->getAffectorType(), affector->getName(), 8);
		serializer->writeLine("{", 8);

		// Write base attributes
		ParticleAffectorWriter::write(serializer, element);

		// Write own attributes
		if (!almostEquals(affector->getAnimationTimeStep(), TextureAnimator::DEFAULT_TIME_STEP)) {
			serializer->writeLine(token[TOKEN_TIME_STEP], StringConverter::toString(affector->getAnimationTimeStep()), 12);
		}
		if (affector->getTextureCoordsStart() != TextureAnimator::DEFAULT_TEXCOORDS_START) {
			serializer->writeLine(token[TOKEN_START_TEXANIM_TEXCOORDS_RANGE], StringConverter::toString(affector->getTextureCoordsStart()), 12);
		}
		if (affector->getTextureCoordsEnd() != TextureAnimator::DEFAULT_TEXCOORDS_END) {
			serializer->writeLine(token[TOKEN_END_TEXANIM_TEXCOORDS_RANGE], StringConverter::toString(affector->getTextureCoordsEnd()), 12);
		}
		if (affector->isStartRandom() != TextureAnimator::DEFAULT_START_RANDOM) {
			serializer->writeLine(token[TOKEN_TEXANIM_START_RANDOM], StringConverter::toString(affector->isStartRandom()), 12);
		}

		if (affector->getTextureAnimationType() != TextureAnimator::DEFAULT_ANIMATION_TYPE) {
			String animationType = token[TOKEN_TEXANIM_LOOP];
			if (affector->getTextureAnimationType() == TextureAnimator::TAT_LOOP) {
				animationType = token[TOKEN_TEXANIM_LOOP];
			} else if (affector->getTextureAnimationType() == TextureAnimator::TAT_UP_DOWN) {
				animationType = token[TOKEN_TEXANIM_UP_DOWN];
			} else if (affector->getTextureAnimationType() == TextureAnimator::TAT_RANDOM) {
				animationType = token[TOKEN_TEXANIM_RANDOM];
			}
			serializer->writeLine(token[TOKEN_TEXANIM_ANIMATION_TYPE], animationType, 12);
		}

		// Write the close bracket
		serializer->writeLine("}", 8);
	}

} /* namespace ParticleUniverse */
