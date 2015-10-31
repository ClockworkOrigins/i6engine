/*
-----------------------------------------------------------------------------------------------
Copyright (C) 2014 Henry van Merode. All rights reserved.

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

#include "ParticleUniversePropertyWindow.h"

#include "ParticleUniverseSystemManager.h"

#include "ParticleUniverseControlPointDialog.h"
#include "ParticleUniverseEditComponent.h"

#include "i6engine/utils/i6eString.h"

#include "wx/ogre/utils.h"
#include "wx/propgrid/editors.h"

EnumPropertyWithButton::EnumPropertyWithButton(const wxString & label, const wxString & name, const wxArrayString & choices) : wxEnumProperty(label, name, choices) {
}

bool EnumPropertyWithButton::OnEvent(wxPropertyGrid * propgrid, wxWindow * wnd_primary, wxEvent & event) {
	return true;
}

PropertyWindow::PropertyWindow(wxWindow * parent, EditComponent * owner, const Ogre::String & name) : wxPropertyGrid(parent, wxID_ANY, wxPoint(0, 99999), wxSize(PROPERTIES_WIDTH, PROPERTIES_HEIGHT), wxPG_SPLITTER_AUTO_CENTER | wxPG_DEFAULT_STYLE, wxT("")), mOwner(owner), mName(name), _types() {
	/* Fix Version 1.3.1: Set y-coord by default to infinite (99999) to prevent that it is displayed over the listbox with templates. This prevents
	   jittering of the listbox (= wxTreeCtrl from  version 1.4).
	*/
	_types.Add(DYN_FIXED);
	_types.Add(DYN_RANDOM);
	_types.Add(DYN_CURVED);
	_types.Add(DYN_OSCILLATE);
	_initProperties();
//	Connect(wxID_ANY, wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) &wxPropertyGrid::OnCustomEditorEvent); TODO (Michael) uncomment
	Connect(wxID_ANY, wxEVT_PG_CHANGED, wxPropertyGridEventHandler(PropertyWindow::onPropertyChanged));
}

const Ogre::String & PropertyWindow::getComponentName() const {
	return mName;
}

void PropertyWindow::setComponentName(const Ogre::String & name) {
	mName = name;
	wxPGProperty * prop = GetPropertyByName(PRNL_NAME);
	if (prop) {
		wxString n = ogre2wx(name);
		prop->SetValueFromString(n);
	}
}

void PropertyWindow::_initProperties() {
	// Set the (internationalized) property names
	PRNL_NAME = _("Name");
	PRNL_POST_RGB = _(" RGB");
	PRNL_POST_ALPHA = _(" Alpha");
	PRNL_W = _(".w");
	PRNL_X = _(".x");
	PRNL_Y = _(".y");
	PRNL_Z = _(".z");
	PRNL_TYPE = _("Attribute type");
	PRNL_VALUE = _("Value");
	PRNL_MIN_VALUE = _("Minimum");
	PRNL_MAX_VALUE = _("Maximum");
	PRNL_OSC_TYPE = _("Oscillation type");
	PRNL_OSC_SINE = _("Sine");
	PRNL_OSC_SQUARE = _("Square");
	PRNL_OSC_FREQUENCY = _("Frequency");
	PRNL_OSC_PHASE = _("Phase");
	PRNL_OSC_BASE = _("Base");
	PRNL_OSC_AMPLITUDE = _("Amplitude");
	PRNL_CURVED_INTERPOLATION = _("Interpolation");
	PRNL_CONTROL_POINT = _("Control Point");
	PRNL_POSITION = _("Position");
	PRNL_CONTROL_POINT_X = _(".x");
	PRNL_CONTROL_POINT_Y = _(".y");
	PRNL_POSITION_X = _(".x");
	PRNL_POSITION_Y = _(".y");
	PRNL_POSITION_Z = _(".z");
	PRNL_FLOAT = _("Value");
	DYN_FIXED = _("Fixed");
	DYN_RANDOM = _("Random");
	DYN_CURVED = _("Curved");
	DYN_OSCILLATE = _("Oscillate");
	DYN_CURVED_LINEAR = _("Linear");
	DYN_CURVED_SPLINE = _("Spline");
	PT_ALL = _("All types");
	PT_VISUAL = _("Visual");
	PT_EMITTER = _("Emitter");
	PT_AFFECTOR = _("Affector");
	PT_SYSTEM = _("System");
	PT_TECHNIQUE = _("Technique");

	mHelpHtml = wxT("index.html");
	if (mName.length() > 0) {
		Append (new wxStringProperty(PRNL_NAME, PRNL_NAME, ogre2wx(mName)));
	}
}

void PropertyWindow::doSetBool(const wxString & name, bool boolValue) {
	wxPGProperty * prop = GetPropertyByName(name);
	if (!prop) {
		return;
	}

	prop->SetValueFromInt(boolValue);
}

bool PropertyWindow::doGetBool(const wxString & name) {
	wxPGProperty * prop = GetPropertyByName(name);
	if (!prop) {
		return false;
	}
	return prop->DoGetValue().GetBool();
}

void PropertyWindow::doSetDouble(const wxString & name, double doubleValue) {
	wxPGProperty * prop = GetPropertyByName(name);
	if (!prop) {
		return;
	}
	prop->SetValue(doubleValue);
}

double PropertyWindow::doGetDouble(const wxString & name) {
	wxPGProperty * prop = GetPropertyByName(name);
	if (!prop) {
		return 0.0;
	}
	return prop->DoGetValue().GetDouble();
}

void PropertyWindow::doSetLong(const wxString & name, long longValue) {
	wxPGProperty * prop = GetPropertyByName(name);
	if (!prop) {
		return;
	}
	prop->SetValue(longValue);
}

