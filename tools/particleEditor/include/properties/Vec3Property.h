#ifndef __I6ENGINE_PARTICLEEDITOR_PROPERTIES_VEC3PROPERTY_H__
#define __I6ENGINE_PARTICLEEDITOR_PROPERTIES_VEC3PROPERTY_H__

#include "properties/Property.h"

#include "ParticleUniversePrerequisites.h"

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

	protected:
		ParticleUniverse::Vector3 _value;
	};

} /* namespace properties */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_PROPERTIES_VEC3PROPERTY_H__ */
