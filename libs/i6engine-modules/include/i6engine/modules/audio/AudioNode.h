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

/**
 * \addtogroup Audio
 * @{
 */

#ifndef __I6ENGINE_MODULES_AUDIONODE_H__
#define __I6ENGINE_MODULES_AUDIONODE_H__

#include "i6engine/utils/i6eThreadSafety.h"

#include "i6engine/math/i6eVector.h"

#include "i6engine/api/GameMessage.h"

#include "AL/al.h"

#include "boost/shared_ptr.hpp"

namespace i6e {
namespace modules {

	class AudioManager;
	struct WavFile;

	/**
	 * \class AudioNode
	 * \brief represents one sound
	 */
	class AudioNode {
		friend class AudioManager;

	public:
		/**
		 * \brief constructor loading file
		 */
		AudioNode(const std::string & file, bool looping, double maxDist, const Vec3 & position, const Vec3 & direction, bool cacheable, const std::string & category);

		/**
		 * \brief constructor using cached sound
		 */
		AudioNode(boost::shared_ptr<WavFile> file, bool looping, double maxDist, const Vec3 & position, const Vec3 & direction, bool cacheable, const std::string & category);

		/**
		 * \brief destructor
		 */
		~AudioNode();

	private:
		ALuint _source;
		ALuint _buffer;
		boost::shared_ptr<WavFile> _wavFile;
		std::string _category;

		void News(const api::GameMessage::Ptr & msg);

		ASSERT_THREAD_SAFETY_HEADER
	};

} /* namespace modules */
} /* namespace i6e */

#endif /* __I6ENGINE_MODULES_AUDIONODE_H__ */

/**
 * @}
 */
