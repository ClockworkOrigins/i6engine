/*
 * i6engine
 * Copyright (2016) Daniel Bonrath, Michael Baer, All rights reserved.
 *
 * This file is part of i6engine; i6engine is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "widgets/WidgetEdit.h"

#include "connections/Connection.h"
#include "connections/LineConnector.h"

#include "widgets/AffectorPropertyWindow.h"
#include "widgets/BehaviourPropertyWindow.h"
#include "widgets/EmitterPropertyWindow.h"
#include "widgets/EventHandlerPropertyWindow.h"
#include "widgets/ExternPropertyWindow.h"
#include "widgets/GraphicsScene.h"
#include "widgets/ObserverPropertyWindow.h"
#include "widgets/RendererPropertyWindow.h"
#include "widgets/SystemPropertyWindow.h"
#include "widgets/TechniquePropertyWindow.h"
#include "widgets/WidgetEditComponent.h"

#include "ParticleUniverseAffector.h"
#include "ParticleUniverseBehaviour.h"
#include "ParticleUniverseEmitter.h"
#include "ParticleUniverseEventHandler.h"
#include "ParticleUniverseExtern.h"
#include "ParticleUniverseObserver.h"
#include "ParticleUniverseRenderer.h"
#include "ParticleUniverseSystem.h"
#include "ParticleUniverseSystemManager.h"
#include "ParticleUniverseTechnique.h"
#include "ParticleEmitters/ParticleUniverseSlaveEmitter.h"
#include "ParticleEventHandlers/ParticleUniverseDoEnableComponentEventHandler.h"
#include "ParticleEventHandlers/ParticleUniverseDoAffectorEventHandler.h"
#include "ParticleEventHandlers/ParticleUniverseDoPlacementParticleEventHandler.h"

#include <QGraphicsView>

namespace i6e {
namespace particleEditor {
namespace widgets {
	
	const QColor DRAW_DEFAULT_COLOURCODE = QColor(0, 0, 0);
	const QColor DRAW_EMITTED_COLOURCODE = QColor(255, 0, 0);
	const QColor DRAW_SPECIAL_CASE_COLOURCODE = QColor(56, 124, 68);

	WidgetEdit::WidgetEdit(QWidget * par, QWidget * renderWidget) : QWidget(par), _graphicsScene(new GraphicsScene(this)), _graphicsView(new QGraphicsView(_graphicsScene)), _components(), _offsetX(48), _offsetY(8), _techniqueCounter(1), _rendererCounter(1), _emitterCounter(1), _affectorCounter(1), _observerCounter(1), _handlerCounter(1), _behaviourCounter(1), _externCounter(1), _connectionMode(CM_CONNECT_NONE), _startConnector(nullptr), _endConnector(nullptr), _connections() {
		setupUi(this);

		verticalLayout->addWidget(_graphicsView);
		verticalLayout->addWidget(renderWidget);

		verticalLayout->setStretch(0, 1);
		verticalLayout->setStretch(1, 1);

		_graphicsView->show();
		_graphicsView->setDragMode(QGraphicsView::DragMode::ScrollHandDrag);
		_graphicsView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
		_graphicsView->setCacheMode(QGraphicsView::CacheBackground);
	}

	WidgetEdit::~WidgetEdit() {
		while (!_connections.empty()) {
			auto it = _connections.begin();
			_graphicsScene->removeItem(it->second);
			delete it->second;
			auto first = it->first.first;
			auto second = it->first.second;
			_connections.erase(std::make_pair(first, second));
			_connections.erase(std::make_pair(second, first));
		}
		for (WidgetEditComponent * wec : _components) {
			_graphicsScene->removeItem(wec);
			wec->deleteLater();
		}
	}

	void WidgetEdit::notifyComponentActivated(WidgetEditComponent * component) {
		// A component is activated, check the mode
		if (_connectionMode == CM_CONNECT_STARTING) {
			// Start connecting
			//mCanvas->createMouseConnector(component);
			_startConnector = component;
			setConnectionMode(CM_CONNECT_ENDING);
		} else if (_connectionMode == CM_CONNECT_ENDING && component != _startConnector) {
			if (isConnectionPossible(component)) {
				// A policy has already been selected
				setConnectionMode(CM_CONNECT_STARTING);
				_endConnector = component;
				//mCanvas->destroyMouseConnector();
				connections::ConnectionPolicy * policyEnd = _endConnector->getSelectedPolicy();
				if (!policyEnd) {
					return;
				}

				connections::LineConnector * lc = new connections::LineConnector(_startConnector, _endConnector, policyEnd->getColour(), policyEnd->getLineStyle());
				_connections.insert(std::make_pair(std::make_pair(_startConnector, _endConnector), lc));
				_connections.insert(std::make_pair(std::make_pair(_endConnector, _startConnector), lc));
				_graphicsScene->addItem(lc);
				_endConnector->addConnection(_startConnector, policyEnd->getRelation(), policyEnd->getRelationDirection(), lc);

				// Use the relation of the policyEnd, because this is the same for Start and End
				_startConnector->addConnection(_endConnector, policyEnd->getRelation(), getOppositeRelationDirection(policyEnd->getRelationDirection()), lc);
				notifyConnectionAdded(_startConnector, _endConnector, policyEnd->getRelation(), policyEnd->getRelationDirection());
				notifyConnectionsChanged();
				_startConnector = nullptr;
				_endConnector = nullptr;
			}
		} else {
			emit setPropertyWindow(component->getPropertyWindow());
		}
	}

	void WidgetEdit::notifyConnectionsChanged() {
		emit notifyChanged();
	}

	void WidgetEdit::notifyConnectionAdded(WidgetEditComponent * node1, WidgetEditComponent * node2, ComponentRelation relation, ComponentRelationDirection relationDirection) {
		// Stop the system if needed
		bool wasStarted = _mustStopParticleSystem();

		if (relation == ComponentRelation::CR_INCLUDE) {
			if (!_processIncludeAdded(node1, node2)) {
				// Switch
				_processIncludeAdded(node2, node1);
			}
		} else if (relation == ComponentRelation::CR_EXCLUDE) {
			if (!_processExcludeAdded(node1, node2)) {
				// Switch
				_processExcludeAdded(node2, node1);
			}
		} else if (relation == ComponentRelation::CR_EMIT) {
			if (!_processEmitAdded(node1, node2, relationDirection)) {
				// Switch
				_processEmitAdded(node2, node1, getOppositeRelationDirection(relationDirection));
			}
		} else if (relation == ComponentRelation::CR_INTERFACE) {
			if (!_processInterfaceAdded(node1, node2)) {
				// Switch
				_processInterfaceAdded(node2, node1);
			}
		} else if (relation == ComponentRelation::CR_SLAVE) {
			if (!_processSlaveAdded(node1, node2)) {
				// Switch
				_processSlaveAdded(node2, node1);
			}
		} else if (relation == ComponentRelation::CR_ENABLE) {
			if (!_processEnableAdded(node1, node2)) {
				// Switch
				_processEnableAdded(node2, node1);
			}
		} else if (relation == ComponentRelation::CR_FORCE) {
			if (!_processForceAdded(node1, node2)) {
				// Switch
				_processForceAdded(node2, node1);
			}
		} else if (relation == ComponentRelation::CR_PLACE) {
			if (!_processPlaceAdded(node1, node2)) {
				// Switch
				_processPlaceAdded(node2, node1);
			}
		}

		_mustRestartParticleSystem(wasStarted);
	}

	void WidgetEdit::notifyConnectionRemoved(WidgetEditComponent * node1, WidgetEditComponent * node2, ComponentRelation relation, ComponentRelationDirection relationDirection) {
		// Stop the system if needed
		bool wasStarted = _mustStopParticleSystem();

		std::pair<WidgetEditComponent *, WidgetEditComponent *> p = std::make_pair(node1, node2);
		_graphicsScene->removeItem(_connections[p]);

		delete _connections[p];
		_connections.erase(p);
		_connections.erase(p);

		// Deleting connections also means removing (not deleting) them from the ParticleSystem or its related components.
		if (relation == ComponentRelation::CR_INCLUDE) {
			if (!_processIncludeRemoved(node1, node2)) {
				// Switch
				_processIncludeRemoved(node2, node1);
			}
		} else if (relation == ComponentRelation::CR_EXCLUDE) {
			if (!_processExcludeRemoved(node1, node2)) {
				// Switch
				_processExcludeRemoved(node2, node1);
			}
		} else if (relation == ComponentRelation::CR_EMIT) {
			if (!_processEmitRemoved(node1, node2)) {
				// Switch
				_processEmitRemoved(node2, node1);
			}
		} else if (relation == ComponentRelation::CR_INTERFACE) {
			if (!_processInterfaceRemoved(node1, node2)) {
				// Switch
				_processInterfaceRemoved(node2, node1);
			}
		} else if (relation == ComponentRelation::CR_SLAVE) {
			if (!_processSlaveRemoved(node1, node2)) {
				// Switch
				_processSlaveRemoved(node2, node1);
			}
		} else if (relation == ComponentRelation::CR_ENABLE) {
			if (!_processEnableRemoved(node1, node2)) {
				// Switch
				_processEnableRemoved(node2, node1);
			}
		} else if (relation == ComponentRelation::CR_FORCE) {
			if (!_processForceRemoved(node1, node2)) {
				// Switch
				_processForceRemoved(node2, node1);
			}
		} else if (relation == ComponentRelation::CR_PLACE) {
			if (!_processPlaceRemoved(node1, node2)) {
				// Switch
				_processPlaceRemoved(node2, node1);
			}
		}

		_mustRestartParticleSystem(wasStarted);
	}

	void WidgetEdit::resetConnectionMode() {
		_startConnector = nullptr;
		_endConnector = nullptr;
		setConnectionMode(CM_CONNECT_NONE);
	}

	void WidgetEdit::setNewParticleSystem(ParticleUniverse::ParticleSystem * newParticleSystem) {
		while (!_connections.empty()) {
			auto it = _connections.begin();
			_graphicsScene->removeItem(it->second);
			delete it->second;
			auto first = it->first.first;
			auto second = it->first.second;
			_connections.erase(std::make_pair(first, second));
			_connections.erase(std::make_pair(second, first));
		}
		emit setPropertyWindow(nullptr);
		for (WidgetEditComponent * wec : _components) {
			_graphicsScene->removeItem(wec);
			wec->deleteLater();
		}
		_components.clear();
		//_graphicsScene->views().first()->hide();
		//_graphicsScene->views().first()->show();
		WidgetEditComponent * particleSystemEditComponent = forceCreateParticleSystemEditComponent(); // 'Guarantees' a valid particleSystemEditComponent
		if (copyParticleSystemPropertiesToPropertyWindow(particleSystemEditComponent, newParticleSystem)) {
			createParticleSystemComponents(particleSystemEditComponent, newParticleSystem);
		}
	}

	void WidgetEdit::addNewTechnique() {
		createTechniqueForComponent(createTechniqueEditComponent());
	}

	void WidgetEdit::addNewRenderer() {
		createRendererForComponent(RENDERER_BILLBOARD, createRendererEditComponent(CST_RENDERER_BILLBOARD));
	}

	void WidgetEdit::addNewEmitter() {
		createEmitterForComponent(EMITTER_BOX, createEmitterEditComponent(CST_EMITTER_BOX));
	}

	void WidgetEdit::addNewAffector() {
		createAffectorForComponent(AFFECTOR_ALIGN, createAffectorEditComponent(CST_AFFECTOR_ALIGN));
	}

	void WidgetEdit::addNewObserver() {
		createObserverForComponent(OBSERVER_ON_CLEAR, createObserverEditComponent(CST_OBSERVER_ON_CLEAR));
	}

	void WidgetEdit::addNewHandler() {
		createHandlerForComponent(HANDLER_DO_AFFECTOR, createHandlerEditComponent(CST_HANDLER_DO_AFFECTOR));
	}

	void WidgetEdit::addNewBehaviour() {
		createBehaviourForComponent(BEHAVIOUR_SLAVE, createBehaviourEditComponent(CST_BEHAVIOUR_SLAVE));
	}

	void WidgetEdit::addNewExtern() {
		createExternForComponent(EXTERN_BOX_COLLIDER, createExternEditComponent(CST_EXTERN_BOX_COLLIDER));
	}

	void WidgetEdit::addConnection() {
		setConnectionMode(CM_CONNECT_STARTING);
	}

	void WidgetEdit::removeConnection() {
		setConnectionMode(CM_DISCONNECT);
	}

	void WidgetEdit::triggerResetConnectionMode() {
		resetConnectionMode();
	}

	void WidgetEdit::triggerRenameParticleSystem(QString oldName, QString newName) {
		emit renameParticleSystem(oldName, newName);
	}

	void WidgetEdit::triggerNotifyChanged() {
		emit notifyChanged();
	}

	void WidgetEdit::createTechniqueForComponent(WidgetEditComponent * component) {
		ParticleUniverse::ParticleSystemManager * particleSystemManager = ParticleUniverse::ParticleSystemManager::getSingletonPtr();
		ParticleUniverse::ParticleTechnique * newTechnique = particleSystemManager->createTechnique();
		newTechnique->setName(component->getPropertyWindow()->getComponentName().toStdString());
		component->setPUElement(newTechnique);
	}

	void WidgetEdit::createRendererForComponent(const QString & type, WidgetEditComponent * component) {
		ParticleUniverse::ParticleSystemManager * particleSystemManager = ParticleUniverse::ParticleSystemManager::getSingletonPtr();
		ParticleUniverse::ParticleRenderer * newRenderer = particleSystemManager->createRenderer(type.toStdString());
		component->setPUElement(newRenderer);
	}

	void WidgetEdit::createEmitterForComponent(const QString & type, WidgetEditComponent * component) {
		ParticleUniverse::ParticleSystemManager * particleSystemManager = ParticleUniverse::ParticleSystemManager::getSingletonPtr();
		ParticleUniverse::ParticleEmitter * newEmitter = particleSystemManager->createEmitter(type.toStdString());
		newEmitter->setName(component->getPropertyWindow()->getComponentName().toStdString());
		component->setPUElement(newEmitter);
	}

	void WidgetEdit::createAffectorForComponent(const QString & type, WidgetEditComponent * component) {
		ParticleUniverse::ParticleSystemManager * particleSystemManager = ParticleUniverse::ParticleSystemManager::getSingletonPtr();
		ParticleUniverse::ParticleAffector * newAffector = particleSystemManager->createAffector(type.toStdString());
		newAffector->setName(component->getPropertyWindow()->getComponentName().toStdString());
		component->setPUElement(newAffector);
	}

	void WidgetEdit::createObserverForComponent(const QString & type, WidgetEditComponent * component) {
		ParticleUniverse::ParticleSystemManager * particleSystemManager = ParticleUniverse::ParticleSystemManager::getSingletonPtr();
		ParticleUniverse::ParticleObserver * newObserver = particleSystemManager->createObserver(type.toStdString());
		newObserver->setName(component->getPropertyWindow()->getComponentName().toStdString());
		component->setPUElement(newObserver);
	}

	void WidgetEdit::createHandlerForComponent(const QString & type, WidgetEditComponent * component) {
		ParticleUniverse::ParticleSystemManager * particleSystemManager = ParticleUniverse::ParticleSystemManager::getSingletonPtr();
		ParticleUniverse::ParticleEventHandler * newHandler = particleSystemManager->createEventHandler(type.toStdString());
		newHandler->setName(component->getPropertyWindow()->getComponentName().toStdString());
		component->setPUElement(newHandler);
	}

	void WidgetEdit::createBehaviourForComponent(const QString & type, WidgetEditComponent * component) {
		ParticleUniverse::ParticleSystemManager * particleSystemManager = ParticleUniverse::ParticleSystemManager::getSingletonPtr();
		ParticleUniverse::ParticleBehaviour * newBehaviour = particleSystemManager->createBehaviour(type.toStdString());
		component->setPUElement(newBehaviour);
	}

	void WidgetEdit::createExternForComponent(const QString & type, WidgetEditComponent * component) {
		ParticleUniverse::ParticleSystemManager * particleSystemManager = ParticleUniverse::ParticleSystemManager::getSingletonPtr();
		ParticleUniverse::Extern * newExtern = particleSystemManager->createExtern(type.toStdString());
		newExtern->setName(component->getPropertyWindow()->getComponentName().toStdString());
		component->setPUElement(newExtern);
	}

	WidgetEditComponent * WidgetEdit::forceCreateParticleSystemEditComponent() {
		WidgetEditComponent * particleSystemEditComponent = nullptr;
		for (WidgetEditComponent * wec : _components) {
			if (wec->getComponentType() == CT_SYSTEM) {
				particleSystemEditComponent = wec;
				break;
			}
		}
		if (!particleSystemEditComponent) {
			// create a particle system and particle system edit component
			particleSystemEditComponent = createParticleSystemEditComponent();
		}
		// return the SYSTEM EditComponent
		return particleSystemEditComponent;
	}

	WidgetEditComponent * WidgetEdit::createParticleSystemEditComponent() {
		WidgetEditComponent * systemComponent = new WidgetEditComponent(this, _graphicsScene, "mySystem", CT_SYSTEM, CST_UNDEFINED);
		
		systemComponent->createPropertyWindow(CST_UNDEFINED); // Recreate it, so it contains the root frame

		// Altough it is possible that a particle system itself is emitted, the connection to the emitter is not defined (there can only be one system on the canvas)
		systemComponent->addPolicy(ComponentRelation::CR_INCLUDE, ComponentRelationDirection::CRDIR_PRIMARY, CRD_INCLUDES, CT_TECHNIQUE, CST_UNDEFINED);

		_graphicsScene->addItem(systemComponent);
		_components.push_back(systemComponent);
		connect(systemComponent, SIGNAL(renameParticleSystem(QString, QString)), this, SLOT(triggerRenameParticleSystem(QString, QString)), Qt::QueuedConnection);
		connect(systemComponent, SIGNAL(notifyChanged()), this, SLOT(triggerNotifyChanged()), Qt::QueuedConnection);
		return systemComponent;
	}

	WidgetEditComponent * WidgetEdit::createTechniqueEditComponent() {
		QString name = "Technique" + QString::number(_techniqueCounter++);
		WidgetEditComponent * technique = new WidgetEditComponent(this, _graphicsScene, name, CT_TECHNIQUE, CST_UNDEFINED);
		technique->addPolicy(ComponentRelation::CR_INCLUDE, ComponentRelationDirection::CRDIR_SECUNDAIRY, CRD_INCLUDED_BY, CT_SYSTEM, CST_UNDEFINED, false);
		technique->addPolicy(ComponentRelation::CR_INCLUDE, ComponentRelationDirection::CRDIR_PRIMARY, CRD_INCLUDES, CT_RENDERER, CST_UNDEFINED, false);
		technique->addPolicy(ComponentRelation::CR_INCLUDE, ComponentRelationDirection::CRDIR_PRIMARY, CRD_INCLUDES, CT_EMITTER, CST_UNDEFINED);
		technique->addPolicy(ComponentRelation::CR_INCLUDE, ComponentRelationDirection::CRDIR_PRIMARY, CRD_INCLUDES, CT_AFFECTOR, CST_UNDEFINED);
		technique->addPolicy(ComponentRelation::CR_INCLUDE, ComponentRelationDirection::CRDIR_PRIMARY, CRD_INCLUDES, CT_OBSERVER, CST_UNDEFINED);
		technique->addPolicy(ComponentRelation::CR_INCLUDE, ComponentRelationDirection::CRDIR_PRIMARY, CRD_INCLUDES, CT_BEHAVIOUR, CST_UNDEFINED);
		technique->addPolicy(ComponentRelation::CR_INCLUDE, ComponentRelationDirection::CRDIR_PRIMARY, CRD_INCLUDES, CT_EXTERN, CST_UNDEFINED);
		technique->addPolicy(ComponentRelation::CR_EMIT, ComponentRelationDirection::CRDIR_SECUNDAIRY, CRD_EMITTED_BY, CT_EMITTER, CST_UNDEFINED, true, true, DRAW_EMITTED_COLOURCODE);
		technique->addPolicy(ComponentRelation::CR_ENABLE, ComponentRelationDirection::CRDIR_SECUNDAIRY, CRD_ENABLED_BY, CT_HANDLER, CST_HANDLER_DO_ENABLE_COMPONENT, true, false, DRAW_SPECIAL_CASE_COLOURCODE);
		_graphicsScene->addItem(technique);
		_components.push_back(technique);
		connect(technique, SIGNAL(renameParticleSystem(QString, QString)), this, SLOT(triggerRenameParticleSystem(QString, QString)), Qt::QueuedConnection);
		connect(technique, SIGNAL(notifyChanged()), this, SLOT(triggerNotifyChanged()), Qt::QueuedConnection);
		return technique;
	}

	WidgetEditComponent * WidgetEdit::createRendererEditComponent(const QString & type) {
		QString name = "Renderer" + QString::number(_rendererCounter++);
		WidgetEditComponent * rendererComponent = new WidgetEditComponent(this, _graphicsScene, name, CT_RENDERER, type);
		rendererComponent->addPolicy(ComponentRelation::CR_INCLUDE, ComponentRelationDirection::CRDIR_SECUNDAIRY, CRD_INCLUDED_BY, CT_TECHNIQUE, CST_UNDEFINED, false);
		_graphicsScene->addItem(rendererComponent);
		_components.push_back(rendererComponent);
		connect(rendererComponent, SIGNAL(renameParticleSystem(QString, QString)), this, SLOT(triggerRenameParticleSystem(QString, QString)), Qt::QueuedConnection);
		connect(rendererComponent, SIGNAL(notifyChanged()), this, SLOT(triggerNotifyChanged()), Qt::QueuedConnection);
		return rendererComponent;
	}

	WidgetEditComponent * WidgetEdit::createEmitterEditComponent(const QString & type) {
		QString name = "Emitter" + QString::number(_emitterCounter++);
		WidgetEditComponent * emitterComponent = new WidgetEditComponent(this, _graphicsScene, name, CT_EMITTER, type);

		// Altough it is possible to emit a particle system, the connection to the system is not defined (there can only be one system on the canvas)
		emitterComponent->addUniqueRelation(ComponentRelation::CR_EMIT, ComponentRelationDirection::CRDIR_PRIMARY); // Only emission of one type is allowed
		emitterComponent->addPolicy(ComponentRelation::CR_INCLUDE, ComponentRelationDirection::CRDIR_SECUNDAIRY, CRD_INCLUDED_BY, CT_TECHNIQUE, CST_UNDEFINED, false);
		emitterComponent->addPolicy(ComponentRelation::CR_EMIT, ComponentRelationDirection::CRDIR_PRIMARY, CRD_EMITS, CT_TECHNIQUE, CST_UNDEFINED, false, true, DRAW_EMITTED_COLOURCODE);
		emitterComponent->addPolicy(ComponentRelation::CR_EMIT, ComponentRelationDirection::CRDIR_PRIMARY, CRD_EMITS, CT_EMITTER, CST_UNDEFINED, false, true, DRAW_EMITTED_COLOURCODE);
		emitterComponent->addPolicy(ComponentRelation::CR_EMIT, ComponentRelationDirection::CRDIR_PRIMARY, CRD_EMITS, CT_AFFECTOR, CST_UNDEFINED, false, true, DRAW_EMITTED_COLOURCODE);
		emitterComponent->addPolicy(ComponentRelation::CR_EMIT, ComponentRelationDirection::CRDIR_SECUNDAIRY, CRD_EMITTED_BY, CT_EMITTER, CST_UNDEFINED, true, true, DRAW_EMITTED_COLOURCODE);
		emitterComponent->addPolicy(ComponentRelation::CR_SLAVE, ComponentRelationDirection::CRDIR_SECUNDAIRY, CRD_SLAVES, CT_EMITTER, CST_EMITTER_SLAVE, true, false, DRAW_SPECIAL_CASE_COLOURCODE);
		emitterComponent->addPolicy(ComponentRelation::CR_SLAVE, ComponentRelationDirection::CRDIR_PRIMARY, CRD_SLAVE_OF, CT_EMITTER, CST_UNDEFINED, false, true, DRAW_SPECIAL_CASE_COLOURCODE);
		emitterComponent->addPolicy(ComponentRelation::CR_EXCLUDE, ComponentRelationDirection::CRDIR_SECUNDAIRY, CRD_EXCLUDED_BY, CT_AFFECTOR, CST_UNDEFINED, true, true, DRAW_DEFAULT_COLOURCODE, Qt::DashLine);
		emitterComponent->addPolicy(ComponentRelation::CR_ENABLE, ComponentRelationDirection::CRDIR_SECUNDAIRY, CRD_ENABLED_BY, CT_HANDLER, CST_HANDLER_DO_ENABLE_COMPONENT, true, false, DRAW_SPECIAL_CASE_COLOURCODE);
		emitterComponent->addPolicy(ComponentRelation::CR_PLACE, ComponentRelationDirection::CRDIR_SECUNDAIRY, CRD_PLACED_BY, CT_HANDLER, CST_HANDLER_DO_PLACEMENT_PARTICLE, true, false, DRAW_DEFAULT_COLOURCODE, Qt::DotLine);
		_graphicsScene->addItem(emitterComponent);
		_components.push_back(emitterComponent);
		connect(emitterComponent, SIGNAL(renameParticleSystem(QString, QString)), this, SLOT(triggerRenameParticleSystem(QString, QString)), Qt::QueuedConnection);
		connect(emitterComponent, SIGNAL(notifyChanged()), this, SLOT(triggerNotifyChanged()), Qt::QueuedConnection);
		return emitterComponent;
	}

	WidgetEditComponent * WidgetEdit::createAffectorEditComponent(const QString & type) {
		QString name = "Affector" + QString::number(_affectorCounter++);
		WidgetEditComponent * affectorComponent = new WidgetEditComponent(this, _graphicsScene, name, CT_AFFECTOR, type);
		affectorComponent->addPolicy(ComponentRelation::CR_INCLUDE, ComponentRelationDirection::CRDIR_SECUNDAIRY, CRD_INCLUDED_BY, CT_TECHNIQUE, CST_UNDEFINED, false);
		affectorComponent->addPolicy(ComponentRelation::CR_EMIT, ComponentRelationDirection::CRDIR_SECUNDAIRY, CRD_EMITTED_BY, CT_EMITTER, CST_UNDEFINED, true, true, DRAW_EMITTED_COLOURCODE);
		affectorComponent->addPolicy(ComponentRelation::CR_EXCLUDE, ComponentRelationDirection::CRDIR_PRIMARY, CRD_EXCLUDES, CT_EMITTER, CST_UNDEFINED, true, true, DRAW_DEFAULT_COLOURCODE, Qt::DashLine);
		affectorComponent->addPolicy(ComponentRelation::CR_ENABLE, ComponentRelationDirection::CRDIR_SECUNDAIRY, CRD_ENABLED_BY, CT_HANDLER, CST_HANDLER_DO_ENABLE_COMPONENT, true, false, DRAW_SPECIAL_CASE_COLOURCODE);
		affectorComponent->addPolicy(ComponentRelation::CR_FORCE, ComponentRelationDirection::CRDIR_SECUNDAIRY, CRD_FORCED_BY, CT_HANDLER, CST_HANDLER_DO_AFFECTOR, true, false, DRAW_DEFAULT_COLOURCODE, Qt::DotLine);
		_graphicsScene->addItem(affectorComponent);
		_components.push_back(affectorComponent);
		connect(affectorComponent, SIGNAL(renameParticleSystem(QString, QString)), this, SLOT(triggerRenameParticleSystem(QString, QString)), Qt::QueuedConnection);
		connect(affectorComponent, SIGNAL(notifyChanged()), this, SLOT(triggerNotifyChanged()), Qt::QueuedConnection);
		return affectorComponent;
	}

	WidgetEditComponent * WidgetEdit::createObserverEditComponent(const QString & type) {
		QString name = "Observer" + QString::number(_observerCounter++);
		WidgetEditComponent * observerComponent = new WidgetEditComponent(this, _graphicsScene, name, CT_OBSERVER, type);
		observerComponent->addPolicy(ComponentRelation::CR_INCLUDE, ComponentRelationDirection::CRDIR_PRIMARY, CRD_INCLUDES, CT_HANDLER, CST_UNDEFINED, true);
		observerComponent->addPolicy(ComponentRelation::CR_INCLUDE, ComponentRelationDirection::CRDIR_SECUNDAIRY, CRD_INCLUDED_BY, CT_TECHNIQUE, CST_UNDEFINED, false);
		observerComponent->addPolicy(ComponentRelation::CR_ENABLE, ComponentRelationDirection::CRDIR_SECUNDAIRY, CRD_ENABLED_BY, CT_HANDLER, CST_HANDLER_DO_ENABLE_COMPONENT, true, false, DRAW_SPECIAL_CASE_COLOURCODE);
		_graphicsScene->addItem(observerComponent);
		_components.push_back(observerComponent);
		connect(observerComponent, SIGNAL(renameParticleSystem(QString, QString)), this, SLOT(triggerRenameParticleSystem(QString, QString)), Qt::QueuedConnection);
		connect(observerComponent, SIGNAL(notifyChanged()), this, SLOT(triggerNotifyChanged()), Qt::QueuedConnection);
		return observerComponent;
	}

	WidgetEditComponent * WidgetEdit::createHandlerEditComponent(const QString & type) {
		QString name = "Handler" + QString::number(_handlerCounter++);
		WidgetEditComponent * handler = new WidgetEditComponent(this, _graphicsScene, name, CT_HANDLER, type);
		handler->addUniqueRelation(ComponentRelation::CR_ENABLE, ComponentRelationDirection::CRDIR_PRIMARY); // Only enabling of one type is allowed
		handler->addPolicy(ComponentRelation::CR_INCLUDE, ComponentRelationDirection::CRDIR_SECUNDAIRY, CRD_INCLUDED_BY, CT_OBSERVER, CST_UNDEFINED, false);
		handler->addPolicy(ComponentRelation::CR_ENABLE, ComponentRelationDirection::CRDIR_PRIMARY, CRD_ENABLES, CT_TECHNIQUE, CST_UNDEFINED, false, true, DRAW_SPECIAL_CASE_COLOURCODE);
		handler->addPolicy(ComponentRelation::CR_ENABLE, ComponentRelationDirection::CRDIR_PRIMARY, CRD_ENABLES, CT_EMITTER, CST_UNDEFINED, false, true, DRAW_SPECIAL_CASE_COLOURCODE);
		handler->addPolicy(ComponentRelation::CR_ENABLE, ComponentRelationDirection::CRDIR_PRIMARY, CRD_ENABLES, CT_AFFECTOR, CST_UNDEFINED, false, true, DRAW_SPECIAL_CASE_COLOURCODE);
		handler->addPolicy(ComponentRelation::CR_ENABLE, ComponentRelationDirection::CRDIR_PRIMARY, CRD_ENABLES, CT_OBSERVER, CST_UNDEFINED, false, true, DRAW_SPECIAL_CASE_COLOURCODE);
		handler->addPolicy(ComponentRelation::CR_PLACE, ComponentRelationDirection::CRDIR_PRIMARY, CRD_PLACES, CT_EMITTER, CST_UNDEFINED, false, true, DRAW_DEFAULT_COLOURCODE, Qt::DotLine);
		handler->addPolicy(ComponentRelation::CR_FORCE, ComponentRelationDirection::CRDIR_PRIMARY, CRD_FORCES, CT_AFFECTOR, CST_UNDEFINED, false, true, DRAW_DEFAULT_COLOURCODE, Qt::DotLine);
		_graphicsScene->addItem(handler);
		_components.push_back(handler);
		connect(handler, SIGNAL(renameParticleSystem(QString, QString)), this, SLOT(triggerRenameParticleSystem(QString, QString)), Qt::QueuedConnection);
		connect(handler, SIGNAL(notifyChanged()), this, SLOT(triggerNotifyChanged()), Qt::QueuedConnection);
		return handler;
	}

	WidgetEditComponent * WidgetEdit::createBehaviourEditComponent(const QString & type) {
		QString name = "Behaviour" + QString::number(_behaviourCounter++);
		WidgetEditComponent * behaviourComponent = new WidgetEditComponent(this, _graphicsScene, name, CT_BEHAVIOUR, type);
		behaviourComponent->addPolicy(ComponentRelation::CR_INCLUDE, ComponentRelationDirection::CRDIR_SECUNDAIRY, CRD_INCLUDED_BY, CT_TECHNIQUE, CST_UNDEFINED, false);
		_graphicsScene->addItem(behaviourComponent);
		_components.push_back(behaviourComponent);
		connect(behaviourComponent, SIGNAL(renameParticleSystem(QString, QString)), this, SLOT(triggerRenameParticleSystem(QString, QString)), Qt::QueuedConnection);
		connect(behaviourComponent, SIGNAL(notifyChanged()), this, SLOT(triggerNotifyChanged()), Qt::QueuedConnection);
		return behaviourComponent;
	}

	WidgetEditComponent * WidgetEdit::createExternEditComponent(const QString & type) {
		QString name = "Extern" + QString::number(_externCounter++);
		WidgetEditComponent * externObjectComponent = new WidgetEditComponent(this, _graphicsScene, name, CT_EXTERN, type);
		externObjectComponent->addPolicy(ComponentRelation::CR_INCLUDE, ComponentRelationDirection::CRDIR_SECUNDAIRY, CRD_INCLUDED_BY, CT_TECHNIQUE, CST_UNDEFINED, false);
		_graphicsScene->addItem(externObjectComponent);
		_components.push_back(externObjectComponent);
		connect(externObjectComponent, SIGNAL(renameParticleSystem(QString, QString)), this, SLOT(triggerRenameParticleSystem(QString, QString)), Qt::QueuedConnection);
		connect(externObjectComponent, SIGNAL(notifyChanged()), this, SLOT(triggerNotifyChanged()), Qt::QueuedConnection);
		return externObjectComponent;
	}

	bool WidgetEdit::copyParticleSystemPropertiesToPropertyWindow(WidgetEditComponent * particleSystemEditComponent, ParticleUniverse::ParticleSystem * particleSystem) {
		if (!particleSystem || !particleSystemEditComponent) {
			return false;
		}

		SystemPropertyWindow * systemPropertyWindow = static_cast<SystemPropertyWindow *>(particleSystemEditComponent->getPropertyWindow());
		particleSystemEditComponent->setName(QString::fromStdString(particleSystem->getTemplateName()));
		particleSystemEditComponent->setCaption();
		systemPropertyWindow->copyAttributesFromSystem(particleSystem);
		return true;
	}

	bool WidgetEdit::createParticleSystemComponents(WidgetEditComponent * particleSystemEditComponent, ParticleUniverse::ParticleSystem * particleSystem) {
		if (!particleSystem || !particleSystemEditComponent) {
			return false;
		}

		particleSystemEditComponent->setPUElement(particleSystem);

		// Run through the components of the particle system and create edit components and
		// property windows and also propagate the data to the property windows.
		QPoint position = QPoint(64 + 16, 16);
		particleSystemEditComponent->setPos(position);
		QSizeF s = particleSystemEditComponent->size();
		position += QPoint(s.width(), s.height());
		position += QPoint(_offsetX, _offsetY);
		size_t numberTechniques = particleSystem->getNumTechniques();
		ParticleUniverse::ParticleTechnique * technique;
		for (size_t i = 0; i < numberTechniques; ++i) {
			technique = particleSystem->getTechnique(i);
			position = createComponentsFromTechnique(particleSystemEditComponent, technique, position);
		}

		// Create the other connections, which can only be done when all components are available
		for (size_t i = 0; i < numberTechniques; ++i) {
			technique = particleSystem->getTechnique(i);
			createOtherConnections(technique);
		}
		return true;
	}

	QPoint WidgetEdit::createComponentsFromTechnique(WidgetEditComponent * systemEditComponent, ParticleUniverse::ParticleTechnique * technique, QPoint position) {
		// First create the component of the technique itself
		WidgetEditComponent * techniqueEditComponent = createTechniqueEditComponent();
		techniqueEditComponent->setName(QString::fromStdString(technique->getName()));
		techniqueEditComponent->setCaption();
		techniqueEditComponent->setPos(position);
		techniqueEditComponent->setPUElement(technique);
		int componentWidth = techniqueEditComponent->size().width();
		int componentHeight = techniqueEditComponent->size().height();
		static_cast<TechniquePropertyWindow *>(techniqueEditComponent->getPropertyWindow())->copyAttributesFromTechnique(technique);
		createConnection(systemEditComponent, techniqueEditComponent, ComponentRelation::CR_INCLUDE, ComponentRelationDirection::CRDIR_PRIMARY);
		int highestY = position.y() + componentHeight + _offsetY;

		// Iterate trough the technique and create emitters, affectors, etc...

		// 1. Renderer
		position.setX(dynamic_cast<QGraphicsItem *>(techniqueEditComponent)->pos().x() - _offsetX - componentWidth);
		position.setY(position.y() + 0.5 * componentHeight + _offsetY);
		createComponentFromRenderer(techniqueEditComponent, technique->getRenderer(), position);
		highestY = std::max(highestY, position.y());

		// 2. Emitters
		position.setX(dynamic_cast<QGraphicsItem *>(techniqueEditComponent)->pos().x() + componentWidth + _offsetX);
		size_t numEmitters = technique->getNumEmitters();
		for (size_t i = 0; i < numEmitters; ++i) {
			ParticleUniverse::ParticleEmitter * emitter = technique->getEmitter(i);
			createComponentFromEmitter(techniqueEditComponent, emitter, position);
			position.setY(position.y() + componentHeight + _offsetY);
		}
		highestY = std::max(highestY, position.y());

		// 3. Affectors
		if (numEmitters > 0) {
			// Align it next to the emitters
			position.setX(dynamic_cast<QGraphicsItem *>(techniqueEditComponent)->pos().x() + 2 * (componentWidth + _offsetX));
			position.setY(dynamic_cast<QGraphicsItem *>(techniqueEditComponent)->pos().y() + 1.5 * componentHeight + _offsetY);
		}

		size_t numAffectors = technique->getNumAffectors();
		for (size_t i = 0; i < numAffectors; ++i) {
			ParticleUniverse::ParticleAffector * affector = technique->getAffector(i);
			createComponentFromAffector(techniqueEditComponent, affector, position);
			position.setY(position.y() + componentHeight + _offsetY);
		}
		highestY = std::max(highestY, position.y());

		// 4 + 5. Observers and Event Handlers
		size_t numObservers = technique->getNumObservers();
		position.setX(dynamic_cast<QGraphicsItem *>(techniqueEditComponent)->pos().x());
		position.setY(dynamic_cast<QGraphicsItem *>(techniqueEditComponent)->pos().y() + componentHeight + _offsetY);
		int latestHandlerY = position.y() + 0.5 * componentHeight;
		for (size_t i = 0; i < numObservers; ++i) {
			ParticleUniverse::ParticleObserver * observer = technique->getObserver(i);
			latestHandlerY = createComponentFromObserver(techniqueEditComponent, observer, position, latestHandlerY);
			position.setY(position.y() + componentHeight + _offsetY);
		}
		highestY = std::max(highestY, latestHandlerY);

		// 6. Behaviours
		position.setX(dynamic_cast<QGraphicsItem *>(techniqueEditComponent)->pos().x());
		size_t numBehaviours = technique->_getNumBehaviourTemplates();
		for (size_t i = 0; i < numBehaviours; ++i) {
			ParticleUniverse::ParticleBehaviour * behaviour = technique->_getBehaviourTemplate(i);
			createComponentFromBehaviour(techniqueEditComponent, behaviour, position);
			position.setY(position.y() + componentHeight + _offsetY);
		}
		highestY = std::max(highestY, position.y());

		// 7. Externs
		size_t numExterns = technique->getNumExterns();
		for (size_t i = 0; i < numExterns; ++i) {
			ParticleUniverse::Extern * externObject = technique->getExtern(i);
			createComponentFromExtern(techniqueEditComponent, externObject, position);
			position.setY(position.y() + componentHeight + _offsetY);
		}
		highestY = std::max(highestY, position.y());

		// Set position for next technique edit component
		position.setX(dynamic_cast<QGraphicsItem *>(techniqueEditComponent)->pos().x());
		position.setY(highestY);
		return position;
	}

	void WidgetEdit::createComponentFromRenderer(WidgetEditComponent * techniqueEditComponent, ParticleUniverse::ParticleRenderer * renderer, QPoint position) {
		// First create the component of the renderer itself
		WidgetEditComponent * rendererEditComponent = createRendererEditComponent(QString::fromStdString(renderer->getRendererType()));
		rendererEditComponent->setName("");
		rendererEditComponent->setCaption();
		rendererEditComponent->setPos(position);
		rendererEditComponent->setPUElement(renderer);
		static_cast<RendererPropertyWindow *>(rendererEditComponent->getPropertyWindow())->copyAttributesFromRenderer(renderer);
		createConnection(techniqueEditComponent, rendererEditComponent, ComponentRelation::CR_INCLUDE, ComponentRelationDirection::CRDIR_PRIMARY);
	}

	void WidgetEdit::createComponentFromEmitter(WidgetEditComponent * techniqueEditComponent, ParticleUniverse::ParticleEmitter * emitter, QPoint position) {
		// First create the component of the emitter itself
		WidgetEditComponent * emitterEditComponent = createEmitterEditComponent(QString::fromStdString(emitter->getEmitterType()));
		emitterEditComponent->setName(QString::fromStdString(emitter->getName()));
		emitterEditComponent->setCaption();
		emitterEditComponent->setPos(position);
		emitterEditComponent->setPUElement(emitter);
		static_cast<EmitterPropertyWindow *>(emitterEditComponent->getPropertyWindow())->copyAttributesFromEmitter(emitter);
		createConnection(techniqueEditComponent, emitterEditComponent, ComponentRelation::CR_INCLUDE, ComponentRelationDirection::CRDIR_PRIMARY);
	}

	void WidgetEdit::createComponentFromAffector(WidgetEditComponent * techniqueEditComponent, ParticleUniverse::ParticleAffector * affector, QPoint position) {
		// First create the component of the affector itself
		WidgetEditComponent * affectorEditComponent = createAffectorEditComponent(QString::fromStdString(affector->getAffectorType()));
		affectorEditComponent->setName(QString::fromStdString(affector->getName()));
		affectorEditComponent->setCaption();
		affectorEditComponent->setPos(position);
		affectorEditComponent->setPUElement(affector);
		static_cast<AffectorPropertyWindow *>(affectorEditComponent->getPropertyWindow())->copyAttributesFromAffector(affector);
		createConnection(techniqueEditComponent, affectorEditComponent, ComponentRelation::CR_INCLUDE, ComponentRelationDirection::CRDIR_PRIMARY);
	}

	int WidgetEdit::createComponentFromObserver(WidgetEditComponent * techniqueEditComponent, ParticleUniverse::ParticleObserver * observer, QPoint position, int latestHandlerY) {
		// First create the component of the observer itself
		WidgetEditComponent * observerEditComponent = createObserverEditComponent(QString::fromStdString(observer->getObserverType()));
		observerEditComponent->setName(QString::fromStdString(observer->getName()));
		observerEditComponent->setCaption();
		observerEditComponent->setPos(position);
		observerEditComponent->setPUElement(observer);
		static_cast<ObserverPropertyWindow *>(observerEditComponent->getPropertyWindow())->copyAttributesFromObserver(observer);
		createConnection(techniqueEditComponent, observerEditComponent, ComponentRelation::CR_INCLUDE, ComponentRelationDirection::CRDIR_PRIMARY);

		// Create all handlers
		int componentWidth = techniqueEditComponent->size().width();
		int componentHeight = techniqueEditComponent->size().height();
		position.setX(dynamic_cast<QGraphicsItem *>(techniqueEditComponent)->pos().x() - componentWidth - _offsetX);
		position.setY(latestHandlerY + _offsetY);
		size_t numEventHandlers = observer->getNumEventHandlers();
		for (size_t i = 0; i < numEventHandlers; ++i) {
			ParticleUniverse::ParticleEventHandler * handler = observer->getEventHandler(i);
			createComponentFromEventHandler(observerEditComponent, handler, position);
			position.setY(position.y() + componentHeight + _offsetY);
		}

		return position.y();
	}

	void WidgetEdit::createComponentFromEventHandler(WidgetEditComponent * observerEditComponent, ParticleUniverse::ParticleEventHandler * eventHandler, QPoint position) {
		WidgetEditComponent * handlerEditComponent = createHandlerEditComponent(QString::fromStdString(eventHandler->getEventHandlerType()));
		handlerEditComponent->setName(QString::fromStdString(eventHandler->getName()));
		handlerEditComponent->setCaption();
		handlerEditComponent->setPos(position);
		handlerEditComponent->setPUElement(eventHandler);
		static_cast<EventHandlerPropertyWindow *>(handlerEditComponent->getPropertyWindow())->copyAttributesFromEventHandler(eventHandler);
		createConnection(observerEditComponent, handlerEditComponent, ComponentRelation::CR_INCLUDE, ComponentRelationDirection::CRDIR_PRIMARY);
	}

	void WidgetEdit::createComponentFromBehaviour(WidgetEditComponent * techniqueEditComponent, ParticleUniverse::ParticleBehaviour * behaviour, QPoint position) {
		// First create the component of the behaviour itself
		WidgetEditComponent * behaviourEditComponent = createBehaviourEditComponent(QString::fromStdString(behaviour->getBehaviourType()));
		behaviourEditComponent->setName("");
		behaviourEditComponent->setCaption();
		behaviourEditComponent->setPos(position);
		behaviourEditComponent->setPUElement(behaviour);
		static_cast<BehaviourPropertyWindow *>(behaviourEditComponent->getPropertyWindow())->copyAttributesFromBehaviour(behaviour);
		createConnection(techniqueEditComponent, behaviourEditComponent, ComponentRelation::CR_INCLUDE, ComponentRelationDirection::CRDIR_PRIMARY);
	}

	void WidgetEdit::createComponentFromExtern(WidgetEditComponent * techniqueEditComponent, ParticleUniverse::Extern * externObject, QPoint position) {
		// First create the component of the extern itself
		WidgetEditComponent * externEditComponent = createExternEditComponent(QString::fromStdString(externObject->getExternType()));
		externEditComponent->setName(QString::fromStdString(externObject->getName()));
		externEditComponent->setCaption();
		externEditComponent->setPos(position);
		externEditComponent->setPUElement(externObject);
		static_cast<ExternPropertyWindow *>(externEditComponent->getPropertyWindow())->copyAttributesFromExtern(externObject);
		createConnection(techniqueEditComponent, externEditComponent, ComponentRelation::CR_INCLUDE, ComponentRelationDirection::CRDIR_PRIMARY);
	}

	void WidgetEdit::createOtherConnections(const ParticleUniverse::ParticleTechnique * technique) {
		/** Creating the other connections can only be done if all components are already created, so you need to run through
		the particle technique again.
		*/

		// 1. Emitters
		size_t numEmitters = technique->getNumEmitters();
		for (size_t i = 0; i < numEmitters; ++i) {
			ParticleUniverse::ParticleEmitter * emitter = technique->getEmitter(i);
			WidgetEditComponent * emitterEditComponent = findEditComponent(emitter);
			if (emitterEditComponent) {
				// Create connections: ComponentRelation::CR_SLAVE
				if (QString::fromStdString(emitter->getEmitterType()) == CST_EMITTER_SLAVE) {
					ParticleUniverse::SlaveEmitter * slaveEmiter = static_cast<ParticleUniverse::SlaveEmitter *>(emitter);
					QString wxName = QString::fromStdString(slaveEmiter->getMasterEmitterName());
					QString techniqueName = QString::fromStdString(slaveEmiter->getMasterTechniqueName());
					WidgetEditComponent * masterEmitterEditComponent = findEditComponentForTechnique(wxName, techniqueName);
					if (masterEmitterEditComponent) {
						createConnection(emitterEditComponent, masterEmitterEditComponent, ComponentRelation::CR_SLAVE, ComponentRelationDirection::CRDIR_PRIMARY);
					}
				}

				// Create connections: ComponentRelation::CR_EMIT
				if (emitter->getEmitsType() != ParticleUniverse::Particle::PT_VISUAL) {
					// This version of the editor doesn't allow emitted particle systems, because emitting particle systems is not recommended.
					QString componentType = CT_VISUAL;
					if (emitter->getEmitsType() == ParticleUniverse::Particle::PT_EMITTER) {
						componentType = CT_EMITTER;
					} else if (emitter->getEmitsType() == ParticleUniverse::Particle::PT_AFFECTOR) {
						componentType = CT_AFFECTOR;
					} else if (emitter->getEmitsType() == ParticleUniverse::Particle::PT_TECHNIQUE) {
						componentType = CT_TECHNIQUE;
					}
					if (componentType != CT_VISUAL) {
						QString wxName = QString::fromStdString(emitter->getEmitsName());
						WidgetEditComponent * editComponent = findEditComponent(wxName, componentType);
						if (editComponent) {
							createConnection(emitterEditComponent, editComponent, ComponentRelation::CR_EMIT, ComponentRelationDirection::CRDIR_PRIMARY);
						}
					}
				}
			}
		}

		// 2. Affectors
		// Create connections: ComponentRelation::CR_EXCLUDE
		size_t numAffector = technique->getNumAffectors();
		for (size_t i = 0; i < numAffector; ++i) {
			ParticleUniverse::ParticleAffector * affector = technique->getAffector(i);
			WidgetEditComponent * affectorEditComponent = findEditComponent(affector);
			WidgetEditComponent * editComponent;
			if (affectorEditComponent) {
				ParticleUniverse::list<Ogre::String> emittersToExclude = affector->getEmittersToExclude();
				for (ParticleUniverse::list<Ogre::String>::iterator it = emittersToExclude.begin(); it != emittersToExclude.end(); ++it) {
					// Remove the double quotes
					Ogre::String name = (*it);
					Ogre::String::size_type index = name.find_first_of("\"");
					while (index != Ogre::String::npos) {
						name = name.erase(index, 1);
						index = name.find_first_of("\"");
					}
					QString wxName = QString::fromStdString(name);
					editComponent = findEditComponent(wxName, CT_EMITTER);
					if (editComponent) {
						createConnection(affectorEditComponent, editComponent, ComponentRelation::CR_EXCLUDE, ComponentRelationDirection::CRDIR_PRIMARY);
					}
				}
			}
		}

		// 3. Event Handlers
		size_t numObservers = technique->getNumObservers();
		WidgetEditComponent * editComponent;
		for (size_t i = 0; i < numObservers; ++i) {
			ParticleUniverse::ParticleObserver * observer = technique->getObserver(i);
			size_t numEventHandlers = observer->getNumEventHandlers();
			for (size_t j = 0; j < numEventHandlers; ++j) {
				ParticleUniverse::ParticleEventHandler * handler = observer->getEventHandler(j);
				WidgetEditComponent * eventHandlerEditComponent = findEditComponent(handler);
				if (QString::fromStdString(handler->getEventHandlerType()) == CST_HANDLER_DO_ENABLE_COMPONENT) {
					// Create connections: ComponentRelation::CR_ENABLE
					ParticleUniverse::DoEnableComponentEventHandler * doEnableComponentEventHandler = static_cast<ParticleUniverse::DoEnableComponentEventHandler *>(handler);
					ParticleUniverse::ComponentType componentType = doEnableComponentEventHandler->getComponentType();
					QString ct = CT_EMITTER;
					if (componentType == ParticleUniverse::CT_AFFECTOR) {
						ct = CT_AFFECTOR;
					} else if (componentType == ParticleUniverse::CT_TECHNIQUE) {
						ct = CT_TECHNIQUE;
					} else if (componentType == ParticleUniverse::CT_OBSERVER) {
						ct = CT_OBSERVER;
					}
					QString wxName = QString::fromStdString(doEnableComponentEventHandler->getComponentName());
					editComponent = findEditComponent(wxName, ct);
					if (editComponent) {
						createConnection(eventHandlerEditComponent, editComponent, ComponentRelation::CR_ENABLE, ComponentRelationDirection::CRDIR_PRIMARY);
					}
				} else if (QString::fromStdString(handler->getEventHandlerType()) == CST_HANDLER_DO_AFFECTOR) {
					// Create connections: ComponentRelation::CR_FORCE
					ParticleUniverse::DoAffectorEventHandler * doAffectorEventHandler = static_cast<ParticleUniverse::DoAffectorEventHandler *>(handler);
					QString wxName = QString::fromStdString(doAffectorEventHandler->getAffectorName());
					editComponent = findEditComponent(wxName, CT_AFFECTOR);
					if (editComponent) {
						createConnection(eventHandlerEditComponent, editComponent, ComponentRelation::CR_FORCE, ComponentRelationDirection::CRDIR_PRIMARY);
					}
				} else if (QString::fromStdString(handler->getEventHandlerType()) == CST_HANDLER_DO_PLACEMENT_PARTICLE) {
					// Create connections: ComponentRelation::CR_PLACE
					ParticleUniverse::DoPlacementParticleEventHandler * doPlacementParticleEventHandler = static_cast<ParticleUniverse::DoPlacementParticleEventHandler *>(handler);
					QString wxName = QString::fromStdString(doPlacementParticleEventHandler->getForceEmitterName());
					editComponent = findEditComponent(wxName, CT_EMITTER);
					if (editComponent) {
						createConnection(eventHandlerEditComponent, editComponent, ComponentRelation::CR_PLACE, ComponentRelationDirection::CRDIR_PRIMARY);
					}
				}
			}
		}
	}

	void WidgetEdit::createConnection(WidgetEditComponent * componentPrimary, WidgetEditComponent * componentSecondary, ComponentRelation relation, ComponentRelationDirection direction) {
		// Establish the connection between the two
		connections::ConnectionPolicy * policy = componentPrimary->getPolicy(relation, direction, componentSecondary->getComponentType());
		connections::LineConnector * lc = new connections::LineConnector(componentPrimary, componentSecondary, policy->getColour(), policy->getLineStyle());
		_connections.insert(std::make_pair(std::make_pair(componentPrimary, componentSecondary), lc));
		_connections.insert(std::make_pair(std::make_pair(componentSecondary, componentPrimary), lc));
		_graphicsScene->addItem(lc);
		componentPrimary->addConnection(componentSecondary, relation, direction, lc);
		componentSecondary->addConnection(componentPrimary, relation, getOppositeRelationDirection(direction), lc);
	}

	WidgetEditComponent * WidgetEdit::findEditComponent(const ParticleUniverse::IElement * puElement) const {
		WidgetEditComponent * component = nullptr;
		for (std::vector<WidgetEditComponent *>::const_iterator it = _components.begin(); it != _components.end(); ++it) {
			component = *it;
			if (component->getPUElement() == puElement) {
				break;
			}
		}
		return component;
	}

	WidgetEditComponent * WidgetEdit::findEditComponent(const QString & name, const QString & type, WidgetEditComponent * skip) const {
		/*  This implementation returns the first component found, but it ignores the fact that the same type with the same name
		can occur multiple times (i.e. an emitter with the same name in another technique).
		TODO: Add another parameter to make the search unique.
		*/
		WidgetEditComponent * component = nullptr;
		for (std::vector<WidgetEditComponent *>::const_iterator it = _components.begin(); it != _components.end(); ++it) {
			component = *it;
			if (skip != component && !component->getName().isEmpty() && component->getName() == name && component->getComponentType() == type) {
				break;
			}
		}
		return component;
	}

	WidgetEditComponent * WidgetEdit::findEditComponentForTechnique(const QString & name, const QString & techniqueName) const {
		// Search for a component that is included by a certain technique component
		WidgetEditComponent * editComponent = nullptr;
		WidgetEditComponent * techniqueEditComponent = findEditComponent(techniqueName, CT_TECHNIQUE);
		for (std::vector<WidgetEditComponent *>::const_iterator it = _components.begin(); it != _components.end(); ++it) {
			editComponent = *it;
			if (editComponent->getName() == name) {
				if (techniqueEditComponent->isConnected(editComponent, ComponentRelation::CR_INCLUDE, ComponentRelationDirection::CRDIR_PRIMARY)) {
					break;
				}
			}
		}
		return editComponent;
	}

	void WidgetEdit::setConnectionMode(ConnectionMode connectionMode) {
		_connectionMode = connectionMode;
		QCursor connectCursor = QCursor();

		if (connectionMode == CM_CONNECT_STARTING || connectionMode == CM_CONNECT_ENDING) {
			// Change the cursor
			connectCursor = QCursor(QPixmap("../media/textures/connect.png"));
		} else if (connectionMode == CM_DISCONNECT) {
			// Change the cursor
			connectCursor = QCursor(QPixmap("../media/textures/disconnect.png"));
		}

		_graphicsView->setCursor(connectCursor);
		for (QGraphicsItem * gi : _graphicsScene->items()) {
			gi->setCursor(connectCursor);
			for (QGraphicsItem * gi2 : gi->childItems()) {
				gi2->setCursor(connectCursor);
			}
		}
		for (std::vector<WidgetEditComponent *>::iterator it = _components.begin(); it != _components.end(); ++it) {
			(*it)->setCursor(connectCursor);
		}
	}

	bool WidgetEdit::isConnectionPossible(WidgetEditComponent * component) const {
		// Check both sides
		return _startConnector && component->isConnectionPossible(_startConnector) && _startConnector->isConnectionPossible(component);
	}

	bool WidgetEdit::_mustStopParticleSystem() {
		// Get the Particle System Edit Component, because it is associated with the Particle System
		WidgetEditComponent * component = getParticleSystemEditComponent();
		if (!component) {
			return false;
		}

		// Set it to stop
		ParticleUniverse::ParticleSystem * particleSystem = static_cast<ParticleUniverse::ParticleSystem *>(component->getPUElement());
		bool wasStarted = false;
		if (particleSystem && particleSystem->getState() == ParticleUniverse::ParticleSystem::PSS_STARTED) {
			wasStarted = true;
			particleSystem->stop();
		}
		return wasStarted;
	}

	void WidgetEdit::_mustRestartParticleSystem(bool wasStarted) {
		// Start the system if needed
		if (wasStarted) {
			// Get the Particle System Edit Component, because it is associated with the Particle System
			WidgetEditComponent * component = getParticleSystemEditComponent();
			if (component) {
				ParticleUniverse::ParticleSystem * particleSystem = dynamic_cast<ParticleUniverse::ParticleSystem *>(component->getPUElement());
				if (particleSystem) {
					particleSystem->start();
				}
			}
		}
	}

	void WidgetEdit::_generateNameForComponentAndPUElement(WidgetEditComponent * component, QString type) {
		if (!component) {
			return;
		}
		if (!component->getPUElement()) {
			return;
		}
		Ogre::String name;
		if (type == CT_TECHNIQUE) {
			name = "Technique" + Ogre::StringConverter::toString(_techniqueCounter++);
			static_cast<ParticleUniverse::ParticleTechnique *>(component->getPUElement())->setName(name);
		} else if (type == CT_EMITTER) {
			name = "Emitter" + Ogre::StringConverter::toString(_emitterCounter++);
			static_cast<ParticleUniverse::ParticleEmitter *>(component->getPUElement())->setName(name);
		} else if (type == CT_AFFECTOR) {
			name = "Affector" + Ogre::StringConverter::toString(_affectorCounter++);
			static_cast<ParticleUniverse::ParticleAffector *>(component->getPUElement())->setName(name);
		} else if (type == CT_OBSERVER) {
			name = "Observer" + Ogre::StringConverter::toString(_observerCounter++);
			static_cast<ParticleUniverse::ParticleObserver *>(component->getPUElement())->setName(name);
		}

		if (component->getPropertyWindow()) {
			component->getPropertyWindow()->setComponentName(QString::fromStdString(name));
		}
		component->setName(QString::fromStdString(name));
		component->setCaption();
	}
	
	WidgetEditComponent * WidgetEdit::getParticleSystemEditComponent() const {
		WidgetEditComponent * component = nullptr;
		for (std::vector<WidgetEditComponent *>::const_iterator it = _components.begin(); it != _components.end(); ++it) {
			component = *it;
			if (component->getComponentType() == CT_SYSTEM) {
				// There is only one!
				break;
			}
		}
		return component;
	}

	bool WidgetEdit::_processIncludeAdded(WidgetEditComponent * node1, WidgetEditComponent * node2) {
		// If element is not set, ignore removing it.
		ParticleUniverse::IElement * element1 = node1->getPUElement();
		if (!element1) {
			return false;
		}

		ParticleUniverse::IElement * element2 = node2->getPUElement();
		if (!element2) {
			return false;
		}

		if (node1->getComponentType() == CT_SYSTEM) {
			// Add the technique to the system
			ParticleUniverse::ParticleSystem * system = static_cast<ParticleUniverse::ParticleSystem *>(element1);
			ParticleUniverse::ParticleTechnique * technique = static_cast<ParticleUniverse::ParticleTechnique *>(element2);
			if (system) {
				system->addTechnique(technique);
			}
			return true;
		} else if (node1->getComponentType() == CT_TECHNIQUE) {
			ParticleUniverse::ParticleTechnique * technique = static_cast<ParticleUniverse::ParticleTechnique *>(element1);
			if (node2->getComponentType() == CT_RENDERER) {
				// Set the renderer in the technique
				ParticleUniverse::ParticleRenderer * renderer = static_cast<ParticleUniverse::ParticleRenderer *>(element2);
				if (technique) {
					technique->setRenderer(renderer);
				}
				return true;
			} else if (node2->getComponentType() == CT_EMITTER) {
				// Add the emitter to the technique
				ParticleUniverse::ParticleEmitter * emitter = static_cast<ParticleUniverse::ParticleEmitter *>(element2);
				if (technique) {
					technique->addEmitter(emitter);
					if (emitter->_isMarkedForEmission()) {
						technique->_unprepareEmitters(); // Destroy pool of emitted emitters
					}
				}

				return true;
			} else if (node2->getComponentType() == CT_AFFECTOR) {
				// Add the affector to the technique
				ParticleUniverse::ParticleAffector * affector = static_cast<ParticleUniverse::ParticleAffector *>(element2);
				if (technique) {
					technique->addAffector(affector);
					if (affector->_isMarkedForEmission()) {
						technique->_unprepareAffectors(); // Destroy pool of emitted affectors
					}
				}
				return true;
			} else if (node2->getComponentType() == CT_OBSERVER) {
				// Add the observer to the technique
				ParticleUniverse::ParticleObserver * observer = static_cast<ParticleUniverse::ParticleObserver *>(element2);
				if (technique) {
					technique->addObserver(observer);
				}
				return true;
			} else if (node2->getComponentType() == CT_BEHAVIOUR) {
				// Add the behaviour to the technique
				ParticleUniverse::ParticleBehaviour * behaviour = static_cast<ParticleUniverse::ParticleBehaviour *>(element2);
				if (technique) {
					technique->_addBehaviourTemplate(behaviour);
				}
				return true;
			} else if (node2->getComponentType() == CT_EXTERN) {
				// Add the extern to the technique
				ParticleUniverse::Extern * externObject = static_cast<ParticleUniverse::Extern *>(element2);
				if (technique) {
					technique->addExtern(externObject);
				}
				return true;
			}
		} else if (node1->getComponentType() == CT_OBSERVER) {
			ParticleUniverse::ParticleObserver * observer = static_cast<ParticleUniverse::ParticleObserver *>(element1);
			if (node2->getComponentType() == CT_HANDLER) {
				// Add the handler to the observer
				ParticleUniverse::ParticleEventHandler * handler = static_cast<ParticleUniverse::ParticleEventHandler *>(element2);
				if (observer) {
					observer->addEventHandler(handler);
				}
				return true;
			}
		}
		return false;
	}

	bool WidgetEdit::_processExcludeAdded(WidgetEditComponent * node1, WidgetEditComponent * node2) {
		ParticleUniverse::IElement * element1 = node1->getPUElement();
		if (!element1) {
			return false;
		}

		ParticleUniverse::IElement * element2 = node2->getPUElement();
		if (!element2) {
			return false;
		}

		if (node1->getComponentType() == CT_AFFECTOR) {
			if (node2->getComponentType() == CT_EMITTER) {
				// Add the emittername to the excluded names
				ParticleUniverse::ParticleAffector * affector = static_cast<ParticleUniverse::ParticleAffector *>(element1);
				ParticleUniverse::ParticleEmitter * emitter = static_cast<ParticleUniverse::ParticleEmitter *>(element2);
				if (emitter->getName() == Ogre::StringUtil::BLANK) {
					// The emitter doesn't have a name, so assign a name to it
					_generateNameForComponentAndPUElement(node2, CT_EMITTER);
				}
				affector->addEmitterToExclude(emitter->getName());
				return true;
			}
		}

		return false;
	}

	bool WidgetEdit::_processEmitAdded(WidgetEditComponent * node1, WidgetEditComponent * node2, ComponentRelationDirection relationDirection) {
		ParticleUniverse::IElement * element1 = node1->getPUElement();
		if (!element1) {
			return false;
		}

		ParticleUniverse::IElement * element2 = node2->getPUElement();
		if (!element2) {
			return false;
		}

		if (node1->getComponentType() == CT_EMITTER) {
			// Check what type of particle is emitted.
			ParticleUniverse::ParticleEmitter * emitter = static_cast<ParticleUniverse::ParticleEmitter *>(element1);

			// ComponentRelationDirection::CRDIR_SECUNDAIRY is the relation of node2 towards node1
			if (node2->getComponentType() == CT_EMITTER && relationDirection == ComponentRelationDirection::CRDIR_SECUNDAIRY) {
				ParticleUniverse::ParticleEmitter * emittedEmitter = static_cast<ParticleUniverse::ParticleEmitter *>(element2);
				emitter->setEmitsType(ParticleUniverse::Particle::PT_EMITTER);
				if (emittedEmitter->getName() == Ogre::StringUtil::BLANK) {
					// The emitter doesn't have a name, so assign a name to it
					_generateNameForComponentAndPUElement(node2, CT_EMITTER);
				}
				emitter->setEmitsName(emittedEmitter->getName());
				if (emitter->getParentTechnique()) {
					// Force creation of emitted emitters
					emitter->getParentTechnique()->_unprepareEmitters();
				}
				return true;
			} else if (node2->getComponentType() == CT_TECHNIQUE) {
				ParticleUniverse::ParticleTechnique * emittedTechnique = static_cast<ParticleUniverse::ParticleTechnique *>(element2);
				emitter->setEmitsType(ParticleUniverse::Particle::PT_TECHNIQUE);
				if (emittedTechnique->getName() == Ogre::StringUtil::BLANK) {
					// The emittedTechnique doesn't have a name, so assign a name to it
					_generateNameForComponentAndPUElement(node2, CT_TECHNIQUE);
				}
				emitter->setEmitsName(emittedTechnique->getName());
				if (emitter->getParentTechnique()) {
					// Force creation of emitted techniques
					emitter->getParentTechnique()->_unprepareTechnique();
				}
				return true;
			} else if (node2->getComponentType() == CT_AFFECTOR) {
				ParticleUniverse::ParticleAffector * emittedAffector = static_cast<ParticleUniverse::ParticleAffector *>(element2);
				emitter->setEmitsType(ParticleUniverse::Particle::PT_AFFECTOR);
				if (emittedAffector->getName() == Ogre::StringUtil::BLANK) {
					// The emittedAffector doesn't have a name, so assign a name to it
					_generateNameForComponentAndPUElement(node2, CT_AFFECTOR);
				}
				emitter->setEmitsName(emittedAffector->getName());
				if (emitter->getParentTechnique()) {
					// Force creation of emitted affectors
					emitter->getParentTechnique()->_unprepareAffectors();
				}
				return true;
			} else if (node2->getComponentType() == CT_SYSTEM) {
				ParticleUniverse::ParticleSystem * emittedSystem = static_cast<ParticleUniverse::ParticleSystem *>(element2);
				emitter->setEmitsType(ParticleUniverse::Particle::PT_SYSTEM);
				emitter->setEmitsName(emittedSystem->getName());
				if (emitter->getParentTechnique()) {
					// Force creation of emitted systems
					emitter->getParentTechnique()->_unprepareSystem();
				}
				return true;
			}
		}

		return false;
	}

	bool WidgetEdit::_processInterfaceAdded(WidgetEditComponent * node1, WidgetEditComponent * node2) {
		ParticleUniverse::IElement * element1 = node1->getPUElement();
		if (!element1) {
			return false;
		}

		ParticleUniverse::IElement * element2 = node2->getPUElement();
		if (!element2) {
			return false;
		}

		if (node1->getComponentType() == CT_OBSERVER) {
			if (node2->getComponentType() == CT_HANDLER) {
				// Add the handler to the observer
				ParticleUniverse::ParticleObserver * observer = static_cast<ParticleUniverse::ParticleObserver *>(element1);
				ParticleUniverse::ParticleEventHandler * handler = static_cast<ParticleUniverse::ParticleEventHandler *>(element2);
				observer->addEventHandler(handler);
				return true;
			}
		}

		return false;
	}

	bool WidgetEdit::_processSlaveAdded(WidgetEditComponent * node1, WidgetEditComponent * node2) {
		ParticleUniverse::IElement * element1 = node1->getPUElement();
		if (!element1) {
			return false;
		}

		ParticleUniverse::IElement * element2 = node2->getPUElement();
		if (!element2) {
			return false;
		}

		if (node1->getComponentType() == CT_EMITTER && node1->getComponentSubType() == CST_EMITTER_SLAVE) {
			ParticleUniverse::SlaveEmitter * emitter1 = static_cast<ParticleUniverse::SlaveEmitter *>(element1);
			if (node2->getComponentType() == CT_EMITTER) {
				ParticleUniverse::ParticleEmitter * emitter2 = static_cast<ParticleUniverse::ParticleEmitter *>(element2);
				if (emitter1->getParentTechnique()) {
					emitter1->_unprepare(emitter1->getParentTechnique());
				}
				if (emitter2->getName() == Ogre::StringUtil::BLANK) {
					// The emitter2 doesn't have a name, so assign a name to it
					_generateNameForComponentAndPUElement(node2, CT_EMITTER);
				}
				emitter1->setMasterEmitterName(emitter2->getName());
				ParticleUniverse::ParticleTechnique * technique = emitter2->getParentTechnique();
				if (technique) {
					if (technique->getName() == Ogre::StringUtil::BLANK) {
						// The technique doesn't have a name, so assign a name to it
						Ogre::String name = "Technique" + Ogre::StringConverter::toString(_techniqueCounter++);
						technique->setName(name);

						// Todo: Update the technique component
					}
					emitter1->setMasterTechniqueName(technique->getName());
				}
				if (emitter1->getParentTechnique()) {
					emitter1->_prepare(emitter1->getParentTechnique());
				}
				return true;
			}
		}

		return false;
	}

	bool WidgetEdit::_processEnableAdded(WidgetEditComponent * node1, WidgetEditComponent * node2) {
		ParticleUniverse::IElement * element1 = node1->getPUElement();
		if (!element1) {
			return false;
		}

		ParticleUniverse::IElement * element2 = node2->getPUElement();
		if (!element2) {
			return false;
		}

		if (node1->getComponentType() == CT_HANDLER && node1->getComponentSubType() == CST_HANDLER_DO_ENABLE_COMPONENT) {
			ParticleUniverse::DoEnableComponentEventHandler * handler = static_cast<ParticleUniverse::DoEnableComponentEventHandler *>(element1);
			if (node2->getComponentType() == CT_TECHNIQUE) {
				ParticleUniverse::ParticleTechnique * technique = static_cast<ParticleUniverse::ParticleTechnique *>(element2);
				if (technique->getName() == Ogre::StringUtil::BLANK) {
					// The technique doesn't have a name, so assign a name to it
					_generateNameForComponentAndPUElement(node2, CT_TECHNIQUE);
				}
				handler->setComponentName(technique->getName());
				handler->setComponentType(ParticleUniverse::CT_TECHNIQUE);
				return true;
			} else if (node2->getComponentType() == CT_EMITTER) {
				ParticleUniverse::ParticleEmitter * emitter = static_cast<ParticleUniverse::ParticleEmitter *>(element2);
				if (emitter->getName() == Ogre::StringUtil::BLANK) {
					// The emitter doesn't have a name, so assign a name to it
					_generateNameForComponentAndPUElement(node2, CT_EMITTER);
				}
				handler->setComponentName(emitter->getName());
				handler->setComponentType(ParticleUniverse::CT_EMITTER);
				return true;
			} else if (node2->getComponentType() == CT_AFFECTOR) {
				ParticleUniverse::ParticleAffector * affector = static_cast<ParticleUniverse::ParticleAffector *>(element2);
				if (affector->getName() == Ogre::StringUtil::BLANK) {
					// The affector doesn't have a name, so assign a name to it
					_generateNameForComponentAndPUElement(node2, CT_AFFECTOR);
				}
				handler->setComponentName(affector->getName());
				handler->setComponentType(ParticleUniverse::CT_AFFECTOR);
				return true;
			} else if (node2->getComponentType() == CT_OBSERVER) {
				ParticleUniverse::ParticleObserver * observer = static_cast<ParticleUniverse::ParticleObserver *>(element2);
				if (observer->getName() == Ogre::StringUtil::BLANK) {
					// The observer doesn't have a name, so assign a name to it
					_generateNameForComponentAndPUElement(node2, CT_OBSERVER);
				}
				handler->setComponentName(observer->getName());
				handler->setComponentType(ParticleUniverse::CT_OBSERVER);
				return true;
			}
		}

		return false;
	}

	bool WidgetEdit::_processForceAdded(WidgetEditComponent * node1, WidgetEditComponent * node2) {
		ParticleUniverse::IElement * element1 = node1->getPUElement();
		if (!element1) {
			return false;
		}

		ParticleUniverse::IElement * element2 = node2->getPUElement();
		if (!element2) {
			return false;
		}

		if (node1->getComponentType() == CT_HANDLER && node1->getComponentSubType() == CST_HANDLER_DO_AFFECTOR) {
			if (node2->getComponentType() == CT_AFFECTOR) {
				ParticleUniverse::DoAffectorEventHandler * handler = static_cast<ParticleUniverse::DoAffectorEventHandler *>(element1);
				ParticleUniverse::ParticleAffector * affector = static_cast<ParticleUniverse::ParticleAffector *>(element2);
				if (affector->getName() == Ogre::StringUtil::BLANK) {
					// The affector doesn't have a name, so assign a name to it
					_generateNameForComponentAndPUElement(node2, CT_AFFECTOR);
				}
				handler->setAffectorName(affector->getName());
				return true;
			}
		}

		return false;
	}

	bool WidgetEdit::_processPlaceAdded(WidgetEditComponent * node1, WidgetEditComponent * node2) {
		ParticleUniverse::IElement * element1 = node1->getPUElement();
		if (!element1) {
			return false;
		}

		ParticleUniverse::IElement * element2 = node2->getPUElement();
		if (!element2) {
			return false;
		}

		if (node1->getComponentType() == CT_HANDLER && node1->getComponentSubType() == CST_HANDLER_DO_PLACEMENT_PARTICLE) {
			if (node2->getComponentType() == CT_EMITTER) {
				ParticleUniverse::DoPlacementParticleEventHandler * handler = static_cast<ParticleUniverse::DoPlacementParticleEventHandler *>(element1);
				ParticleUniverse::ParticleEmitter * emitter = static_cast<ParticleUniverse::ParticleEmitter *>(element2);
				handler->removeAsListener();
				if (emitter->getName() == Ogre::StringUtil::BLANK) {
					// The emitter doesn't have a name, so assign a name to it
					_generateNameForComponentAndPUElement(node2, CT_EMITTER);
				}
				handler->setForceEmitterName(emitter->getName());
				return true;
			}
		}

		return false;
	}

	bool WidgetEdit::_processIncludeRemoved(WidgetEditComponent * node1, WidgetEditComponent * node2) {
		// If element is not set, ignore removing it.
		ParticleUniverse::IElement * element2 = node2->getPUElement();
		if (!element2) {
			return false;
		}

		if (node1->getComponentType() == CT_SYSTEM) {
			// Remove the technique from the system
			ParticleUniverse::ParticleTechnique * technique = static_cast<ParticleUniverse::ParticleTechnique *>(element2);
			if (technique->getParentSystem()) {
				technique->getParentSystem()->removeTechnique(technique);
			}
			return true;
		} else if (node1->getComponentType() == CT_TECHNIQUE) {
			if (node2->getComponentType() == CT_RENDERER) {
				// Remove renderer from the technique
				ParticleUniverse::ParticleRenderer * renderer = static_cast<ParticleUniverse::ParticleRenderer *>(element2);
				if (renderer->getParentTechnique()) {
					renderer->getParentTechnique()->removeRenderer(renderer);
				}
				return true;
			} else if (node2->getComponentType() == CT_EMITTER) {
				// Remove the emitter from the technique
				ParticleUniverse::ParticleEmitter * emitter = static_cast<ParticleUniverse::ParticleEmitter *>(element2);
				if (emitter->getParentTechnique()) {
					if (emitter->_isMarkedForEmission()) {
						emitter->getParentTechnique()->_unprepareEmitters(); // Destroy pool of emitted emitters
					}
					emitter->getParentTechnique()->removeEmitter(emitter);
				}
				return true;
			} else if (node2->getComponentType() == CT_AFFECTOR) {
				// Remove the affector from the technique
				ParticleUniverse::ParticleAffector * affector = static_cast<ParticleUniverse::ParticleAffector *>(element2);
				if (affector->getParentTechnique()) {
					if (affector->_isMarkedForEmission()) {
						affector->getParentTechnique()->_unprepareAffectors(); // Destroy pool of emitted affectors
					}
					affector->getParentTechnique()->removeAffector(affector);
				}
				return true;
			} else if (node2->getComponentType() == CT_OBSERVER) {
				// Remove the observer from the technique
				ParticleUniverse::ParticleObserver * observer = static_cast<ParticleUniverse::ParticleObserver *>(element2);
				if (observer->getParentTechnique()) {
					observer->getParentTechnique()->removeObserver(observer);
				}
				return true;
			} else if (node2->getComponentType() == CT_BEHAVIOUR) {
				// Remove the behaviour from the technique
				ParticleUniverse::ParticleBehaviour * behaviour = static_cast<ParticleUniverse::ParticleBehaviour *>(element2);
				if (behaviour->getParentTechnique()) {
					behaviour->getParentTechnique()->_removeBehaviourTemplate(behaviour);
				}
				return true;
			} else if (node2->getComponentType() == CT_EXTERN) {
				// Remove the extern from the technique
				ParticleUniverse::Extern * externObject = static_cast<ParticleUniverse::Extern *>(element2);
				if (externObject->getParentTechnique()) {
					externObject->getParentTechnique()->removeExtern(externObject);
				}
				return true;
			}
		} else if (node1->getComponentType() == CT_OBSERVER) {
			if (node2->getComponentType() == CT_HANDLER) {
				// Remove the handler from the observer
				ParticleUniverse::ParticleEventHandler * handler = static_cast<ParticleUniverse::ParticleEventHandler *>(element2);
				if (handler->getParentObserver()) {
					handler->getParentObserver()->removeEventHandler(handler);
				}
				return true;
			}
		}

		return false;
	}

	bool WidgetEdit::_processExcludeRemoved(WidgetEditComponent * node1, WidgetEditComponent * node2) {
		ParticleUniverse::IElement * element1 = node1->getPUElement();
		if (!element1) {
			return false;
		}

		ParticleUniverse::IElement * element2 = node2->getPUElement();
		if (!element2) {
			return false;
		}

		if (node1->getComponentType() == CT_AFFECTOR) {
			if (node2->getComponentType() == CT_EMITTER) {
				// Remove the emittername from the excluded names
				ParticleUniverse::ParticleAffector * affector = static_cast<ParticleUniverse::ParticleAffector *>(element1);
				ParticleUniverse::ParticleEmitter * emitter = static_cast<ParticleUniverse::ParticleEmitter *>(element2);
				affector->removeEmitterToExclude(emitter->getName());
				return true;
			}
		}

		return false;
	}

	bool WidgetEdit::_processEmitRemoved(WidgetEditComponent * node1, WidgetEditComponent * node2) {
		ParticleUniverse::IElement * element1 = node1->getPUElement();
		if (!element1) {
			return false;
		}

		ParticleUniverse::IElement * element2 = node2->getPUElement();
		if (node1->getComponentType() == CT_EMITTER) {
			// Check what type of particle is emitted.
			ParticleUniverse::ParticleEmitter * emitter = static_cast<ParticleUniverse::ParticleEmitter *>(element1);
			ParticleUniverse::ParticleTechnique * technique = nullptr;
			if (element2) {
				// Use the technique from the emitted component; this is preferred, because the technique from the emitter might be 0
				// (if the emitter component was closed, the ComponentRelation::CR_INCLUDE connection is deleted first and removes the emitter from its
				// parent technique).
				ParticleUniverse::ParticleEmitter * emittedEmitter = static_cast<ParticleUniverse::ParticleEmitter *>(element2);
				technique = emittedEmitter->getParentTechnique();
			}
			if (!technique) {
				// Use this one instead.
				technique = emitter->getParentTechnique();
			}

			switch (emitter->getEmitsType()) {
			case ParticleUniverse::Particle::PT_EMITTER: {
				if (technique) {
					technique->_unprepareEmitters();
					technique->_markForEmission(emitter, false); // Unmarks the emitted component
				}
				emitter->setEmitsType(ParticleUniverse::ParticleEmitter::DEFAULT_EMITS);
				emitter->setEmitsName(Ogre::StringUtil::BLANK);
				return true;
				break;
			}
			case ParticleUniverse::Particle::PT_TECHNIQUE: {
				technique = emitter->getParentTechnique();
				if (technique) {
					technique->_unprepareTechnique();
					technique->_markForEmission(emitter, false); // Unmarks the emitted component
				}
				emitter->setEmitsType(ParticleUniverse::ParticleEmitter::DEFAULT_EMITS);
				emitter->setEmitsName(Ogre::StringUtil::BLANK);
				return true;
				break;
			}
			case ParticleUniverse::Particle::PT_AFFECTOR: {
				if (technique) {
					technique->_unprepareAffectors();
					technique->_markForEmission(emitter, false); // Unmarks the emitted component
				}
				emitter->setEmitsType(ParticleUniverse::ParticleEmitter::DEFAULT_EMITS);
				emitter->setEmitsName(Ogre::StringUtil::BLANK);
				return true;
				break;
			}
			case ParticleUniverse::Particle::PT_SYSTEM: {
				if (technique) {
					technique->_unprepareSystem();
					technique->_markForEmission(emitter, false); // Unmarks the emitted component
				}
				emitter->setEmitsType(ParticleUniverse::ParticleEmitter::DEFAULT_EMITS);
				emitter->setEmitsName(Ogre::StringUtil::BLANK);
				return true;
				break;
			}
			default: {
				break;
			}
			}
		}
		return false;
	}

	bool WidgetEdit::_processInterfaceRemoved(WidgetEditComponent * node1, WidgetEditComponent * node2) {
		ParticleUniverse::IElement * element1 = node1->getPUElement();
		if (!element1) {
			return false;
		}

		ParticleUniverse::IElement * element2 = node2->getPUElement();
		if (!element2) {
			return false;
		}

		if (node1->getComponentType() == CT_OBSERVER) {
			if (node2->getComponentType() == CT_HANDLER) {
				// Remove the handler from the observer
				ParticleUniverse::ParticleObserver * observer = static_cast<ParticleUniverse::ParticleObserver *>(element1);
				ParticleUniverse::ParticleEventHandler * handler = static_cast<ParticleUniverse::ParticleEventHandler *>(element2);
				observer->removeEventHandler(handler);
				return true;
			}
		}

		return false;
	}

	bool WidgetEdit::_processSlaveRemoved(WidgetEditComponent * node1, WidgetEditComponent * node2) {
		ParticleUniverse::IElement * element1 = node1->getPUElement();
		if (!element1) {
			return false;
		}

		if (node1->getComponentType() == CT_EMITTER && node1->getComponentSubType() == CST_EMITTER_SLAVE) {
			ParticleUniverse::SlaveEmitter * emitter = static_cast<ParticleUniverse::SlaveEmitter *>(element1);
			if (emitter->getParentTechnique()) {
				emitter->_unprepare(emitter->getParentTechnique());
			}
			emitter->setMasterTechniqueName(Ogre::StringUtil::BLANK);
			emitter->setMasterEmitterName(Ogre::StringUtil::BLANK);
			if (emitter->getParentTechnique()) {
				emitter->_prepare(emitter->getParentTechnique());
			}
			return true;
		}

		return false;
	}

	bool WidgetEdit::_processEnableRemoved(WidgetEditComponent * node1, WidgetEditComponent * node2) {
		ParticleUniverse::IElement * element1 = node1->getPUElement();
		if (!element1) {
			return false;
		}

		if (node1->getComponentType() == CT_HANDLER && node1->getComponentSubType() == CST_HANDLER_DO_ENABLE_COMPONENT) {
			ParticleUniverse::DoEnableComponentEventHandler * handler = static_cast<ParticleUniverse::DoEnableComponentEventHandler *>(element1);
			handler->setComponentEnabled(true);
			handler->setComponentName(Ogre::StringUtil::BLANK);
			handler->setComponentType(ParticleUniverse::CT_EMITTER);
			return true;
		}

		return false;
	}

	bool WidgetEdit::_processForceRemoved(WidgetEditComponent * node1, WidgetEditComponent * node2) {
		ParticleUniverse::IElement * element1 = node1->getPUElement();
		if (!element1) {
			return false;
		}

		if (node1->getComponentType() == CT_HANDLER && node1->getComponentSubType() == CST_HANDLER_DO_AFFECTOR) {
			ParticleUniverse::DoAffectorEventHandler * handler = static_cast<ParticleUniverse::DoAffectorEventHandler *>(element1);
			handler->setAffectorName(Ogre::StringUtil::BLANK);
			handler->setPrePost(ParticleUniverse::DoAffectorEventHandler::DEFAULT_PRE_POST);
			return true;
		}

		return false;
	}

	bool WidgetEdit::_processPlaceRemoved(WidgetEditComponent * node1, WidgetEditComponent * node2) {
		ParticleUniverse::IElement * element1 = node1->getPUElement();
		if (!element1) {
			return false;
		}

		if (node1->getComponentType() == CT_HANDLER && node1->getComponentSubType() == CST_HANDLER_DO_PLACEMENT_PARTICLE) {
			ParticleUniverse::DoPlacementParticleEventHandler * handler = static_cast<ParticleUniverse::DoPlacementParticleEventHandler *>(element1);
			handler->removeAsListener();
			handler->setForceEmitterName(Ogre::StringUtil::BLANK);
			return true;
		}

		return false;
	}

	void WidgetEdit::mousePressEvent(QMouseEvent * evt) {
		if (evt->button() == Qt::MouseButton::RightButton) {
			resetConnectionMode();
		}
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6e */
