#include "widgets/WidgetScript.h"

#include "ParticleUniverseSystemManager.h"

namespace i6e {
namespace particleEditor {
namespace widgets {

	WidgetScript::WidgetScript(QWidget * par) : QWidget(par), _changeable(true) {
		setupUi(this);

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
