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

#include "i6engine/modules/physics/collisionShapes/HeightmapCollisionShape.h"
#include "i6engine/modules/physics/collisionShapes/MeshStriderCollisionShape.h"

#include "gtest/gtest.h"

TEST(CollisionShape, CollisionShape) {
	i6engine::modules::CollisionShapeData * csd = new i6engine::modules::CollisionShapeData();
	std::string serialized = csd->Serialize();
	delete csd;
	csd = i6engine::modules::CollisionShapeData::Deserialize(serialized);
	delete csd;
}

TEST(CollisionShape, HeightmapCollisionShape) {
	i6engine::modules::CollisionShapeData * csd = new i6engine::modules::HeightmapCollisionShapeData();
	std::string serialized = csd->Serialize();
	delete csd;
	csd = i6engine::modules::CollisionShapeData::Deserialize(serialized);
	delete csd;
}

TEST(CollisionShape, MeshStriderCollisionShape) {
	i6engine::modules::CollisionShapeData * csd = new i6engine::modules::MeshStriderCollisionShapeData();
	std::string serialized = csd->Serialize();
	delete csd;
	csd = i6engine::modules::CollisionShapeData::Deserialize(serialized);
	delete csd;
}
