#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_TECHNIQUEPROPERTYWINDOW_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_TECHNIQUEPROPERTYWINDOW_H__

#include "widgets/PropertyWindow.h"

namespace ParticleUniverse {
	class ParticleTechnique;
} /* namespace ParticleUniverse */

namespace i6e {
namespace particleEditor {
namespace widgets {

	// Property names
	static QString PRNL_TECHNIQUE_ENABLED = "";
	static QString PRNL_TECHNIQUE_POSITION = "";
	static QString PRNL_TECHNIQUE_KEEP_LOCAL = "";
	static QString PRNL_TECHNIQUE_VISUAL_PARTICLE_QUOTA = "";
	static QString PRNL_TECHNIQUE_EMITTED_EMITTER_QUOTA = "";
	static QString PRNL_TECHNIQUE_EMITTED_AFFECTOR_QUOTA = "";
	static QString PRNL_TECHNIQUE_EMITTED_TECHNIQUE_QUOTA = "";
	static QString PRNL_TECHNIQUE_EMITTED_SYSTEM_QUOTA = "";
	static QString PRNL_TECHNIQUE_MATERIAL = "";
	static QString PRNL_TECHNIQUE_LOD_INDEX = "";
	static QString PRNL_TECHNIQUE_DEFAULT_PARTICLE_WIDTH = "";
	static QString PRNL_TECHNIQUE_DEFAULT_PARTICLE_HEIGHT = "";
	static QString PRNL_TECHNIQUE_DEFAULT_PARTICLE_DEPTH = "";
	static QString PRNL_TECHNIQUE_SPHASHING_CELL_DIMENSION = "";
	static QString PRNL_TECHNIQUE_SPHASHING_CELL_OVERLAP = "";
	static QString PRNL_TECHNIQUE_SPHASHING_SIZE = "";
	static QString PRNL_TECHNIQUE_SPHASHING_UPDATE_INTERVAL = "";
	static QString PRNL_TECHNIQUE_MAX_VELOCITY = "";

	class TechniquePropertyWindow : public PropertyWindow {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		TechniquePropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name);

		/**
		 * \brief destructor
		 */
		~TechniquePropertyWindow();

		virtual void copyAttributesFromTechnique(ParticleUniverse::ParticleTechnique * technique);

	protected:
		void changedProperty(properties::Property * prop, QString name) override;
		void copyAttributeToTechnique(properties::Property * prop, QString propertyName);
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6e */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_TECHNIQUEPROPERTYWINDOW_H__ */
