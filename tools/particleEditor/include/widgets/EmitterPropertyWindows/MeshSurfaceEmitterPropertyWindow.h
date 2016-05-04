#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_MESHSURFACEEMITTERPROPERTYWINDOW_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_MESHSURFACEEMITTERPROPERTYWINDOW_H__

#include "widgets/EmitterPropertyWindow.h"

namespace i6e {
namespace particleEditor {
namespace widgets {

	// Property names
	static QString PRNL_MESH_SURFACE_EMITTER_NAME = "";
	static QString PRNL_MESH_SURFACE_EMITTER_DISTRIBUTION = "";
	static QString PRNL_MESH_SURFACE_EMITTER_SCALE = "";
	static QString PRNL_MESH_SURFACE_EMITTER_DISTRIBUTION_EDGE = "";
	static QString PRNL_MESH_SURFACE_EMITTER_DISTRIBUTION_HETRG_1 = "";
	static QString PRNL_MESH_SURFACE_EMITTER_DISTRIBUTION_HETRG_2 = "";
	static QString PRNL_MESH_SURFACE_EMITTER_DISTRIBUTION_HOMOGENEUS = "";
	static QString PRNL_MESH_SURFACE_EMITTER_DISTRIBUTION_VERTEX = "";

	class MeshSurfaceEmitterPropertyWindow : public EmitterPropertyWindow {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		MeshSurfaceEmitterPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name = "");

		/**
		 * \brief destructor
		 */
		~MeshSurfaceEmitterPropertyWindow();

		void copyAttributesFromEmitter(ParticleUniverse::ParticleEmitter * emitter) override;

	private:
		void copyAttributeToEmitter(properties::Property * prop, QString propertyName) override;
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6e */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_MESHSURFACEEMITTERPROPERTYWINDOW_H__ */
