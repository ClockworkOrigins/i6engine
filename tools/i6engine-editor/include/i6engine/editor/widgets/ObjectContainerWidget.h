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

#ifndef __I6ENGINE_EDITOR_WIDGETS_OBJECTCONTAINERWIDGET_H__
#define __I6ENGINE_EDITOR_WIDGETS_OBJECTCONTAINERWIDGET_H__

#include "ui_widgetObjectContainer.h"

#include <cstdint>

namespace i6e {
namespace editor {
namespace widgets {

	class ObjectInfoWidget;
	class ObjectListWidget;

	class ObjectContainerWidget : public QWidget, public Ui::objectContainerWidget {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		ObjectContainerWidget(QWidget * par);

		/**
		 * \brief destructor
		 */
		~ObjectContainerWidget();

		ObjectListWidget * objectListWidget;
		ObjectInfoWidget * objectInfoWidget;
	};

} /* namespace widgets */
} /* namespace editor */
} /* namespace i6e */

#endif /* __I6ENGINE_EDITOR_WIDGETS_OBJECTCONTAINERWIDGET_H__ */
