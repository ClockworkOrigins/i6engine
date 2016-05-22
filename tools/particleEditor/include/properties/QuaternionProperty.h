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

#ifndef __I6ENGINE_PARTICLEEDITOR_PROPERTIES_QUATERNIONPROPERTY_H__
#define __I6ENGINE_PARTICLEEDITOR_PROPERTIES_QUATERNIONPROPERTY_H__

#include "properties/Property.h"

#include "i6engine/math/i6eQuaternion.h"

class QDoubleSpinBox;

namespace i6e {
namespace particleEditor {
namespace properties {

	class Vec3Property;

	class QuaternionProperty : public Property {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		QuaternionProperty(QWidget * par, QString label, QString name, Quaternion value);

		/**
		 * \brief destructor
		 */
		~QuaternionProperty();

		void setQuaternion(Quaternion value) override;

		Quaternion getQuaternion() const override {
			return _value;
		}

	private slots:
		void changedValue();

	private:
		QGridLayout * _layout;
		Quaternion _value;
		Vec3Property * _vec3Property;
		QDoubleSpinBox * _doubleSpinBox;
	};

} /* namespace properties */
} /* namespace particleEditor */
} /* namespace i6e */

#endif /* __I6ENGINE_PARTICLEEDITOR_PROPERTIES_QUATERNIONPROPERTY_H__ */
