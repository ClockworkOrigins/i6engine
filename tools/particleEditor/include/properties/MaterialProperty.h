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

#ifndef __I6ENGINE_PARTICLEEDITOR_PROPERTIES_MATERIALPROPERTY_H__
#define __I6ENGINE_PARTICLEEDITOR_PROPERTIES_MATERIALPROPERTY_H__

#include "properties/Property.h"

namespace i6e {
namespace particleEditor {
namespace properties {

	class MaterialProperty : public Property {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		MaterialProperty(QWidget * par, QString label, QString name, QString value);

		/**
		 * \brief destructor
		 */
		~MaterialProperty();

		void setString(QString value) override;

		QString getString() const override {
			return _value;
		}

	private slots:
		void stringChanged();
		void selectMaterial();

	protected:
		QString _value;
		QLineEdit * _lineEdit;
	};

} /* namespace properties */
} /* namespace particleEditor */
} /* namespace i6e */

#endif /* __I6ENGINE_PARTICLEEDITOR_PROPERTIES_MATERIALPROPERTY_H__ */
