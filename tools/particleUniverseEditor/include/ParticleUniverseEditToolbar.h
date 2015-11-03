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

#ifndef __PUED_EDIT_TOOLBAR_H__
#define __PUED_EDIT_TOOLBAR_H__

#include "wx/ogre/prerequisites.h"

#include "wx/toolbar.h"

class UIEditIcons1;
class UIEditIcons2;

/**	Edit toolbar with all icons
*/
class UIEditToolbar {
public:
	// Constructor / Destructor
	UIEditToolbar(wxWindow * parent);
	~UIEditToolbar();
	UIEditIcons1 * getEditIcons1() const;
	UIEditIcons2 * getEditIcons2() const;
		
protected:
	wxToolBar * mWxToolBar1;
	wxToolBar * mWxToolBar2;
	UIEditIcons1 * mUIEditIcons1;
	UIEditIcons2 * mUIEditIcons2;

private:
	UIEditToolbar() {}
};

/**	Edit icons
*/
class UIEditIcons1 {
public:
	// Constructor / Destructor
	UIEditIcons1(wxToolBar * toolbar);
	~UIEditIcons1() {}

	// Actions
	void reset();

protected:
	wxToolBar * mToolbar;
	
private:
	UIEditIcons1() {}
};

/**	Edit icons
*/
class UIEditIcons2
{
public:
	// Constructor / Destructor
	UIEditIcons2(wxToolBar * toolbar);
	~UIEditIcons2() {}

	// Actions
	void reset(unsigned int numberOfListItems, unsigned int numberOfConnections = 0);

protected:
	wxToolBar * mToolbar;
	
private:
	UIEditIcons2() {}
};

#endif /* __PUED_EDIT_TOOLBAR_H__ */
