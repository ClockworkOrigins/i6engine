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

#ifndef __I6ENGINE_PLUGINS_EDITORRPGNPCOBJECTTYPE_H__
#define __I6ENGINE_PLUGINS_EDITORRPGNPCOBJECTTYPE_H__

#include "i6engine/editor/plugins/ObjectTypePluginInterface.h"

#include <QModelIndex>

class QListView;

namespace i6e {
namespace plugins {

	class EditorRPGNPCObjectTypeWidget : public editor::plugins::ObjectTypeWidgetInterface {
		Q_OBJECT

	public:
		EditorRPGNPCObjectTypeWidget(QWidget * parent);

	private slots:
		void selectIndex(QModelIndex index);

	private:
		QListView * _listView;
	};

	class EditorRPGNPCObjectType : public QObject, public editor::plugins::ObjectTypePluginInterface {
		Q_OBJECT
		Q_PLUGIN_METADATA(IID "i6e.editor.objectType.ObjectTypePluginInterface")
		Q_INTERFACES(i6e::editor::plugins::ObjectTypePluginInterface)

	public:
		std::string getIdentifier() const override {
			return "RPGNPC";
		}

		editor::plugins::ObjectTypeWidgetInterface * createWidget(QWidget * parent);
	};

} /* namespace plugins */
} /* namespace i6e */

#endif /* __I6ENGINE_PLUGINS_EDITORRPGNPCOBJECTTYPE_H__ */
