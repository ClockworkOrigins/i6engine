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

#ifndef __PU_ATLAS_IMAGE_H__
#define __PU_ATLAS_IMAGE_H__

#include "ParticleUniversePrerequisites.h"

#include "OGRE/OgreImage.h"

namespace ParticleUniverse {

	/** The AtlasImage class offers the possibility to create one (atlas) image from other images.
	*/
	class _ParticleUniverseExport AtlasImage {
	public:
		AtlasImage();
		virtual ~AtlasImage();

		/** If true, the atlas image if recompiled after each change (i.e. an image is added).
		@remarks
			If false, compilation must be done manually (calling the _compile() function)
		*/
		bool getAlwaysUpdate() const;

		/** See getAlwaysUpdate
		*/
		void setAlwaysUpdate(const bool alwaysUpdate);

		/** Add an Ogre Image to the atlas.
		@remarks
			All added images must have the same format. The AtlasImage tries to optimise its own image size.
		@par
			Internally, a copy is made from each added image. This means that the AtlasImage class isn´t responsible
			for destruction of the the added image pointer.
		*/
		bool addImage(const Ogre::Image * image);

		/** Get a pointer to the Ogre image of the atlas.
		*/
		const Ogre::Image * getImage() const;

		/** Construct the atlas image from all added images.
		@remarks
			This internal method is called as soon as an image is added to the atlas, so it can be updated.
		*/
		void _compile();

		/** Save the atlas image as a file. The filename must include the extention (png, jpg)
		*/
		void save(const String & filename);

	protected:
		typedef vector<Ogre::Image *> ImageList;
		ImageList mImageList;
		Ogre::Image * mAtlas;
		size_t mIndividualImageWidth;
		size_t mIndividualImageHeight;
		size_t mIndividualImageRowSpan;
		Ogre::PixelFormat mPixelFormat;
		bool mAlwaysUpdate;
	};

} /* namespace ParticleUniverse */

#endif /* __PU_ATLAS_IMAGE_H__ */
