/*
 * i6engine
 * Copyright (2016) Daniel Bonrath, Michael Baer, All rights reserved.
 *
 * This file is part of i6engine; i6engine is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "widgets/AffectorPropertyWindows/AlignAffectorPropertyWindow.h"

#include "properties/BoolProperty.h"

#include "widgets/WidgetEditComponent.h"

#include "ParticleAffectors/ParticleUniverseAlignAffector.h"

namespace i6e {
namespace particleEditor {
namespace widgets {

	AlignAffectorPropertyWindow::AlignAffectorPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name) : AffectorPropertyWindow(par, owner, name) {
		PRNL_AFFECTOR_ALIGN_RESIZE = QApplication::tr("Resize");

		// Resize: bool
		append(new properties::BoolProperty(this, PRNL_AFFECTOR_ALIGN_RESIZE, PRNL_AFFECTOR_ALIGN_RESIZE, ParticleUniverse::AlignAffector::DEFAULT_RESIZE));
	}

	AlignAffectorPropertyWindow::~AlignAffectorPropertyWindow() {
	}

	void AlignAffectorPropertyWindow::copyAttributesFromAffector(ParticleUniverse::ParticleAffector * affector) {
		AffectorPropertyWindow::copyAttributesFromAffector(affector);

		// Copy properties from affector to property window
		ParticleUniverse::AlignAffector * alignAffector = static_cast<ParticleUniverse::AlignAffector *>(affector);

		// Resize: bool
		setBool(PRNL_AFFECTOR_ALIGN_RESIZE, alignAffector->isResize());
	}

	void AlignAffectorPropertyWindow::copyAttributeToAffector(properties::Property * prop, QString propertyName) {
		if (!prop) {
			return;
		}

		ParticleUniverse::AlignAffector * affector = static_cast<ParticleUniverse::AlignAffector *>(_owner->getPUElement());
		if (!affector) {
			return;
		}

		if (propertyName == PRNL_AFFECTOR_ALIGN_RESIZE) {
			affector->setResize(prop->getBool());
		} else {
			// Update affector with another attribute
			AffectorPropertyWindow::copyAttributeToAffector(prop, propertyName);
		}
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6e */
