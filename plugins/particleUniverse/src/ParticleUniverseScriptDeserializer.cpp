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

#include "ParticleUniverseScriptDeserializer.h"

namespace ParticleUniverse {
	
	ScriptTranslator::ScriptTranslator() {
	}
	
	bool ScriptTranslator::getVector2(AbstractNodeList::const_iterator i, AbstractNodeList::const_iterator end, Vector2 * result, int maxEntries) {
		int n = 0;
		while (i != end && n < maxEntries) {
			float v = 0;
			if (getFloat(*i, &v)) {
				switch (n) {
				case 0: {
					result->x = v;
					break;
				}
				case 1: {
					result->y = v;
					break;
				}
				default: {
					break;
				}
				}
			} else {
				return false;
			}
			++n;
			++i;
		}
		// return error if we found less than xy before end, unless constrained
		return (n >= 2 || n == maxEntries);
	}
	
	bool ScriptTranslator::getVector3(AbstractNodeList::const_iterator i, AbstractNodeList::const_iterator end, Vector3 * result, int maxEntries) {
		int n = 0;
		while (i != end && n < maxEntries) {
			float v = 0;
			if (getFloat(*i, &v)) {
				switch (n) {
				case 0: {
					result->x = v;
					break;
				}
				case 1: {
					result->y = v;
					break;
				}
				case 2: {
					result->z = v;
					break;
				}
				default: {
					break;
				}
				}
			} else {
				return false;
			}
			++n;
			++i;
		}
		// return error if we found less than xyz before end, unless constrained
		return (n >= 3 || n == maxEntries);
	}
	
	bool ScriptTranslator::getVector4(AbstractNodeList::const_iterator i, AbstractNodeList::const_iterator end, Vector4 * result, int maxEntries) {
		int n = 0;
		while (i != end && n < maxEntries) {
			float v = 0;
			if (getFloat(*i, &v)) {
				switch (n) {
				case 0: {
					result->x = v;
					break;
				}
				case 1: {
					result->y = v;
					break;
				}
				case 2: {
					result->z = v;
					break;
				}
				case 3: {
					result->w = v;
					break;
				}
				default: {
					break;
				}
				}
			} else {
				return false;
			}
			++n;
			++i;
		}
		// return error if we found less than xyz before end, unless constrained
		return (n >= 4 || n == maxEntries);
	}
	
	bool ScriptTranslator::getQuaternion(AbstractNodeList::const_iterator i, AbstractNodeList::const_iterator end, Quaternion * result, int maxEntries) {
		int n = 0;
		while (i != end && n < maxEntries) {
			float v = 0;
			if (getFloat(*i, &v)) {
				switch (n) {
				case 0: {
					result->w = v;
					break;
				}
				case 1: {
					result->x = v;
					break;
				}
				case 2: {
					result->y = v;
					break;
				}
				case 3: {
					result->z = v;
					break;
				}
				default: {
					break;
				}
				}
			} else {
				return false;
			}
			++n;
			++i;
		}
		// return error if we found less than xyz before end, unless constrained
		return (n >= 4 || n == maxEntries);
	}
	
	bool ScriptTranslator::passValidateProperty(ScriptCompiler * compiler, PropertyAbstractNode * prop, const String & t, ValidationType validationType) {
		if (!passValidatePropertyNoValues(compiler, prop, t)) {
			return false;
		}

		bool ret = true;
		switch (validationType) {
		case VAL_BOOL: {
			ret = passValidatePropertyNumberOfValues(compiler, prop, t, 1) && passValidatePropertyValidBool(compiler, prop);
			break;
		}
		case VAL_COLOURVALUE: {
			ret = passValidatePropertyNumberOfValuesRange(compiler, prop, t, 3, 4);
			break;
		}
		case VAL_INT: {
			ret = passValidatePropertyNumberOfValues(compiler, prop, t, 1) && passValidatePropertyValidInt(compiler, prop);
			break;
		}
		case VAL_QUATERNION: {
			ret = passValidatePropertyNumberOfValues(compiler, prop, t, 4) && passValidatePropertyValidQuaternion(compiler, prop);
			break;
		}
		case VAL_REAL: {
			ret = passValidatePropertyNumberOfValues(compiler, prop, t, 1) && passValidatePropertyValidReal(compiler, prop);
			break;
		}
		case VAL_STRING: {
			ret = passValidatePropertyNumberOfValues(compiler, prop, t, 1);
			break;
		}
		case VAL_UINT: {
			ret = passValidatePropertyNumberOfValues(compiler, prop, t, 1) && passValidatePropertyValidUint(compiler, prop);
			break;
		}
		case VAL_VECTOR2: {
			ret = passValidatePropertyNumberOfValues(compiler, prop, t, 2) && passValidatePropertyValidVector2(compiler, prop);
			break;
		}
		case VAL_VECTOR3: {
			ret = passValidatePropertyNumberOfValues(compiler, prop, t, 3) && passValidatePropertyValidVector3(compiler, prop);
			break;
		}
		case VAL_VECTOR4: {
			ret = passValidatePropertyNumberOfValues(compiler, prop, t, 4) && passValidatePropertyValidVector4(compiler, prop);
			break;
		}
		default: {
			break;
		}
		}

		return ret;
	}
	
	bool ScriptTranslator::passValidatePropertyNoValues(ScriptCompiler * compiler, PropertyAbstractNode * prop, const String & t) {
		if (prop->values.empty()) {
			compiler->addError(ScriptCompiler::CE_INVALIDPARAMETERS, prop->file, int(prop->line), "PU Compiler: No values found for " + t + ".");
			return false;
		}
		return true;
	}
	
