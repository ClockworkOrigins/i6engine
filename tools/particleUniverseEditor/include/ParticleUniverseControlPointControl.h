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

#ifndef __PUED_CONTROLPOINTCONTROL_H__
#define __PUED_CONTROLPOINTCONTROL_H__

#include "ParticleUniverseContextMenu.h"

class ControlPoint;

/**	Control that represents a controlpoint, needed in the ControlPointSlider
*/
class ControlPointControl : public wxControl {
public:
	ControlPointControl(std::vector<ControlPoint *> & v, wxWindow * parent, wxWindowID id, const wxPoint & pos = wxDefaultPosition, const wxSize & size = wxDefaultSize, const wxString & name = wxT("ControlPointControl"));
	virtual ~ControlPointControl();
	void reset();
	void addInitialControlpoints();
	std::vector<ControlPoint *> & getControlPointList();
	void drawGraph(wxPaintDC & dc);
	void sortControlPoints();
	bool useLinearInterpolation();
	void setLinearInterpolation(bool);

	// Getters/Setters
	float getMinXGraph() const;
	float getMinYGraph() const;
	float getMaxXGraph() const;
	float getMaxYGraph() const;
	void scaleMinMaxXGraph(float scale);
	void scaleMinMaxYGraph(float scale);
	void scrollMinMaxXGraph(float scroll);
	void scrollMinMaxYGraph(float scroll);

protected:
	wxWindow * mParent;
	ControlPoint * mMovingControlPoint;
	bool mMoving;
	std::vector<ControlPoint *> mControlPointList;
	std::vector<ControlPoint *> mInitialControlPointList;
	float mMinXGraph;
	float mMaxXGraph;
	float mMinYGraph;
	float mMaxYGraph;
	int mNumberOfGridLines;
	bool mLinearInterpolation; // Default is linear interpolation, otherwise use spline

	void OnPaint(wxPaintEvent & event);
	void OnClear(wxEraseEvent & event);
	void OnMouseLButtonPressed(wxMouseEvent & event);
	void OnMouseLButtonReleased(wxMouseEvent & event);
	void OnMove(wxMouseEvent & event);
	void OnMouseRButtonPressed(wxMouseEvent & event);
	void OnWindowEnter(wxMouseEvent & event);
};

#endif /* __PUED_CONTROLPOINTCONTROL_H__ */