long PropertyWindow::doGetLong(const wxString & name) {
	wxPGProperty * prop = GetPropertyByName(name);
	if (!prop) {
		return 0;
	}
	return prop->DoGetValue().GetLong();
}

void PropertyWindow::doSetUint16(const wxString & name, wxUint16 uInt16Value) {
	wxPGProperty * prop = GetPropertyByName(name);
	if (!prop) {
		return;
	}
	prop->SetValue(uInt16Value);
}

wxUint16 PropertyWindow::doGetUint16(const wxString & name) {
	wxPGProperty * prop = GetPropertyByName(name);
	if (!prop) {
		return 0;
	}
	return prop->DoGetValue().GetLong();
}

void PropertyWindow::doSetString(const wxString & name, const wxString & stringValue) {
	wxPGProperty * prop = GetPropertyByName(name);
	if (!prop) {
		return;
	}
	prop->SetValue(stringValue);
}

const wxString & PropertyWindow::doGetString(const wxString & name) {
	wxPGProperty * prop = GetPropertyByName(name);
	if (!prop) {
		return WX_STRING_BLANK;
	}
	wxString res = prop->DoGetValue().GetString();
	return res;
}

void PropertyWindow::appendColourWithAlphaProperty(const wxString & label, const wxString & name, const wxColour & colour) {
//	wxPGProperty * pid = Append(new wxParentProperty(label, name));
//	AppendIn(pid, wxColourProperty(name + PRNL_POST_RGB, name + PRNL_POST_RGB, colour));
//	wxPGProperty * aid = AppendIn(pid, wxIntProperty(name + PRNL_POST_ALPHA, name + PRNL_POST_ALPHA, colour.Alpha()));
//	SetPropertyEditor(aid, wxPG_EDITOR(SpinCtrl));
}

void PropertyWindow::appendInColourWithAlphaProperty(wxPGId & id, const wxString & label, const wxString & name, const wxColour & colour) {
//	wxPGProperty * pid = AppendIn(id, wxParentProperty(label, name));
//	AppendIn(pid, wxColourProperty(name + PRNL_POST_RGB, name + PRNL_POST_RGB, colour));
//	wxPGProperty * aid = AppendIn(pid, wxIntProperty(name + PRNL_POST_ALPHA, name + PRNL_POST_ALPHA, colour.Alpha()));
//	SetPropertyEditor(aid, wxPG_EDITOR(SpinCtrl));
}

void PropertyWindow::doSetColourWithAlpha(const wxString & name, const wxColour & colour, bool nameIsBaseName) {
	wxString baseName;
	if (nameIsBaseName) {
		baseName = name;
	} else {
		// Child accessable with name "parent.string"
		baseName = name + wxT(".") + name;
	}
	wxPGProperty * prop = GetPropertyByName(baseName + PRNL_POST_RGB);
	if (!prop) {
		return;
	}
	prop->SetValue(wxAny(colour));

	prop = GetPropertyByName(baseName + PRNL_POST_ALPHA);
	if (!prop) {
		return;
	}

	prop->SetValue(colour.Alpha());
}

const wxColour & PropertyWindow::doGetColourWithAlpha(const wxString & name, wxColour & colour, bool nameIsBaseName) {
	wxString baseName;
	if (nameIsBaseName) {
		baseName = name;
	} else {
		// Child accessable with name "parent.string"
		baseName = name + wxT(".") + name;
	}
	wxPGProperty * prop = GetPropertyByName(baseName + PRNL_POST_RGB);
	if (!prop) {
		return colour;
	}
	wxVariant value = prop->GetValue();
	wxColour * txcol = wxGetVariantCast(value, wxColour);
	colour = wxColour(txcol->Red(), txcol->Green(), txcol->Blue());

	prop = GetPropertyByName(baseName + PRNL_POST_ALPHA);
	if (!prop) {
		return colour;
	}
	unsigned char alpha = prop->DoGetValue().GetLong();
	colour = wxColor(colour.Red(), colour.Green(), colour.Blue(), alpha);
	return colour;
}

void PropertyWindow::appendVector3(const wxString & label, const wxString & name, const Ogre::Vector3 & vector3) {
	// TODO
}

void PropertyWindow::doSetVector3(const wxString & name, const Ogre::Vector3 & vector3) {
	// Child accessable with name "parent.string"
	wxString baseName = name + wxT(".") + name;
	wxPGProperty * prop = GetPropertyByName(baseName + PRNL_X);
	if (!prop) {
		return;
	}
	prop->SetValue(vector3.x);
	prop = GetPropertyByName(baseName + PRNL_Y);
	if (!prop) {
		return;
	}
	prop->SetValue(vector3.y);
	prop = GetPropertyByName(baseName + PRNL_Z);
	if (!prop) {
		return;
	}
	prop->SetValue(vector3.z);
}

const Ogre::Vector3 & PropertyWindow::doGetVector3(const wxString & name, Ogre::Vector3 & vector3) {
	// Child accessable with name "parent.string"
	wxString baseName = name + wxT(".") + name;
	wxPGProperty * prop = GetPropertyByName(baseName + PRNL_X);
	if (!prop) {
		return Ogre::Vector3::ZERO;
	}
	vector3.x = prop->DoGetValue().GetDouble();
	prop = GetPropertyByName(baseName + PRNL_Y);
	if (!prop) {
		return Ogre::Vector3::ZERO;
	}
	vector3.y = prop->DoGetValue().GetDouble();
	prop = GetPropertyByName(baseName + PRNL_Z);
	if (!prop) {
		return Ogre::Vector3::ZERO;
	}
	vector3.z = prop->DoGetValue().GetDouble();
	return vector3;
}

