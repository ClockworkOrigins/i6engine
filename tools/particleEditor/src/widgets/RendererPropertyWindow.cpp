#include "widgets/RendererPropertyWindow.h"

#include "properties/BoolProperty.h"
#include "properties/EnumProperty.h"
#include "properties/UIntProperty.h"

#include "widgets/WidgetEditComponent.h"

#include "ParticleUniverseRenderer.h"
#include "ParticleUniverseSystem.h"
#include "ParticleUniverseSystemManager.h"
#include "ParticleUniverseTechnique.h"
#include "ParticleRenderers/ParticleUniverseEntityRenderer.h"

namespace i6e {
namespace particleEditor {
namespace widgets {

	RendererPropertyWindow::RendererPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name) : PropertyWindow(par, owner, name) {
		// Set the (internationalized) property names
		// Renderers
		CST_RENDERER_BEAM = RENDERER_BEAM;
		CST_RENDERER_BILLBOARD = RENDERER_BILLBOARD;
		CST_RENDERER_SPHERE = RENDERER_SPHERE;
		CST_RENDERER_BOX = RENDERER_BOX;
		CST_RENDERER_ENTITY = RENDERER_ENTITY;
		CST_RENDERER_LIGHT = RENDERER_LIGHT;
		CST_RENDERER_RIBBONTRAIL = RENDERER_RIBBONTRAIL;
		PRNL_RENDERER_TYPE = QApplication::tr("Renderer type");
		PRNL_USE_VERTEX_COLOURS = QApplication::tr("Use vertex colours");
		PRNL_MAX_ELEMENTS = QApplication::tr("Maximum elements");
		PRNL_RENDERER_RENDER_Q_GROUP = QApplication::tr("Render queue group");
		PRNL_RENDERER_SORTING = QApplication::tr("Sorting");
		PRNL_RENDERER_TEXCOORDS_DEFINE = QApplication::tr("Texture coords define");
		PRNL_RENDERER_TEXCOORDS_SET = QApplication::tr("Texture coords set");
		PRNL_RENDERER_TEXCOORDS_ROWS = QApplication::tr("Texture coords rows");
		PRNL_RENDERER_TEXCOORDS_COLUMNS = QApplication::tr("Texture coords columns");
		PRNL_RENDERER_USE_SOFT_PARTICLES = QApplication::tr("Use soft particles");
		PRNL_RENDERER_SOFT_PARTICLES_CONTRAST_POWER = QApplication::tr("Soft particles contrast power");
		PRNL_RENDERER_SOFT_PARTICLES_SCALE = QApplication::tr("Soft particles scale");
		PRNL_RENDERER_SOFT_PARTICLES_DELTA = QApplication::tr("Soft particles delta");
		PRNL_NAME = QApplication::tr("Name");
		PRNL_MESH_NAME = QApplication::tr("Mesh name");

		// Type: List of types (DO NOT CHANGE THE ORDER!
		// Internationalization means that the string values of this property are unusable to create a renderer)
		QStringList types;
		types.append(CST_RENDERER_BILLBOARD);
		types.append(CST_RENDERER_BEAM);
		types.append(CST_RENDERER_BOX);
		types.append(CST_RENDERER_ENTITY);
		types.append(CST_RENDERER_LIGHT);
		types.append(CST_RENDERER_RIBBONTRAIL);
		types.append(CST_RENDERER_SPHERE);
		append(new properties::EnumProperty(this, PRNL_RENDERER_TYPE, PRNL_RENDERER_TYPE, types));

		// Render queue group: ParticleUniverse::uint8
		append(new properties::UIntProperty(this, PRNL_RENDERER_RENDER_Q_GROUP, PRNL_RENDERER_RENDER_Q_GROUP, ParticleUniverse::ParticleRenderer::DEFAULT_RENDER_QUEUE_GROUP));

		// Sorting: Bool
		append(new properties::BoolProperty(this, PRNL_RENDERER_SORTING, PRNL_RENDERER_SORTING, ParticleUniverse::ParticleRenderer::DEFAULT_SORTED));

		// Texture coords define: Is no attribute, but only a 'container' in the script

		// Texture coords set: List of 4 x ParticleUniverse::Real
		// TODO

		// Texture coords rows: uchar
		append(new properties::UIntProperty(this, PRNL_RENDERER_TEXCOORDS_ROWS, PRNL_RENDERER_TEXCOORDS_ROWS, ParticleUniverse::ParticleRenderer::DEFAULT_TEXTURECOORDS_ROWS));

		// Texture coords columns: uchar
		append(new properties::UIntProperty(this, PRNL_RENDERER_TEXCOORDS_COLUMNS, PRNL_RENDERER_TEXCOORDS_COLUMNS, ParticleUniverse::ParticleRenderer::DEFAULT_TEXTURECOORDS_COLUMNS));
	}

	RendererPropertyWindow::~RendererPropertyWindow() {
	}

