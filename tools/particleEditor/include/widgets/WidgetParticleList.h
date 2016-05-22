/*
 * i6engine
 * Copyright (2016) Daniel Bonrath, Michael Baer, All rights reserved.
 *
 * This file is part of i6engine; i6engine is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_WIDGETPARTICLELIST_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_WIDGETPARTICLELIST_H__

#include "ui_widgetParticleList.h"

namespace ParticleUniverse {
	class ParticleSystem;
} /* namespace ParticleUniverse */

namespace i6e {
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
} /* namespace i6e */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_WIDGETPARTICLELIST_H__ */
