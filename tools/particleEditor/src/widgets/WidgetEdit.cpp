#include "widgets/WidgetEdit.h"

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

#include <QGraphicsScene>
#include <QGraphicsView>

namespace i6engine {
namespace particleEditor {
namespace widgets {

	WidgetEdit::WidgetEdit(QWidget * par, QWidget * renderWidget) : QWidget(par), _graphicsScene(new QGraphicsScene(this)), _graphicsView(new QGraphicsView(_graphicsScene)), _components(), _offsetX(48), _offsetY(8), _techniqueCounter(1), _rendererCounter(1), _emitterCounter(1), _affectorCounter(1), _observerCounter(1), _handlerCounter(1), _behaviourCounter(1), _externCounter(1) {
		setupUi(this);

		verticalLayout->addWidget(_graphicsView);
		verticalLayout->addWidget(renderWidget);

		verticalLayout->setStretch(0, 1);
		verticalLayout->setStretch(1, 1);

		_graphicsView->show();
	}

	WidgetEdit::~WidgetEdit() {
	}

	void WidgetEdit::setNewParticleSystem(ParticleUniverse::ParticleSystem * newParticleSystem) {
		for (WidgetEditComponent * wec : _components) {
			_graphicsScene->removeItem(wec);
			delete wec;
		}
		_components.clear();
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

	void WidgetEdit::createTechniqueForComponent(WidgetEditComponent * component) {
		ParticleUniverse::ParticleSystemManager * particleSystemManager = ParticleUniverse::ParticleSystemManager::getSingletonPtr();
		ParticleUniverse::ParticleTechnique * newTechnique = particleSystemManager->createTechnique();
		//newTechnique->setName(component->getPropertyWindow()->getComponentName());
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
		//newEmitter->setName(component->getPropertyWindow()->getComponentName());
		component->setPUElement(newEmitter);
	}

	void WidgetEdit::createAffectorForComponent(const QString & type, WidgetEditComponent * component) {
		ParticleUniverse::ParticleSystemManager * particleSystemManager = ParticleUniverse::ParticleSystemManager::getSingletonPtr();
		ParticleUniverse::ParticleAffector * newAffector = particleSystemManager->createAffector(type.toStdString());
		//newAffector->setName(component->getPropertyWindow()->getComponentName());
		component->setPUElement(newAffector);
	}

	void WidgetEdit::createObserverForComponent(const QString & type, WidgetEditComponent * component) {
		ParticleUniverse::ParticleSystemManager * particleSystemManager = ParticleUniverse::ParticleSystemManager::getSingletonPtr();
		ParticleUniverse::ParticleObserver * newObserver = particleSystemManager->createObserver(type.toStdString());
		//newObserver->setName(component->getPropertyWindow()->getComponentName());
		component->setPUElement(newObserver);
	}

	void WidgetEdit::createHandlerForComponent(const QString & type, WidgetEditComponent * component) {
		ParticleUniverse::ParticleSystemManager * particleSystemManager = ParticleUniverse::ParticleSystemManager::getSingletonPtr();
		ParticleUniverse::ParticleEventHandler * newHandler = particleSystemManager->createEventHandler(type.toStdString());
		//newHandler->setName(component->getPropertyWindow()->getComponentName());
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
		//newExtern->setName(component->getPropertyWindow()->getComponentName());
		component->setPUElement(newExtern);
	}

	WidgetEditComponent * WidgetEdit::forceCreateParticleSystemEditComponent() {
		WidgetEditComponent * particleSystemEditComponent = nullptr;
		for (WidgetEditComponent * wec : _components) {
			if (wec->getType() == WidgetEditComponent::EC_SYSTEM) {
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
		WidgetEditComponent * systemComponent = new WidgetEditComponent(_graphicsScene, "mySystem", WidgetEditComponent::EC_SYSTEM, WidgetEditComponent::CST_UNDEFINED);
		
		//systemComponent->createPropertyWindow(CST_UNDEFINED); // Recreate it, so it contains the root frame

		// Altough it is possible that a particle system itself is emitted, the connection to the emitter is not defined (there can only be one system on the canvas)
		//systemComponent->addPolicy(CR_INCLUDE, CRDIR_PRIMARY, CRD_INCLUDES, CT_TECHNIQUE, CST_UNDEFINED);

		_graphicsScene->addItem(systemComponent);
		_components.push_back(systemComponent);
		return systemComponent;
	}

	WidgetEditComponent * WidgetEdit::createTechniqueEditComponent() {
		QString name = "Technique" + QString::number(_techniqueCounter++);
		WidgetEditComponent * technique = new WidgetEditComponent(_graphicsScene, name, WidgetEditComponent::EC_TECHNIQUE, WidgetEditComponent::CST_UNDEFINED);
		/*technique->addPolicy(CR_INCLUDE, CRDIR_SECUNDAIRY, CRD_INCLUDED_BY, CT_SYSTEM, CST_UNDEFINED, false);
		technique->addPolicy(CR_INCLUDE, CRDIR_PRIMARY, CRD_INCLUDES, CT_RENDERER, CST_UNDEFINED, false);
		technique->addPolicy(CR_INCLUDE, CRDIR_PRIMARY, CRD_INCLUDES, CT_EMITTER, CST_UNDEFINED);
		technique->addPolicy(CR_INCLUDE, CRDIR_PRIMARY, CRD_INCLUDES, CT_AFFECTOR, CST_UNDEFINED);
		technique->addPolicy(CR_INCLUDE, CRDIR_PRIMARY, CRD_INCLUDES, CT_OBSERVER, CST_UNDEFINED);
		technique->addPolicy(CR_INCLUDE, CRDIR_PRIMARY, CRD_INCLUDES, CT_BEHAVIOUR, CST_UNDEFINED);
		technique->addPolicy(CR_INCLUDE, CRDIR_PRIMARY, CRD_INCLUDES, CT_EXTERN, CST_UNDEFINED);
		technique->addPolicy(CR_EMIT, CRDIR_SECUNDAIRY, CRD_EMITTED_BY, CT_EMITTER, CST_UNDEFINED, true, true, DRAW_EMITTED_COLOURCODE);
		technique->addPolicy(CR_ENABLE, CRDIR_SECUNDAIRY, CRD_ENABLED_BY, CT_HANDLER, CST_HANDLER_DO_ENABLE_COMPONENT, true, false, DRAW_SPECIAL_CASE_COLOURCODE);*/
		_graphicsScene->addItem(technique);
		_components.push_back(technique);
		return technique;
	}

	WidgetEditComponent * WidgetEdit::createRendererEditComponent(const QString & type) {
		QString name = "Renderer" + QString::number(_rendererCounter++);
		WidgetEditComponent * rendererComponent = new WidgetEditComponent(_graphicsScene, name, WidgetEditComponent::EC_RENDERER, type);
		//rendererComponent->addPolicy(CR_INCLUDE, CRDIR_SECUNDAIRY, CRD_INCLUDED_BY, CT_TECHNIQUE, CST_UNDEFINED, false);
		_graphicsScene->addItem(rendererComponent);
		_components.push_back(rendererComponent);
		return rendererComponent;
	}

	WidgetEditComponent * WidgetEdit::createEmitterEditComponent(const QString & type) {
		QString name = "Emitter" + QString::number(_emitterCounter++);
		WidgetEditComponent * emitterComponent = new WidgetEditComponent(_graphicsScene, name, WidgetEditComponent::EC_EMITTER, type);

		// Altough it is possible to emit a particle system, the connection to the system is not defined (there can only be one system on the canvas)
		/*emitterComponent->addUniqueRelation(CR_EMIT, CRDIR_PRIMARY); // Only emission of one type is allowed
		emitterComponent->addPolicy(CR_INCLUDE, CRDIR_SECUNDAIRY, CRD_INCLUDED_BY, CT_TECHNIQUE, CST_UNDEFINED, false);
		emitterComponent->addPolicy(CR_EMIT, CRDIR_PRIMARY, CRD_EMITS, CT_TECHNIQUE, CST_UNDEFINED, false, true, DRAW_EMITTED_COLOURCODE);
		emitterComponent->addPolicy(CR_EMIT, CRDIR_PRIMARY, CRD_EMITS, CT_EMITTER, CST_UNDEFINED, false, true, DRAW_EMITTED_COLOURCODE);
		emitterComponent->addPolicy(CR_EMIT, CRDIR_PRIMARY, CRD_EMITS, CT_AFFECTOR, CST_UNDEFINED, false, true, DRAW_EMITTED_COLOURCODE);
		emitterComponent->addPolicy(CR_EMIT, CRDIR_SECUNDAIRY, CRD_EMITTED_BY, CT_EMITTER, CST_UNDEFINED, true, true, DRAW_EMITTED_COLOURCODE);
		emitterComponent->addPolicy(CR_SLAVE, CRDIR_SECUNDAIRY, CRD_SLAVES, CT_EMITTER, CST_EMITTER_SLAVE, true, false, DRAW_SPECIAL_CASE_COLOURCODE);
		emitterComponent->addPolicy(CR_SLAVE, CRDIR_PRIMARY, CRD_SLAVE_OF, CT_EMITTER, CST_UNDEFINED, false, true, DRAW_SPECIAL_CASE_COLOURCODE);
		emitterComponent->addPolicy(CR_EXCLUDE, CRDIR_SECUNDAIRY, CRD_EXCLUDED_BY, CT_AFFECTOR, CST_UNDEFINED, true, true, DRAW_DEFAULT_COLOURCODE, wxSHORT_DASH);
		emitterComponent->addPolicy(CR_ENABLE, CRDIR_SECUNDAIRY, CRD_ENABLED_BY, CT_HANDLER, CST_HANDLER_DO_ENABLE_COMPONENT, true, false, DRAW_SPECIAL_CASE_COLOURCODE);
		emitterComponent->addPolicy(CR_PLACE, CRDIR_SECUNDAIRY, CRD_PLACED_BY, CT_HANDLER, CST_HANDLER_DO_PLACEMENT_PARTICLE, true, false, DRAW_DEFAULT_COLOURCODE, wxDOT);*/
		_graphicsScene->addItem(emitterComponent);
		_components.push_back(emitterComponent);
		return emitterComponent;
	}

	WidgetEditComponent * WidgetEdit::createAffectorEditComponent(const QString & type) {
		QString name = "Affector" + QString::number(_affectorCounter++);
		WidgetEditComponent * affectorComponent = new WidgetEditComponent(_graphicsScene, name, WidgetEditComponent::EC_AFFECTOR, type);
		/*affectorComponent->addPolicy(CR_INCLUDE, CRDIR_SECUNDAIRY, CRD_INCLUDED_BY, CT_TECHNIQUE, CST_UNDEFINED, false);
		affectorComponent->addPolicy(CR_EMIT, CRDIR_SECUNDAIRY, CRD_EMITTED_BY, CT_EMITTER, CST_UNDEFINED, true, true, DRAW_EMITTED_COLOURCODE);
		affectorComponent->addPolicy(CR_EXCLUDE, CRDIR_PRIMARY, CRD_EXCLUDES, CT_EMITTER, CST_UNDEFINED, true, true, DRAW_DEFAULT_COLOURCODE, wxSHORT_DASH);
		affectorComponent->addPolicy(CR_ENABLE, CRDIR_SECUNDAIRY, CRD_ENABLED_BY, CT_HANDLER, CST_HANDLER_DO_ENABLE_COMPONENT, true, false, DRAW_SPECIAL_CASE_COLOURCODE);
		affectorComponent->addPolicy(CR_FORCE, CRDIR_SECUNDAIRY, CRD_FORCED_BY, CT_HANDLER, CST_HANDLER_DO_AFFECTOR, true, false, DRAW_DEFAULT_COLOURCODE, wxDOT);*/
		_graphicsScene->addItem(affectorComponent);
		_components.push_back(affectorComponent);
		return affectorComponent;
	}

	WidgetEditComponent * WidgetEdit::createObserverEditComponent(const QString & type) {
		QString name = "Observer" + QString::number(_observerCounter++);
		WidgetEditComponent * observerComponent = new WidgetEditComponent(_graphicsScene, name, WidgetEditComponent::EC_OBSERVER, type);
		/*observerComponent->addPolicy(CR_INCLUDE, CRDIR_PRIMARY, CRD_INCLUDES, CT_HANDLER, CST_UNDEFINED, true);
		observerComponent->addPolicy(CR_INCLUDE, CRDIR_SECUNDAIRY, CRD_INCLUDED_BY, CT_TECHNIQUE, CST_UNDEFINED, false);
		observerComponent->addPolicy(CR_ENABLE, CRDIR_SECUNDAIRY, CRD_ENABLED_BY, CT_HANDLER, CST_HANDLER_DO_ENABLE_COMPONENT, true, false, DRAW_SPECIAL_CASE_COLOURCODE);*/
		_graphicsScene->addItem(observerComponent);
		_components.push_back(observerComponent);
		return observerComponent;
	}

	WidgetEditComponent * WidgetEdit::createHandlerEditComponent(const QString & type) {
		QString name = "Handler" + QString::number(_handlerCounter++);
		WidgetEditComponent * handler = new WidgetEditComponent(_graphicsScene, name, WidgetEditComponent::EC_HANDLER, type);
		/*handler->addUniqueRelation(CR_ENABLE, CRDIR_PRIMARY); // Only enabling of one type is allowed
		handler->addPolicy(CR_INCLUDE, CRDIR_SECUNDAIRY, CRD_INCLUDED_BY, CT_OBSERVER, CST_UNDEFINED, false);
		handler->addPolicy(CR_ENABLE, CRDIR_PRIMARY, CRD_ENABLES, CT_TECHNIQUE, CST_UNDEFINED, false, true, DRAW_SPECIAL_CASE_COLOURCODE);
		handler->addPolicy(CR_ENABLE, CRDIR_PRIMARY, CRD_ENABLES, CT_EMITTER, CST_UNDEFINED, false, true, DRAW_SPECIAL_CASE_COLOURCODE);
		handler->addPolicy(CR_ENABLE, CRDIR_PRIMARY, CRD_ENABLES, CT_AFFECTOR, CST_UNDEFINED, false, true, DRAW_SPECIAL_CASE_COLOURCODE);
		handler->addPolicy(CR_ENABLE, CRDIR_PRIMARY, CRD_ENABLES, CT_OBSERVER, CST_UNDEFINED, false, true, DRAW_SPECIAL_CASE_COLOURCODE);
		handler->addPolicy(CR_PLACE, CRDIR_PRIMARY, CRD_PLACES, CT_EMITTER, CST_UNDEFINED, false, true, DRAW_DEFAULT_COLOURCODE, wxDOT);
		handler->addPolicy(CR_FORCE, CRDIR_PRIMARY, CRD_FORCES, CT_AFFECTOR, CST_UNDEFINED, false, true, DRAW_DEFAULT_COLOURCODE, wxDOT);*/
		_graphicsScene->addItem(handler);
		_components.push_back(handler);
		return handler;
	}

	WidgetEditComponent * WidgetEdit::createBehaviourEditComponent(const QString & type) {
		QString name = "Behaviour" + QString::number(_behaviourCounter++);
		WidgetEditComponent * behaviourComponent = new WidgetEditComponent(_graphicsScene, name, WidgetEditComponent::EC_BEHAVIOUR, type);
		//behaviourComponent->addPolicy(CR_INCLUDE, CRDIR_SECUNDAIRY, CRD_INCLUDED_BY, CT_TECHNIQUE, CST_UNDEFINED, false);
		_graphicsScene->addItem(behaviourComponent);
		_components.push_back(behaviourComponent);
		return behaviourComponent;
	}

	WidgetEditComponent * WidgetEdit::createExternEditComponent(const QString & type) {
		QString name = "Extern" + QString::number(_externCounter++);
		WidgetEditComponent * externObjectComponent = new WidgetEditComponent(_graphicsScene, name, WidgetEditComponent::EC_EXTERN, type);
		//externObjectComponent->addPolicy(CR_INCLUDE, CRDIR_SECUNDAIRY, CRD_INCLUDED_BY, CT_TECHNIQUE, CST_UNDEFINED, false);
		_graphicsScene->addItem(externObjectComponent);
		_components.push_back(externObjectComponent);
		return externObjectComponent;
	}

	bool WidgetEdit::copyParticleSystemPropertiesToPropertyWindow(WidgetEditComponent * particleSystemEditComponent, ParticleUniverse::ParticleSystem * particleSystem) {
		if (!particleSystem || !particleSystemEditComponent) {
			return false;
		}

		/*SystemPropertyWindow * systemPropertyWindow = static_cast<SystemPropertyWindow *>(particleSystemEditComponent->getPropertyWindow());
		particleSystemEditComponent->setComponentName(particleSystem->getTemplateName());
		particleSystemEditComponent->setCaption();
		systemPropertyWindow->copyAttributesFromSystem(particleSystem);
		particleSystemEditComponent->SetFocus(); // Causes the property window to refresh
		*/
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
		/*for (size_t i = 0; i < numberTechniques; ++i) {
			technique = particleSystem->getTechnique(i);
			createOtherConnections(technique);
		}*/
		return true;
	}

	QPoint WidgetEdit::createComponentsFromTechnique(WidgetEditComponent * systemEditComponent, ParticleUniverse::ParticleTechnique * technique, QPoint position) {
		// First create the component of the technique itself
		WidgetEditComponent * techniqueEditComponent = createTechniqueEditComponent();
		/*techniqueEditComponent->setComponentName(technique->getName());
		techniqueEditComponent->setCaption();*/
		techniqueEditComponent->setPos(position);
		techniqueEditComponent->setPUElement(technique);
		int componentWidth = techniqueEditComponent->size().width();
		int componentHeight = techniqueEditComponent->size().height();
		//static_cast<TechniquePropertyWindow *>(techniqueEditComponent->getPropertyWindow())->copyAttributesFromTechnique(technique);
		//createConnection(systemEditComponent, techniqueEditComponent, CR_INCLUDE, CRDIR_PRIMARY);
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
		//rendererEditComponent->setComponentName(QStringUtil::BLANK);
		//rendererEditComponent->setCaption();
		rendererEditComponent->setPos(position);
		rendererEditComponent->setPUElement(renderer);
		//static_cast<RendererPropertyWindow *>(rendererEditComponent->getPropertyWindow())->copyAttributesFromRenderer(renderer);
		//createConnection(techniqueEditComponent, rendererEditComponent, CR_INCLUDE, CRDIR_PRIMARY);
	}

	void WidgetEdit::createComponentFromEmitter(WidgetEditComponent * techniqueEditComponent, ParticleUniverse::ParticleEmitter * emitter, QPoint position) {
		// First create the component of the emitter itself
		WidgetEditComponent * emitterEditComponent = createEmitterEditComponent(QString::fromStdString(emitter->getEmitterType()));
		//emitterEditComponent->setComponentName(emitter->getName());
		//emitterEditComponent->setCaption();
		emitterEditComponent->setPos(position);
		emitterEditComponent->setPUElement(emitter);
		//static_cast<EmitterPropertyWindow *>(emitterEditComponent->getPropertyWindow())->copyAttributesFromEmitter(emitter);
		//createConnection(techniqueEditComponent, emitterEditComponent, CR_INCLUDE, CRDIR_PRIMARY);
	}

	void WidgetEdit::createComponentFromAffector(WidgetEditComponent * techniqueEditComponent, ParticleUniverse::ParticleAffector * affector, QPoint position) {
		// First create the component of the affector itself
		WidgetEditComponent * affectorEditComponent = createAffectorEditComponent(QString::fromStdString(affector->getAffectorType()));
		//affectorEditComponent->setComponentName(affector->getName());
		//affectorEditComponent->setCaption();
		affectorEditComponent->setPos(position);
		affectorEditComponent->setPUElement(affector);
		//static_cast<AffectorPropertyWindow *>(affectorEditComponent->getPropertyWindow())->copyAttributesFromAffector(affector);
		//createConnection(techniqueEditComponent, affectorEditComponent, CR_INCLUDE, CRDIR_PRIMARY);
	}

	int WidgetEdit::createComponentFromObserver(WidgetEditComponent * techniqueEditComponent, ParticleUniverse::ParticleObserver * observer, QPoint position, int latestHandlerY) {
		// First create the component of the observer itself
		WidgetEditComponent * observerEditComponent = createObserverEditComponent(QString::fromStdString(observer->getObserverType()));
		//observerEditComponent->setComponentName(observer->getName());
		//observerEditComponent->setCaption();
		observerEditComponent->setPos(position);
		observerEditComponent->setPUElement(observer);
		//static_cast<ObserverPropertyWindow *>(observerEditComponent->getPropertyWindow())->copyAttributesFromObserver(observer);
		//createConnection(techniqueEditComponent, observerEditComponent, CR_INCLUDE, CRDIR_PRIMARY);

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
		//handlerEditComponent->setComponentName(eventHandler->getName());
		//handlerEditComponent->setCaption();
		handlerEditComponent->setPos(position);
		handlerEditComponent->setPUElement(eventHandler);
		//static_cast<EventHandlerPropertyWindow *>(handlerEditComponent->getPropertyWindow())->copyAttributesFromEventHandler(eventHandler);
		//createConnection(observerEditComponent, handlerEditComponent, CR_INCLUDE, CRDIR_PRIMARY);
	}

	void WidgetEdit::createComponentFromBehaviour(WidgetEditComponent * techniqueEditComponent, ParticleUniverse::ParticleBehaviour * behaviour, QPoint position) {
		// First create the component of the behaviour itself
		WidgetEditComponent * behaviourEditComponent = createBehaviourEditComponent(QString::fromStdString(behaviour->getBehaviourType()));
		//behaviourEditComponent->setComponentName(QStringUtil::BLANK);
		//behaviourEditComponent->setCaption();
		behaviourEditComponent->setPos(position);
		behaviourEditComponent->setPUElement(behaviour);
		//static_cast<BehaviourPropertyWindow *>(behaviourEditComponent->getPropertyWindow())->copyAttributesFromBehaviour(behaviour);
		//createConnection(techniqueEditComponent, behaviourEditComponent, CR_INCLUDE, CRDIR_PRIMARY);
	}

	void WidgetEdit::createComponentFromExtern(WidgetEditComponent * techniqueEditComponent, ParticleUniverse::Extern * externObject, QPoint position) {
		// First create the component of the extern itself
		WidgetEditComponent * externEditComponent = createExternEditComponent(QString::fromStdString(externObject->getExternType()));
		//externEditComponent->setComponentName(externObject->getName());
		//externEditComponent->setCaption();
		externEditComponent->setPos(position);
		externEditComponent->setPUElement(externObject);
		//static_cast<ExternPropertyWindow *>(externEditComponent->getPropertyWindow())->copyAttributesFromExtern(externObject);
		//createConnection(techniqueEditComponent, externEditComponent, CR_INCLUDE, CRDIR_PRIMARY);
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */
