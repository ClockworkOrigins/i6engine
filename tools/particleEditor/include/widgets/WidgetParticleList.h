#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_WIDGETPARTICLELIST_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_WIDGETPARTICLELIST_H__

#include "ui_widgetParticleList.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	class MainWindow;

	class WidgetParticleList : public QWidget, public Ui::particleListWidget {
		Q_OBJECT

		friend class MainWindow;

	public:
		/**
		 * \brief constructor
		 */
		WidgetParticleList(QWidget * par);

		/**
		 * \brief destructor
		 */
		~WidgetParticleList();

		QString getTemplateName() const {
			return _currentParticleTemplate;
		}

	signals:
		void createNewSystem(const QString & templateName);

	private slots:
		void selectParticle(QTreeWidgetItem * item);
		void selectParticle(QString templateName);

	private:
		QString _currentParticleTemplate;
		std::map<QString, QTreeWidgetItem *> _templateMap;

		void refreshParticleList();
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_WIDGETPARTICLELIST_H__ */
