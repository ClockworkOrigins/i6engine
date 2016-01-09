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

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */
