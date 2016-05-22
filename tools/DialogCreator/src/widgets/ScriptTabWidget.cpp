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

#include "widgets/ScriptTabWidget.h"

#include <QStringListModel>

namespace i6e {
namespace dialogCreator {
namespace widgets {

	ScriptTabWidget::ScriptTabWidget(QWidget * par, plugins::ScriptLanguagePluginInterface * plugin) : QWidget(par), _plugin(plugin), _scriptFunctions(_plugin->getScriptFunctions()) {
		setupUi(this);

		QStringList list;

		for (dialogCreator::plugins::ScriptFunction scriptFunc : _scriptFunctions) {
			QString entry = scriptFunc.name + "(";
			for (int i = 0; i < scriptFunc.signatureTypes.size(); i++) {
				entry += scriptFunc.signatureTypes[i];
				if (i < scriptFunc.signatureTypes.size() - 1) {
					entry += ", ";
				}
			}
			entry += ")";
			if (scriptFunc.returnType != "void") {
				entry += ": " + scriptFunc.returnType;
			}
			list.append(entry);
		}

		QStringListModel * model = new QStringListModel(listView);
		model->setStringList(list);
		listView->setModel(model);
	}

	ScriptTabWidget::~ScriptTabWidget() {
	}

} /* namespace widgets */
} /* namespace dialogCreator */
} /* namespace i6e */
