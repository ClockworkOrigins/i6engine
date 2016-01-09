#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_EMITTERPROPERTYWINDOW_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_EMITTERPROPERTYWINDOW_H__

#include "widgets/PropertyWindow.h"

namespace ParticleUniverse {
	class ParticleEmitter;
} /* namespace ParticleUniverse */

namespace i6engine {
namespace particleEditor {
namespace widgets {

	// Property names
	static QString PRNL_EMITTER_TYPE = "";
	static QString PRNL_EMITTER_ENABLED = "";
	static QString PRNL_EMITTER_POSITION = "";
	static QString PRNL_EMITTER_KEEP_LOCAL = "";
	static QString PRNL_EMITTER_DIRECTION = "";
	static QString PRNL_EMITTER_ORIENTATION = "";
	static QString PRNL_EMITTER_ORIENTATION_START = "";
	static QString PRNL_EMITTER_ORIENTATION_END = "";
	static QString PRNL_EMITTER_VELOCITY = "";
	static QString PRNL_EMITTER_DURATION = "";
	static QString PRNL_EMITTER_REPEAT_DELAY = "";
	static QString PRNL_EMITTER_ANGLE = "";
	static QString PRNL_EMITTER_EMISSION_RATE = "";
	static QString PRNL_EMITTER_TIME_TO_LIVE = "";
	static QString PRNL_EMITTER_MASS = "";
	static QString PRNL_EMITTER_TEXTURE_COORD = "";
	static QString PRNL_EMITTER_TEXTURE_COORD_START = "";
	static QString PRNL_EMITTER_TEXTURE_COORD_END = "";
	static QString PRNL_EMITTER_COLOUR = "";
	static QString PRNL_EMITTER_COLOUR_RANGE_START = "";
	static QString PRNL_EMITTER_COLOUR_RANGE_END = "";
	static QString PRNL_EMITTER_ALL_PARTICLE_DIM = "";
	static QString PRNL_EMITTER_PARTICLE_WIDTH = "";
	static QString PRNL_EMITTER_PARTICLE_HEIGHT = "";
	static QString PRNL_EMITTER_PARTICLE_DEPTH = "";
	static QString PRNL_EMITTER_AUTO_DIRECTION = "";
	static QString PRNL_EMITTER_FORCE_EMISSION = "";

	class EmitterPropertyWindow : public PropertyWindow {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		EmitterPropertyWindow(QWidget * par, QString name = "");

		/**
		 * \brief destructor
		 */
		virtual ~EmitterPropertyWindow();

		void copyAttributesFromEmitter(ParticleUniverse::ParticleEmitter * emitter);
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_EMITTERPROPERTYWINDOW_H__ */
