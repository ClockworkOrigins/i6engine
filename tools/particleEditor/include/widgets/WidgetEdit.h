#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_WIDGETEDIT_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_WIDGETEDIT_H__

#include "ui_widgetEdit.h"

class QGraphicsScene;
class QGraphicsView;

namespace ParticleUniverse {
	class Extern;
	class ParticleAffector;
	class ParticleBehaviour;
	class ParticleEmitter;
	class ParticleEventHandler;
	class ParticleObserver;
	class ParticleRenderer;
	class ParticleSystem;
	class ParticleTechnique;
} /* namespace ParticleUniverse */

namespace i6engine {
namespace particleEditor {
namespace widgets {

	class WidgetEditComponent;

	class WidgetEdit : public QWidget, public Ui::editWidget {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		WidgetEdit(QWidget * par, QWidget * renderWidget);

		/**
		 * \brief destructor
		 */
		~WidgetEdit();

	private slots:
		void setNewParticleSystem(ParticleUniverse::ParticleSystem * newParticleSystem);
		void addNewTechnique();
		void addNewRenderer();
		void addNewEmitter();
		void addNewAffector();
		void addNewObserver();
		void addNewHandler();
		void addNewBehaviour();
		void addNewExtern();

	private:
		QGraphicsScene * _graphicsScene;
		QGraphicsView * _graphicsView;
		std::vector<WidgetEditComponent *> _components;
		int _offsetX;
		int _offsetY;
		int _techniqueCounter;
		int _rendererCounter;
		int _emitterCounter;
		int _affectorCounter;
		int _observerCounter;
		int _handlerCounter;
		int _behaviourCounter;
		int _externCounter;

		void createTechniqueForComponent(WidgetEditComponent * component);
		void createRendererForComponent(const QString & type, WidgetEditComponent * component);
		void createEmitterForComponent(const QString & type, WidgetEditComponent * component);
		void createAffectorForComponent(const QString & type, WidgetEditComponent * component);
		void createObserverForComponent(const QString & type, WidgetEditComponent * component);
		void createHandlerForComponent(const QString & type, WidgetEditComponent * component);
		void createBehaviourForComponent(const QString & type, WidgetEditComponent * component); 
		void createExternForComponent(const QString & type, WidgetEditComponent * component);
		WidgetEditComponent * forceCreateParticleSystemEditComponent();
		WidgetEditComponent * createParticleSystemEditComponent();
		WidgetEditComponent * createTechniqueEditComponent();
		WidgetEditComponent * createRendererEditComponent(const QString & type);
		WidgetEditComponent * createEmitterEditComponent(const QString & type);
		WidgetEditComponent * createAffectorEditComponent(const QString & type);
		WidgetEditComponent * createObserverEditComponent(const QString & type);
		WidgetEditComponent * createHandlerEditComponent(const QString & type);
		WidgetEditComponent * createBehaviourEditComponent(const QString & type);
		WidgetEditComponent * createExternEditComponent(const QString & type);
		bool copyParticleSystemPropertiesToPropertyWindow(WidgetEditComponent * particleSystemEditComponent, ParticleUniverse::ParticleSystem * particleSystem);
		bool createParticleSystemComponents(WidgetEditComponent * particleSystemEditComponent, ParticleUniverse::ParticleSystem * particleSystem);
		QPoint createComponentsFromTechnique(WidgetEditComponent * systemEditComponent, ParticleUniverse::ParticleTechnique * technique, QPoint position);
		void createComponentFromRenderer(WidgetEditComponent * techniqueEditComponent, ParticleUniverse::ParticleRenderer * renderer, QPoint position);
		void createComponentFromEmitter(WidgetEditComponent * techniqueEditComponent, ParticleUniverse::ParticleEmitter * emitter, QPoint position);
		void createComponentFromAffector(WidgetEditComponent * techniqueEditComponent, ParticleUniverse::ParticleAffector * affector, QPoint position);
		int createComponentFromObserver(WidgetEditComponent * techniqueEditComponent, ParticleUniverse::ParticleObserver * observer, QPoint position, int latestHandlerY);
		void createComponentFromEventHandler(WidgetEditComponent * observerEditComponent, ParticleUniverse::ParticleEventHandler * eventHandler, QPoint position);
		void createComponentFromBehaviour(WidgetEditComponent * techniqueEditComponent, ParticleUniverse::ParticleBehaviour * behaviour, QPoint position);
		void createComponentFromExtern(WidgetEditComponent * techniqueEditComponent, ParticleUniverse::Extern * externObject, QPoint position);
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_WIDGETEDIT_H__ */
