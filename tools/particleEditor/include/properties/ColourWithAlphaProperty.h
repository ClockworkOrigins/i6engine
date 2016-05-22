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

#ifndef __I6ENGINE_PARTICLEEDITOR_PROPERTIES_COLOURWITHALPHAPROPERTY_H__
#define __I6ENGINE_PARTICLEEDITOR_PROPERTIES_COLOURWITHALPHAPROPERTY_H__

#include "properties/Property.h"

#include "i6engine/math/i6eVector4.h"

class QSpinBox;

namespace i6e {
namespace particleEditor {
namespace properties {

	class ColourWithAlphaProperty : public Property {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		ColourWithAlphaProperty(QWidget * par, QString label, QString name, Vec4 value);

		/**
		 * \brief destructor
		 */
		~ColourWithAlphaProperty();

		void setColourWithAlpha(Vec4 value) override;

		Vec4 getColourWithAlpha() const override {
			return _value;
		}

	private slots:
		void valueChanged();

	private:
		QGridLayout * _layout;
		Vec4 _value;
		QSpinBox * _spinBoxR;
		QSpinBox * _spinBoxG;
		QSpinBox * _spinBoxB;
		QSpinBox * _spinBoxA;
		bool _set;
	};

} /* namespace properties */
} /* namespace particleEditor */
} /* namespace i6e */

#endif /* __I6ENGINE_PARTICLEEDITOR_PROPERTIES_COLOURWITHALPHAPROPERTY_H__ */
