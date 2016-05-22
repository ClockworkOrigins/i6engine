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

#ifndef __I6ENGINE_PARTICLEEDITOR_PROPERTIES_PROPERTY_H__
#define __I6ENGINE_PARTICLEEDITOR_PROPERTIES_PROPERTY_H__

#include "ui_widgetProperty.h"

#include "i6engine/math/i6eQuaternion.h"
#include "i6engine/math/i6eVector4.h"

#include "ParticleUniversePrerequisites.h"

namespace ParticleUniverse {
	class DynamicAttribute;
} /* namespace ParticleUniverse */

namespace i6e {
namespace particleEditor {
namespace properties {

	class Property : public QWidget, public Ui::propertyWidget {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		Property(QWidget * par, QString label, QString name);

		/**
		 * \brief destructor
		 */
		virtual ~Property();

		QString getName() const {
			return _name;
		}

		virtual void setBool(bool value) {
			assert(false);
		}
		virtual void setColourWithAlpha(Vec4 value) {
			assert(false);
		}
		virtual void setDouble(double value) {
			assert(false);
		}
		virtual void setDynamicAttribute(ParticleUniverse::DynamicAttribute * value) {
			assert(false);
		}
		virtual void setEnumString(QString value) {
			assert(false);
		}
		virtual void setQuaternion(Quaternion value) {
			assert(false);
		}
		virtual void setString(QString value) {
			assert(false);
		}
		virtual void setTimeAndColourList(std::vector<std::pair<double, Vec4>> value) {
			assert(false);
		}
		virtual void setUInt(unsigned int value) {
			assert(false);
		}
		virtual void setVector3(ParticleUniverse::Vector3 value) {
			assert(false);
		}
		virtual void setVector3List(std::vector<ParticleUniverse::Vector3> value) {
			assert(false);
		}

		virtual bool getBool() const {
			assert(false);
			return false;
		}
		virtual Vec4 getColourWithAlpha() const {
			assert(false);
			return Vec4();
		}
		virtual double getDouble() const {
			assert(false);
			return 0.0;
		}
		virtual ParticleUniverse::DynamicAttribute * getDynamicAttribute() const {
			assert(false);
			return nullptr;
		}
		virtual int getCurrentIndex() const {
			assert(false);
			return 0;
		}
		virtual QString getEnumString() const {
			assert(false);
			return QString();
		}
		virtual Quaternion getQuaternion() const {
			assert(false);
			return Quaternion();
		}
		virtual QString getString() const {
			assert(false);
			return QString();
		}
		virtual std::vector<std::pair<double, Vec4>> getTimeAndColourList() const {
			assert(false);
			return std::vector<std::pair<double, Vec4>>();
		}
		virtual unsigned int getUInt() const {
			assert(false);
			return 0;
		}
		virtual ParticleUniverse::Vector3 getVector3() const {
			assert(false);
			return ParticleUniverse::Vector3();
		}
		virtual std::vector<ParticleUniverse::Vector3> getVector3List() const {
			assert(false);
			return std::vector<ParticleUniverse::Vector3>();
		}

	signals:
		void changed(QString);

	protected:
		QString _label;
		QString _name;

		void triggerChangedSignal();
	};

} /* namespace properties */
} /* namespace particleEditor */
} /* namespace i6e */

#endif /* __I6ENGINE_PARTICLEEDITOR_PROPERTIES_PROPERTY_H__ */
