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

#ifndef __WX_OGRE_APP_H__
#define __WX_OGRE_APP_H__

#include <wx/wx.h>

class wxOgreRenderSystem;
class wxOgreResources;

/** Base class for any wxOgre based application.
 *
 * @author Martin Pieuchot
 */
class wxOgreApp : public wxApp {
public:
    /** Initialize the Ogre Render Engine.
     * @remark This is the very first function called for a newly
     *         created wxOgreApp object, it is used by the library to
     *         initialize the Ogre Engine. If, for some reason, you 
     *         must override it, do not forget to call the base class
     *         version!
     */
	virtual bool Initialize(int & ac, wxChar ** av) override;

    /** Clean the Ogre Render Engine instance. 
     * @remark This is the very last function called on wxApp object 
     *         before it is destroyed. If you override it do not 
     *         forget to call the base class version!
     */
    virtual void CleanUp() override;

protected:
    wxOgreRenderSystem * _renderSystem;
    wxOgreResources * _resources;
};

#endif /* __WX_OGRE_APP_H__ */
