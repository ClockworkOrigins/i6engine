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

#include "i6engine/modules/graphics/Debug.h"

#include "OGRE/OgreAxisAlignedBox.h"
#include "OGRE/OgreManualObject.h"
#include "OGRE/OgreRenderQueue.h"
#include "OGRE/OgreSceneManager.h"

template<> i6engine::modules::Debug * Ogre::Singleton<i6engine::modules::Debug>::msSingleton = nullptr;

namespace i6engine {
namespace modules {

	Debug * Debug::getSingletonPtr() {
		return msSingleton;
	}

	Debug & Debug::getSingleton() {
		assert(msSingleton);
		return *msSingleton;
	}


	Debug::Debug(Ogre::SceneManager * _sceneManager, float _fillAlpha) : sceneManager(_sceneManager), fillAlpha(_fillAlpha), manualObject(0), isEnabled(true), lineVertices(), triangleVertices(), lineIndices(), triangleIndices(), linesIndex(0), trianglesIndex(0) {
		initialise();
	}

	Debug::~Debug() {
		shutdown();
	}

	void Debug::initialise() {
		manualObject = sceneManager->createManualObject("debug_object");
		sceneManager->getRootSceneNode()->createChildSceneNode("debug_object")->attachObject(manualObject);
		manualObject->setDynamic(true);

		manualObject->begin("debug_draw", Ogre::RenderOperation::OT_LINE_LIST);
		manualObject->position(Ogre::Vector3::ZERO);
		manualObject->colour(Ogre::ColourValue::ZERO);
		manualObject->index(0);
		manualObject->end();
		manualObject->begin("debug_draw", Ogre::RenderOperation::OT_TRIANGLE_LIST);
		manualObject->position(Ogre::Vector3::ZERO);
		manualObject->colour(Ogre::ColourValue::ZERO);
		manualObject->index(0);
		manualObject->end();
		linesIndex = trianglesIndex = 0;
	}

	void Debug::shutdown() {
		sceneManager->destroySceneNode("debug_object");
		sceneManager->destroyManualObject(manualObject);
	}

	void Debug::buildLine(const Ogre::Vector3 & start, const Ogre::Vector3 & end, const Ogre::ColourValue & colour, float alpha) {
		int i = addLineVertex(start, Ogre::ColourValue(colour.r, colour.g, colour.b, alpha));
		addLineVertex(end, Ogre::ColourValue(colour.r, colour.g, colour.b, alpha));
		addLineIndices(i, i + 1);
	}

	void Debug::buildQuad(const Ogre::Vector3 * vertices, const Ogre::ColourValue & colour, float alpha) {
		int index = addLineVertex(vertices[0], Ogre::ColourValue(colour.r, colour.g, colour.b, alpha));
		addLineVertex(vertices[1], Ogre::ColourValue(colour.r, colour.g, colour.b, alpha));
		addLineVertex(vertices[2], Ogre::ColourValue(colour.r, colour.g, colour.b, alpha));
		addLineVertex(vertices[3], Ogre::ColourValue(colour.r, colour.g, colour.b, alpha));

		for (int i = 0; i < 4; ++i) {
			addLineIndices(index + i, index + ((i + 1) % 4));
		}
	}

	void Debug::buildCircle(const Ogre::Vector3 & centre, float radius, int segmentsCount, const Ogre::ColourValue & colour, float alpha) {
		int index = linesIndex;
		double increment = 2 * Ogre::Math::PI / segmentsCount;
		double angle = 0.0f;

		for (int i = 0; i < segmentsCount; i++) {
			addLineVertex(Ogre::Vector3(centre.x + radius * Ogre::Math::Cos(angle), centre.y, centre.z + radius * Ogre::Math::Sin(angle)), Ogre::ColourValue(colour.r, colour.g, colour.b, alpha));
			angle += increment;
		}

		for (int i = 0; i < segmentsCount; ++i) {
			addLineIndices(index + i, i + 1 < segmentsCount ? index + i + 1 : index);
		}
	}

