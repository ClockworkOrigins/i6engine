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

#ifndef __I6ENGINE_EDITOR_WIDGETS_OBJECTINFOWIDGET_H__
#define __I6ENGINE_EDITOR_WIDGETS_OBJECTINFOWIDGET_H__

#include "ui_widgetObjectInfo.h"

#include <cstdint>

#include "boost/function.hpp"

namespace i6e {
namespace editor {
namespace plugins {

	class TypePluginInterface;
	class TypeWidgetInterface;

} /* namespace plugins */
namespace widgets {

	class ObjectInfoWidget : public QWidget, public Ui::objectInfoWidget {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		ObjectInfoWidget(QWidget * par);

		/**
		 * \brief destructor
		 */
		~ObjectInfoWidget();

	signals:
		void selectObject(int64_t);
		void removeObject();
		void updateObjectList();
		void changedLevel();
		void selectedObject(int64_t);

	private slots:
		void doSelectObject(int64_t id);
		void doRemoveObject();
		void applyChanges();

	private:
		int64_t _selectedObjectID;
		std::vector<QWidget *> _infos;
		std::map<plugins::TypeWidgetInterface *, boost::function<void(std::string)>> _entries;
		std::map<std::string, plugins::TypePluginInterface *> _typePlugins;

		void loadTypePlugins();
	};

} /* namespace widgets */
} /* namespace editor */
} /* namespace i6e */

#endif /* __I6ENGINE_EDITOR_WIDGETS_OBJECTINFOWIDGET_H__ */
