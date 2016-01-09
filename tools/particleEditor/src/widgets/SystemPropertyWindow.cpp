#include "widgets/SystemPropertyWindow.h"

#include "properties/BoolProperty.h"
#include "properties/DoubleProperty.h"
#include "properties/StringProperty.h"
#include "properties/Vec3Property.h"

#include "ParticleUniverseSystem.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	SystemPropertyWindow::SystemPropertyWindow(QWidget * par, QString name) : PropertyWindow(par, name) {
		// Set the (internationalized) property names
		PRNL_NAME = "Name";
		PRNL_SYSTEM_CATEGORY = "Category";
		PRNL_SYSTEM_KEEP_LOCAL = "Keep local";
		PRNL_SYSTEM_ITERATION_INTERVAL = "Iteration interval";
		PRNL_SYSTEM_FIXED_TIMEOUT = "Fixed timeout";
		PRNL_SYSTEM_NONVIS_UPDATE_TIMEOUT = "Nonvisible update timeout";
		PRNL_SYSTEM_LOD_DISTANCES = "Lod distances";
		PRNL_SYSTEM_SMOOTH_LOD = "Smooth lod";
		PRNL_SYSTEM_FAST_FORWARD = "Fast forward";
		PRNL_SYSTEM_FAST_FORWARD_TIME = "Time";
		PRNL_SYSTEM_FAST_FORWARD_INTERVAL = "Interval";
		PRNL_SYSTEM_MAIN_CAMERA_NAME = "Main camera name";
		PRNL_SYSTEM_SCALE_VELOCITY = "Scale velocity";
		PRNL_SYSTEM_SCALE_TIME = "Scale time";
		PRNL_SYSTEM_SCALE = "Scale dimensions";
		PRNL_SYSTEM_TIGHT_BOUNDING_BOX = "Use tight bounding box";
		PRNL_X = ".x";
		PRNL_Y = ".y";
		PRNL_Z = ".z";
		PRNL_FLOAT = "Value";

		// Category: String
		append(new properties::StringProperty(this, PRNL_SYSTEM_CATEGORY, PRNL_SYSTEM_CATEGORY, ""));

		// Keep local: Bool
		append(new properties::BoolProperty(this, PRNL_SYSTEM_KEEP_LOCAL, PRNL_SYSTEM_KEEP_LOCAL, ParticleUniverse::ParticleSystem::DEFAULT_KEEP_LOCAL));

		// Iteration interval: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_SYSTEM_ITERATION_INTERVAL, PRNL_SYSTEM_ITERATION_INTERVAL, ParticleUniverse::ParticleSystem::DEFAULT_ITERATION_INTERVAL));

		// Fixed timeout: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_SYSTEM_FIXED_TIMEOUT, PRNL_SYSTEM_FIXED_TIMEOUT, ParticleUniverse::ParticleSystem::DEFAULT_FIXED_TIMEOUT));

		// Non-visible update timeout: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_SYSTEM_NONVIS_UPDATE_TIMEOUT, PRNL_SYSTEM_NONVIS_UPDATE_TIMEOUT, ParticleUniverse::ParticleSystem::DEFAULT_NON_VISIBLE_UPDATE_TIMEOUT));

		// Lod distances: List of ParticleUniverse::Real
		//append(new ParentPropertyWithButtonAndFloats(PRNL_SYSTEM_LOD_DISTANCES, PRNL_SYSTEM_LOD_DISTANCES));

		// Smooth lod: bool
		append(new properties::BoolProperty(this, PRNL_SYSTEM_SMOOTH_LOD, PRNL_SYSTEM_SMOOTH_LOD, ParticleUniverse::ParticleSystem::DEFAULT_SMOOTH_LOD));

		// Fast forward: ParticleUniverse::Real (time) + ParticleUniverse::Real (interval)
		append(new properties::StringProperty(this, PRNL_SYSTEM_FAST_FORWARD, PRNL_SYSTEM_FAST_FORWARD, ""));
		append(new properties::DoubleProperty(this, PRNL_SYSTEM_FAST_FORWARD_TIME, PRNL_SYSTEM_FAST_FORWARD_TIME, ParticleUniverse::ParticleSystem::DEFAULT_FAST_FORWARD_TIME));
		append(new properties::DoubleProperty(this, PRNL_SYSTEM_FAST_FORWARD_INTERVAL, PRNL_SYSTEM_FAST_FORWARD_INTERVAL, 0.0f));

		// Main camera name: Ogre::String
		append(new properties::StringProperty(this, PRNL_SYSTEM_MAIN_CAMERA_NAME, PRNL_SYSTEM_MAIN_CAMERA_NAME, ""));

		// Scale velocity: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_SYSTEM_SCALE_VELOCITY, PRNL_SYSTEM_SCALE_VELOCITY, ParticleUniverse::ParticleSystem::DEFAULT_SCALE_VELOCITY));

		// Scale time: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_SYSTEM_SCALE_TIME, PRNL_SYSTEM_SCALE_TIME, ParticleUniverse::ParticleSystem::DEFAULT_SCALE_TIME));

		// Scale dimensions: Ogre::Vector3
		append(new properties::Vec3Property(this, PRNL_SYSTEM_SCALE, PRNL_SYSTEM_SCALE, Ogre::Vector3::UNIT_SCALE));

		// Use tight bounding box: bool
		append(new properties::BoolProperty(this, PRNL_SYSTEM_TIGHT_BOUNDING_BOX, PRNL_SYSTEM_TIGHT_BOUNDING_BOX, ParticleUniverse::ParticleSystem::DEFAULT_TIGHT_BOUNDINGBOX));
	}

	SystemPropertyWindow::~SystemPropertyWindow() {
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */
