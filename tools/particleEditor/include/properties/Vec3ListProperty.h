#ifndef __I6ENGINE_PARTICLEEDITOR_PROPERTIES_DYNAMICPROPERTYPROPERTY_H__
#define __I6ENGINE_PARTICLEEDITOR_PROPERTIES_DYNAMICPROPERTYPROPERTY_H__

#include "properties/Property.h"

namespace i6engine {
namespace particleEditor {
namespace properties {

	class Vec3Property;

	class Vec3ListProperty : public Property {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		Vec3ListProperty(QWidget * par, QString label, QString name);

		/**
		 * \brief destructor
		 */
		~Vec3ListProperty();

	private slots:
		void addPosition();

	protected:
		QVBoxLayout * _layout;
		std::vector<Vec3Property *> _positions;
	};

} /* namespace properties */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_PROPERTIES_DYNAMICPROPERTYPROPERTY_H__ */
