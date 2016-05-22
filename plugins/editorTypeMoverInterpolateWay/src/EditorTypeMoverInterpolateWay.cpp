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

#include "EditorTypeMoverInterpolateWay.h"

#include <QHBoxLayout>
#include <QComboBox>

namespace i6e {
namespace plugins {

	std::map<QString, api::MoverInterpolateComponent::Way> EditorTypeMoverInterpolateWayWidget::ways = { { "LINEAR", api::MoverInterpolateComponent::Way::LINEAR }, { "BEZIER", api::MoverInterpolateComponent::Way::BEZIER } };

	EditorTypeMoverInterpolateWayWidget::EditorTypeMoverInterpolateWayWidget(QWidget * parent) : TypeWidgetInterface(parent), _comboBox(nullptr) {
		QLayout * layout = new QHBoxLayout(this);
		_comboBox = new QComboBox(this);
		for (auto & p : ways) {
			_comboBox->addItem(p.first);
		}
		layout->addWidget(_comboBox);
		setLayout(layout);
	}

	void EditorTypeMoverInterpolateWayWidget::setValues(const std::string & values) {
		api::MoverInterpolateComponent::Way way = api::MoverInterpolateComponent::Way(std::stoi(values));
		for (auto & p : ways) {
			if (p.second == way) {
				_comboBox->setCurrentText(p.first);
				break;
			}
		}
	}

	std::string EditorTypeMoverInterpolateWayWidget::getValues() const {
		return std::to_string(int(ways[_comboBox->currentText()]));
	}

	void EditorTypeMoverInterpolateWayWidget::setReadOnly(bool readOnly) {
		_comboBox->setEnabled(!readOnly);
	}

	editor::plugins::TypeWidgetInterface * EditorTypeMoverInterpolateWay::createWidget(QWidget * parent) {
		return new EditorTypeMoverInterpolateWayWidget(parent);
	}

} /* namespace plugins */
} /* namespace i6e */

Q_PLUGIN_METADATA(IID "i6e.editor.type.TypePluginInterface")