void PropertyWindow::appendVector4(const wxString & label, const wxString & name, const Ogre::Vector4 & vector4) {
/*	wxPGProperty * pid = Append(new wxParentProperty(label, name));
	wxPGProperty * xid = AppendIn(pid, wxFloatProperty(name + PRNL_X, name + PRNL_X, vector4.x));
	SetPropertyEditor(xid, wxPG_EDITOR(SpinCtrl));
	wxPGProperty * yid = AppendIn(pid, wxFloatProperty(name + PRNL_Y, name + PRNL_Y, vector4.y));
	SetPropertyEditor(yid, wxPG_EDITOR(SpinCtrl));
	wxPGProperty * zid = AppendIn(pid, wxFloatProperty(name + PRNL_Z, name + PRNL_Z, vector4.z));
	SetPropertyEditor(zid, wxPG_EDITOR(SpinCtrl));
	wxPGProperty * wid = AppendIn(pid, wxFloatProperty(name + PRNL_W, name + PRNL_W, vector4.w));
	SetPropertyEditor(wid, wxPG_EDITOR(SpinCtrl));*/
}

void PropertyWindow::doSetVector4(const wxString & name, const Ogre::Vector4 & vector4) {
	// Child accessable with name "parent.string"
	wxString baseName = name + wxT(".") + name;
	wxPGProperty * prop = GetPropertyByName(baseName + PRNL_X);
	if (!prop) {
		return;
	}
	prop->SetValue(vector4.x);

	prop = GetPropertyByName(baseName + PRNL_Y);
	if (!prop) {
		return;
	}
	prop->SetValue(vector4.y);

	prop = GetPropertyByName(baseName + PRNL_Z);
	if (!prop) {
		return;
	}
	prop->SetValue(vector4.z);

	prop = GetPropertyByName(baseName + PRNL_W);
	if (!prop) {
		return;
	}
	prop->SetValue(vector4.w);
}

const Ogre::Vector4 & PropertyWindow::doGetVector4(const wxString & name, Ogre::Vector4 & vector4) {
	// Child accessable with name "parent.string"
	wxString baseName = name + wxT(".") + name;
	wxPGProperty * prop = GetPropertyByName(baseName + PRNL_X);
	if (!prop) {
		return Ogre::Vector4::ZERO;
	}
	vector4.x = prop->DoGetValue().GetDouble();
	prop = GetPropertyByName(baseName + PRNL_Y);
	if (!prop) {
		return Ogre::Vector4::ZERO;
	}
	vector4.y = prop->DoGetValue().GetDouble();
	prop = GetPropertyByName(baseName + PRNL_Z);
	if (!prop) {
		return Ogre::Vector4::ZERO;
	}
	vector4.z = prop->DoGetValue().GetDouble();
	prop = GetPropertyByName(baseName + PRNL_W);
	if (!prop) {
		return Ogre::Vector4::ZERO;
	}
	vector4.w = prop->DoGetValue().GetDouble();
	return vector4;
}

void PropertyWindow::appendQuaternion(const wxString & label, const wxString & name, const Ogre::Quaternion & quaternion) {
/*	wxPGProperty * pid = Append(new wxParentProperty(label, name));
	wxPGProperty * wid = AppendIn(pid, wxFloatProperty(name + PRNL_W, name + PRNL_W, quaternion.w));
	SetPropertyEditor(wid, wxPG_EDITOR(SpinCtrl));
	wxPGProperty * xid = AppendIn(pid, wxFloatProperty(name + PRNL_X, name + PRNL_X, quaternion.x));
	SetPropertyEditor(xid, wxPG_EDITOR(SpinCtrl));
	wxPGProperty * yid = AppendIn(pid, wxFloatProperty(name + PRNL_Y, name + PRNL_Y, quaternion.y));
	SetPropertyEditor(yid, wxPG_EDITOR(SpinCtrl));
	wxPGProperty * zid = AppendIn(pid, wxFloatProperty(name + PRNL_Z, name + PRNL_Z, quaternion.z));
	SetPropertyEditor(zid, wxPG_EDITOR(SpinCtrl));*/
}

void PropertyWindow::doSetQuaternion(const wxString & name, const Ogre::Quaternion & quaternion) {
	wxString baseName = name + wxT(".") + name;
	wxPGProperty * prop = GetPropertyByName(baseName + PRNL_W);
	if (!prop) {
		return;
	}
	prop->SetValue(quaternion.w);
	prop = GetPropertyByName(baseName + PRNL_X);
	if (!prop) {
		return;
	}
	prop->SetValue(quaternion.x);
	prop = GetPropertyByName(baseName + PRNL_Y);
	if (!prop) {
		return;
	}
	prop->SetValue(quaternion.y);
	prop = GetPropertyByName(baseName + PRNL_Z);
	if (!prop) {
		return;
	}
	prop->SetValue(quaternion.z);
}

const Ogre::Quaternion & PropertyWindow::doGetQuaternion(const wxString & name, Ogre::Quaternion & quaternion) {
	wxString baseName = name + wxT(".") + name;
	wxPGProperty * prop = GetPropertyByName(baseName + PRNL_W);
	if (!prop) {
		return Ogre::Quaternion::IDENTITY;
	}
	quaternion.w = prop->DoGetValue().GetDouble();
	prop = GetPropertyByName(baseName + PRNL_X);
	if (!prop) {
		return Ogre::Quaternion::IDENTITY;
	}
	quaternion.x = prop->DoGetValue().GetDouble();
	prop = GetPropertyByName(baseName + PRNL_Y);
	if (!prop) {
		return Ogre::Quaternion::IDENTITY;
	}
	quaternion.y = prop->DoGetValue().GetDouble();
	prop = GetPropertyByName(baseName + PRNL_Z);
	if (!prop) {
		return Ogre::Quaternion::IDENTITY;
	}
	quaternion.z = prop->DoGetValue().GetDouble();
	return quaternion;
}

void PropertyWindow::appendDynamicAttribute(const wxString & label, const wxString & name, ParticleUniverse::DynamicAttribute & dynamicAttribute) {
	// TODO
	// Remove previous property and replace it with the new values
	wxPGProperty * pid = GetPropertyByName(name);
	if (pid) {
		//wxPGProperty * prop = GetPropertyByName(name); // To avoid ambiguity
		wxPGProperty * newPid = new wxStringProperty(label, name);
		ReplaceProperty(pid, newPid);
		pid = newPid;
	} else {
		pid = Append(new wxStringProperty(label, name));
	}

	// Show properties of certain type
	switch (dynamicAttribute.getType()) {
		case ParticleUniverse::DynamicAttribute::DAT_FIXED: {
			// Show fixed value
			AppendIn(pid, new wxEnumProperty(PRNL_TYPE, name + PRNL_TYPE, _types));
			ParticleUniverse::DynamicAttributeFixed * df = static_cast<ParticleUniverse::DynamicAttributeFixed *>(&dynamicAttribute);
			wxPGProperty * id = AppendIn(pid, new wxFloatProperty(PRNL_VALUE, name + PRNL_VALUE, df->getValue()));
			SetPropertyEditor(id, wxPG_EDITOR(SpinCtrl));
		}
		break;

		case ParticleUniverse::DynamicAttribute::DAT_RANDOM: {
			// Show min and max
			AppendIn(pid, new wxEnumProperty(PRNL_TYPE, name + PRNL_TYPE, _types));
			ParticleUniverse::DynamicAttributeRandom * dr = static_cast<ParticleUniverse::DynamicAttributeRandom *>(&dynamicAttribute);
			wxPGProperty * id = AppendIn(pid, new wxFloatProperty(PRNL_MIN_VALUE, name + PRNL_MIN_VALUE, dr->getMin()));
			SetPropertyEditor(id, wxPG_EDITOR(SpinCtrl));
			id = AppendIn(pid, new wxFloatProperty(PRNL_MAX_VALUE, name + PRNL_MAX_VALUE, dr->getMax()));
			SetPropertyEditor(id, wxPG_EDITOR(SpinCtrl));
		}
		break;

		case ParticleUniverse::DynamicAttribute::DAT_CURVED: {
			EnumPropertyWithButton* enumProperty = new EnumPropertyWithButton(PRNL_TYPE, name + PRNL_TYPE, _types);
			wxPGProperty * id = AppendIn(pid, enumProperty);
			SetPropertyEditor(id, wxPG_EDITOR(ChoiceAndButton)); // Add a button

			// Copy controlpoints from dynamicAttribute to internal mControlPointListMap
			ParticleUniverse::DynamicAttributeCurved * dynCurved = static_cast<ParticleUniverse::DynamicAttributeCurved *>(&dynamicAttribute);
			ParticleUniverse::DynamicAttributeCurved::ControlPointList list = dynCurved->getControlPoints();
			mControlPointListMap.insert(std::make_pair(name, list));

			// Copy linear / spline interpolation
			ParticleUniverse::InterpolationType type = dynCurved->getInterpolationType();
			mInterpolationTypeMap.insert(std::make_pair(name, type));
		}
		break;

		case ParticleUniverse::DynamicAttribute::DAT_OSCILLATE: {
			// Show oscillation type, frequency, phase, base and amplitude
			AppendIn(pid, new wxEnumProperty(PRNL_TYPE, name + PRNL_TYPE, _types));
			ParticleUniverse::DynamicAttributeOscillate * dosc = static_cast<ParticleUniverse::DynamicAttributeOscillate *>(&dynamicAttribute);
			wxArrayString oscillationTypes;
			oscillationTypes.Add(PRNL_OSC_SINE);
			oscillationTypes.Add(PRNL_OSC_SQUARE);
			AppendIn(pid, new wxEnumProperty(PRNL_OSC_TYPE, PRNL_OSC_TYPE, oscillationTypes));
			wxPGProperty * id = AppendIn(pid, new wxFloatProperty(PRNL_OSC_FREQUENCY, name + PRNL_OSC_FREQUENCY, dosc->getFrequency()));
			SetPropertyEditor(id, wxPG_EDITOR(SpinCtrl));
			id = AppendIn(pid, new wxFloatProperty(PRNL_OSC_PHASE, name + PRNL_OSC_PHASE, dosc->getPhase()));
			SetPropertyEditor(id, wxPG_EDITOR(SpinCtrl));
			id = AppendIn(pid, new wxFloatProperty(PRNL_OSC_BASE, name + PRNL_OSC_BASE, dosc->getBase()));
			SetPropertyEditor(id, wxPG_EDITOR(SpinCtrl));
			id = AppendIn(pid, new wxFloatProperty(PRNL_OSC_AMPLITUDE, name + PRNL_OSC_AMPLITUDE, dosc->getAmplitude()));
			SetPropertyEditor(id, wxPG_EDITOR(SpinCtrl));
		}
		break;
		default: {
			break;
		}
	}
}

void PropertyWindow::doSetDynamicAttribute(const wxString & name, PropertyWindow * propertyWindow) {
	// Get the type
	wxString baseName = name + wxT(".") + name;
	wxPGProperty * propFrom = propertyWindow->GetPropertyByName(baseName + PRNL_TYPE);
	wxPGProperty * propTo = GetPropertyByName(baseName + PRNL_TYPE);
	if (propFrom && propTo) {
		propTo->SetValue(propFrom->DoGetValue());
	} else {
		return;
	}

	if (propFrom->GetValueAsString() == DYN_FIXED) {
		// Be sure that both types are the same, so replace the existing one
		ParticleUniverse::DynamicAttributeFixed dynAttr;
		appendDynamicAttribute(name, name, dynAttr);
		propTo = GetPropertyByName(baseName + PRNL_TYPE);
		if (!propTo) {
			return;
		}
		propTo->SetValueFromString(DYN_FIXED);

		propFrom = propertyWindow->GetPropertyByName(baseName + PRNL_VALUE);
		propTo = GetPropertyByName(baseName + PRNL_VALUE);
		if (propFrom && propTo) {
			propTo->SetValue(propFrom->DoGetValue());
		}
	} else if (propFrom->GetValueAsString() == DYN_RANDOM) {
		// Be sure that both types are the same, so replace the existing one
		ParticleUniverse::DynamicAttributeRandom dynAttr;
		appendDynamicAttribute(name, name, dynAttr);
		propTo = GetPropertyByName(baseName + PRNL_TYPE);
		if (!propTo) {
			return;
		}
		propTo->SetValueFromString(DYN_RANDOM);

		propFrom = propertyWindow->GetPropertyByName(baseName + PRNL_MIN_VALUE);
		propTo = GetPropertyByName(baseName + PRNL_MIN_VALUE);
		if (propFrom && propTo) {
			propTo->SetValue(propFrom->DoGetValue());
		}
		propFrom = propertyWindow->GetPropertyByName(baseName + PRNL_MAX_VALUE);
		propTo = GetPropertyByName(baseName + PRNL_MAX_VALUE);
		if (propFrom && propTo) {
			propTo->SetValue(propFrom->DoGetValue());
		}
	} else if (propFrom->GetValueAsString() == DYN_CURVED) {
		ParticleUniverse::DynamicAttributeCurved dynAttr;
		appendDynamicAttribute(name, name, dynAttr);
		propTo = GetPropertyByName(baseName + PRNL_TYPE);
		if (!propTo) {
			return;
		}
		propTo->SetValueFromString(DYN_CURVED);

		// Copy controlpoints from other property window to mControlPointListMap
		mControlPointListMap = propertyWindow->mControlPointListMap;

		// Copy linear / spline interpolation
		mInterpolationTypeMap = propertyWindow->mInterpolationTypeMap;
	} else if (propFrom->GetValueAsString() == DYN_OSCILLATE) {
		// Be sure that both types are the same, so replace the existing one
		ParticleUniverse::DynamicAttributeOscillate dynAttr;
		appendDynamicAttribute(name, name, dynAttr);
		propTo = GetPropertyByName(baseName + PRNL_TYPE);
		if (!propTo) {
			return;
		}
		propTo->SetValueFromString(DYN_OSCILLATE);

		propFrom = propertyWindow->GetPropertyByName(baseName + PRNL_OSC_TYPE);
		propTo = GetPropertyByName(baseName + PRNL_OSC_TYPE);
		if (propFrom && propTo) {
			propTo->SetValue(propFrom->DoGetValue());
		}
		propFrom = propertyWindow->GetPropertyByName(baseName + PRNL_OSC_FREQUENCY);
		propTo = GetPropertyByName(baseName + PRNL_OSC_FREQUENCY);
		if (propFrom && propTo) {
			propTo->SetValue(propFrom->DoGetValue());
		}
		propFrom = propertyWindow->GetPropertyByName(baseName + PRNL_OSC_PHASE);
		propTo = GetPropertyByName(baseName + PRNL_OSC_PHASE);
		if (propFrom && propTo) {
			propTo->SetValue(propFrom->DoGetValue());
		}
		propFrom = propertyWindow->GetPropertyByName(baseName + PRNL_OSC_BASE);
		propTo = GetPropertyByName(baseName + PRNL_OSC_BASE);
		if (propFrom && propTo) {
			propTo->SetValue(propFrom->DoGetValue());
		}
		propFrom = propertyWindow->GetPropertyByName(baseName + PRNL_OSC_AMPLITUDE);
		propTo = GetPropertyByName(baseName + PRNL_OSC_AMPLITUDE);
		if (propFrom && propTo) {
			propTo->SetValue(propFrom->DoGetValue());
		}
	}
}

void PropertyWindow::doSetDynamicAttribute(const wxString & name, ParticleUniverse::DynamicAttribute * dynamicAttribute) {
	if (!dynamicAttribute) {
		return;
	}

	wxString baseName = name + wxT(".") + name;
	appendDynamicAttribute(name, name, *dynamicAttribute);
	wxPGProperty * propTo = GetPropertyByName(baseName + PRNL_TYPE);
	if (!propTo) {
		std::cerr << "PropertyWindow::doSetDynamicAttribute: " << (baseName + PRNL_TYPE) << " not found" << std::endl;
		return;
	}

	switch (dynamicAttribute->getType()) {
	case ParticleUniverse::DynamicAttribute::DAT_FIXED: {
		propTo->SetValueFromString(DYN_FIXED);
		break;
	}
	case ParticleUniverse::DynamicAttribute::DAT_RANDOM: {
		propTo->SetValueFromString(DYN_RANDOM);
		break;
	}
	case ParticleUniverse::DynamicAttribute::DAT_CURVED: {
		// Copy controlpoints from dynamicAttribute to internal mControlPointListMap
		//ParticleUniverse::DynamicAttributeCurved * dynCurved = static_cast<ParticleUniverse::DynamicAttributeCurved *>(dynamicAttribute);
		//ParticleUniverse::DynamicAttributeCurved::ControlPointList list = dynCurved->getControlPoints();
		//mControlPointListMap.insert(std::make_pair(wx2ogre(name), list));
		propTo->SetValueFromString(DYN_CURVED);
		break;
	}
	case ParticleUniverse::DynamicAttribute::DAT_OSCILLATE: {
		propTo->SetValueFromString(DYN_OSCILLATE);
		break;
	}
	default: {
		break;
	}
	}
}

EditComponent * PropertyWindow::getOwner() {
	return mOwner;
}

void PropertyWindow::onPropertyChanged(wxPropertyGridEvent & event) {
	// Inform others that a property has been changed.
	// TODO:

	notifyPropertyChanged();

	// Handle other changes
	wxPGProperty * prop = event.GetProperty()->GetMainParent();
	wxString name = prop->GetName();
	wxString label = prop->GetLabel();

	label = event.GetPropertyName();

	// Perform additional validations
	if (mName.length() > 0 && !_validatePropertyStringNoSpaces(event.GetProperty(), PRNL_NAME))
		return;

	if (name.Contains(PRNL_NAME)) {
		Ogre::String newName = wx2ogre(event.GetPropertyValue().GetString());
		mOwner->notifyAdjustNames(newName); // Must perform this first if the name changes (may not be included in setComponentName())
		mOwner->setComponentName(newName);
		mOwner->setCaption();
	} else if (label.Contains(PRNL_TYPE)) {
		// Dynamic attribute changed.
		if (!prop) return;
		assert(event.GetPropertyValue().GetInteger() < _types.size());
		name = i6engine::utils::split(event.GetPropertyName().ToStdString(), ".").front(); // Reuse of 'name'
		label = i6engine::utils::split(event.GetPropertyName().ToStdString(), ".").front(); // Reuse of 'label'
		if (_types[event.GetPropertyValue().GetInteger()] == DYN_FIXED) {
			ParticleUniverse::DynamicAttributeFixed dynAttr;
			appendDynamicAttribute(label, name, dynAttr);
			prop = GetPropertyByName(event.GetPropertyName()); // Property has been replaced, so search it again to get the new pointer
			prop->SetValueFromString(DYN_FIXED);
		}
		else if (_types[event.GetPropertyValue().GetInteger()] == DYN_RANDOM) {
			ParticleUniverse::DynamicAttributeRandom dynAttr;
			appendDynamicAttribute(label, name, dynAttr);
			prop = GetPropertyByName(event.GetPropertyName()); // Property has been replaced, so search it again to get the new pointer
			prop->SetValueFromString(DYN_RANDOM);
		}
		else if (_types[event.GetPropertyValue().GetInteger()] == DYN_CURVED) {
			// Show dialog to create a curve with control points
			ParticleUniverse::DynamicAttributeCurved dynAttr;
			appendDynamicAttribute(label, name, dynAttr);
			prop = GetPropertyByName(event.GetPropertyName()); // Property has been replaced, so search it again to get the new pointer
			prop->SetValueFromString(DYN_CURVED);

			// Open dialog
			ControlPointListMap::iterator itCp = mControlPointListMap.find(name);
			ParticleUniverse::DynamicAttributeCurved::ControlPointList list;
			if (itCp != mControlPointListMap.end()) {
				// Found controlpoint list
				list = itCp->second;
			}
			InterpolationTypeMap::iterator itIt = mInterpolationTypeMap.find(name);
			ParticleUniverse::InterpolationType type = ParticleUniverse::IT_LINEAR;
			if (itIt != mInterpolationTypeMap.end()) {
				// Found interpolation type
				type = itIt->second;
			}

			wxString s = label + _(" - Curved");
			ControlPointDialog dlg(type, list, this, wxID_ANY, s, wxDefaultPosition, wxSize(800, 640));
			if (dlg.ShowModal() == wxID_OK) {
				list = dlg.getControlPointList();
				mControlPointListMap.erase(name);
				mControlPointListMap.insert(std::make_pair(name, list));
				type = dlg.getInterpolationType();
				mInterpolationTypeMap.erase(name);
				mInterpolationTypeMap.insert(std::make_pair(name, type));
			}
		}
		else if (_types[event.GetPropertyValue().GetInteger()] == DYN_OSCILLATE) {
			ParticleUniverse::DynamicAttributeOscillate dynAttr;
			appendDynamicAttribute(label, name, dynAttr);
			prop = GetPropertyByName(event.GetPropertyName()); // Property has been replaced, so search it again to get the new pointer
			prop->SetValueFromString(DYN_OSCILLATE);
		}
		wxPGProperty * pid = GetPropertyByName(name);
		Expand(pid);
	}
}

void PropertyWindow::notifyPropertyChanged() {
	// Inform the owner
	if (mOwner) {
		mOwner->notifyPropertyChanged();
	}
}

void PropertyWindow::copyAttributeToComponent(wxPGProperty * prop, wxString propertyName) {
	// Nothing to do right now
	return;
}

bool PropertyWindow::_validatePropertyStringNoSpaces(wxPGProperty * prop, const wxString & name) {
	if (!prop) {
		return true;
	}

	if (prop->GetName() == name) {
		if (prop->GetValueAsString().Contains(wxT(" "))) {
			return _setPropertyError(prop, _("No spaces allowed"));
		}
	}
	return true;
}

bool PropertyWindow::_validatePropertyColourWithAlpha(wxPGProperty * prop, const wxString & name) {
	return _validatePropertyIntMinMax(prop, name + PRNL_POST_ALPHA, 0, 255);
}

bool PropertyWindow::_validatePropertyIntMinMax(wxPGProperty * prop, const wxString & name, int min, int max) {
	if (!prop) {
		return true;
	}

	if (prop->GetName() == name) {
		if (prop->DoGetValue().GetLong() < min || prop->DoGetValue().GetLong() > max) {
			return _setPropertyError(prop, _("Value must be between ") + ogre2wx(Ogre::StringConverter::toString(min)) + _(" and ") + ogre2wx(Ogre::StringConverter::toString(max)));
		}
	}
	return true;
}

bool PropertyWindow::_validatePropertyIntPositive(wxPGProperty * prop, const wxString & name) {
	if (!prop) {
		return true;
	}

	if (prop->GetName() == name) {
		if (prop->DoGetValue().GetLong() < 0L) {
			return _setPropertyError(prop, _("Value must be positive"));
		}
	}
	return true;
}

bool PropertyWindow::_validatePropertyFloatMinMax(wxPGProperty * prop, const wxString & name, float min, float max) {
	if (!prop) {
		return true;
	}

	if (prop->GetName() == name) {
		if (prop->DoGetValue().GetDouble() < min || prop->DoGetValue().GetDouble() > max) {
			return _setPropertyError(prop, _("Value must be between ") + ogre2wx(Ogre::StringConverter::toString(min)) + _("and ") + ogre2wx(Ogre::StringConverter::toString(max)));
		}
	}
	return true;
}

bool PropertyWindow::_validatePropertyFloatPositive(wxPGProperty * prop, const wxString & name) {
	if (!prop) {
		return true;
	}

	if (prop->GetName() == name) {
		if (prop->DoGetValue().GetDouble() < 0.0f) {
			return _setPropertyError(prop, _("Value must be positive"));
		}
	}
	return true;
}

bool PropertyWindow::_validatePropertyDynamicAttribute(wxPGProperty * prop, const wxString & name) {
	if (!prop) {
		return true;
	}

	// Validate on positive values
	if (!_validatePropertyFloatPositive(prop, name + PRNL_VALUE)) {
		return false;
	}
	if (!_validatePropertyFloatPositive(prop, name + PRNL_MIN_VALUE)) {
		return false;
	}
	if (!_validatePropertyFloatPositive(prop, name + PRNL_MAX_VALUE)) {
		return false;
	}

	return true;
}

bool PropertyWindow::_setPropertyError(wxPGProperty * prop, const wxString & message) {
	_showMessage(_("'") + prop->GetValueAsString() + _("': ") + message);
	prop->SetValueToUnspecified();
//	SelectProperty(prop->GetId(), true); TODO: (Michael) uncomment
	return false;
}

void PropertyWindow::_showMessage(const wxString & message, long style) {
	wxMessageDialog m(this, message, _("Message"), style);
	m.ShowModal();
}

ParticleUniverse::DynamicAttribute * PropertyWindow::copyValuesToDynamicAttribute(wxString name, wxPGProperty * prop, wxString baseName, ParticleUniverse::DynamicAttribute * dynAttr) {
	if (name == baseName + PRNL_TYPE) {
		// Change the type of dynamic attribute. Note that the prop that is passed is invalid. because it has been changed. So search it again.
		prop = GetPropertyByName(baseName + wxT(".") + name);
		if (prop) {
			wxString type = prop->GetValueAsString();
			ParticleUniverse::ParticleSystemManager * particleSystemManager = ParticleUniverse::ParticleSystemManager::getSingletonPtr();
			if (type == DYN_FIXED) {
				return particleSystemManager->createDynamicAttribute(ParticleUniverse::DynamicAttribute::DAT_FIXED);
			} else if (type == DYN_RANDOM) {
				return particleSystemManager->createDynamicAttribute(ParticleUniverse::DynamicAttribute::DAT_RANDOM);
			} else if (type == DYN_CURVED) {
				// This one does not have properties, because all editting is done in the dialog
				ParticleUniverse::DynamicAttributeCurved * dynCurved = static_cast<ParticleUniverse::DynamicAttributeCurved *>(particleSystemManager->createDynamicAttribute(ParticleUniverse::DynamicAttribute::DAT_CURVED));

				// Copy the interpolation type
				InterpolationTypeMap::iterator it = mInterpolationTypeMap.find(baseName);
				ParticleUniverse::InterpolationType interpolationType = ParticleUniverse::IT_LINEAR;
				if (it != mInterpolationTypeMap.end()) {
					// Found interpolation type
					interpolationType = it->second;
				}
				dynCurved->setInterpolationType(interpolationType);

				// Copy controlpoints and process them
				copyControlPointsToDynamicAttribute(baseName, dynCurved);
				return dynCurved;
			} else if (type == DYN_OSCILLATE) {
				return particleSystemManager->createDynamicAttribute(ParticleUniverse::DynamicAttribute::DAT_OSCILLATE);
			}
		}
	} else if (name == baseName + PRNL_VALUE) {
		// Change the value of the fixed dyn attr.
		ParticleUniverse::DynamicAttributeFixed * dynFixed = static_cast<ParticleUniverse::DynamicAttributeFixed *>(dynAttr);
		dynFixed->setValue(prop->DoGetValue().GetDouble());
	} else if (name == baseName + PRNL_MIN_VALUE) {
		// Change the value of the random dyn attr.
		ParticleUniverse::DynamicAttributeRandom * dynRandom = static_cast<ParticleUniverse::DynamicAttributeRandom *>(dynAttr);
		dynRandom->setMin(prop->DoGetValue().GetDouble());
	} else if (name == baseName + PRNL_MAX_VALUE) {
		// Change the value of the random dyn attr.
		ParticleUniverse::DynamicAttributeRandom * dynRandom = static_cast<ParticleUniverse::DynamicAttributeRandom *>(dynAttr);
		dynRandom->setMax(prop->DoGetValue().GetDouble());
	} else if (name == baseName + PRNL_OSC_TYPE) {
		// Change the value of the oscillating dyn attr.
		ParticleUniverse::DynamicAttributeOscillate * dynOscillate = static_cast<ParticleUniverse::DynamicAttributeOscillate *>(dynAttr);
		wxString type = prop->GetValueAsString();
		if (type == PRNL_OSC_SQUARE) {
			dynOscillate->setOscillationType(ParticleUniverse::DynamicAttributeOscillate::OSCT_SQUARE);
		} else {
			dynOscillate->setOscillationType(ParticleUniverse::DynamicAttributeOscillate::OSCT_SINE);
		}
	} else if (name == baseName + PRNL_OSC_FREQUENCY) {
		// Change the value of the oscillating dyn attr.
		ParticleUniverse::DynamicAttributeOscillate * dynOscillate = static_cast<ParticleUniverse::DynamicAttributeOscillate *>(dynAttr);
		dynOscillate->setFrequency(prop->DoGetValue().GetDouble());
	} else if (name == baseName + PRNL_OSC_PHASE) {
		// Change the value of the oscillating dyn attr.
		ParticleUniverse::DynamicAttributeOscillate * dynOscillate = static_cast<ParticleUniverse::DynamicAttributeOscillate *>(dynAttr);
		dynOscillate->setPhase(prop->DoGetValue().GetDouble());
	} else if (name == baseName + PRNL_OSC_BASE) {
		// Change the value of the oscillating dyn attr.
		ParticleUniverse::DynamicAttributeOscillate * dynOscillate = static_cast<ParticleUniverse::DynamicAttributeOscillate *>(dynAttr);
		dynOscillate->setBase(prop->DoGetValue().GetDouble());
	} else if (name == baseName + PRNL_OSC_AMPLITUDE) {
		// Change the value of the oscillating dyn attr.
		ParticleUniverse::DynamicAttributeOscillate * dynOscillate = static_cast<ParticleUniverse::DynamicAttributeOscillate *>(dynAttr);
		dynOscillate->setAmplitude(prop->DoGetValue().GetDouble());
	}
	return nullptr;
}

void PropertyWindow::copyControlPointsToDynamicAttribute(wxString baseName, ParticleUniverse::DynamicAttribute * dynAttr) {
	// Controlpoints to be copied from internal mControlPointListMap
	if (dynAttr->getType() != ParticleUniverse::DynamicAttribute::DAT_CURVED) {
		return;
	}

	ParticleUniverse::DynamicAttributeCurved * dynCurved = static_cast<ParticleUniverse::DynamicAttributeCurved *>(dynAttr);
	dynCurved->removeAllControlPoints();
	ControlPointListMap::iterator it = mControlPointListMap.find(baseName);
	ParticleUniverse::DynamicAttributeCurved::ControlPointList list;
	if (it != mControlPointListMap.end()) {
		list = it->second;
	}

	for (ParticleUniverse::DynamicAttributeCurved::ControlPointList::iterator itCp = list.begin(); itCp != list.end(); ++itCp) {
		dynCurved->addControlPoint((*itCp).x, (*itCp).y);
	}

	dynCurved->processControlPoints();
}

bool PropertyWindow::_mustStopParticleSystem(ParticleUniverse::ParticleSystem * system) {
	if (!system) {
		return false;
	}

	bool wasStarted = false;
	if (system && system->getState() == ParticleUniverse::ParticleSystem::PSS_STARTED) {
		wasStarted = true;
		system->stop();
	}
	return wasStarted;
}

void PropertyWindow::_mustRestartParticleSystem(ParticleUniverse::ParticleSystem * system, bool wasStarted) {
	if (!system) {
		return;
	}

	// Start the system if needed
	if (wasStarted) {
		if (system) {
			system->start();
		}
	}
}

void PropertyWindow::_unprepare(ParticleUniverse::IElement * element, ParticleUniverse::Particle::ParticleType elementType, ParticleUniverse::Particle::ParticleType unprepareType) {
	if (!element) {
		return;
	}

	ParticleUniverse::ParticleTechnique * technique = nullptr;
	switch (elementType) {
	case ParticleUniverse::Particle::PT_TECHNIQUE: {
		technique = static_cast<ParticleUniverse::ParticleTechnique *>(element);
		break;
	}
	case ParticleUniverse::Particle::PT_EMITTER: {
		ParticleUniverse::ParticleEmitter * emitter = static_cast<ParticleUniverse::ParticleEmitter *>(element);
		technique = emitter->getParentTechnique();
		break;
	}
	case ParticleUniverse::Particle::PT_AFFECTOR: {
		ParticleUniverse::ParticleAffector * affector = static_cast<ParticleUniverse::ParticleAffector *>(element);
		technique = affector->getParentTechnique();
		break;
	}
	default: {
		break;
	}
	}

	if (!technique) {
		return;
	}

	ParticleUniverse::ParticleSystem * system = technique->getParentSystem();
	bool wasStarted = _mustStopParticleSystem(system);
	switch (unprepareType) {
	case  ParticleUniverse::Particle::PT_SYSTEM: {
		technique->_unprepareSystem();
		// TODO: (Daniel) no break here, is this correct?
	}
	case  ParticleUniverse::Particle::PT_TECHNIQUE: {
		technique->_unprepareTechnique();
		break;
	}
	case  ParticleUniverse::Particle::PT_EMITTER: {
		technique->_unprepareEmitters();
		break;
	}
	case  ParticleUniverse::Particle::PT_AFFECTOR: {
		technique->_unprepareAffectors();
		break;
	}
	case  ParticleUniverse::Particle::PT_VISUAL: {
		technique->_unprepareVisualParticles();
		break;
	}
	default: {
		break;
	}
	}
	_mustRestartParticleSystem(system, wasStarted);
}

void PropertyWindow::propertyDump(wxPropertyGrid * propgrid) {
	if (!propgrid) {
		return;
	}

	Ogre::LogManager * log = Ogre::LogManager::getSingletonPtr();
	log->setLogDetail(Ogre::LL_BOREME);
	log->createLog("Dump.log", true);
	wxPropertyGridIterator gridIt = propgrid->GetIterator();
	while (!gridIt.AtEnd()) {
		wxPGProperty * prop = gridIt.GetProperty();
		wxString s = prop->GetName();
		log->logMessage("Property name" + wx2ogre(s));
		gridIt.Next();
	}
	log->destroyLog("Dump.log");
}

const wxString & PropertyWindow::getHelpHtml() const {
	return mHelpHtml;
}