	bool ScriptTranslator::passValidatePropertyNumberOfValues(ScriptCompiler * compiler, PropertyAbstractNode * prop, const String & t, ushort numberOfValues) {
		if (prop->values.size() > numberOfValues) {
			compiler->addError(ScriptCompiler::CE_FEWERPARAMETERSEXPECTED, prop->file, int(prop->line), "PU Compiler: " + t + " must have " + StringConverter::toString(numberOfValues) + " argument(s).");
			return false;
		}
		return true;
	}
	
	bool ScriptTranslator::passValidatePropertyNumberOfValuesRange(ScriptCompiler * compiler, PropertyAbstractNode * prop, const String & t, ushort minNumberOfValues, ushort maxNumberOfValues) {
		if (prop->values.size() < minNumberOfValues || prop->values.size() > maxNumberOfValues) {
			compiler->addError(ScriptCompiler::CE_FEWERPARAMETERSEXPECTED, prop->file, int(prop->line), "PU Compiler: " + t + " must have between" + StringConverter::toString(minNumberOfValues) + " and " + StringConverter::toString(maxNumberOfValues) + " argument(s).");
			return false;
		}
		return true;
	}
	
	bool ScriptTranslator::passValidatePropertyValidReal(ScriptCompiler * compiler, PropertyAbstractNode * prop) {
		Real val = 0.0;
		if (getReal(prop->values.front(), &val)) {
			return true;
		}

		compiler->addError(ScriptCompiler::CE_INVALIDPARAMETERS, prop->file, int(prop->line), "PU Compiler: " + prop->values.front()->getValue() + " is not a valid Real");
		return false;
	}
	
	bool ScriptTranslator::passValidatePropertyValidInt(ScriptCompiler * compiler, PropertyAbstractNode * prop) {
		int val = 0;
		if (getInt(prop->values.front(), &val)) {
			return true;
		}

		compiler->addError(ScriptCompiler::CE_INVALIDPARAMETERS, prop->file, int(prop->line), "PU Compiler: " + prop->values.front()->getValue() + " is not a valid int");
		return false;
	}
	
	bool ScriptTranslator::passValidatePropertyValidUint(ScriptCompiler * compiler, PropertyAbstractNode * prop) {
		uint val = 0;
		if (getUInt(prop->values.front(), &val)) {
			return true;
		}

		compiler->addError(ScriptCompiler::CE_INVALIDPARAMETERS, prop->file, int(prop->line), "PU Compiler: " + prop->values.front()->getValue() + " is not a valid uint");
		return false;
	}
	
	bool ScriptTranslator::passValidatePropertyValidBool(ScriptCompiler * compiler, PropertyAbstractNode * prop) {
		bool val;
		if (getBoolean(prop->values.front(), &val)) {
			return true;
		}

		compiler->addError(ScriptCompiler::CE_INVALIDPARAMETERS, prop->file, int(prop->line), "PU Compiler: " + prop->values.front()->getValue() + " is not a valid bool");
		return false;
	}
	
	bool ScriptTranslator::passValidatePropertyValidVector2(ScriptCompiler * compiler, PropertyAbstractNode * prop) {
		Vector2 val;
		if (getVector2(prop->values.begin(), prop->values.end(), &val)) {
			return true;
		}

		compiler->addError(ScriptCompiler::CE_INVALIDPARAMETERS, prop->file, int(prop->line), "PU Compiler: " + prop->values.front()->getValue() + " is not a valid Vector2");
		return false;
	}
	
	bool ScriptTranslator::passValidatePropertyValidVector3(ScriptCompiler * compiler, PropertyAbstractNode * prop) {
		Vector3 val;
		if (getVector3(prop->values.begin(), prop->values.end(), &val)) {
			return true;
		}

		compiler->addError(ScriptCompiler::CE_INVALIDPARAMETERS, prop->file, int(prop->line), "PU Compiler: " + prop->values.front()->getValue() + " is not a valid Vector3");
		return false;
	}
	
	bool ScriptTranslator::passValidatePropertyValidVector4(ScriptCompiler * compiler, PropertyAbstractNode * prop) {
		Vector4 val;
		if (getVector4(prop->values.begin(), prop->values.end(), &val)) {
			return true;
		}

		compiler->addError(ScriptCompiler::CE_INVALIDPARAMETERS, prop->file, int(prop->line), "PU Compiler: " + prop->values.front()->getValue() + " is not a valid Vector4");
		return false;
	}
	
	bool ScriptTranslator::passValidatePropertyValidQuaternion(ScriptCompiler * compiler, PropertyAbstractNode * prop) {
		Quaternion val;
		if (getQuaternion(prop->values.begin(), prop->values.end(), &val)) {
			return true;
		}

		compiler->addError(ScriptCompiler::CE_INVALIDPARAMETERS, prop->file, int(prop->line), "PU Compiler: " + prop->values.front()->getValue() + " is not a valid Quaternion");
		return false;
	}
	
	void ScriptTranslator::errorUnexpectedToken(ScriptCompiler * compiler, AbstractNodePtr t) {
		compiler->addError(ScriptCompiler::CE_UNEXPECTEDTOKEN, t.getPointer()->file, int(t.getPointer()->line), "PU Compiler: token is not recognized");
	}
	
	void ScriptTranslator::errorUnexpectedProperty(ScriptCompiler * compiler, PropertyAbstractNode * prop) {
		compiler->addError(ScriptCompiler::CE_UNEXPECTEDTOKEN, prop->file, int(prop->line), "PU Compiler: token \"" + prop->name + "\" is not recognized");
	}

} /* namespace ParticleUniverse */
