#ifndef __I6ENGINE_EDITOR_WIDGETS_OBJECTINFOWIDGET_H__
#define __I6ENGINE_EDITOR_WIDGETS_OBJECTINFOWIDGET_H__

#include "ui_widgetObjectInfo.h"

#include <cstdint>

#include "boost/function.hpp"

namespace i6engine {
namespace editor {
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
		void selectObject(int64_t id);
		void removeObject();
		void updateObjectList();

	private slots:
		void doSelectObject(int64_t id);
		void doRemoveObject();
		void applyChanges();

	private:
		int64_t _selectedObjectID;
		std::vector<QWidget *> _infos;
		std::map<QLineEdit *, boost::function<void(std::string)>> _entries;
	};

} /* namespace widgets */
} /* namespace editor */
} /* namespace i6engine */

#endif /* __I6ENGINE_EDITOR_WIDGETS_OBJECTINFOWIDGET_H__ */
