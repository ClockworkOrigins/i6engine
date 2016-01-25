#ifndef __I6ENGINE_PLUGINS_EDITORTYPEQUATERNION_H__
#define __I6ENGINE_PLUGINS_EDITORTYPEQUATERNION_H__

#include "i6engine/editor/plugins/TypePluginInterface.h"

class QDoubleSpinBox;
class QLineEdit;

namespace i6engine {
namespace plugins {

	class EditorTypeQuaternionWidget : public editor::plugins::TypeWidgetInterface {
		Q_OBJECT

	public:
		EditorTypeQuaternionWidget(QWidget * parent);

		void setValues(const std::string & values);

		std::string getValues() const;

		void setReadOnly(bool readOnly);

	private:
		QLineEdit * _lineEditX;
		QLineEdit * _lineEditY;
		QLineEdit * _lineEditZ;
		QDoubleSpinBox * _spinBox;
	};

	class EditorTypeQuaternion : public QObject, public editor::plugins::TypePluginInterface {
		Q_OBJECT
		Q_PLUGIN_METADATA(IID "i6engine.editor.type.TypePluginInterface")
		Q_INTERFACES(i6engine::editor::plugins::TypePluginInterface)

	public:
		std::string getIdentifier() const override {
			return "Quaternion";
		}

		editor::plugins::TypeWidgetInterface * createWidget(QWidget * parent);
	};

} /* namespace plugins */
} /* namespace i6engine */

#endif /* __I6ENGINE_PLUGINS_EDITORTYPEQUATERNION_H__ */
