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

#include "EditorTypeShapeType.h"

#include <QHBoxLayout>
#include <QComboBox>

namespace i6e {
namespace plugins {

	std::map<QString, api::PhysicalStateComponent::ShapeType> EditorTypeShapeTypeWidget::shapeTypes = { { "PLANE", api::PhysicalStateComponent::ShapeType::PLANE }, { "BOX", api::PhysicalStateComponent::ShapeType::BOX }, { "SPHERE", api::PhysicalStateComponent::ShapeType::SPHERE }, { "FILE", api::PhysicalStateComponent::ShapeType::FILE } };

	EditorTypeShapeTypeWidget::EditorTypeShapeTypeWidget(QWidget * parent) : TypeWidgetInterface(parent), _comboBox(nullptr) {
		QLayout * layout = new QHBoxLayout(this);
		_comboBox = new QComboBox(this);
		for (auto & p : shapeTypes) {
			_comboBox->addItem(p.first);
		}
		layout->addWidget(_comboBox);
		setLayout(layout);
	}

	void EditorTypeShapeTypeWidget::setValues(const std::string & values) {
		api::PhysicalStateComponent::ShapeType st = api::PhysicalStateComponent::ShapeType(std::stoi(values));
		for (auto & p : shapeTypes) {
			if (p.second == st) {
				_comboBox->setCurrentText(p.first);
				break;
			}
		}
	}

	std::string EditorTypeShapeTypeWidget::getValues() const {
		return std::to_string(int(shapeTypes[_comboBox->currentText()]));
	}

	void EditorTypeShapeTypeWidget::setReadOnly(bool readOnly) {
		_comboBox->setEnabled(!readOnly);
	}

	editor::plugins::TypeWidgetInterface * EditorTypeShapeType::createWidget(QWidget * parent) {
		return new EditorTypeShapeTypeWidget(parent);
	}

} /* namespace plugins */
} /* namespace i6e */

Q_PLUGIN_METADATA(IID "i6e.editor.type.TypePluginInterface")
