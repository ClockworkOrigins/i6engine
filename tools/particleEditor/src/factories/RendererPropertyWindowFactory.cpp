#include "factories/RendererPropertyWindowFactory.h"

#include "widgets/WidgetEditComponent.h"

#include "widgets/RendererPropertyWindows/BeamRendererPropertyWindow.h"
#include "widgets/RendererPropertyWindows/BillboardRendererPropertyWindow.h"
#include "widgets/RendererPropertyWindows/BoxRendererPropertyWindow.h"
#include "widgets/RendererPropertyWindows/EntityRendererPropertyWindow.h"
#include "widgets/RendererPropertyWindows/LightRendererPropertyWindow.h"
#include "widgets/RendererPropertyWindows/RibbonTrailRendererPropertyWindow.h"
#include "widgets/RendererPropertyWindows/SphereRendererPropertyWindow.h"

namespace i6e {
namespace particleEditor {
namespace factories {

	/**
	Create a specific Renderer Property Window
	*/
	widgets::RendererPropertyWindow * RendererPropertyWindowFactory::createRendererPropertyWindow(QWidget * parent, widgets::WidgetEditComponent * owner, const QString & name, QString subType) {
		if (subType == widgets::CST_UNDEFINED) {
			return new widgets::RendererPropertyWindow(parent, owner, name);
		} else if (subType == widgets::CST_RENDERER_BILLBOARD) {
			return new widgets::BillboardRendererPropertyWindow(parent, owner, name);
		} else if (subType == widgets::CST_RENDERER_BEAM) {
			return new widgets::BeamRendererPropertyWindow(parent, owner, name);
		} else if (subType == widgets::CST_RENDERER_BOX) {
			return new widgets::BoxRendererPropertyWindow(parent, owner, name);
		} else if (subType == widgets::CST_RENDERER_ENTITY) {
			return new widgets::EntityRendererPropertyWindow(parent, owner, name);
		} else if (subType == widgets::CST_RENDERER_LIGHT) {
			return new widgets::LightRendererPropertyWindow(parent, owner, name);
		} else if (subType == widgets::CST_RENDERER_RIBBONTRAIL) {
			return new widgets::RibbonTrailRendererPropertyWindow(parent, owner, name);
		} else if (subType == widgets::CST_RENDERER_SPHERE) {
			return new widgets::SphereRendererPropertyWindow(parent, owner, name);
		} else {
			return new widgets::RendererPropertyWindow(parent, owner, name);
		}
		return nullptr;
	}

	/**
	Create a specific Renderer Property Window and propagate the attributes from the RendererPropertyWindow
	*/
	widgets::RendererPropertyWindow * RendererPropertyWindowFactory::createRendererPropertyWindow(QString subType, widgets::WidgetEditComponent * owner, widgets::RendererPropertyWindow * rendererPropertyWindow) {
		if (subType == widgets::CST_UNDEFINED) {
			return new widgets::RendererPropertyWindow(rendererPropertyWindow, owner);
		} else if (subType == widgets::CST_RENDERER_BILLBOARD) {
			return new widgets::BillboardRendererPropertyWindow(rendererPropertyWindow, owner);
		} else if (subType == widgets::CST_RENDERER_BEAM) {
			return new widgets::BeamRendererPropertyWindow(rendererPropertyWindow, owner);
		} else if (subType == widgets::CST_RENDERER_BOX) {
			return new widgets::BoxRendererPropertyWindow(rendererPropertyWindow, owner);
		} else if (subType == widgets::CST_RENDERER_ENTITY) {
			return new widgets::EntityRendererPropertyWindow(rendererPropertyWindow, owner);
		} else if (subType == widgets::CST_RENDERER_LIGHT) {
			return new widgets::LightRendererPropertyWindow(rendererPropertyWindow, owner);
		} else if (subType == widgets::CST_RENDERER_RIBBONTRAIL) {
			return new widgets::RibbonTrailRendererPropertyWindow(rendererPropertyWindow, owner);
		} else if (subType == widgets::CST_RENDERER_SPHERE) {
			return new widgets::SphereRendererPropertyWindow(rendererPropertyWindow, owner);
		} else {
			return new widgets::RendererPropertyWindow(rendererPropertyWindow, owner);
		}
		return nullptr;
	}

} /* namespace factories */
} /* namespace particleEditor */
} /* namespace i6e */