	void Debug::buildFilledCircle(const Ogre::Vector3 & centre, float radius, int segmentsCount, const Ogre::ColourValue & colour, float alpha) {
		int index = trianglesIndex;
		double increment = 2 * Ogre::Math::PI / segmentsCount;
		double angle = 0.0f;

		for (int i = 0; i < segmentsCount; ++i) {
			addTriangleVertex(Ogre::Vector3(centre.x + radius * Ogre::Math::Cos(angle), centre.y, centre.z + radius * Ogre::Math::Sin(angle)), Ogre::ColourValue(colour.r, colour.g, colour.b, alpha));
			angle += increment;
		}

		addTriangleVertex(centre, Ogre::ColourValue(colour.r, colour.g, colour.b, alpha));

		for (int i = 0; i < segmentsCount; ++i) {
			addTriangleIndices(i + 1 < segmentsCount ? index + i + 1 : index, index + i, index + segmentsCount);
		}
	}

	void Debug::buildCylinder(const Ogre::Vector3 & centre, float radius, int segmentsCount, float height, const Ogre::ColourValue & colour, float alpha) {
		int index = linesIndex;
		double increment = 2 * Ogre::Math::PI / segmentsCount;
		double angle = 0.0f;

		// Top circle
		for (int i = 0; i < segmentsCount; ++i) {
			addLineVertex(Ogre::Vector3(centre.x + radius * Ogre::Math::Cos(angle), centre.y + height / 2, centre.z + radius * Ogre::Math::Sin(angle)), Ogre::ColourValue(colour.r, colour.g, colour.b, alpha));
			angle += increment;
		}

		angle = 0.0f;

		// Bottom circle
		for (int i = 0; i < segmentsCount; ++i) {
			addLineVertex(Ogre::Vector3(centre.x + radius * Ogre::Math::Cos(angle), centre.y - height / 2, centre.z + radius * Ogre::Math::Sin(angle)), Ogre::ColourValue(colour.r, colour.g, colour.b, alpha));
			angle += increment;
		}

		for (int i = 0; i < segmentsCount; ++i) {
			addLineIndices(index + i, i + 1 < segmentsCount ? index + i + 1 : index);
			addLineIndices(segmentsCount + index + i, i + 1 < segmentsCount ? segmentsCount + index + i + 1 : segmentsCount + index);
			addLineIndices(index + i, segmentsCount + index + i);
		}
	}

	void Debug::buildFilledCylinder(const Ogre::Vector3 & centre, float radius, int segmentsCount, float height, const Ogre::ColourValue & colour, float alpha) {
		int index = trianglesIndex;
		double increment = 2 * Ogre::Math::PI / segmentsCount;
		double angle = 0.0f;

		// Top circle
		for (int i = 0; i < segmentsCount; ++i) {
			addTriangleVertex(Ogre::Vector3(centre.x + radius * Ogre::Math::Cos(angle), centre.y + height / 2, centre.z + radius * Ogre::Math::Sin(angle)), Ogre::ColourValue(colour.r, colour.g, colour.b, alpha));
			angle += increment;
		}

		addTriangleVertex(Ogre::Vector3(centre.x, centre.y + height / 2, centre.z), Ogre::ColourValue(colour.r, colour.g, colour.b, alpha));

		angle = 0.0f;

		// Bottom circle
		for (int i = 0; i < segmentsCount; ++i) {
			addTriangleVertex(Ogre::Vector3(centre.x + radius * Ogre::Math::Cos(angle), centre.y - height / 2, centre.z + radius * Ogre::Math::Sin(angle)), Ogre::ColourValue(colour.r, colour.g, colour.b, alpha));
			angle += increment;
		}

		addTriangleVertex(Ogre::Vector3(centre.x, centre.y - height / 2, centre.z), Ogre::ColourValue(colour.r, colour.g, colour.b, alpha));

		for (int i = 0; i < segmentsCount; ++i) {
			addTriangleIndices(i + 1 < segmentsCount ? index + i + 1 : index, index + i, index + segmentsCount);

			addTriangleIndices(i + 1 < segmentsCount ? (segmentsCount + 1) + index + i + 1 : (segmentsCount + 1) + index, (segmentsCount + 1) + index + segmentsCount, (segmentsCount + 1) + index + i);

			addQuadIndices(index + i, i + 1 < segmentsCount ? index + i + 1 : index, i + 1 < segmentsCount ? (segmentsCount + 1) + index + i + 1 : (segmentsCount + 1) + index, (segmentsCount + 1) + index + i);
		}
	}

