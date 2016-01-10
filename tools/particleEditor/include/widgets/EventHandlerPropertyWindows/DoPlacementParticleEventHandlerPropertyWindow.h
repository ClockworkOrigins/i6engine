#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_DOPLACEMENTPARTICLEEVENTHANDLERPROPERTYWINDOW_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_DOPLACEMENTPARTICLEEVENTHANDLERPROPERTYWINDOW_H__

#include "widgets/EventHandlerPropertyWindow.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	// Property names
	static QString PRNL_NUMBER_OF_PARTICLES = "";
	static QString PRNL_INHERIT_POSITION = "";
	static QString PRNL_INHERIT_DIRECTION = "";
	static QString PRNL_INHERIT_ORIENTATION = "";
	static QString PRNL_INHERIT_TIME_TO_LIVE = "";
	static QString PRNL_INHERIT_MASS = "";
	static QString PRNL_INHERIT_TEXTURE_COORDINATE = "";
	static QString PRNL_INHERIT_COLOUR = "";
	static QString PRNL_INHERIT_PARTICLE_WIDTH = "";
	static QString PRNL_INHERIT_PARTICLE_HEIGHT = "";
	static QString PRNL_INHERIT_PARTICLE_DEPTH = "";

	class DoPlacementParticleEventHandlerPropertyWindow : public EventHandlerPropertyWindow {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		DoPlacementParticleEventHandlerPropertyWindow(QWidget * par, QString name = "");

		/**
		 * \brief destructor
		 */
		~DoPlacementParticleEventHandlerPropertyWindow();

		void copyAttributesFromEventHandler(ParticleUniverse::ParticleEventHandler * eventHandler) override;
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_DOPLACEMENTPARTICLEEVENTHANDLERPROPERTYWINDOW_H__ */
