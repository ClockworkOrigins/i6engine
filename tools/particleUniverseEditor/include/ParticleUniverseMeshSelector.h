/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2012 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#ifndef __PUED_MESHSELECTOR_H__
#define __PUED_MESHSELECTOR_H__

#include "wx/ogre/utils.h"

/**	Selection dialog for meshes
*/
class MeshSelector : public wxSingleChoiceDialog {
public:
	// Constructor / Destructor
	MeshSelector(wxWindow * parent, const wxString & message, const wxString & caption, int n, const wxString * choices, long style = wxCHOICEDLG_STYLE, const wxPoint & pos = wxDefaultPosition) : wxSingleChoiceDialog(parent, message, caption, n, choices, reinterpret_cast<void **>(0), style, pos) {}
	~MeshSelector() {}
};

/**	Class that displays a mesh selection dialog
*/
class MeshDialog {
public:
	MeshDialog() {}
	virtual ~MeshDialog() {}

	/**	Open the dialog
	*/
	const Ogre::String & openDialog (wxWindow * parent);

	/**	Returns the selected meshname
	*/
	const Ogre::String & getMeshName() const;

protected:
	Ogre::String mMeshName;
};

#endif /* __PUED_MESHSELECTOR_H__ */
