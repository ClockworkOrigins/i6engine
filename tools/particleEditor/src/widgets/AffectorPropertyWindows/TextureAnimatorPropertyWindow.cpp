#include "widgets/AffectorPropertyWindows/TextureAnimatorPropertyWindow.h"

#include "properties/BoolProperty.h"
#include "properties/DoubleProperty.h"
#include "properties/EnumProperty.h"
#include "properties/UIntProperty.h"

#include "ParticleAffectors/ParticleUniverseTextureAnimator.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	TextureAnimatorPropertyWindow::TextureAnimatorPropertyWindow(QWidget * par, QString name) : AffectorPropertyWindow(par, name) {
		// Set the (internationalized) property names
		PRNL_ANIMATION_TYPE = "Animation type";
		TAT_LOOP = "Loop";
		TAT_UP_DOWN = "Up - Down";
		TAT_RANDOM = "Random";
		PRNL_TEXCOORDS_START = "Texture coordinate start";
		PRNL_TEXCOORDS_END = "Texture coordinate end";
		PRNL_START_RANDOM = "Start random";
		PRNL_TIME_STEP = "Time step";

		// Time Step Animation: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_TIME_STEP, PRNL_TIME_STEP, ParticleUniverse::TextureAnimator::DEFAULT_TIME_STEP));

		// Animation Type: List
		QStringList animationTypes;
		animationTypes.append(TAT_LOOP);
		animationTypes.append(TAT_UP_DOWN);
		animationTypes.append(TAT_RANDOM);
		append(new properties::EnumProperty(this, PRNL_ANIMATION_TYPE, PRNL_ANIMATION_TYPE, animationTypes));

		// Start Texture Coordinates: ParticleUniverse::uint16
		append(new properties::UIntProperty(this, PRNL_TEXCOORDS_START, PRNL_TEXCOORDS_START, ParticleUniverse::TextureAnimator::DEFAULT_TEXCOORDS_START));

		// End Texture Coordinates: ParticleUniverse::uint16
		append(new properties::UIntProperty(this, PRNL_TEXCOORDS_END, PRNL_TEXCOORDS_END, ParticleUniverse::TextureAnimator::DEFAULT_TEXCOORDS_END));

		// Random Start: bool
		append(new properties::BoolProperty(this, PRNL_START_RANDOM, PRNL_START_RANDOM, ParticleUniverse::TextureAnimator::DEFAULT_START_RANDOM));
	}

	TextureAnimatorPropertyWindow::~TextureAnimatorPropertyWindow() {
	}

	void TextureAnimatorPropertyWindow::copyAttributesFromAffector(ParticleUniverse::ParticleAffector * affector) {
		AffectorPropertyWindow::copyAttributesFromAffector(affector);

		// Copy properties from affector to property window
		ParticleUniverse::TextureAnimator * textureAnimator = static_cast<ParticleUniverse::TextureAnimator *>(affector);

		// Time Step Animation: ParticleUniverse::Real
		setDouble(PRNL_TIME_STEP, textureAnimator->getAnimationTimeStep());

		// Animation Type: List
		ParticleUniverse::TextureAnimator::TextureAnimationType animationType = textureAnimator->getTextureAnimationType();
		QString animationTypeString = TAT_LOOP;
		if (animationType == ParticleUniverse::TextureAnimator::TAT_UP_DOWN) {
			animationTypeString = TAT_UP_DOWN;
		} else if (animationType == ParticleUniverse::TextureAnimator::TAT_RANDOM) {
			animationTypeString = TAT_RANDOM;
		}
		setEnumString(PRNL_ANIMATION_TYPE, animationTypeString);

		// Start Texture Coordinates: ParticleUniverse::uint16
		setUint16(PRNL_TEXCOORDS_START, textureAnimator->getTextureCoordsStart());

		// End Texture Coordinates: ParticleUniverse::uint16
		setUint16(PRNL_TEXCOORDS_END, textureAnimator->getTextureCoordsEnd());

		// Random Start: bool
		setBool(PRNL_START_RANDOM, textureAnimator->isStartRandom());
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */
