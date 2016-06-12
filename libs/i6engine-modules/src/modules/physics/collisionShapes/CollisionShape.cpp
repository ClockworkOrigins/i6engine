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

#include "i6engine/modules/physics/collisionShapes/CollisionShape.h"

#include <sstream>

#include "clockUtils/errors.h"
#include "clockUtils/compression/Compression.h"
#include "clockUtils/compression/algorithm/HuffmanGeneric.h"

namespace i6e {
namespace modules {

	std::string CollisionShapeData::Serialize() const {
		std::stringstream ss;
		boost::archive::text_oarchive arch(ss, boost::archive::no_header | boost::archive::no_codecvt | boost::archive::no_xml_tag_checking | boost::archive::archive_flags::no_tracking);
		// Needed to compile on some compilers, because operator<< requires a non-const reference.
		// See http://stackoverflow.com/questions/23440210/error-no-match-for-operator-using-boostserialisation
		CollisionShapeData * m = const_cast<CollisionShapeData *>(this);
		arch << m;
		clockUtils::compression::Compression<clockUtils::compression::algorithm::HuffmanGeneric> compressor;
		std::string compressed;
		return compressor.compress(ss.str(), compressed) == clockUtils::ClockError::SUCCESS ? compressed : "";
	}

	CollisionShapeData * CollisionShapeData::Deserialize(const std::string & serialized) {
		clockUtils::compression::Compression<clockUtils::compression::algorithm::HuffmanGeneric> compressor;
		std::string decompressed;
		if (clockUtils::ClockError::SUCCESS != compressor.decompress(serialized, decompressed)) {
			return nullptr;
		}
		std::stringstream ss(decompressed);
		CollisionShapeData * m;
		boost::archive::text_iarchive arch(ss, boost::archive::no_header | boost::archive::no_codecvt | boost::archive::no_xml_tag_checking | boost::archive::archive_flags::no_tracking);
		arch >> m;
		return m;
	}

} /* namespace modules */
} /* namespace i6e */

BOOST_CLASS_EXPORT_GUID(i6e::modules::CollisionShapeData, "CS0")
BOOST_CLASS_IMPLEMENTATION(i6e::modules::CollisionShapeData, boost::serialization::object_serializable)
