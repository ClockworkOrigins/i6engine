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

#include "ParticleUniverseAtlasImage.h"

#include "ParticleUniverseMath.h"

namespace ParticleUniverse {

	AtlasImage::AtlasImage() : mAtlas(nullptr), mIndividualImageWidth(0), mIndividualImageHeight(0), mIndividualImageRowSpan(0), mPixelFormat(Ogre::PF_UNKNOWN), mAlwaysUpdate(true) {
	}

	AtlasImage::~AtlasImage() {
		// Delete all images
		for (ImageList::iterator it = mImageList.begin(); it != mImageList.end(); ++it) {
			PU_DELETE *it;
		}

		// Delete the atlas itself
		if (mAtlas) {
			PU_DELETE mAtlas;
		}
	}

	bool AtlasImage::addImage(const Ogre::Image * image) {
		if (!image) {
			return false;
		}

		if (mImageList.empty()) {
			mIndividualImageWidth = image->getWidth();
			mIndividualImageHeight = image->getHeight();
			mIndividualImageRowSpan = image->getRowSpan();
			mPixelFormat = image->getFormat();
		} else {
			if (image->getWidth() != mIndividualImageWidth || image->getHeight() != mIndividualImageHeight) {
				// Image not added, because its size doesn't match
				return false;
			}
		}

		mImageList.push_back(PU_NEW Ogre::Image(*image));
		if (mAlwaysUpdate) {
			_compile();
		}

		return true;
	}

	bool AtlasImage::getAlwaysUpdate() const {
		return mAlwaysUpdate;
	}

	void AtlasImage::setAlwaysUpdate(const bool alwaysUpdate) {
		mAlwaysUpdate = alwaysUpdate;
	}

	const Ogre::Image * AtlasImage::getImage() const {
		return mAtlas;
	}

	void AtlasImage::save(const String & filename) {
		if (mAtlas) {
			mAtlas->save(filename);
		}
	}

	void AtlasImage::_compile() {
		// 0. Reject if no images are added
		if (mImageList.empty()) {
			return;
		}

		// 1. Destroy old atlas
		if (mAtlas) {
			PU_DELETE mAtlas;
		}

		// 2. Determine size of the atlas image and alloc memory - Is it possibe to create a square image?
		Ogre::uint imageColums = Ogre::uint(Math::Ceil(Math::Sqrt(Real(mImageList.size()))));
		Ogre::uint imageRows = Ogre::uint(Math::Ceil(Real(mImageList.size()) / Real(imageColums)));
		Ogre::uint width = Ogre::uint(imageColums * mIndividualImageWidth);
		Ogre::uint height = Ogre::uint(imageRows * mIndividualImageHeight);
		size_t pixelSize = Ogre::PixelUtil::getNumElemBytes(mPixelFormat);
		size_t bufferSize = width * height * pixelSize;
		uchar * data = OGRE_ALLOC_T(uchar, bufferSize, MEMCATEGORY_GENERAL);
		memset(data, 0, bufferSize);
		mAtlas = PU_NEW Ogre::Image();
		mAtlas->loadDynamicImage(data, width, height, Ogre::PixelFormat::PF_BYTE_L, mPixelFormat, true); // Create atlas image, no mipmaps

		// 3. Add the buffers of all images to the atlas buffer
		ImageList::iterator it = mImageList.begin();
		Ogre::Image * image = *it;
		size_t atlasPointer = 0;
		for (Ogre::uint imageRow = 0; imageRow < imageRows; ++imageRow) {
			atlasPointer = imageRow * mIndividualImageHeight * mAtlas->getRowSpan();
			for (Ogre::uint imageColum = 0; imageColum < imageColums; ++imageColum) {
				atlasPointer = (imageRow * mIndividualImageHeight * mAtlas->getRowSpan()) + (imageColum * mIndividualImageRowSpan);
				for (size_t pixelLine = 0; pixelLine < mIndividualImageHeight; ++pixelLine) {
					memcpy(data + atlasPointer, image->getData() + pixelLine * mIndividualImageRowSpan, mIndividualImageRowSpan);	
					atlasPointer += mAtlas->getRowSpan();
				}
				it++;
				if (it != mImageList.end()) {
					image = *it;
				} else {
					return;
				}
			}
		}
	}

} /* namespace ParticleUniverse */
