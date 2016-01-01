#ifndef __I6ENGINE_PLUGINS_EDITORTYPEVEC3_H__
#define __I6ENGINE_PLUGINS_EDITORTYPEVEC3_H__

#include "i6engine/editor/plugins/TypePluginInterface.h"

class QLineEdit;

namespace i6engine {
namespace plugins {

	class EditorTypeVec3Widget : public editor::plugins::TypeWidgetInterface {
		Q_OBJECT

	public:
		EditorTypeVec3Widget(QWidget * parent);

		void setValues(const std::string & values);

		std::string getValues() const;

		void setReadOnly(bool readOnly);

	private:
		QLineEdit * _lineEditX;
		QLineEdit * _lineEditY;
		QLineEdit * _lineEditZ;
	};

	class EditorTypeVec3 : public QObject, public editor::plugins::TypePluginInterface {
		Q_OBJECT
		Q_PLUGIN_METADATA(IID "i6engine.editor.type.TypePluginInterface")
		Q_INTERFACES(i6engine::editor::plugins::TypePluginInterface)

	public:
		std::string getIdentifier() const override {
			return "Vec3";
		}

		editor::plugins::TypeWidgetInterface * createWidget(QWidget * parent);
	};

} /* namespace plugins */
} /* namespace i6engine */

#endif /* __I6ENGINE_PLUGINS_EDITORTYPEVEC3_H__ */
