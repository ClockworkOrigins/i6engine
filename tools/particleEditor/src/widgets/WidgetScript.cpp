#include "widgets/WidgetScript.h"

#include "ParticleUniverseSystemManager.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	WidgetScript::WidgetScript(QWidget * par) : QWidget(par) {
		setupUi(this);

		connect(textEdit, SIGNAL(textChanged()), this, SLOT(changedText()));
	}

	WidgetScript::~WidgetScript() {
	}

	QString WidgetScript::getScript() const {
		return textEdit->toPlainText();
	}

	void WidgetScript::loadScript(ParticleUniverse::ParticleSystem * system) {
		ParticleUniverse::String script = ParticleUniverse::ParticleSystemManager::getSingleton().writeScript(system);
		textEdit->setText(QString::fromStdString(script));
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
		emit notifyChanged();
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */
