#include "factories/EmitterPropertyWindowFactory.h"

#include "widgets/WidgetEditComponent.h"

#include "widgets/EmitterPropertyWindows/BoxEmitterPropertyWindow.h"
#include "widgets/EmitterPropertyWindows/CircleEmitterPropertyWindow.h"
#include "widgets/EmitterPropertyWindows/LineEmitterPropertyWindow.h"
#include "widgets/EmitterPropertyWindows/MeshSurfaceEmitterPropertyWindow.h"
#include "widgets/EmitterPropertyWindows/PointEmitterPropertyWindow.h"
#include "widgets/EmitterPropertyWindows/PositionEmitterPropertyWindow.h"
#include "widgets/EmitterPropertyWindows/SlaveEmitterPropertyWindow.h"
#include "widgets/EmitterPropertyWindows/SphereSurfaceEmitterPropertyWindow.h"
#include "widgets/EmitterPropertyWindows/VertexEmitterPropertyWindow.h"

namespace i6engine {
namespace particleEditor {
namespace factories {

	/**
	Create a specific Emitter Property Window
	*/
	widgets::EmitterPropertyWindow * EmitterPropertyWindowFactory::createEmitterPropertyWindow(QWidget * parent, const QString & name, QString subType) {
		if (subType == widgets::CST_UNDEFINED) {
			return new widgets::EmitterPropertyWindow(parent, name);
		} else if (subType == widgets::CST_EMITTER_BOX) {
			return new widgets::BoxEmitterPropertyWindow(parent, name);
		} else if (subType == widgets::CST_EMITTER_CIRCLE) {
			return new widgets::CircleEmitterPropertyWindow(parent, name);
		} else if (subType == widgets::CST_EMITTER_LINE) {
			return new widgets::LineEmitterPropertyWindow(parent, name);
		} else if (subType == widgets::CST_EMITTER_MESH_SURFACE) {
			return new widgets::MeshSurfaceEmitterPropertyWindow(parent, name);
		} else if (subType == widgets::CST_EMITTER_POINT) {
			return new widgets::PointEmitterPropertyWindow(parent, name);
		} else if (subType == widgets::CST_EMITTER_POSITION) {
			return new widgets::PositionEmitterPropertyWindow(parent, name);
		} else if (subType == widgets::CST_EMITTER_SLAVE) {
			return new widgets::SlaveEmitterPropertyWindow(parent, name);
		} else if (subType == widgets::CST_EMITTER_SPHERE) {
			return new widgets::SphereSurfaceEmitterPropertyWindow(parent, name);
		} else if (subType == widgets::CST_EMITTER_VERTEX) {
			return new widgets::VertexEmitterPropertyWindow(parent, name);
		} else {
			return new widgets::EmitterPropertyWindow(parent, name);
		}
		return 0;
	};

	/**
	Create a specific Emitter Property Window and propagate the attributes from the EmitterPropertyWindow
	*/
	widgets::EmitterPropertyWindow * EmitterPropertyWindowFactory::createEmitterPropertyWindow(QString subType, widgets::EmitterPropertyWindow * emitterPropertyWindow) {
		if (subType == widgets::CST_UNDEFINED) {
			return new widgets::EmitterPropertyWindow(emitterPropertyWindow);
		} else if (subType == widgets::CST_EMITTER_BOX) {
			return new widgets::BoxEmitterPropertyWindow(emitterPropertyWindow);
		} else if (subType == widgets::CST_EMITTER_CIRCLE) {
			return new widgets::CircleEmitterPropertyWindow(emitterPropertyWindow);
		} else if (subType == widgets::CST_EMITTER_LINE) {
			return new widgets::LineEmitterPropertyWindow(emitterPropertyWindow);
		} else if (subType == widgets::CST_EMITTER_MESH_SURFACE) {
			return new widgets::MeshSurfaceEmitterPropertyWindow(emitterPropertyWindow);
		} else if (subType == widgets::CST_EMITTER_POINT) {
			return new widgets::PointEmitterPropertyWindow(emitterPropertyWindow);
		} else if (subType == widgets::CST_EMITTER_POSITION) {
			return new widgets::PositionEmitterPropertyWindow(emitterPropertyWindow);
		} else if (subType == widgets::CST_EMITTER_SLAVE) {
			return new widgets::SlaveEmitterPropertyWindow(emitterPropertyWindow);
		} else if (subType == widgets::CST_EMITTER_SPHERE) {
			return new widgets::SphereSurfaceEmitterPropertyWindow(emitterPropertyWindow);
		} else if (subType == widgets::CST_EMITTER_VERTEX) {
			return new widgets::VertexEmitterPropertyWindow(emitterPropertyWindow);
		} else {
			return new widgets::EmitterPropertyWindow(emitterPropertyWindow);
		}
		return nullptr;
	}

} /* namespace factories */
} /* namespace particleEditor */
} /* namespace i6engine */
