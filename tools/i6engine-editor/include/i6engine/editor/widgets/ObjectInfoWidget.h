#ifndef __I6ENGINE_EDITOR_WIDGETS_OBJECTINFOWIDGET_H__
#define __I6ENGINE_EDITOR_WIDGETS_OBJECTINFOWIDGET_H__

#include "ui_widgetObjectInfo.h"

#include <cstdint>

#include "boost/function.hpp"

namespace i6e {
namespace editor {
namespace plugins {

	class TypePluginInterface;
	class TypeWidgetInterface;

} /* namespace plugins */
namespace widgets {

	class ObjectInfoWidget : public QWidget, public Ui::objectInfoWidget {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		ObjectInfoWidget(QWidget * par);

		/**
		 * \brief destructor
		 */
		~ObjectInfoWidget();

	signals:
		void selectObject(int64_t);
		void removeObject();
		void updateObjectList();
		void changedLevel();
		void selectedObject(int64_t);

	private slots:
		void doSelectObject(int64_t id);
		void doRemoveObject();
		void applyChanges();

	private:
		int64_t _selectedObjectID;
		std::vector<QWidget *> _infos;
		std::map<plugins::TypeWidgetInterface *, boost::function<void(std::string)>> _entries;
		std::map<std::string, plugins::TypePluginInterface *> _typePlugins;

		void loadTypePlugins();
	};

} /* namespace widgets */
} /* namespace editor */
} /* namespace i6e */

#endif /* __I6ENGINE_EDITOR_WIDGETS_OBJECTINFOWIDGET_H__ */
