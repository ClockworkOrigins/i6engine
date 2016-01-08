#include "factories/RendererPropertyWindowFactory.h"

#include "widgets/WidgetEditComponent.h"

#include "widgets/RendererPropertyWindows/BeamRendererPropertyWindow.h"
#include "widgets/RendererPropertyWindows/BillboardRendererPropertyWindow.h"
#include "widgets/RendererPropertyWindows/BoxRendererPropertyWindow.h"
#include "widgets/RendererPropertyWindows/EntityRendererPropertyWindow.h"
#include "widgets/RendererPropertyWindows/LightRendererPropertyWindow.h"
#include "widgets/RendererPropertyWindows/RibbonTrailRendererPropertyWindow.h"
#include "widgets/RendererPropertyWindows/SphereRendererPropertyWindow.h"

namespace i6engine {
namespace particleEditor {
namespace factories {

	/**
	Create a specific Renderer Property Window
	*/
	widgets::RendererPropertyWindow * RendererPropertyWindowFactory::createRendererPropertyWindow(QWidget * parent, const QString & name, QString subType) {
		if (subType == widgets::CST_UNDEFINED) {
			return new widgets::RendererPropertyWindow(parent, name);
		} else if (subType == widgets::CST_RENDERER_BILLBOARD) {
			return new widgets::BillboardRendererPropertyWindow(parent, name);
		} else if (subType == widgets::CST_RENDERER_BEAM) {
			return new widgets::BeamRendererPropertyWindow(parent, name);
		} else if (subType == widgets::CST_RENDERER_BOX) {
			return new widgets::BoxRendererPropertyWindow(parent, name);
		} else if (subType == widgets::CST_RENDERER_ENTITY) {
			return new widgets::EntityRendererPropertyWindow(parent, name);
		} else if (subType == widgets::CST_RENDERER_LIGHT) {
			return new widgets::LightRendererPropertyWindow(parent, name);
		} else if (subType == widgets::CST_RENDERER_RIBBONTRAIL) {
			return new widgets::RibbonTrailRendererPropertyWindow(parent, name);
		} else if (subType == widgets::CST_RENDERER_SPHERE) {
			return new widgets::SphereRendererPropertyWindow(parent, name);
		} else {
			return new widgets::RendererPropertyWindow(parent, name);
		}
		return nullptr;
	}

	/**
	Create a specific Renderer Property Window and propagate the attributes from the RendererPropertyWindow
	*/
	widgets::RendererPropertyWindow * RendererPropertyWindowFactory::createRendererPropertyWindow(QString subType, widgets::RendererPropertyWindow * rendererPropertyWindow) {
		if (subType == widgets::CST_UNDEFINED) {
			return new widgets::RendererPropertyWindow(rendererPropertyWindow);
		} else if (subType == widgets::CST_RENDERER_BILLBOARD) {
			return new widgets::BillboardRendererPropertyWindow(rendererPropertyWindow);
		} else if (subType == widgets::CST_RENDERER_BEAM) {
			return new widgets::BeamRendererPropertyWindow(rendererPropertyWindow);
		} else if (subType == widgets::CST_RENDERER_BOX) {
			return new widgets::BoxRendererPropertyWindow(rendererPropertyWindow);
		} else if (subType == widgets::CST_RENDERER_ENTITY) {
			return new widgets::EntityRendererPropertyWindow(rendererPropertyWindow);
		} else if (subType == widgets::CST_RENDERER_LIGHT) {
			return new widgets::LightRendererPropertyWindow(rendererPropertyWindow);
		} else if (subType == widgets::CST_RENDERER_RIBBONTRAIL) {
			return new widgets::RibbonTrailRendererPropertyWindow(rendererPropertyWindow);
		} else if (subType == widgets::CST_RENDERER_SPHERE) {
			return new widgets::SphereRendererPropertyWindow(rendererPropertyWindow);
		} else {
			return new widgets::RendererPropertyWindow(rendererPropertyWindow);
		}
		return nullptr;
	}

} /* namespace factories */
} /* namespace particleEditor */
} /* namespace i6engine */
