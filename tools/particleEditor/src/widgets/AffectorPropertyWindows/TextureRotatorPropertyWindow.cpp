#include "widgets/AffectorPropertyWindows/TextureRotatorPropertyWindow.h"

#include "properties/BoolProperty.h"
#include "properties/DynamicAttributeProperty.h"

#include "ParticleAffectors/ParticleUniverseTextureRotator.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	TextureRotatorPropertyWindow::TextureRotatorPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name) : AffectorPropertyWindow(par, owner, name) {
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

	void TextureRotatorPropertyWindow::copyAttributesFromAffector(ParticleUniverse::ParticleAffector * affector) {
		AffectorPropertyWindow::copyAttributesFromAffector(affector);

		// Copy properties from affector to property window
		ParticleUniverse::TextureRotator * textureRotator = static_cast<ParticleUniverse::TextureRotator *>(affector);

		// Use Own Rotation speed
		setBool(PRNL_USE_OWN_SPEED, textureRotator->useOwnRotationSpeed());

		// Rotation: Dynamic Attribute
		setDynamicAttribute(PRNL_ROTATION, textureRotator->getRotation());

		// Rotation Speed: Dynamic Attribute
		setDynamicAttribute(PRNL_ROTATION_SPEED, textureRotator->getRotationSpeed());
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */
