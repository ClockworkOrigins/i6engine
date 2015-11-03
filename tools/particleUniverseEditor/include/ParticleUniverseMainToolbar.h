/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2012 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#ifndef __PUED_MAINTOOLBAR_H__
#define __PUED_MAINTOOLBAR_H__

#include "wx/ogre/prerequisites.h"

#include "wx/toolbar.h"

class ParticleUniverseEditorFrame;
class SpinCtrlXYZ;
class TimeScaleSlider;
class UICompileIcons;
class UIFileIcons;
class UIMiscIcons;
class UIPlayIcons;

/**	Main toolbar with all icons
*/
class UIMainToolbar {
public:
	// Constructor / Destructor
	UIMainToolbar(ParticleUniverseEditorFrame * parent);
	~UIMainToolbar();
	wxToolBar * getWxToolbar() const;
	UIFileIcons * getFileIcons() const;
	UICompileIcons * getCompileIcons() const;
	UIPlayIcons * getPlayIcons() const;
	UIMiscIcons * getMiscIcons() const;
	void resetTimeScale();

protected:
	ParticleUniverseEditorFrame * mParent;
	wxToolBar * mWxToolBar;
	UIFileIcons * mUIFileIcons;
	UICompileIcons * mUICompileIcons;
	UIPlayIcons * mUIPlayIcons;
	UIMiscIcons * mUIMiscIcons;

private:
	UIMainToolbar() {}
};


/**	Play, pause, stop, rewind, wind and record icons
*/
class UIPlayIcons {
public:
	// Constructor / Destructor
	UIPlayIcons(wxToolBar * toolbar);
	~UIPlayIcons() {}

	// Actions
	void reset(unsigned int numberOfListItems);
	void disable(unsigned int numberOfListItems);
	void disableAll();
	void disableListFilled();
	void resetListFilled();
	void play();
	void pause();
	void record();

protected:
	wxToolBar * mToolbar;
	
private:
	UIPlayIcons() {}
};

/**	New, Load, Save icons
*/
class UIFileIcons {
public:
	// Constructor / Destructor
	UIFileIcons(wxToolBar * toolbar);
	~UIFileIcons() {}

	// Actions
	void reset(unsigned int numberOfListItems);
	void disableAll();

protected:
	wxToolBar * mToolbar;
	
private:
	UIFileIcons() {}
};

/**	Compile icons
*/
class UICompileIcons {
public:
	// Constructor / Destructor
	UICompileIcons(wxToolBar * toolbar);
	~UICompileIcons() {}

	// Actions
	void reset(unsigned int numberOfListItems);
	void disableAll();

protected:
	wxToolBar * mToolbar;
	
private:
	UICompileIcons() {}
};

/**	Miscelanious icons
*/
class UIMiscIcons {
public:
	// Constructor / Destructor
	UIMiscIcons(ParticleUniverseEditorFrame * frame, wxToolBar * toolbar);
	~UIMiscIcons() {}

	// Enable/Disable the icon
	void enableRemoveEntity(bool enable);
	void enableOrthoCameraSelected(bool enabled);

	// Handle spin controls
	void callbackSpinCtrlXYZ(SpinCtrlXYZ * spinCtrl);

	// Global/local event
	bool doToggleWorldLocalSpace();

	// Camera projection type event
	bool doToggleCameraProjectionType();
	bool isCameraPerspective() const;

	// Camera angle
	bool doToggleCameraAngle();
	bool isCamera45Angle() const;

	// Actions
	void resetTimeScale();
	void enable(bool enabled);
	void reset();
	void disableAll();

protected:
	ParticleUniverseEditorFrame * mFrame;
	wxToolBar * mToolbar;
	SpinCtrlXYZ * mSpinX;
	SpinCtrlXYZ * mSpinY;
	SpinCtrlXYZ * mSpinZ;
	TimeScaleSlider * mTimeScaleSlider;
	bool mWorldSpace;
	bool mCameraPerspective;
	bool mCamera45Angle;
	wxBitmap mWorldSpaceBitmap;
	wxBitmap mLocalSpaceBitmap;
	wxBitmap mPerspectiveBitmap;
	wxBitmap mOrthographicBitmap;
	wxBitmap mOrtho45AngleBitmap;
	wxBitmap mOrtho90AngleBitmap;
	
private:
	UIMiscIcons() {}
};

/**	
*/
class TimeScaleSlider : public wxSlider {
public:
	// Constructor / Destructor
	TimeScaleSlider(ParticleUniverseEditorFrame * frame, wxControl * panel, int value , int minValue, int maxValue);
	virtual ~TimeScaleSlider() {}
	ParticleUniverse::Real getTimeScale() const;
	void setTimeScale(ParticleUniverse::Real scale);
	void resetTimeScale();
	void OnSliderUpdate(wxCommandEvent & event);
	
protected:
	ParticleUniverseEditorFrame * mFrame;
};

#endif /* __PUED_MAINTOOLBAR_H__ */
