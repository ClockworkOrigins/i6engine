/*
 * Copyright (C) 2007-2008 Martin Pieuchot
 *
 * This library is free software; you can redistribute it and/or
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301
 * USA
 */

#include "wx/ogre/control.h"

#include "ParticleUniverseEditor.h"

#include "i6engine/utils/i6eSystemParameters.h"

#include "OGRE/OgreRenderWindow.h"
#include "OGRE/OgreRoot.h"

#ifdef __WXGTK20__
extern "C" {
#if ISIXE_MPLATFORM == ISIXE_MPLATFORM_LINUX
	#include <gdk/gdkx.h>
	#include <gtk/gtk.h>
#endif
}
#endif

unsigned int wxOgreControl::instances = 0;

BEGIN_EVENT_TABLE(wxOgreControl, wxControl)
	EVT_MOUSE_EVENTS	(wxOgreControl::OnMouseMove)
	EVT_ERASE_BACKGROUND(wxOgreControl::OnEraseBackground)
	EVT_PAINT		   (wxOgreControl::OnPaint)
	EVT_SIZE			(wxOgreControl::OnSize)
	EVT_IDLE			(wxOgreControl::OnIdle)
END_EVENT_TABLE()

IMPLEMENT_DYNAMIC_CLASS(wxOgreControl, wxControl)

wxOgreControl::wxOgreControl() {
	Init();
}

wxOgreControl::wxOgreControl(wxWindow * parent, wxWindowID id, const wxPoint & pos, const wxSize & size, long style, const wxValidator & val, const wxString & name) /*: wxControl(parent, id, pos, size, style, val, name) */: _sceneManager(nullptr), _camera(nullptr), _callbackFrame(nullptr), _root(nullptr), _renderWindow(nullptr), _viewport(nullptr), _lastX(0), _lastY(0) {
	Init();
	Create(parent, id, pos, size, style, val, name);
}

wxOgreControl::~wxOgreControl() {
	Destroy();
}

bool wxOgreControl::Create(wxWindow * parent, wxWindowID id, const wxPoint & pos, const wxSize & size, long style, const wxValidator & val, const wxString & name) {
	wxString instance_name = name + wxString::Format(wxT("%u"), instances);
	if (!wxControl::Create(parent, id, pos, size, style, val, instance_name)) {
		wxFAIL_MSG(_("wxOgreControl creation failed"));
		return false;
	}
	return true;
}

void wxOgreControl::Init() {
	_camera = nullptr;
	_renderWindow = nullptr;
	_viewport = nullptr;

	_lastX = 0;
	_lastY = 0;

	_root = Ogre::Root::getSingletonPtr();
	instances++;
}

bool wxOgreControl::Destroy() {
	if (_camera && _sceneManager) {
		_sceneManager->destroyCamera(_camera);
		_camera = nullptr;
	}

	/* Don't delete the SceneManager, it can be used by others. */
	_sceneManager = nullptr;

	if (_viewport) {
		_renderWindow->removeAllViewports();
		_viewport = nullptr;
	}

	DestroyRenderWindow();

	return true;
}

void wxOgreControl::Update() {
	if (_root) {
		_root->renderOneFrame();

		_root->_fireFrameStarted();

		if (_renderWindow) {
			_renderWindow->update();
		}

		_root->_updateAllRenderTargets();

		_root->_fireFrameEnded();
	}
}

void wxOgreControl::OnPaint(wxPaintEvent & WXUNUSED(event)) {
	wxPaintDC dc(this);
	Update();
}

void wxOgreControl::OnEraseBackground(wxEraseEvent & WXUNUSED(event)) {
	Update();
}

void wxOgreControl::OnSize(wxSizeEvent & WXUNUSED(event)) {
	if (_renderWindow) {
		int width, height;
		GetSize(&width, &height);
		_renderWindow->resize(static_cast<unsigned int>(width), static_cast<unsigned int>(height));
		_renderWindow->windowMovedOrResized();
	}

#ifdef __WXGTK20__
	// Fix because it is not automaticaly done with gtk+
	if (_viewport) {
		_viewport->_updateDimensions();
	}
#endif

	// Let Ogre know the window has been resized;
	// Set the aspect ratio for the new size;
//	if (_camera)
//		_camera->setAspectRatio(ParticleUniverse::Real(width) / ParticleUniverse::Real(height));
}

void wxOgreControl::OnIdle(wxIdleEvent & WXUNUSED(event)) {
	if (GetSize() != GetParent()->GetSize()) {
		SetSize(GetParent()->GetSize()); // This is needed to auto resize so it always fits its parent
	}
	Refresh();
}

void wxOgreControl::AddViewport(Ogre::Camera * cam, int ZOrder, float left, float top, float  width, float height) {
	if (_viewport) {
		_renderWindow->removeAllViewports();
	}
	if (_renderWindow) {
		_viewport = _renderWindow->addViewport(cam, ZOrder, left, top, width, height);
	}
}

