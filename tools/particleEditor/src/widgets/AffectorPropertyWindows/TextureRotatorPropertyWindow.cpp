#include "widgets/AffectorPropertyWindows/TextureRotatorPropertyWindow.h"

#include "properties/BoolProperty.h"
#include "properties/DynamicAttributeProperty.h"

#include "ParticleAffectors/ParticleUniverseTextureRotator.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	TextureRotatorPropertyWindow::TextureRotatorPropertyWindow(QWidget * par, QString name) : AffectorPropertyWindow(par, name) {
		// Set the (internationalized) property names
		PRNL_USE_OWN_SPEED = "Use own speed";
		PRNL_ROTATION = "Rotation";
		PRNL_ROTATION_SPEED = "Rotation speed";

		// Use Own Rotation speed
		append(new properties::BoolProperty(this, PRNL_USE_OWN_SPEED, PRNL_USE_OWN_SPEED, ParticleUniverse::TextureRotator::DEFAULT_USE_OWN_SPEED));

		// Rotation: Dynamic Attribute
		ParticleUniverse::DynamicAttributeFixed * dynAttr = new ParticleUniverse::DynamicAttributeFixed();
		dynAttr->setValue(ParticleUniverse::TextureRotator::DEFAULT_ROTATION);
		append(new properties::DynamicAttributeProperty(this, PRNL_ROTATION, PRNL_ROTATION, dynAttr));

		// Rotation Speed: Dynamic Attribute
		dynAttr = new ParticleUniverse::DynamicAttributeFixed();
		dynAttr->setValue(ParticleUniverse::TextureRotator::DEFAULT_ROTATION_SPEED);
		append(new properties::DynamicAttributeProperty(this, PRNL_ROTATION_SPEED, PRNL_ROTATION_SPEED, dynAttr));
	}

	TextureRotatorPropertyWindow::~TextureRotatorPropertyWindow() {
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */
