#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_VERTEXEMITTERPROPERTYWINDOW_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_VERTEXEMITTERPROPERTYWINDOW_H__

#include "widgets/EmitterPropertyWindow.h"

namespace i6e {
namespace particleEditor {
namespace widgets {

	// Property names
	static QString PRNL_VERTEX_EMITTER_STEP = "";
	static QString PRNL_VERTEX_EMITTER_SEGMENTS = "";
	static QString PRNL_VERTEX_EMITTER_ITERATION = "";
	static QString PRNL_VERTEX_EMITTER_MESH_NAME = "";

	class VertexEmitterPropertyWindow : public EmitterPropertyWindow {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		VertexEmitterPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name = "");

		/**
		 * \brief destructor
		 */
		~VertexEmitterPropertyWindow();

		void copyAttributesFromEmitter(ParticleUniverse::ParticleEmitter * emitter) override;

	private:
		void copyAttributeToEmitter(properties::Property * prop, QString propertyName) override;
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6e */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_VERTEXEMITTERPROPERTYWINDOW_H__ */
