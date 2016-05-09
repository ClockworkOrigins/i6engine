#ifndef __I6ENGINE_PLUGINS_EDITORRPGNPCOBJECTTYPE_H__
#define __I6ENGINE_PLUGINS_EDITORRPGNPCOBJECTTYPE_H__

#include "i6engine/editor/plugins/ObjectTypePluginInterface.h"

#include <QModelIndex>

class QListView;

namespace i6e {
namespace plugins {

	class EditorRPGNPCObjectTypeWidget : public editor::plugins::ObjectTypeWidgetInterface {
		Q_OBJECT

	public:
		EditorRPGNPCObjectTypeWidget(QWidget * parent);

	private slots:
		void selectIndex(QModelIndex index);

	private:
		QListView * _listView;
	};

	class EditorRPGNPCObjectType : public QObject, public editor::plugins::ObjectTypePluginInterface {
		Q_OBJECT
		Q_PLUGIN_METADATA(IID "i6e.editor.objectType.ObjectTypePluginInterface")
		Q_INTERFACES(i6e::editor::plugins::ObjectTypePluginInterface)

	public:
		std::string getIdentifier() const override {
			return "RPGNPC";
		}

		editor::plugins::ObjectTypeWidgetInterface * createWidget(QWidget * parent);
	};

} /* namespace plugins */
} /* namespace i6e */

#endif /* __I6ENGINE_PLUGINS_EDITORRPGNPCOBJECTTYPE_H__ */
