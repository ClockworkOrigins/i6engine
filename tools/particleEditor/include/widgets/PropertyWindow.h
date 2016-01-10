#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_PROPERTYWINDOW_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_PROPERTYWINDOW_H__

#include "ui_widgetPropertyWindow.h"

#include "i6engine/math/i6eQuaternion.h"
#include "i6engine/math/i6eVector4.h"

#include "ParticleUniverseParticle.h"
#include "ParticleUniversePrerequisites.h"

#include <QStringList>

namespace ParticleUniverse {
	class DynamicAttribute;
	class IElement;
} /* namespace ParticleUniverse */

namespace i6engine {
namespace particleEditor {
namespace properties {
	class Property;
} /* namespace properties */
namespace widgets {

	class WidgetEditComponent;

	// Dynamic Attributes
	static QString DYN_FIXED = "";
	static QString DYN_RANDOM = "";
	static QString DYN_CURVED = "";
	static QString DYN_OSCILLATE = "";
	static QString DYN_CURVED_LINEAR = "";
	static QString DYN_CURVED_SPLINE = "";

	// Miscelanious
	static QString PRNL_NAME = "";
	static QString PRNL_POST_RGB = "";
	static QString PRNL_POST_ALPHA = "";
	static QString PRNL_W = "";
	static QString PRNL_X = "";
	static QString PRNL_Y = "";
	static QString PRNL_Z = "";
	static QString PRNL_TYPE = "";
	static QString PRNL_VALUE = "";
	static QString PRNL_MIN_VALUE = "";
	static QString PRNL_MAX_VALUE = "";
	static QString PRNL_OSC_TYPE = "";
	static QString PRNL_OSC_SINE = "";
	static QString PRNL_OSC_SQUARE = "";
	static QString PRNL_OSC_FREQUENCY = "";
	static QString PRNL_OSC_PHASE = "";
	static QString PRNL_OSC_BASE = "";
	static QString PRNL_OSC_AMPLITUDE = "";
	static QString PRNL_CURVED_INTERPOLATION = "";
	static QString PRNL_CONTROL_POINT = "";
	static QString PRNL_POSITION = "";
	static QString PRNL_CONTROL_POINT_X = "";
	static QString PRNL_CONTROL_POINT_Y = "";
	static QString PRNL_POSITION_X = "";
	static QString PRNL_POSITION_Y = "";
	static QString PRNL_POSITION_Z = "";
	static QString PRNL_FLOAT = "";
	static QString PRNL_SINCE_START_SYSTEM = "";
	static QString PRNL_COLLIDER_FRICTION = "";
	static QString PRNL_COLLIDER_BOUNCYNESS = "";
	static QString PRNL_INTERSECTION_TYPE = "";
	static QString PRNL_COLLISION_TYPE = "";
	static QString IST_POINT = "";
	static QString IST_BOX = "";
	static QString COLLT_NONE = "";
	static QString COLLT_BOUNCE = "";
	static QString COLLT_FLOW = "";
	static QString PRNL_FORCE_VECTOR = "";
	static QString PRNL_LINEAR_FORCE_APPLICATION = "";
	static QString APP_ADD = "";
	static QString APP_AVG = "";
	static QString PRNL_MAX_DEVIATION = "";
	static QString PRNL_TIME_STEP = "";
	static QString PRNL_ROTATION = "";
	static QString PRNL_ROTATION_SPEED = "";
	static QString PRNL_ROTATION_AXIS = "";
	static QString PRNL_MESH_NAME = "";
	static QString PRNL_DIFFUSE_COLOUR = "";
	static QString PRNL_SPECULAR_COLOUR = "";
	static QString PRNL_PARTICLE_TYPE = "";
	static QString PRNL_EXTERN_THRESHOLD = "";
	static QString PRNL_COMPARE_NOT_USED = "";
	static QString PRNL_COMPARE_LESS_THAN = "";
	static QString PRNL_COMPARE_GREATER_THAN = "";
	static QString PRNL_COMPARE_EQUALS = "";
	static QString PRNL_USE_OWN_SPEED = "";
	static QString PT_ALL = "";
	static QString PT_VISUAL = "";
	static QString PT_EMITTER = "";
	static QString PT_AFFECTOR = "";
	static QString PT_SYSTEM = "";
	static QString PT_TECHNIQUE = "";

	class PropertyWindow : public QWidget, public Ui::propertyWindowWidget {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		PropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name);

		/**
		 * \brief destructor
		 */
		virtual ~PropertyWindow();

		void setComponentName(QString name) {
			_name = name;
		}

		QString getComponentName() const {
			return _name;
		}

	signals:
		void replacePropertyWindow(QString);

	protected:
		WidgetEditComponent * _owner;
		QString _name;
		QStringList _types;
		std::map<QString, properties::Property *> _properties;

		void append(properties::Property * prop);

		void setBool(QString name, bool value);
		void setColourWithAlpha(QString name, Vec4 value);
		void setDouble(QString name, double value);
		void setDynamicAttribute(QString name, ParticleUniverse::DynamicAttribute * value);
		void setEnumString(QString name, QString value);
		void setQuaternion(QString name, Quaternion value);
		void setString(QString name, QString value);
		void setUint16(QString name, uint16_t value);
		void setVector3(QString name, ParticleUniverse::Vector3 value);
		void setVector3List(QString name, std::vector<ParticleUniverse::Vector3> value);

		virtual void changedProperty(properties::Property * prop, QString name);

		void restartParticle(ParticleUniverse::IElement * element, ParticleUniverse::Particle::ParticleType elementType, ParticleUniverse::Particle::ParticleType unprepareType);

	private slots:
		void changedProperty(QString name);

	private:
		bool mustStopParticleSystem(ParticleUniverse::ParticleSystem * system);
		void mustRestartParticleSystem(ParticleUniverse::ParticleSystem * system, bool wasStarted);
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_PROPERTYWINDOW_H__ */
