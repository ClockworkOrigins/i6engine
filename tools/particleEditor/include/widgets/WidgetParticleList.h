#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_WIDGETPARTICLELIST_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_WIDGETPARTICLELIST_H__

#include "ui_widgetParticleList.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	class WidgetParticleList : public QWidget, public Ui::particleListWidget {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		WidgetParticleList(QWidget * par);

		/**
		 * \brief destructor
		 */
		~WidgetParticleList();

	signals:
		void createNewSystem(const QString & templateName);

	private slots:
		void selectParticle(QTreeWidgetItem * item);

	private:
		QString _currentParticleTemplate;

		void refreshParticleList();
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_WIDGETPARTICLELIST_H__ */
