/**
 * Copyright 2012 FAU (Friedrich Alexander University of Erlangen-Nuremberg)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "i6engine/modules/physics/collisionShapes/CollisionShape.h"

#include <sstream>

#include "clockUtils/compression/Compression.h"
#include "clockUtils/compression/algorithm/HuffmanGeneric.h"

namespace i6engine {
namespace modules {

	std::string CollisionShapeData::Serialize() const {
		std::stringstream ss;
		boost::archive::text_oarchive arch(ss, boost::archive::no_header | boost::archive::no_codecvt | boost::archive::no_xml_tag_checking | boost::archive::archive_flags::no_tracking);
		// Needed to compile on some compilers, because operator<< requires a non-const reference.
		// See http://stackoverflow.com/questions/23440210/error-no-match-for-operator-using-boostserialisation
		CollisionShapeData * m = const_cast<CollisionShapeData *>(this);
		arch << m;
		clockUtils::compression::Compression<clockUtils::compression::algorithm::HuffmanGeneric> compressor;
		return compressor.compress(ss.str());
	}

	CollisionShapeData * CollisionShapeData::Deserialize(const std::string & serialized) {
		clockUtils::compression::Compression<clockUtils::compression::algorithm::HuffmanGeneric> compressor;
		std::stringstream ss(compressor.decompress(serialized));
		CollisionShapeData * m;
		boost::archive::text_iarchive arch(ss, boost::archive::no_header | boost::archive::no_codecvt | boost::archive::no_xml_tag_checking | boost::archive::archive_flags::no_tracking);
		arch >> m;
		return m;
	}

	template<class Archive>
	void CollisionShapeData::serialize(Archive & ar, const unsigned int /*version*/) {
		ar & type;
	}

} /* namespace modules */
} /* namespace i6engine */

BOOST_CLASS_EXPORT_GUID(i6engine::modules::CollisionShapeData, "CS0")
BOOST_CLASS_IMPLEMENTATION(i6engine::modules::CollisionShapeData, boost::serialization::object_serializable)
