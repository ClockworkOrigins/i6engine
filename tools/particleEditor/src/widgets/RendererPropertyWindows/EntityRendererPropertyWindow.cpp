#include "widgets/RendererPropertyWindows/EntityRendererPropertyWindow.h"

#include "properties/EnumProperty.h"
#include "properties/StringProperty.h"

#include "widgets/WidgetEditComponent.h"

#include "ParticleRenderers/ParticleUniverseEntityRenderer.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	EntityRendererPropertyWindow::EntityRendererPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name) : RendererPropertyWindow(par, owner, name) {
		// Set the (internationalized) property names
		PRNL_RENDERER_TYPE = "Renderer type";
		PRNL_ORIENTATION_TYPE = "Orientation type";
		OTT_ORIENTED_SELF = "Oriented self";
		OTT_ORIENTED_SELF_MIRRORED = "Oriented self mirrored";
		OTT_ORIENTED_SHAPE = "Oriented shape";
		PRNL_MESH_NAME = "Mesh name";

		// Mesh Name: Ogre::String
		append(new properties::StringProperty(this, PRNL_MESH_NAME, PRNL_MESH_NAME, ""));

		// Orientation Type: List
		QStringList orientationTypes;
		orientationTypes.append(OTT_ORIENTED_SELF);
		orientationTypes.append(OTT_ORIENTED_SELF_MIRRORED);
		orientationTypes.append(OTT_ORIENTED_SHAPE);
		append(new properties::EnumProperty(this, PRNL_ORIENTATION_TYPE, PRNL_ORIENTATION_TYPE, orientationTypes));
	}

	EntityRendererPropertyWindow::~EntityRendererPropertyWindow() {
	}

	void EntityRendererPropertyWindow::copyAttributesFromRenderer(ParticleUniverse::ParticleRenderer * renderer) {
		RendererPropertyWindow::copyAttributesFromRenderer(renderer);

		// Copy properties from renderer to property window
		ParticleUniverse::EntityRenderer * entityRenderer = static_cast<ParticleUniverse::EntityRenderer *>(renderer);

		// Mesh Name: Ogre::String
		setString(PRNL_MESH_NAME, QString::fromStdString(entityRenderer->getMeshName()));

		// Orientation Type: List
		ParticleUniverse::EntityRenderer::EntityOrientationType orientationType = entityRenderer->getEntityOrientationType();
		QString orientationTypeString = OTT_ORIENTED_SELF;
		if (orientationType == ParticleUniverse::EntityRenderer::ENT_ORIENTED_SELF_MIRRORED) {
			orientationTypeString = OTT_ORIENTED_SELF_MIRRORED;
		} else if (orientationType == ParticleUniverse::EntityRenderer::ENT_ORIENTED_SHAPE) {
			orientationTypeString = OTT_ORIENTED_SHAPE;
		}
		setEnumString(PRNL_ORIENTATION_TYPE, orientationTypeString);
	}

	void EntityRendererPropertyWindow::copyAttributeToRenderer(properties::Property * prop, QString propertyName) {
		if (!prop) {
			return;
		}

		ParticleUniverse::EntityRenderer * renderer = static_cast<ParticleUniverse::EntityRenderer *>(_owner->getPUElement());
		if (!renderer) {
			return;
		}

		if (propertyName == PRNL_ORIENTATION_TYPE) {
			// Orientation Type: List
			QString orientationType = prop->getEnumString();
			if (orientationType == OTT_ORIENTED_SELF) {
				renderer->setEntityOrientationType(ParticleUniverse::EntityRenderer::ENT_ORIENTED_SELF);
			} else if (orientationType == OTT_ORIENTED_SELF_MIRRORED) {
				renderer->setEntityOrientationType(ParticleUniverse::EntityRenderer::ENT_ORIENTED_SELF_MIRRORED);
			} else if (orientationType == OTT_ORIENTED_SHAPE) {
				renderer->setEntityOrientationType(ParticleUniverse::EntityRenderer::ENT_ORIENTED_SHAPE);
			}
		} else if (propertyName == PRNL_MESH_NAME) {
			// Mesh Name: Ogre::String
			Ogre::String meshName = prop->getString().toStdString();
			replaceRendererType(_properties[PRNL_RENDERER_TYPE]);

			// A bit heavy solution to re-create the renderer, but only changing the meshname is not sufficient
			renderer = static_cast<ParticleUniverse::EntityRenderer *>(_owner->getPUElement());
			if (!renderer)
				return;

			renderer->setMeshName(meshName);
			prop->setString(QString::fromStdString(meshName));
		} else {
			// Update renderer with another attribute
			RendererPropertyWindow::copyAttributeToRenderer(prop, propertyName);
		}
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */
