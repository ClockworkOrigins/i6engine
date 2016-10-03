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

#include "EditorTypeCollisionFlags.h"

#include "CollisionFlagsDialog.h"

#include "i6engine/api/components/PhysicalStateComponent.h"

#include <QApplication>
#include <QHBoxLayout>
#include <QPushButton>

namespace i6e {
namespace plugins {

	EditorTypeCollisionFlagsWidget::EditorTypeCollisionFlagsWidget(QWidget * parent) : TypeWidgetInterface(parent), _openCollisionFlagsDialogButton(nullptr), _collisionFlagsDialog(nullptr) {
		QLayout * layout = new QHBoxLayout(this);
		_openCollisionFlagsDialogButton = new QPushButton(QApplication::tr("Open CollisionFlags"), this);
		layout->addWidget(_openCollisionFlagsDialogButton);
		setLayout(layout);

		_collisionFlagsDialog = new CollisionFlagsDialog(this);

		connect(_openCollisionFlagsDialogButton, SIGNAL(clicked()), _collisionFlagsDialog, SLOT(exec()));
	}

	void EditorTypeCollisionFlagsWidget::setValues(const std::string & values) {
		api::CollisionGroup cg(values);
		_collisionFlagsDialog->setCollisionGroup(cg);
	}

	std::string EditorTypeCollisionFlagsWidget::getValues() const {
		return _collisionFlagsDialog->getCollisionGroup().toString();
	}

	void EditorTypeCollisionFlagsWidget::setReadOnly(bool readOnly) {
		_collisionFlagsDialog->setReadOnly(readOnly);
	}

	void EditorTypeCollisionFlagsWidget::openCollisionFlagsDialog() {
		_collisionFlagsDialog->exec();
	}

	editor::plugins::TypeWidgetInterface * EditorTypeCollisionFlags::createWidget(QWidget * parent) {
		return new EditorTypeCollisionFlagsWidget(parent);
	}

} /* namespace plugins */
} /* namespace i6e */

Q_PLUGIN_METADATA(IID "i6e.editor.type.TypePluginInterface")