Ogre::RenderWindow * wxOgreControl::CreateRenderWindow(const Ogre::String & name) {
	if (!_root->isInitialised()) {
		_renderWindow = _root->initialise(false);
	}

	SetBackgroundStyle(wxBG_STYLE_PAINT);

	Ogre::NameValuePairList params;
	GetParentWindowHandle(params);

	try {
		int w, h;
		GetSize(&w, &h);
		_renderWindow = _root->createRenderWindow(name, uint32_t(w), uint32_t(h), false, &params);
		_renderWindow->setActive(true);

		// Even if we are not always using Ogre's
		// rendering loop, set it as AutoUpdated
		// in case of...
		_renderWindow->setAutoUpdated(true);
	} catch (Ogre::Exception & e) {
		wxOgreExceptionBox(e);
	}

	return _renderWindow;
}

void wxOgreControl::DestroyRenderWindow() {
	if (_renderWindow) {
		_root->detachRenderTarget(_renderWindow);
		_renderWindow = nullptr;
		SetBackgroundStyle(wxBG_STYLE_SYSTEM);
		_root = nullptr;
	}
}

void wxOgreControl::setCallbackFrame(ParticleUniverseEditorFrame * frame) {
	_callbackFrame = frame;
}

void wxOgreControl::GetParentWindowHandle(Ogre::NameValuePairList & pl) {
#ifdef __WXMSW__
	pl["externalWindowHandle"] = all2std(size_t(GetHandle()));
#elif defined(__WXGTK20__)
	/*
	 * Ok here is the most important comment about the GTK+
	 * part of this lib.
	 *
	 * Why we don't use GetHandle() here? Because it returns a
	 * generic GtkWidget* that isn't one of the internals used
	 * by wxGTK and can't be passed to the GTK_PIZZA() macro.
	 *
	 * This becomes a problem when we need to know the window ID
	 * of the current widget. If you know Gtk+ you may want to use
	 * gtk_widget_get_window() but in that case it doesn't return
	 * the good pointer and the Ogre render window will be painted
	 * under the background of this wxControl.
	 *
	 * Look at "wx/gtk/win_gtk.c" for more detailes.
	 *
	 * UPDATE: after changing to wxWidgets 3.0, GTK_PIZZA diesn't exist anymore
	 * GDK_WINDOW_XWINDOW( widget->window ); should do the job (according to a forum post)
	 */
	GtkWidget * widget = m_wxwindow;

	/* May prevent from flickering */
	gtk_widget_set_double_buffered(widget, false);

	/*
	 * The frame need to be realize unless the parent
	 * is already shown.
	 */
	gtk_widget_realize(widget);

	Window window = GDK_WINDOW_XWINDOW(widget->window);   // Window is a typedef for XID, which is a typedef for unsigned int

#if WXOGRE_OGRE_VER < 150

	/* Get the display */
	Display * display = GDK_WINDOW_XDISPLAY(gdkWin);

	/* Get the Screen */
	unsigned int screen = DefaultScreen(display);

	pl["externalWindowHandle"] = all2std((unsigned long)display) + ":"
							 + all2std(screen) + ":"
							 + all2std(window);

#else // WXOGRE_OGRE_VER < 150

	pl["externalWindowHandle"] = all2std(window);

#endif
#else
# error Not supported on this platform.
#endif
}

Ogre::SceneManager * wxOgreControl::CreateSceneManager(const Ogre::String & tn, const Ogre::String & name) {
	SetSceneManager(Ogre::Root::getSingleton().createSceneManager(tn, name));
	return _sceneManager;
}

Ogre::SceneManager * wxOgreControl::CreateSceneManager(Ogre::SceneTypeMask tm, const Ogre::String & name) {
	SetSceneManager(Ogre::Root::getSingleton().createSceneManager(tm, name));
	return _sceneManager;
}

void wxOgreControl::SetSceneManager(Ogre::SceneManager * sm) {
	SetCamera(sm->createCamera("MainCamera"));
	_sceneManager = sm;
}

Ogre::SceneManager * wxOgreControl::GetSceneManager() {
	return _sceneManager;
}

void wxOgreControl::SetCamera(Ogre::Camera * cam) {
	if (_camera) {
		_sceneManager->destroyCamera(_camera);
	}

	int width, height;
	GetSize(&width, &height);

	_camera = cam;
	_camera->setAspectRatio(ParticleUniverse::Real(width) / ParticleUniverse::Real(height));

	AddViewport(_camera);
}

void wxOgreControl::RotateCamera(float relX, float relY, float relZ) {
	if (_camera) {
		_camera->roll(Ogre::Radian(relZ));
		_camera->yaw(Ogre::Radian(relY));
		_camera->pitch(Ogre::Radian(relX));
	}
}

void wxOgreControl::TranslateCamera(float x, float y, float z) {
	if (_camera) {
		_camera->moveRelative(Ogre::Vector3(x, y, z));
	}
}

void wxOgreControl::SetPolygonMode(const Ogre::PolygonMode & pm) {
	if (_camera) {
		_camera->setPolygonMode(pm);
	}
}

void wxOgreControl::ProcessSelection(const wxPoint & pt) {
}

void wxOgreControl::OnMouseMove(wxMouseEvent & event) {
	if (!_callbackFrame) {
		return;
	}
	_callbackFrame->OnMouseMoveCallback(event);
	Refresh();
}
