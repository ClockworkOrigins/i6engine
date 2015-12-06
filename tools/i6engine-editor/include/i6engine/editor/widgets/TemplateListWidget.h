#ifndef __I6ENGINE_EDITOR_WIDGETS_TEMPLATELISTWIDGET_H__
#define __I6ENGINE_EDITOR_WIDGETS_TEMPLATELISTWIDGET_H__

#include "ui_widgetTemplateList.h"

#include <cstdint>

namespace i6engine {
namespace editor {

	class Editor;

namespace widgets {

	class TemplateListWidget : public QWidget, public Ui::templateListWidget {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		TemplateListWidget(QWidget * par);

		/**
		 * \brief destructor
		 */
		~TemplateListWidget();

	signals:
		void loadTemplates();

	private slots:
		void doLoadTemplates();
		void selectIndex(QModelIndex index);

	private:
		Editor * _editor;
	};

} /* namespace widgets */
} /* namespace editor */
} /* namespace i6engine */

#endif /* __I6ENGINE_EDITOR_WIDGETS_TEMPLATELISTWIDGET_H__ */
