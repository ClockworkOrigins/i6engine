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

/**
 * \addtogroup Graphics
 * @{
 */

#ifndef __I6ENGINE_MODULES_DEBUG_H__
#define __I6ENGINE_MODULES_DEBUG_H__

#include <map>

#include "OGRE/OgreSingleton.h"

typedef std::pair<Ogre::Vector3, Ogre::ColourValue> VertexPair;

#define DEFAULT_ICOSPHERE_RECURSION_LEVEL    1

namespace i6engine {
namespace modules {

	class IcoSphere {
	public:
		struct TriangleIndices {
			int v1, v2, v3;

			TriangleIndices(int _v1, int _v2, int _v3) : v1(_v1), v2(_v2), v3(_v3) {}

			bool operator<(const TriangleIndices & o) const { return v1 < o.v1 && v2 < o.v2 && v3 < o.v3; }
		};

		struct LineIndices {
			int v1, v2;

			LineIndices(int _v1, int _v2) : v1(_v1), v2(_v2) {}

			bool operator==(const LineIndices & o) const {
			    return (v1 == o.v1 && v2 == o.v2) || (v1 == o.v2 && v2 == o.v1);
			}
		};

		IcoSphere();
		~IcoSphere();

		void create(int recursionLevel);
		void addToLineIndices(int baseIndex, std::list<int> * target);
		int addToVertices(std::list<VertexPair> *target, const Ogre::Vector3 & position, const Ogre::ColourValue & colour, float scale);
		void addToTriangleIndices(int baseIndex, std::list<int> * target);

	private:
		int addVertex(const Ogre::Vector3 & vertex);
		void addLineIndices(int index0, int index1);
		void addTriangleLines(int index0, int index1, int index2);
		int getMiddlePoint(int index0, int index1);
		void addFace(int index0, int index1, int index2);

		void removeLineIndices(int index0, int index1);

		std::vector<Ogre::Vector3> vertices;
		std::list<LineIndices> lineIndices;
		std::list<int> triangleIndices;
		std::list<TriangleIndices> faces;
		std::map<long, int> middlePointIndexCache;
		int index;
	};

	class Debug : public Ogre::Singleton<Debug> {
	public:
		Debug(Ogre::SceneManager * _sceneManager, float _fillAlpha);
		~Debug();

		static Debug & getSingleton(void);
		static Debug * getSingletonPtr(void);

		void build();

		void drawLine(const Ogre::Vector3 &start, const Ogre::Vector3 &end, const Ogre::ColourValue &colour);
		void drawCircle(const Ogre::Vector3 &centre, float radius, int segmentsCount, const Ogre::ColourValue& colour, bool isFilled = false);
		void drawCylinder(const Ogre::Vector3 &centre, float radius, int segmentsCount, float height, const Ogre::ColourValue& colour, bool isFilled = false);
		void drawQuad(const Ogre::Vector3 *vertices, const Ogre::ColourValue& colour, bool isFilled = false);
		void drawCuboid(const Ogre::Vector3 *vertices, const Ogre::ColourValue& colour, bool isFilled = false);
		void drawSphere(const Ogre::Vector3 &centre, float radius, const Ogre::ColourValue& colour, bool isFilled = false);

		bool getEnabled() { return isEnabled; }
		void setEnabled(bool _isEnabled) { isEnabled = _isEnabled; }
		void switchEnabled() { isEnabled = !isEnabled; }

		void clear();

	private:
		/**
		 * \brief forbidden
		 */
		Debug(const Debug &);

		/**
		 * \brief forbidden
		 */
		const Debug & operator=(const Debug &);


		Ogre::SceneManager * sceneManager;

		float fillAlpha;

		Ogre::ManualObject * manualObject;

		bool isEnabled;

		std::list<VertexPair> lineVertices, triangleVertices;
		std::list<int> lineIndices, triangleIndices;

		int linesIndex, trianglesIndex;

		void initialise();
		void shutdown();

		void buildLine(const Ogre::Vector3 & start, const Ogre::Vector3 & end, const Ogre::ColourValue & colour, float alpha = 1.0f);
		void buildQuad(const Ogre::Vector3 * vertices, const Ogre::ColourValue & colour, float alpha = 1.0f);
		void buildFilledQuad(const Ogre::Vector3 * vertices, const Ogre::ColourValue & colour, float alpha = 1.0f);
		void buildFilledTriangle(const Ogre::Vector3 *vertices, const Ogre::ColourValue & colour, float alpha = 1.0f);
		void buildCuboid(const Ogre::Vector3 * vertices, const Ogre::ColourValue & colour, float alpha = 1.0f);
		void buildFilledCuboid(const Ogre::Vector3 * vertices, const Ogre::ColourValue & colour, float alpha = 1.0f);

		void buildCircle(const Ogre::Vector3 & centre, float radius, int segmentsCount, const Ogre::ColourValue & colour, float alpha = 1.0f);
		void buildFilledCircle(const Ogre::Vector3 & centre, float radius, int segmentsCount, const Ogre::ColourValue & colour, float alpha = 1.0f);

		void buildCylinder(const Ogre::Vector3 & centre, float radius, int segmentsCount, float height, const Ogre::ColourValue & colour, float alpha = 1.0f);
		void buildFilledCylinder(const Ogre::Vector3 & centre, float radius, int segmentsCount, float height, const Ogre::ColourValue & colour, float alpha = 1.0f);

		int addLineVertex(const Ogre::Vector3 & vertex, const Ogre::ColourValue & colour);
		void addLineIndices(int index1, int index2);

		int addTriangleVertex(const Ogre::Vector3 & vertex, const Ogre::ColourValue & colour);
		void addTriangleIndices(int index1, int index2, int index3);

		void addQuadIndices(int index1, int index2, int index3, int index4);
	};

} /* namespace modules */
} /* namespace i6engine */

#endif /* __I6ENGINE_MODULES_DEBUG_H__ */

/**
 * @}
 */
