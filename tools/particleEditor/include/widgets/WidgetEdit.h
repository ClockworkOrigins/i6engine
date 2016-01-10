#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_WIDGETEDIT_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_WIDGETEDIT_H__

#include "ui_widgetEdit.h"

class QGraphicsView;

namespace ParticleUniverse {
	class Extern;
	class IElement;
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
namespace connections {
	class LineConnector;
} /* namespace connections */
namespace widgets {

	class GraphicsScene;
	class PropertyWindow;
	class WidgetEditComponent;

	enum ComponentRelation;
	enum ComponentRelationDirection;

	class WidgetEdit : public QWidget, public Ui::editWidget {
		Q_OBJECT

	public:
		enum ConnectionMode {
			CM_CONNECT_NONE,
			CM_CONNECT_STARTING,
			CM_CONNECT_ENDING,
			CM_DISCONNECT
		};

		/**
		 * \brief constructor
		 */
		WidgetEdit(QWidget * par, QWidget * renderWidget);

		/**
		 * \brief destructor
		 */
		~WidgetEdit();

		void notifyComponentActivated(WidgetEditComponent * component);
		void notifyConnectionsChanged();
		void notifyConnectionAdded(WidgetEditComponent * node1, WidgetEditComponent * node2, ComponentRelation relation, ComponentRelationDirection relationDirection);
		void notifyConnectionRemoved(WidgetEditComponent * node1, WidgetEditComponent * node2, ComponentRelation relation, ComponentRelationDirection relationDirection);

		ConnectionMode getConnectionMode() const {
			return _connectionMode;
		}

		WidgetEditComponent * getStartConnector() const {
			return _startConnector;
		}

		void resetConnectionMode();

	signals:
		void setPropertyWindow(PropertyWindow *);
		void renameParticleSystem(QString, QString);

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
		void addConnection();
		void removeConnection();
		void triggerResetConnectionMode();
		void triggerRenameParticleSystem(QString oldName, QString newName);

	private:
		GraphicsScene * _graphicsScene;
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
		ConnectionMode _connectionMode;
		WidgetEditComponent * _startConnector;
		WidgetEditComponent * _endConnector;
		std::map<std::pair<WidgetEditComponent *, WidgetEditComponent *>, connections::LineConnector *> _connections;

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
		void createOtherConnections(const ParticleUniverse::ParticleTechnique * technique);
		void createConnection(WidgetEditComponent * componentPrimary, WidgetEditComponent * componentSecundary, ComponentRelation relation, ComponentRelationDirection direction);
		WidgetEditComponent * findEditComponent(const ParticleUniverse::IElement * puElement) const;
		WidgetEditComponent * findEditComponent(const QString & name, const QString & type, WidgetEditComponent * skip = nullptr) const;
		WidgetEditComponent * findEditComponentForTechnique(const QString & name, const QString & techniqueName) const;
		void setConnectionMode(ConnectionMode connectionMode);
		bool isConnectionPossible(WidgetEditComponent * component) const;
		bool _mustStopParticleSystem();
		void _mustRestartParticleSystem(bool wasStarted);
		void _generateNameForComponentAndPUElement(WidgetEditComponent * component, QString type);
		WidgetEditComponent * getParticleSystemEditComponent() const;
		bool _processIncludeAdded(WidgetEditComponent * node1, WidgetEditComponent * node2);
		bool _processExcludeAdded(WidgetEditComponent * node1, WidgetEditComponent * node2);
		bool _processEmitAdded(WidgetEditComponent * node1, WidgetEditComponent * node2, ComponentRelationDirection relationDirection);
		bool _processInterfaceAdded(WidgetEditComponent * node1, WidgetEditComponent * node2);
		bool _processSlaveAdded(WidgetEditComponent * node1, WidgetEditComponent * node2);
		bool _processEnableAdded(WidgetEditComponent * node1, WidgetEditComponent * node2);
		bool _processForceAdded(WidgetEditComponent * node1, WidgetEditComponent * node2);
		bool _processPlaceAdded(WidgetEditComponent * node1, WidgetEditComponent * node2);
		bool _processIncludeRemoved(WidgetEditComponent * node1, WidgetEditComponent * node2);
		bool _processExcludeRemoved(WidgetEditComponent * node1, WidgetEditComponent * node2);
		bool _processEmitRemoved(WidgetEditComponent * node1, WidgetEditComponent * node2);
		bool _processInterfaceRemoved(WidgetEditComponent * node1, WidgetEditComponent * node2);
		bool _processSlaveRemoved(WidgetEditComponent * node1, WidgetEditComponent * node2);
		bool _processEnableRemoved(WidgetEditComponent * node1, WidgetEditComponent * node2);
		bool _processForceRemoved(WidgetEditComponent * node1, WidgetEditComponent * node2);
		bool _processPlaceRemoved(WidgetEditComponent * node1, WidgetEditComponent * node2);

		void mousePressEvent(QMouseEvent * evt) override;
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_WIDGETEDIT_H__ */
