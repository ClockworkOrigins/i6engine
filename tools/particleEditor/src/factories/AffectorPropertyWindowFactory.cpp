#include "factories/AffectorPropertyWindowFactory.h"

#include "widgets/WidgetEditComponent.h"

#include "widgets/AffectorPropertyWindows/AlignAffectorPropertyWindow.h"
#include "widgets/AffectorPropertyWindows/BoxColliderPropertyWindow.h"
#include "widgets/AffectorPropertyWindows/CollisionAvoidanceAffectorPropertyWindow.h"
#include "widgets/AffectorPropertyWindows/ColourAffectorPropertyWindow.h"
#include "widgets/AffectorPropertyWindows/FlockCenteringAffectorPropertyWindow.h"
#include "widgets/AffectorPropertyWindows/ForceFieldAffectorPropertyWindow.h"
#include "widgets/AffectorPropertyWindows/GeometryRotatorPropertyWindow.h"
#include "widgets/AffectorPropertyWindows/GravityAffectorPropertyWindow.h"
#include "widgets/AffectorPropertyWindows/InterParticleColliderPropertyWindow.h"
#include "widgets/AffectorPropertyWindows/JetAffectorPropertyWindow.h"
#include "widgets/AffectorPropertyWindows/LineAffectorPropertyWindow.h"
#include "widgets/AffectorPropertyWindows/LinearForceAffectorPropertyWindow.h"
#include "widgets/AffectorPropertyWindows/ParticleFollowerPropertyWindow.h"
#include "widgets/AffectorPropertyWindows/PathFollowerPropertyWindow.h"
#include "widgets/AffectorPropertyWindows/PlaneColliderPropertyWindow.h"
#include "widgets/AffectorPropertyWindows/RandomiserPropertyWindow.h"
#include "widgets/AffectorPropertyWindows/ScaleAffectorPropertyWindow.h"
#include "widgets/AffectorPropertyWindows/ScaleVelocityAffectorPropertyWindow.h"
#include "widgets/AffectorPropertyWindows/SineForceAffectorPropertyWindow.h"
#include "widgets/AffectorPropertyWindows/SphereColliderPropertyWindow.h"
#include "widgets/AffectorPropertyWindows/TextureAnimatorPropertyWindow.h"
#include "widgets/AffectorPropertyWindows/TextureRotatorPropertyWindow.h"
#include "widgets/AffectorPropertyWindows/VelocityMatchingAffectorPropertyWindow.h"
#include "widgets/AffectorPropertyWindows/VortexAffectorPropertyWindow.h"

namespace i6engine {
namespace particleEditor {
namespace factories {

	/**
	Create a specific Affector Property Window
	*/
	widgets::AffectorPropertyWindow * AffectorPropertyWindowFactory::createAffectorPropertyWindow(QWidget * parent, const QString & name, QString subType) {
		if (subType == widgets::CST_UNDEFINED) {
			return new widgets::AffectorPropertyWindow(parent, name);
		} else if (subType == widgets::CST_AFFECTOR_ALIGN) {
			return new widgets::AlignAffectorPropertyWindow(parent, name);
		} else if (subType == widgets::CST_AFFECTOR_BOX_COLLIDER) {
			return new widgets::BoxColliderPropertyWindow(parent, name);
		} else if (subType == widgets::CST_AFFECTOR_COLLISION_AVOIDANCE) {
			return new widgets::CollisionAvoidanceAffectorPropertyWindow(parent, name);
		} else if (subType == widgets::CST_AFFECTOR_COLOUR) {
			return new widgets::ColourAffectorPropertyWindow(parent, name);
		} else if (subType == widgets::CST_AFFECTOR_FLOCK_CENTERING) {
			return new widgets::FlockCenteringAffectorPropertyWindow(parent, name);
		} else if (subType == widgets::CST_AFFECTOR_FORCEFIELD) {
			return new widgets::ForceFieldAffectorPropertyWindow(parent, name);
		} else if (subType == widgets::CST_AFFECTOR_GEOMETRY_ROTATOR) {
			return new widgets::GeometryRotatorPropertyWindow(parent, name);
		} else if (subType == widgets::CST_AFFECTOR_GRAVITY) {
			return new widgets::GravityAffectorPropertyWindow(parent, name);
		} else if (subType == widgets::CST_AFFECTOR_INTER_PARTICLE_COLLIDER) {
			return new widgets::InterParticleColliderPropertyWindow(parent, name);
		} else if (subType == widgets::CST_AFFECTOR_JET) {
			return new widgets::JetAffectorPropertyWindow(parent, name);
		} else if (subType == widgets::CST_AFFECTOR_LINE) {
			return new widgets::LineAffectorPropertyWindow(parent, name);
		} else if (subType == widgets::CST_AFFECTOR_LINEAR_FORCE) {
			return new widgets::LinearForceAffectorPropertyWindow(parent, name);
		} else if (subType == widgets::CST_AFFECTOR_PARTICLE_FOLLOWER) {
			return new widgets::ParticleFollowerPropertyWindow(parent, name);
		} else if (subType == widgets::CST_AFFECTOR_PATH_FOLLOWER) {
			return new widgets::PathFollowerPropertyWindow(parent, name);
		} else if (subType == widgets::CST_AFFECTOR_PLANE_COLLIDER) {
			return new widgets::PlaneColliderPropertyWindow(parent, name);
		} else if (subType == widgets::CST_AFFECTOR_RANDOMISER) {
			return new widgets::RandomiserPropertyWindow(parent, name);
		} else if (subType == widgets::CST_AFFECTOR_SCALE) {
			return new widgets::ScaleAffectorPropertyWindow(parent, name);
		} else if (subType == widgets::CST_AFFECTOR_SCALE_VELOCITY) {
			return new widgets::ScaleVelocityAffectorPropertyWindow(parent, name);
		} else if (subType == widgets::CST_AFFECTOR_SINE_FORCE) {
			return new widgets::SineForceAffectorPropertyWindow(parent, name);
		} else if (subType == widgets::CST_AFFECTOR_SPHERE_COLLIDER) {
			return new widgets::SphereColliderPropertyWindow(parent, name);
		} else if (subType == widgets::CST_AFFECTOR_TEXTURE_ANIMATOR) {
			return new widgets::TextureAnimatorPropertyWindow(parent, name);
		} else if (subType == widgets::CST_AFFECTOR_TEXTURE_ROTATOR) {
			return new widgets::TextureRotatorPropertyWindow(parent, name);
		} else if (subType == widgets::CST_AFFECTOR_VELOCITY_MATCHING) {
			return new widgets::VelocityMatchingAffectorPropertyWindow(parent, name);
		} else if (subType == widgets::CST_AFFECTOR_VORTEX) {
			return new widgets::VortexAffectorPropertyWindow(parent, name);
		} else {
			return new widgets::AffectorPropertyWindow(parent, name);
		}
		return nullptr;
	};

	/**
	Create a specific Affector Property Window and propagate the attributes from the AffectorPropertyWindow
	*/
	widgets::AffectorPropertyWindow * AffectorPropertyWindowFactory::createAffectorPropertyWindow(QString subType, widgets::AffectorPropertyWindow * affectorPropertyWindow) {
		if (subType == widgets::CST_UNDEFINED) {
			return new widgets::AffectorPropertyWindow(affectorPropertyWindow);
		} else if (subType == widgets::CST_AFFECTOR_ALIGN) {
			return new widgets::AlignAffectorPropertyWindow(affectorPropertyWindow);
		} else if (subType == widgets::CST_AFFECTOR_BOX_COLLIDER) {
			return new widgets::BoxColliderPropertyWindow(affectorPropertyWindow);
		} else if (subType == widgets::CST_AFFECTOR_COLLISION_AVOIDANCE) {
			return new widgets::CollisionAvoidanceAffectorPropertyWindow(affectorPropertyWindow);
		} else if (subType == widgets::CST_AFFECTOR_COLOUR) {
			return new widgets::ColourAffectorPropertyWindow(affectorPropertyWindow);
		} else if (subType == widgets::CST_AFFECTOR_FLOCK_CENTERING) {
			return new widgets::FlockCenteringAffectorPropertyWindow(affectorPropertyWindow);
		} else if (subType == widgets::CST_AFFECTOR_FORCEFIELD) {
			return new widgets::ForceFieldAffectorPropertyWindow(affectorPropertyWindow);
		} else if (subType == widgets::CST_AFFECTOR_GEOMETRY_ROTATOR) {
			return new widgets::GeometryRotatorPropertyWindow(affectorPropertyWindow);
		} else if (subType == widgets::CST_AFFECTOR_GRAVITY) {
			return new widgets::GravityAffectorPropertyWindow(affectorPropertyWindow);
		} else if (subType == widgets::CST_AFFECTOR_INTER_PARTICLE_COLLIDER) {
			return new widgets::InterParticleColliderPropertyWindow(affectorPropertyWindow);
		} else if (subType == widgets::CST_AFFECTOR_JET) {
			return new widgets::JetAffectorPropertyWindow(affectorPropertyWindow);
		} else if (subType == widgets::CST_AFFECTOR_LINE) {
			return new widgets::LineAffectorPropertyWindow(affectorPropertyWindow);
		} else if (subType == widgets::CST_AFFECTOR_LINEAR_FORCE) {
			return new widgets::LinearForceAffectorPropertyWindow(affectorPropertyWindow);
		} else if (subType == widgets::CST_AFFECTOR_PARTICLE_FOLLOWER) {
			return new widgets::ParticleFollowerPropertyWindow(affectorPropertyWindow);
		} else if (subType == widgets::CST_AFFECTOR_PATH_FOLLOWER) {
			return new widgets::PathFollowerPropertyWindow(affectorPropertyWindow);
		} else if (subType == widgets::CST_AFFECTOR_PLANE_COLLIDER) {
			return new widgets::PlaneColliderPropertyWindow(affectorPropertyWindow);
		} else if (subType == widgets::CST_AFFECTOR_RANDOMISER) {
			return new widgets::RandomiserPropertyWindow(affectorPropertyWindow);
		} else if (subType == widgets::CST_AFFECTOR_SCALE) {
			return new widgets::ScaleAffectorPropertyWindow(affectorPropertyWindow);
		} else if (subType == widgets::CST_AFFECTOR_SCALE_VELOCITY) {
			return new widgets::ScaleVelocityAffectorPropertyWindow(affectorPropertyWindow);
		} else if (subType == widgets::CST_AFFECTOR_SINE_FORCE) {
			return new widgets::SineForceAffectorPropertyWindow(affectorPropertyWindow);
		} else if (subType == widgets::CST_AFFECTOR_SPHERE_COLLIDER) {
			return new widgets::SphereColliderPropertyWindow(affectorPropertyWindow);
		} else if (subType == widgets::CST_AFFECTOR_TEXTURE_ANIMATOR) {
			return new widgets::TextureAnimatorPropertyWindow(affectorPropertyWindow);
		} else if (subType == widgets::CST_AFFECTOR_TEXTURE_ROTATOR) {
			return new widgets::TextureRotatorPropertyWindow(affectorPropertyWindow);
		} else if (subType == widgets::CST_AFFECTOR_VELOCITY_MATCHING) {
			return new widgets::VelocityMatchingAffectorPropertyWindow(affectorPropertyWindow);
		} else if (subType == widgets::CST_AFFECTOR_VORTEX) {
			return new widgets::VortexAffectorPropertyWindow(affectorPropertyWindow);
		} else {
			return new widgets::AffectorPropertyWindow(affectorPropertyWindow);
		}
		return nullptr;
	};

} /* namespace factories */
} /* namespace particleEditor */
} /* namespace i6engine */
