#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_WIDGETPARTICLELIST_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_WIDGETPARTICLELIST_H__

#include "ui_widgetParticleList.h"

namespace ParticleUniverse {
	class ParticleSystem;
} /* namespace ParticleUniverse */

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

		bool existsTemplateName(QString templateName) const {
			return _templateMap.find(templateName) != _templateMap.end();
		}

	signals:
		void createNewSystem(const QString & templateName);

	private slots:
		void selectParticle(QTreeWidgetItem * item);
		void selectParticle(QString templateName);
		void setNewParticleSystem(ParticleUniverse::ParticleSystem * system);
		void notifyChanged();
		void saveParticle();

	private:
		QString _currentParticleTemplate;
		std::map<QString, QTreeWidgetItem *> _templateMap;
		ParticleUniverse::ParticleSystem * _system;
		bool _dirty;
		QString _script;
		std::map<QString, QString> _systemFileMapping;

		void refreshParticleList();
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_WIDGETPARTICLELIST_H__ */