	void Debug::buildCuboid(const Ogre::Vector3 * vertices, const Ogre::ColourValue & colour, float alpha) {
		int index = addLineVertex(vertices[0], Ogre::ColourValue(colour.r, colour.g, colour.b, alpha));
		for (int i = 1; i < 8; ++i) {
			addLineVertex(vertices[i], Ogre::ColourValue(colour.r, colour.g, colour.b, alpha));
		}
		for (int i = 0; i < 4; ++i) {
			addLineIndices(index + i, index + ((i + 1) % 4));
		}
		for (int i = 4; i < 8; ++i) {
			addLineIndices(index + i, i == 7 ? index + 4 : index + i + 1);
		}
		addLineIndices(index + 1, index + 5);
		addLineIndices(index + 2, index + 4);
		addLineIndices(index, 	 index + 6);
		addLineIndices(index + 3, index + 7);
	}

	void Debug::buildFilledCuboid(const Ogre::Vector3 * vertices, const Ogre::ColourValue & colour, float alpha) {
		int index = addTriangleVertex(vertices[0], Ogre::ColourValue(colour.r, colour.g, colour.b, alpha));
		for (int i = 1; i < 8; ++i) {
			addTriangleVertex(vertices[i], Ogre::ColourValue(colour.r, colour.g, colour.b, alpha));
		}

		addQuadIndices(index, 	 index + 1, index + 2, index + 3);
		addQuadIndices(index + 4, index + 5, index + 6, index + 7);

		addQuadIndices(index + 1, index + 5, index + 4, index + 2);
		addQuadIndices(index, 	 index + 3, index + 7, index + 6);

		addQuadIndices(index + 1, index	, index + 6, index + 5);
		addQuadIndices(index + 4, index + 7, index + 3, index + 2);
	}

	void Debug::buildFilledQuad(const Ogre::Vector3 * vertices, const Ogre::ColourValue & colour, float alpha) {
		int index = addTriangleVertex(vertices[0], Ogre::ColourValue(colour.r, colour.g, colour.b, alpha));
		addTriangleVertex(vertices[1], Ogre::ColourValue(colour.r, colour.g, colour.b, alpha));
		addTriangleVertex(vertices[2], Ogre::ColourValue(colour.r, colour.g, colour.b, alpha));
		addTriangleVertex(vertices[3], Ogre::ColourValue(colour.r, colour.g, colour.b, alpha));

		addQuadIndices(index, index + 1, index + 2, index + 3);
	}

	void Debug::buildFilledTriangle(const Ogre::Vector3 * vertices, const Ogre::ColourValue & colour, float alpha) {
		int index = addTriangleVertex(vertices[0], Ogre::ColourValue(colour.r, colour.g, colour.b, alpha));
		addTriangleVertex(vertices[1], Ogre::ColourValue(colour.r, colour.g, colour.b, alpha));
		addTriangleVertex(vertices[2], Ogre::ColourValue(colour.r, colour.g, colour.b, alpha));

		addTriangleIndices(index, index + 1, index + 2);
	}

	void Debug::drawLine(const Ogre::Vector3 & start, const Ogre::Vector3 & end, const Ogre::ColourValue & colour) {
		buildLine(start, end, colour);
	}

