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

#ifndef __I6ENGINE_EDITOR_WIDGETS_RENDERWIDGET_H__
#define __I6ENGINE_EDITOR_WIDGETS_RENDERWIDGET_H__

#include "ui_widgetRender.h"

namespace i6e {
namespace editor {
namespace widgets {

	class RenderWidget : public QWidget, public Ui::renderWidget {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		RenderWidget(QWidget * par);

		/**
		 * \brief destructor
		 */
		~RenderWidget();

	private:
		void mouseMoveEvent(QMouseEvent * evt) override;
		bool eventFilter(QObject * obj, QEvent * evt) override;
	};

} /* namespace widgets */
} /* namespace editor */
} /* namespace i6e */

#endif /* __I6ENGINE_EDITOR_WIDGETS_RENDERWIDGET_H__ */
