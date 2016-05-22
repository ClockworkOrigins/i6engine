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

#include "EditorTypeLightType.h"

#include <QHBoxLayout>
#include <QComboBox>

namespace i6e {
namespace plugins {

	std::map<QString, api::LuminousAppearanceComponent::LightType> EditorTypeLightTypeWidget::lightTypes = { { "POINT", api::LuminousAppearanceComponent::LightType::POINT }, { "DIRECTIONAL", api::LuminousAppearanceComponent::LightType::DIRECTIONAL }, { "SPOT", api::LuminousAppearanceComponent::LightType::SPOT } };

	EditorTypeLightTypeWidget::EditorTypeLightTypeWidget(QWidget * parent) : TypeWidgetInterface(parent), _comboBox(nullptr) {
		QLayout * layout = new QHBoxLayout(this);
		_comboBox = new QComboBox(this);
		for (auto & p : lightTypes) {
			_comboBox->addItem(p.first);
		}
		layout->addWidget(_comboBox);
		setLayout(layout);
	}

	void EditorTypeLightTypeWidget::setValues(const std::string & values) {
		api::LuminousAppearanceComponent::LightType lt = api::LuminousAppearanceComponent::LightType(std::stoi(values));
		for (auto & p : lightTypes) {
			if (p.second == lt) {
				_comboBox->setCurrentText(p.first);
				break;
			}
		}
	}

	std::string EditorTypeLightTypeWidget::getValues() const {
		return std::to_string(int(lightTypes[_comboBox->currentText()]));
	}

	void EditorTypeLightTypeWidget::setReadOnly(bool readOnly) {
		_comboBox->setEnabled(!readOnly);
	}

	editor::plugins::TypeWidgetInterface * EditorTypeLightType::createWidget(QWidget * parent) {
		return new EditorTypeLightTypeWidget(parent);
	}

} /* namespace plugins */
} /* namespace i6e */

Q_PLUGIN_METADATA(IID "i6e.editor.type.TypePluginInterface")