	void Debug::drawCircle(const Ogre::Vector3 & centre, float radius, int segmentsCount, const Ogre::ColourValue & colour, bool isFilled) {
		buildCircle(centre, radius, segmentsCount, colour);
		if (isFilled) {
			buildFilledCircle(centre, radius, segmentsCount, colour, fillAlpha);
		}
	}

	void Debug::drawCylinder(const Ogre::Vector3 & centre, float radius, int segmentsCount, float height, const Ogre::ColourValue & colour, bool isFilled) {
		buildCylinder(centre, radius, segmentsCount, height, colour);
		if (isFilled) {
			buildFilledCylinder(centre, radius, segmentsCount, height, colour, fillAlpha);
		}
	}

	void Debug::drawQuad(const Ogre::Vector3 * vertices, const Ogre::ColourValue & colour, bool isFilled) {
		buildQuad(vertices, colour);
		if (isFilled) {
			buildFilledQuad(vertices, colour, fillAlpha);
		}
	}

	void Debug::drawCuboid(const Ogre::Vector3 * vertices, const Ogre::ColourValue & colour, bool isFilled) {
		buildCuboid(vertices, colour);
		if (isFilled) {
			buildFilledCuboid(vertices, colour, fillAlpha);
		}
	}

	void Debug::build() {
		manualObject->beginUpdate(0);
		if (lineVertices.size() > 0 && isEnabled) {
			manualObject->estimateVertexCount(lineVertices.size());
			manualObject->estimateIndexCount(lineIndices.size());
			for (std::list<VertexPair>::iterator i = lineVertices.begin(); i != lineVertices.end(); ++i) {
					manualObject->position(i->first);
					manualObject->colour(i->second);
			}
			for (std::list<int>::iterator i = lineIndices.begin(); i != lineIndices.end(); ++i) {
				manualObject->index(static_cast<Ogre::uint32>(*i));
			}
		}
		manualObject->end();

		manualObject->beginUpdate(1);
		if (triangleVertices.size() > 0 && isEnabled) {
			manualObject->estimateVertexCount(triangleVertices.size());
			manualObject->estimateIndexCount(triangleIndices.size());
			for (std::list<VertexPair>::iterator i = triangleVertices.begin(); i != triangleVertices.end(); ++i) {
					manualObject->position(i->first);
					manualObject->colour(i->second.r, i->second.g, i->second.b, fillAlpha);
			}
			for (std::list<int>::iterator i = triangleIndices.begin(); i != triangleIndices.end(); ++i) {
				manualObject->index(static_cast<Ogre::uint32>(*i));
			}
		}
		manualObject->end();
	}

	void Debug::clear() {
		lineVertices.clear();
		triangleVertices.clear();
		lineIndices.clear();
		triangleIndices.clear();
		linesIndex = trianglesIndex = 0;
	}

	int Debug::addLineVertex(const Ogre::Vector3 & vertex, const Ogre::ColourValue & colour) {
		lineVertices.push_back(VertexPair(vertex, colour));
		return linesIndex++;
	}

	void Debug::addLineIndices(int index1, int index2) {
		lineIndices.push_back(index1);
		lineIndices.push_back(index2);
	}

	int Debug::addTriangleVertex(const Ogre::Vector3 & vertex, const Ogre::ColourValue & colour) {
		triangleVertices.push_back(VertexPair(vertex, colour));
		return trianglesIndex++;
	}

	void Debug::addTriangleIndices(int index1, int index2, int index3) {
		triangleIndices.push_back(index1);
		triangleIndices.push_back(index2);
		triangleIndices.push_back(index3);
	}

	void Debug::addQuadIndices(int index1, int index2, int index3, int index4) {
		triangleIndices.push_back(index1);
		triangleIndices.push_back(index2);
		triangleIndices.push_back(index3);

		triangleIndices.push_back(index1);
		triangleIndices.push_back(index3);
		triangleIndices.push_back(index4);
	}

} /* namespace modules */
} /* namespace i6engine */
