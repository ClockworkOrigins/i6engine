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

#ifndef __I6ENGINE_PLUGINS_COLLISIONFLAGSDIALOG_H__
#define __I6ENGINE_PLUGINS_COLLISIONFLAGSDIALOG_H__

#include "i6engine/api/components/PhysicalStateComponent.h"

#include <QDialog>

class QCheckBox;
class QGridLayout;
class QRadioButton;

namespace i6e {
namespace plugins {

	class CollisionFlagsDialog : public QDialog {
		Q_OBJECT

	public:
		CollisionFlagsDialog(QWidget * par);

		void setCollisionGroup(const api::CollisionGroup & cg);
		api::CollisionGroup getCollisionGroup() const;
		void setReadOnly(bool readOnly);

	private slots:
		void updateCollisionGroup();
		void resetCollisionGroup();
		void addCollisionType();

	private:
		api::CollisionGroup _collisionGroup;
		QVector<QString> _collisionTypes;
		QCheckBox * _staticTypeCheckbox;
		QCheckBox * _ghostTypeCheckbox;
		QCheckBox * _triggerTypeCheckbox;
		QGridLayout * _ownCollisionTypesLayout;
		QVector<QRadioButton *> _ownCollisionTypes;
		QGridLayout * _collidableCollisionTypesLayout;
		QVector<QCheckBox *> _collidableCollisionTypes;

		void loadCollisionTypes();
		void saveCollisionTypes();
		void createGUI();
	};

} /* namespace plugins */
} /* namespace i6e */

#endif /* __I6ENGINE_PLUGINS_COLLISIONFLAGSDIALOG_H__ */