	void RendererPropertyWindow::copyAttributesFromRenderer(ParticleUniverse::ParticleRenderer * renderer) {
		// Type: List of types
		setEnumString(PRNL_RENDERER_TYPE, QString::fromStdString(renderer->getRendererType()));

		// Render queue group: ParticleUniverse::uint8
		setUint16(PRNL_RENDERER_RENDER_Q_GROUP, renderer->getRenderQueueGroup());

		// Sorting: Bool
		setBool(PRNL_RENDERER_SORTING, renderer->isSorted());

		// Texture coords set: List of 4 x ParticleUniverse::Real
		// TODO

		// Texture coords rows: uchar
		setUint16(PRNL_RENDERER_TEXCOORDS_ROWS, renderer->getTextureCoordsRows());

		// Texture coords columns: uchar
		setUint16(PRNL_RENDERER_TEXCOORDS_COLUMNS, renderer->getTextureCoordsColumns());
	}

	void RendererPropertyWindow::changedProperty(properties::Property * prop, QString name) {
		PropertyWindow::changedProperty(prop, name);
		copyAttributeToRenderer(prop, name);
		emit notifyChanged();
	}

	void RendererPropertyWindow::copyAttributeToRenderer(properties::Property * prop, QString propertyName) {
		if (!prop) {
			return;
		}

		ParticleUniverse::ParticleRenderer * renderer = static_cast<ParticleUniverse::ParticleRenderer *>(_owner->getPUElement());
		if (!renderer) {
			return;
		}

		if (propertyName == PRNL_RENDERER_TYPE) {
			// Type: List of types
			// This requires the renderer to be replaced.
			replaceRendererType(prop);
		} else if (propertyName == PRNL_RENDERER_RENDER_Q_GROUP) {
			// Render queue group: ParticleUniverse::uint8
			renderer->setRenderQueueGroup(prop->getUInt());
		} else if (propertyName == PRNL_RENDERER_SORTING) {
			// Sorting: Bool
			renderer->setSorted(prop->getBool());
		} else if (propertyName == PRNL_RENDERER_TEXCOORDS_ROWS) {
			// Texture coords rows: uchar
			renderer->setTextureCoordsRows(prop->getUInt());
			renderer->_unprepare(renderer->getParentTechnique());
			renderer->setRendererInitialised(false);
		} else if (propertyName == PRNL_RENDERER_TEXCOORDS_COLUMNS) {
			// Texture coords columns: uchar
			renderer->setTextureCoordsColumns(prop->getUInt());
			renderer->_unprepare(renderer->getParentTechnique());
			renderer->setRendererInitialised(false);
		}
	}

	void RendererPropertyWindow::replaceRendererType(properties::Property * prop) {
		// Type: List of types
		Ogre::String type = prop->getEnumString().toStdString();
		if (type == Ogre::StringUtil::BLANK) {
			return;
		}

		ParticleUniverse::ParticleRenderer * oldRenderer = static_cast<ParticleUniverse::ParticleRenderer *>(_owner->getPUElement());
		if (type == oldRenderer->getRendererType()) {
			return;
		}
		if (oldRenderer) {
			ParticleUniverse::ParticleSystemManager * particleSystemManager = ParticleUniverse::ParticleSystemManager::getSingletonPtr();
			ParticleUniverse::ParticleRenderer * newRenderer = particleSystemManager->createRenderer(type);
			oldRenderer->copyParentAttributesTo(newRenderer);
			ParticleUniverse::ParticleTechnique * technique = oldRenderer->getParentTechnique();
			if (technique) {
				// V1.5: Bug: Crash when meshname of EntityRenderer is blank FIXME
				if (newRenderer->getRendererType() == RENDERER_ENTITY.toStdString()) {
					ParticleUniverse::EntityRenderer * entityRenderer = static_cast<ParticleUniverse::EntityRenderer *>(newRenderer);
					ParticleUniverse::String s = entityRenderer->getMeshName();
					if (entityRenderer->getMeshName() == Ogre::StringUtil::BLANK) {
						ParticleUniverse::String s2 = "pu_bold_marker.mesh";
						entityRenderer->setMeshName(s2);
						properties::Property * p = _properties[PRNL_MESH_NAME];
						if (p) {
							p->setString(QString::fromStdString(s2));
						}
					}
				}
				// V1.5: Bug end

				bool wasStarted = false;
				ParticleUniverse::ParticleSystem * system = technique->getParentSystem();
				if (system && system->getState() == ParticleUniverse::ParticleSystem::PSS_STARTED) {
					wasStarted = true;
					system->stop();
				}
				technique->setRenderer(newRenderer);
				_owner->setPUElement(newRenderer);
				technique->_unprepareRenderer();
				if (wasStarted) {
					system->start();
				}
			} else {
				/** The old renderer didn't have a technique.
				*/
				particleSystemManager->destroyRenderer(oldRenderer);
				_owner->setPUElement(newRenderer);
			}
		} else {
			// There is no old renderer. Create a new renderer by means of the ParticleSystemManager
			ParticleUniverse::ParticleSystemManager * particleSystemManager = ParticleUniverse::ParticleSystemManager::getSingletonPtr();
			ParticleUniverse::ParticleRenderer * newRenderer = particleSystemManager->createRenderer(type);
			_owner->setPUElement(newRenderer);
		}
		emit replacePropertyWindow(QString::fromStdString(type));
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6e */
