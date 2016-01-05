#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_WIDGETEDITCOMPONENT_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_WIDGETEDITCOMPONENT_H__

#include <QGraphicsWidget>

namespace ParticleUniverse {
	class IElement;
} /* namespace IElement */

namespace i6engine {
namespace particleEditor {
namespace widgets {

	// 'Enums: Component SubType'

	// Renderers
	static QString CST_RENDERER_BEAM;
	static QString CST_RENDERER_BILLBOARD;
	static QString CST_RENDERER_SPHERE;
	static QString CST_RENDERER_BOX;
	static QString CST_RENDERER_ENTITY;
	static QString CST_RENDERER_LIGHT;
	static QString CST_RENDERER_RIBBONTRAIL;

	// Emitters
	static QString CST_EMITTER_BOX;
	static QString CST_EMITTER_CIRCLE;
	static QString CST_EMITTER_LINE;
	static QString CST_EMITTER_MESH_SURFACE;
	static QString CST_EMITTER_POINT;
	static QString CST_EMITTER_POSITION;
	static QString CST_EMITTER_SLAVE;
	static QString CST_EMITTER_SPHERE;
	static QString CST_EMITTER_VERTEX;

	// Affector
	static QString CST_AFFECTOR_ALIGN;
	static QString CST_AFFECTOR_BOX_COLLIDER;
	static QString CST_AFFECTOR_COLLISION_AVOIDANCE;
	static QString CST_AFFECTOR_COLOUR;
	static QString CST_AFFECTOR_FLOCK_CENTERING;
	static QString CST_AFFECTOR_FORCEFIELD;
	static QString CST_AFFECTOR_GEOMETRY_ROTATOR;
	static QString CST_AFFECTOR_GRAVITY;
	static QString CST_AFFECTOR_INTER_PARTICLE_COLLIDER;
	static QString CST_AFFECTOR_JET;
	static QString CST_AFFECTOR_LINE;
	static QString CST_AFFECTOR_LINEAR_FORCE;
	static QString CST_AFFECTOR_PARTICLE_FOLLOWER;
	static QString CST_AFFECTOR_PATH_FOLLOWER;
	static QString CST_AFFECTOR_PLANE_COLLIDER;
	static QString CST_AFFECTOR_RANDOMISER;
	static QString CST_AFFECTOR_SCALE;
	static QString CST_AFFECTOR_SCALE_VELOCITY;
	static QString CST_AFFECTOR_SINE_FORCE;
	static QString CST_AFFECTOR_SPHERE_COLLIDER;
	static QString CST_AFFECTOR_TEXTURE_ANIMATOR;
	static QString CST_AFFECTOR_TEXTURE_ROTATOR;
	static QString CST_AFFECTOR_VELOCITY_MATCHING;
	static QString CST_AFFECTOR_VORTEX;

	// Observers
	static QString CST_OBSERVER_ON_CLEAR;
	static QString CST_OBSERVER_ON_COLLISION;
	static QString CST_OBSERVER_ON_COUNT;
	static QString CST_OBSERVER_ON_EMISSION;
	static QString CST_OBSERVER_ON_EVENTFLAG;
	static QString CST_OBSERVER_ON_EXPIRE;
	static QString CST_OBSERVER_ON_POSITION;
	static QString CST_OBSERVER_ON_QUOTA;
	static QString CST_OBSERVER_ON_RANDOM;
	static QString CST_OBSERVER_ON_TIME;
	static QString CST_OBSERVER_ON_VELOCITY;

	// Event Handlers
	static QString CST_HANDLER_DO_AFFECTOR;
	static QString CST_HANDLER_DO_ENABLE_COMPONENT;
	static QString CST_HANDLER_DO_EXPIRE;
	static QString CST_HANDLER_DO_FREEZE;
	static QString CST_HANDLER_DO_PLACEMENT_PARTICLE;
	static QString CST_HANDLER_DO_SCALE;
	static QString CST_HANDLER_DO_STOP_SYSTEM;

	// Behaviour
	static QString CST_BEHAVIOUR_SLAVE;

	// Extern
	static QString CST_EXTERN_BOX_COLLIDER;
	static QString CST_EXTERN_SPHERE_COLLIDER;
	static QString CST_EXTERN_GRAVITY;
	static QString CST_EXTERN_PHYSX_ACTOR;
	static QString CST_EXTERN_PHYSX_FLUID;
	static QString CST_EXTERN_VORTEX;

	// Particle Universe Renderer names (must not be internationalized)
	static const QString RENDERER_BEAM = "Beam";
	static const QString RENDERER_BILLBOARD = "Billboard";
	static const QString RENDERER_SPHERE = "Sphere";
	static const QString RENDERER_BOX = "Box";
	static const QString RENDERER_ENTITY = "Entity";
	static const QString RENDERER_LIGHT = "Light";
	static const QString RENDERER_RIBBONTRAIL = "RibbonTrail";

	// Particle Universe Emitter names (must not be internationalized)
	static const QString EMITTER_BOX = "Box";
	static const QString EMITTER_CIRCLE = "Circle";
	static const QString EMITTER_LINE = "Line";
	static const QString EMITTER_MESH_SURFACE = "MeshSurface";
	static const QString EMITTER_POINT = "Point";
	static const QString EMITTER_POSITION = "Position";
	static const QString EMITTER_SLAVE = "Slave";
	static const QString EMITTER_SPHERE = "SphereSurface";
	static const QString EMITTER_VERTEX = "Vertex";

	// Particle Universe Affector names (must not be internationalized)
	static const QString AFFECTOR_ALIGN = "Align";
	static const QString AFFECTOR_BOX_COLLIDER = "BoxCollider";
	static const QString AFFECTOR_COLLISION_AVOIDANCE = "CollisionAvoidance";
	static const QString AFFECTOR_COLOUR = "Colour";
	static const QString AFFECTOR_FLOCK_CENTERING = "FlockCentering";
	static const QString AFFECTOR_FORCEFIELD = "ForceField";
	static const QString AFFECTOR_GEOMETRY_ROTATOR = "GeometryRotator";
	static const QString AFFECTOR_GRAVITY = "Gravity";
	static const QString AFFECTOR_INTER_PARTICLE_COLLIDER = "InterParticleCollider";
	static const QString AFFECTOR_JET = "Jet";
	static const QString AFFECTOR_LINE = "Line";
	static const QString AFFECTOR_LINEAR_FORCE = "LinearForce";
	static const QString AFFECTOR_PARTICLE_FOLLOWER = "ParticleFollower";
	static const QString AFFECTOR_PATH_FOLLOWER = "PathFollower";
	static const QString AFFECTOR_PLANE_COLLIDER = "PlaneCollider";
	static const QString AFFECTOR_RANDOMISER = "Randomiser";
	static const QString AFFECTOR_SCALE = "Scale";
	static const QString AFFECTOR_SCALE_VELOCITY = "ScaleVelocity";
	static const QString AFFECTOR_SINE_FORCE = "SineForce";
	static const QString AFFECTOR_SPHERE_COLLIDER = "SphereCollider";
	static const QString AFFECTOR_TEXTURE_ANIMATOR = "TextureAnimator";
	static const QString AFFECTOR_TEXTURE_ROTATOR = "TextureRotator";
	static const QString AFFECTOR_VELOCITY_MATCHING = "VelocityMatching";
	static const QString AFFECTOR_VORTEX = "Vortex";

	// Particle Universe Observer names (must not be internationalized)
	static const QString OBSERVER_ON_CLEAR = "OnClear";
	static const QString OBSERVER_ON_COLLISION = "OnCollision";
	static const QString OBSERVER_ON_COUNT = "OnCount";
	static const QString OBSERVER_ON_EMISSION = "OnEmission";
	static const QString OBSERVER_ON_EVENTFLAG = "OnEventFlag";
	static const QString OBSERVER_ON_EXPIRE = "OnExpire";
	static const QString OBSERVER_ON_POSITION = "OnPosition";
	static const QString OBSERVER_ON_QUOTA = "OnQuota";
	static const QString OBSERVER_ON_RANDOM = "OnRandom";
	static const QString OBSERVER_ON_TIME = "OnTime";
	static const QString OBSERVER_ON_VELOCITY = "OnVelocity";

	// Particle Universe Event Handler names (must not be internationalized)
	static const QString HANDLER_DO_AFFECTOR = "DoAffector";
	static const QString HANDLER_DO_ENABLE_COMPONENT = "DoEnableComponent";
	static const QString HANDLER_DO_EXPIRE = "DoExpire";
	static const QString HANDLER_DO_FREEZE = "DoFreeze";
	static const QString HANDLER_DO_PLACEMENT_PARTICLE = "DoPlacementParticle";
	static const QString HANDLER_DO_SCALE = "DoScale";
	static const QString HANDLER_DO_STOP_SYSTEM = "DoStopSystem";

	// Particle Universe Behaviour names (must not be internationalized)
	static const QString BEHAVIOUR_SLAVE = "Slave";

	// Particle Universe Extern names (must not be internationalized)
	static const QString EXTERN_BOX_COLLIDER = "BoxCollider";
	static const QString EXTERN_SPHERE_COLLIDER = "SphereCollider";
	static const QString EXTERN_GRAVITY = "Gravity";
	static const QString EXTERN_PHYSX_ACTOR = "PhysXActor";
	static const QString EXTERN_PHYSX_FLUID = "PhysXFluid";
	static const QString EXTERN_VORTEX = "Vortex";

	class WidgetEditComponent : public QGraphicsWidget {
		Q_OBJECT
		Q_INTERFACES(QGraphicsItem)

	public:
		static QString EC_SYSTEM;
		static QString EC_RENDERER;
		static QString EC_TECHNIQUE;
		static QString EC_EMITTER;
		static QString EC_AFFECTOR;
		static QString EC_OBSERVER;
		static QString EC_HANDLER;
		static QString EC_BEHAVIOUR;
		static QString EC_EXTERN;
		static QString CST_UNDEFINED;

		/**
		 * \brief constructor
		 */
		WidgetEditComponent(QGraphicsScene * scene, QString name, QString type, QString subType);

		/**
		 * \brief destructor
		 */
		~WidgetEditComponent();

		QString getType() const {
			return _type;
		}

		void setPUElement(ParticleUniverse::IElement * element) {
			_element = element;
		}

	private:
		QString _type;
		ParticleUniverse::IElement * _element;

		/*QRectF boundingRect() const override;

		void paint(QPainter * painter, const QStyleOptionGraphicsItem *, QWidget *) override;*/
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_WIDGETEDITCOMPONENT_H__ */
