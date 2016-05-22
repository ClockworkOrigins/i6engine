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

/**
 * \addtogroup GUI
 * @{
 */

#ifndef __I6ENGINE_MODULES_GUIIMAGEBUTTON_H__
#define __I6ENGINE_MODULES_GUIIMAGEBUTTON_H__

#include "i6engine/api/gui/GUIWidget.h"

namespace CEGUI {
	class EventArgs;
} /* namespace CEGUI */

namespace i6e {
namespace modules {

	class GUIImageButton : public api::GUIWidget {
	public:
		GUIImageButton(const std::string & name, const std::string & type);

		~GUIImageButton();

		void update(uint16_t type, api::gui::GUIUpdateMessageStruct * data) override;
	};

} /* namespace modules */
} /* namespace i6e */

#endif /* __I6ENGINE_MODULES_GUIIMAGEBUTTON_H__ */

/**
 * @}
 */
