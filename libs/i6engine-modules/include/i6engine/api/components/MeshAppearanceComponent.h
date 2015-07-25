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
 * \addtogroup Components
 * @{
 */

#ifndef __I6ENGINE_API_MESHAPPEARANCECOMPONENT_H__
#define __I6ENGINE_API_MESHAPPEARANCECOMPONENT_H__

#include "i6engine/math/i6eQuaternion.h"
#include "i6engine/math/i6eVector.h"

#include "i6engine/api/components/Component.h"

namespace i6engine {
namespace math {
	class i6eVector4;
} /* namespace math */
} /* namespace i6engine */

typedef i6engine::math::i6eVector4 Vec4;

namespace i6engine {
namespace api {

	/**
	 * \class MeshAppearanceComponent
	 * \brief Tells the engine what model/mesh to use when rendering the GameObject.
	 * For creating a MeshAppearanceComponent, these keys are possible:
	 * | Name | Required | Type | Description | Public |
	 * |------|----------|------| ----------- | ------------ |
	 * | mesh | yes | std::string | mesh file | yes |
	 * | visibility | yes | bool | is this mesh visible or not | yes |
	 * | pos | yes | Vec3 | relative position to SceneNode | yes |
	 * | rot | yes | Vec3 | relative rotation to SceneNode | yes |
	 * | scale | yes | Vec3 | relative scale to SceneNode | yes |
	 * | material | no | std::string | optional change of the material on the mesh | yes |
	 */
	class ISIXE_MODULES_API MeshAppearanceComponent : public Component {
	public:
		/**
		 * \brief Constructor of the component
		 *
		 * Sets visibility and ID
		 */
		MeshAppearanceComponent(const int64_t id, const attributeMap & params);

		/**
		 * \brief Destructor
		 */
		~MeshAppearanceComponent();

		/**
		* \brief creates the Component with given attributeMap
		*/
		static ComPtr createC(const int64_t id, const attributeMap & params);

		/**
		 * \brief returns the position of the mesh
		 */
		Vec3 getPosition() const {
			return _position;
		}

		/**
		 * \brief Sets the GameObject's visibility.
		 *
		 * Sets the visibility of the component and sends message to MessagingController
		 *
		 * \param[in] visible Visibility of the component (true/false)
		 */
		void setVisibility(const bool visible);

		/**
		 * \brief Sets the GameObject's mesh.
		 *
		 * Sets the mesh of the component and sends message to MessagingController
		 *
		 * \param[in] meshName Name of the mesh of the component
		 */
		void setMesh(const std::string & meshName);

		/**
		 * \brief Sets a parameter for a shader script.
		 *
		 * The parameter is set for all SubEntities of this mesh.
		 *
		 * \param num The number, the parameter has in the shader script.
		 * \param value The Value to set the parameter to.
		 */
		void setCustomParameter(uint32_t num, const Vec4 & value);

		/**
		 * \brief Gets the GameObject's visibility.
		 *
		 * Gets the GameObject's visibility
		 *
		 * \return Visibility of the GameObject
		 */
		inline bool getVisibility() const { return _isVisible; }

		/**
		 * \brief Gets the GameObject's mesh name.
		 *
		 * Gets the GameObject's mesh name
		 *
		 * \return Name of the mesh file
		 */
		inline std::string getMesh() const { return _meshName; }

		/**
		 * \brief sets the material of the meshEntity to the given material
		 */
		void setMaterial(const std::string & materialName);

		/**
		 * \brief plays given animation
		 */
		void playAnimation(const std::string & anim, bool looping, double offsetPercent);

		/**
		 * \brief set animation speed for current animation
		 */
		void setAnimationSpeed(double animationSpeed);

		/**
		 * \brief stops current animation
		 */
		void stopAnimation();

		/**
		 * \brief synchronizes the Components state
		 */
		attributeMap synchronize() const override;

		std::string getTemplateName() const override {
			return "MeshAppearance";
		}

		std::vector<componentOptions> getComponentOptions() override;

		/**
		 * \brief shows the bounding box of this mesh in given colour
		 */
		void drawBoundingBox(const Vec3 & colour) const;

		/**
		 * \brief removes the bounding box of this mesh
		 */
		void removeBoundingBox() const;

	private:
		/**
		 * Name of the mesh file of the component
		 */
		std::string _meshName;

		/**
		 * Visibility of the component
		 */
		bool _isVisible;

		Vec3 _position;
		Quaternion _rotation;
		Vec3 _scale;

		std::string _material;

		void Init() override;

		virtual std::pair<AddStrategy, int64_t> howToAdd(const ComPtr & comp) const override;

		/**
		 * \brief Sends message to MessagingController
		 * Sends message to MessagingController containing ObjectID, meshname, visibility and component
		 */
		void sendUpdateMessage();
	};

} /* namespace api */
} /* namespace i6engine */

#endif /* __I6ENGINE_API_MESHAPPEARANCECOMPONENT_H__ */

/**
 * @}
 */
