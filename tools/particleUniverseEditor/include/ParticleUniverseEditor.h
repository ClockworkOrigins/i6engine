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

#ifndef __PUED_H__
#define __PUED_H__

#include "ParticleUniverseEditorListener.h"
#include "ParticleUniverseGizmo.h"

#include "wx/ogre/ogre.h"

#include "wx/treebase.h"

namespace Ogre {
	class Overlay;
	class OverlayContainer;
	class OverlaySystem;
	class TextAreaOverlayElement;
} /* namespace Ogre */

// Global Particle System for editor
static const ParticleUniverse::String CURRENT_PS_NAME = "currentParticleSystemName";

class AnimationWindow;
class ConfigDialog;
class EditTab;
class GizmoManager;
class MaterialTab;
class OgreControlComponent;
class PropertyWindow;
class Recorder;
class SystemTreeControl;
class TextControl;
class UIMainToolbar;

// wx Classes
class wxNotebook;
class wxPropertyGrid;

class ParticleUniverseEditorFrame : public wxFrame {
	DECLARE_CLASS(ParticleUniverseEditorFrame)
	DECLARE_EVENT_TABLE()

public:
	ParticleUniverseEditorFrame(wxWindow * parent, wxWindowID id);

	void AfterInit(wxOgreResources * ogreResMan);
	void CreateToolbar();
	void CreateParticleExplorer();
	void LoadParticleExplorer();
	void InitialiseParticleExplorer();
	void CreateTabs();
	void LoadTextControl(wxString text);
	void CreateScene();
	void particleSystemStopped();
	void resetPlayIcons();
	int showMessage(const wxString & message, long style = wxOK);
	void setPropertyWindow(PropertyWindow * propertyWindow);
	void setPropertyWindow();
	PropertyWindow * getPropertyWindow();
	void hidePropertyWindow();
	void removePropertyWindow(wxPropertyGrid * propertyWindow);
	void setLeftSideWindow(wxWindow * window);
	void removeLeftSideWindow(wxWindow * window);
	void resetLeftSideWindow();
	void OnHelp();
	void synchroniseScriptWithEditTab();
	bool isPhysXInstalled();
	void notifyScriptChanged();
	static bool isPhysXUsed();
	void notifyTimeRescaled(ParticleUniverse::Real scale);

	void doNewParticleSystem();
	void doLoadFile();
	void doLoadResourcePath();
	void addResourcePathToConfig(wxString & resourcePath);
	void doSave();
	void doSaveAs();
	void doQuit();
	void doPlay();
	void doRecord();
	void doPause();
	void doStop();
	void doRemove();
	void doClone();
	void doCompile();
	void doBackgroundColour();
	void doCameraReset();
	void doSaveCamera();
	void doCentreObject();
	void doResetTime();
	void doToggleGizmo();
	void doToggleStats();
	void doToggleGridPlane();
	void doOptions();
	void applyGeneralConfigOptions();
	void applyTextConfigOptions();
	void applyPhysXConfigOptions();
	void doAbout();
	void OnMouseMoveCallback(wxMouseEvent & event);
	void zoom(Ogre::Real zoomFactor);
	void doMouseButtonPressed(wxMouseEvent & event);
	void OnAddMesh(wxCommandEvent & event);
	void doAddMesh(); // Add mesh from dialog
	void doAddMesh(const Ogre::String & meshName); // Add mesh from drag and drop
	void OnToggleLight(wxCommandEvent & event);
	void doToggleLight();
	void OnRemoveMesh(wxCommandEvent & event);
	void doRemoveMesh();
	void selectMaterialForAddedMesh();
	void replaceTemplateName(const ParticleUniverse::String & templateName);
	void notifyCategoryChanged(ParticleUniverse::ParticleSystem * pSys);
	void synchronizeSmallOgreControlWithRenderTab();
	void callbackSpinCtrlXYZ(Ogre::Real angleX, Ogre::Real angleY, Ogre::Real angleZ, const Ogre::String & order);
	Ogre::SceneNode * getAddEntitySceneNode();
	Ogre::Entity * getAddEntity();
	void restoreFocus();
	ParticleUniverse::ParticleSystem * getCurrentParticleSystemForRenderer();
	void notifyParticleSystemAttachToEntity();
	void notifyParticleSystemDetachedFromEntity();
	Ogre::Quaternion getOrientationMainSceneNode();
	GizmoManager * getGizmoManager();
	void notifyAnimationStarted();
	void notifyAnimationStopped();

protected:
	wxLocale mLocale;
	wxLanguage mLanguage;

private:
	virtual bool Destroy();
	void SetLanguage();
	void SetMenu();
	void OnNewParticleSystem(wxCommandEvent & event);
	void OnLoadFile(wxCommandEvent & event);
	void OnLoadResourcePath(wxCommandEvent & event);
	void OnSave(wxCommandEvent & event);
	void OnSaveAs(wxCommandEvent & event);
	void OnMoveAndSize(wxMoveEvent & event);
	void OnMinMax(wxSizeEvent & event);
	void OnIdle(wxIdleEvent & event);
	void OnQuit(wxCommandEvent & event);
	void OnCompile(wxCommandEvent & event);
	void OnBackgroundColour(wxCommandEvent & event);
	void OnAttachToMesh(wxCommandEvent & event);
	void OnDetachFromMesh(wxCommandEvent & event);
	void OnCameraReset(wxCommandEvent & event);
	void OnSaveCamera(wxCommandEvent & event);
	void OnCameraProjection(wxCommandEvent & event);
	void OnCameraAngle(wxCommandEvent & event);
	void OnCentreObject(wxCommandEvent & event);
	void OnResetTime(wxCommandEvent & event);
	void OnToggleGizmo(wxCommandEvent & event);
	void OnToggleWorldLocalSpace(wxCommandEvent & event);
	void OnToggleStats(wxCommandEvent & event);
	void OnToggleGridPlane(wxCommandEvent & event);
	void OnGizmoMoveSelect(wxCommandEvent & event);
	void OnGizmoRotateSelect(wxCommandEvent & event);
	void OnGizmoScaleSelect(wxCommandEvent & event);
	void OnOptions(wxCommandEvent & event);
	void OnAbout(wxCommandEvent & event);
	void OnTabChanging(wxCommandEvent & event);
	void OnTabChanged(wxCommandEvent & event);
	void OnRewind(wxCommandEvent & event);
	void OnPlay(wxCommandEvent & event);
	void OnRecord(wxCommandEvent & event);
	void OnPause(wxCommandEvent & event);
	void OnStop(wxCommandEvent & event);
	void OnWind(wxCommandEvent & event);
	void OnClone(wxCommandEvent & event);
	void OnRemove(wxCommandEvent & event);
	void OnPaint(wxPaintEvent & WXUNUSED(event));
	void OnTemplatesClick(wxCommandEvent & event);
	void doWindowLeave(wxMouseEvent & event);
	void OnClose(wxCloseEvent & event);
	void validateAndReparse();
	bool createNewSystem(const ParticleUniverse::String & templateName, bool forcedByEditTab = false);
	void createParticleSystemCopyForEditPage(const ParticleUniverse::String & templateName, ParticleUniverse::ParticleSystem * partSystem);
	void parseScript(const ParticleUniverse::String & script);
	void _generateVideoIfNeeded(bool recording);
	void createStatisticsOverlay();
	void initialiseOrthoCameraGizmo();
	void updateOverlay();
	void showOverlay(bool show);
	void resetOverlay();
	void createGridPlane(ParticleUniverse::Real distance);
	void showGridPlane(bool visible);
	void setOrthoGridVisible(bool visible, Gizmo::Axis axis = Gizmo::AXIS_ALL);
	void setDistanceGridPlane(ParticleUniverse::Real distance);
	void setScaleGridPlane(ParticleUniverse::Real scale);
	void createLight();
	void showLight(bool show);
	void selectLightColour(wxPoint position);
	void hideEditPage();
	void showEditPage();
	void OnKeyPressed(wxKeyEvent & event);
	const Ogre::Vector3 & _getCameraPosition();
	void _setCameraPosition(const Ogre::Vector3& position);
	const Ogre::Quaternion & _getCameraOrientation();
	void _setCameraOrientation(const Ogre::Quaternion & orientation);
	void _cameraLookAt(const Ogre::Vector3 & direction);
	Ogre::ProjectionType _getProjectionType();
	bool _isCameraOrthographic();

	bool mScriptChanged;
	bool mTemplateSelected;

	Ogre::Vector3 mDefaultCameraPosition;
	wxBoxSizer * mMainVSizer;
	wxBoxSizer * mEditSizer;
	wxBoxSizer * mLBAndPropSizer;
	wxBoxSizer * mSubHSizer;
	wxNotebook * mNotebook;
	SystemTreeControl * mParticleExplorer;
	wxTreeItemId mLatestSelection;
	wxOgreControl * mControl;
	TextControl * mTextCtrl;
	wxStatusBar * mStatusBar;
	Ogre::SceneManager * mSceneManager;
	Ogre::SceneNode * mMainSceneNode;
	Ogre::SceneNode * mParticlerSystemSceneNode;
	Ogre::SceneNode * mMarkerSceneNode;
	Ogre::Entity * mMarker;
	Ogre::Entity * mMarkerBold;
	Ogre::SceneNode * mGridPlaneSceneNodeBottom;
	Ogre::SceneNode * mGridPlaneSceneNodeX;
	Ogre::SceneNode * mGridPlaneSceneNodeMinX;
	Ogre::SceneNode * mGridPlaneSceneNodeZ;
	Ogre::SceneNode * mGridPlaneSceneNodeMinZ;
	Ogre::Entity * mGridBottom;
	Ogre::Entity * mGridX;
	Ogre::Entity * mGridMinX;
	Ogre::Entity * mGridZ;
	Ogre::Entity * mGridMinZ;
	Ogre::SceneNode * mAddEntitySceneNode;
	Ogre::Entity * mAddEntity;
	UIMainToolbar * mUIMainToolbar;
	Ogre::Entity * mLightEntity;
	Ogre::Light * mLight;
	Ogre::SceneNode * mLightSceneNode;
	EditTab * mEditNotebookPage;
	MaterialTab * mMaterialNotebookPage;
	ParticleUniverse::ParticleSystem * mCurrentParticleSystemForRenderer;
	SystemListener * mSystemListener;
	unsigned int mNewSystemCounter;
	ParticleUniverse::String mCurrentParticleScript;
	bool mAutoStartRender;
	PropertyWindow * mPropertyWindow;
	bool mMinMax;
	wxString mSavedFile;
	ConfigDialog * mConfigDialog;
	wxPoint mLastPositionGizmo;
	wxPoint mLastPositionAddEntity;
	wxPoint mLastPositionLight;
	Recorder * mRecorder;
	ParticleUniverse::String mVideoPath;
	ParticleUniverse::uint mImageWidth;
	ParticleUniverse::uint mImageHeight;
	ParticleUniverse::uint mFramesPerSecond;
	ParticleUniverse::String mFileNameSuffix;
	ParticleUniverse::Real mStartTime;
	ParticleUniverse::Real mEndTime;
	ParticleUniverse::Real mPauseTime;
	Ogre::ColourValue mBackgroundColour;
	static bool mUsePhysX;
	Ogre::Vector3 mPhysXNormal;
	Ogre::Vector3 mPhysXGravity;
	ParticleUniverse::Real mPhysXDistance;
	size_t mMaxNumberOfEmittedParticles;
	size_t mMaxNumberOfVisualParticles;
	AnimationWindow * mAnimationWindow;

	/** Attributes needed for redirecting the log. */
	Ogre::LogManager * mLogManager;
	Ogre::Log * mLog;
	LogListener * mLogListener;

	/** Attributes needed for debug overlay. */
	Ogre::Overlay * mDebugOverlay;
	Ogre::OverlayContainer * mTextPanel;
	Ogre::TextAreaOverlayElement * mAverageFPS;

	/** Attributes needed for ortho camera gizmo. */
	Ogre::SceneNode * mOrthoCameraGizmoNode;

	/** Menu; attach/detach to/from mesh. */
	wxMenu * mTmenu;

	wxPanel * mControlPanelWithSmallRenderWindow;
	wxOgreControl * mOgreControlSmall;

		// If a template is selected, the focus changes to other objects. This flag is used to set focus on the mParticleExplorer again.
	bool mCheckFocusParticleExplorer;

	// Gizmo's
	GizmoManager * mGizmoManager;
	void activateQueryFlags(bool active); // Set flags when the gizmo is visible/invisible
	const Ogre::Vector3 & getCurrentPivot();

	// Camera animation
	Ogre::Vector3 mPreviousPositionAddEntitySceneNode;
	Ogre::Camera * _getCamera();
	void startCameraAnimation(const Ogre::Vector3 & targetPosition);
	bool mStartCameraAnimation;
	Ogre::Real mStartCameraAnimationDelta;
	Ogre::Vector3 mTargetCameraAnimationPosition;
	Ogre::Vector3 mStartCameraAnimationPosition;
	Ogre::Vector3 mDiffCameraAnimationPosition;
	Ogre::Timer mCameraAnimationTimer;
	bool m45DegreesOrtho;
	Ogre::Radian mOrthoYaw;
	Ogre::Radian mOrthoPitch;
	Ogre::OverlaySystem * _overlaySystem;
};


class ParticleUniverseEditorApp : public wxOgreApp {
public:
	bool OnInit();

private:
	ParticleUniverseEditorFrame * m_Editor;
};

DECLARE_APP(ParticleUniverseEditorApp)

#endif /* __PUED_H__ */
