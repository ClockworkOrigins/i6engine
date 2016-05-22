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

#include "widgets/WidgetScript.h"

#include "ParticleUniverseSystemManager.h"

#include "utils/SyntaxHighlighter.h"

namespace i6e {
namespace particleEditor {
namespace widgets {

	WidgetScript::WidgetScript(QWidget * par) : QWidget(par), _changeable(true) {
		setupUi(this);

		_syntaxHighlighter = new utils::SyntaxHighlighter(textEdit->document());

		connect(textEdit, SIGNAL(textChanged()), this, SLOT(changedText()));
	}

	WidgetScript::~WidgetScript() {
	}

	QString WidgetScript::getScript() const {
		return textEdit->toPlainText();
	}

	void WidgetScript::loadScript(ParticleUniverse::ParticleSystem * system) {
		if (system) {
			_changeable = false;
			ParticleUniverse::String script = ParticleUniverse::ParticleSystemManager::getSingleton().writeScript(system);
			textEdit->setText(QString::fromStdString(script));
			_changeable = true;
		}
	}

	void WidgetScript::parseScript() {
		Ogre::ScriptCompilerManager * scriptCompilerManager = Ogre::ScriptCompilerManager::getSingletonPtr();
		char * buffer = new char[textEdit->toPlainText().length() + 1];
		strcpy(buffer, textEdit->toPlainText().toStdString().c_str());
		Ogre::DataStreamPtr * datastream = new Ogre::DataStreamPtr(new Ogre::MemoryDataStream(buffer, strlen(buffer)));
		scriptCompilerManager->parseScript(*datastream, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
		delete datastream;
		delete[] buffer;
	}

	void WidgetScript::changedText() {
		if (_changeable) {
			emit notifyChanged();
		}
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6e */
