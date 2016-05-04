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

#include "i6engine/modules/graphics/components/BillboardComponent.h"

#include "i6engine/api/configs/GraphicsConfig.h"

#include "i6engine/modules/graphics/GraphicsManager.h"
#include "i6engine/modules/graphics/GraphicsNode.h"

#include "OGRE/OgreBillboard.h"
#include "OGRE/OgreBillboardSet.h"
#include "OGRE/OgreSceneManager.h"

namespace i6e {
namespace modules {

	BillboardComponent::BillboardComponent(GraphicsManager * manager, GraphicsNode * parent, const int64_t goid, const int64_t coid, const std::string & material, double width, double height, api::graphics::BillboardOrigin bo) : _manager(manager), _parent(parent), _billboardSet(nullptr), _billboards() {
		ASSERT_THREAD_SAFETY_CONSTRUCTOR
		std::string name = "SN_" + std::to_string(goid) + "_" + std::to_string(coid);

		_billboardSet = _manager->getSceneManager()->createBillboardSet(name);
		_billboardSet->setMaterialName(material);
		_billboardSet->setBillboardType(Ogre::BillboardType::BBT_POINT);
		_billboardSet->setDefaultDimensions(width, height);
		switch (bo) {
		case api::graphics::BillboardOrigin::CenterLeft: {
			_billboardSet->setBillboardOrigin(Ogre::BillboardOrigin::BBO_CENTER_LEFT);
			break;
		}
		default: {
			ISIXE_THROW_FAILURE("GraphicsNode", "Billboard origin not correct!");
		}
		}
		_parent->getSceneNode()->attachObject(_billboardSet);
	}

	BillboardComponent::~BillboardComponent() {
		ASSERT_THREAD_SAFETY_FUNCTION
		while (!_billboards.empty()) {
			deleteBillboard(_billboards.begin()->first);
		}
	}

	void BillboardComponent::createOrUpdateBillboard(const std::string & identifier, const Vec3 & offset, double width, double height, double u0, double v0, double u1, double v1) {
		ASSERT_THREAD_SAFETY_FUNCTION
		Ogre::Billboard * bb = nullptr;
		auto it = _billboards.find(identifier);
		if (it == _billboards.end()) {
			bb = _billboardSet->createBillboard(offset.toOgre(), Ogre::ColourValue::White);
			_billboards.insert(std::make_pair(identifier, bb));
		} else {
			bb = it->second;
		}
		bb->setDimensions(width, height);
		bb->setTexcoordRect(u0, v0, u1, v1);
		bb->setPosition(offset.toOgre());
		assert(_billboards.find(identifier) != _billboards.end());
	}

	void BillboardComponent::deleteBillboard(const std::string & identifier) {
		ASSERT_THREAD_SAFETY_FUNCTION
		assert(_billboards.find(identifier) != _billboards.end());
		Ogre::Billboard * bb = _billboards[identifier];
		_billboardSet->removeBillboard(bb);
		_billboards.erase(identifier);
		delete bb;
		assert(_billboards.find(identifier) == _billboards.end());
	}

} /* namespace modules */
} /* namespace i6e */
