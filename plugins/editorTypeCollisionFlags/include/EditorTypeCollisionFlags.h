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

#ifndef __I6ENGINE_PLUGINS_EDITORTYPECOLLISIONFLAGS_H__
#define __I6ENGINE_PLUGINS_EDITORTYPECOLLISIONFLAGS_H__

#include "i6engine/editor/plugins/TypePluginInterface.h"

class QPushButton;

namespace i6e {
namespace plugins {

	class CollisionFlagsDialog;

	class EditorTypeCollisionFlagsWidget : public editor::plugins::TypeWidgetInterface {
		Q_OBJECT

	public:
		EditorTypeCollisionFlagsWidget(QWidget * parent);

		void setValues(const std::string & values) override;

		std::string getValues() const override;

		void setReadOnly(bool readOnly) override;

	private slots:
		void openCollisionFlagsDialog();

	private:
		QPushButton * _openCollisionFlagsDialogButton;
		CollisionFlagsDialog * _collisionFlagsDialog;
	};

	class EditorTypeCollisionFlags : public QObject, public editor::plugins::TypePluginInterface {
		Q_OBJECT
		Q_PLUGIN_METADATA(IID "i6e.editor.type.TypePluginInterface")
		Q_INTERFACES(i6e::editor::plugins::TypePluginInterface)

	public:
		std::string getIdentifier() const override {
			return "CollisionFlags";
		}

		editor::plugins::TypeWidgetInterface * createWidget(QWidget * parent) override;
	};

} /* namespace plugins */
} /* namespace i6e */

#endif /* __I6ENGINE_PLUGINS_EDITORTYPECOLLISIONFLAGS_H__ */
