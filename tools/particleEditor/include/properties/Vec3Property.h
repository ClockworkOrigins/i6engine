#ifndef __I6ENGINE_PARTICLEEDITOR_PROPERTIES_VEC3PROPERTY_H__
#define __I6ENGINE_PARTICLEEDITOR_PROPERTIES_VEC3PROPERTY_H__

#include "properties/Property.h"

#include "ParticleUniversePrerequisites.h"

class QDoubleSpinBox;

namespace i6engine {
namespace particleEditor {
namespace properties {

	class Vec3Property : public Property {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		Vec3Property(QWidget * par, QString label, QString name, ParticleUniverse::Vector3 value);

		/**
		 * \brief destructor
		 */
		~Vec3Property();

		void setVector3(ParticleUniverse::Vector3 value) override;

		ParticleUniverse::Vector3 getVector3() const override {
			return _value;
		}

	private slots:
		void changedValue();

	private:
		ParticleUniverse::Vector3 _value;
		QDoubleSpinBox * _doubleSpinBoxX;
		QDoubleSpinBox * _doubleSpinBoxY;
		QDoubleSpinBox * _doubleSpinBoxZ;
		bool _set;
	};

} /* namespace properties */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_PROPERTIES_VEC3PROPERTY_H__ */
