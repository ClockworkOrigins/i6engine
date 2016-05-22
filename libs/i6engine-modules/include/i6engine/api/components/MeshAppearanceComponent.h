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
 * \addtogroup Components
 * @{
 */

#ifndef __I6ENGINE_API_MESHAPPEARANCECOMPONENT_H__
#define __I6ENGINE_API_MESHAPPEARANCECOMPONENT_H__

#include <mutex>

#include "i6engine/math/i6eQuaternion.h"
#include "i6engine/math/i6eVector.h"

#include "i6engine/api/components/Component.h"

namespace i6e {
namespace math {
	class i6eVector4;
} /* namespace math */
} /* namespace i6e */

typedef i6e::math::i6eVector4 Vec4;

namespace i6e {
namespace modules {
	class MeshComponent;
} /* namespace modules */
namespace api {

	typedef std::pair<Vec3, Quaternion> Transform;

	/**
	 * \class MeshAppearanceComponent
	 * \brief Tells the engine what model/mesh to use when rendering the GameObject.
	 * For creating a MeshAppearanceComponent, these keys are possible:
	 * | Name | Required | Type | Description | Public |
	 * |------|----------|------| ----------- | ------------ |
	 * | mesh | yes | std::string | mesh file | yes |
	 * | visibility | no | bool | is this mesh visible or not, default is true | yes |
	 * | pos | yes | Vec3 | relative position to SceneNode | yes |
	 * | rot | yes | Vec3 | relative rotation to SceneNode | yes |
	 * | scale | yes | Vec3 | relative scale to SceneNode | yes |
	 * | material | no | std::string | optional change of the material on the mesh | yes |
	 * | shadowCasting | no | bool | defines whether a mesh casts shadows or not, default is true | yes |
	 */
	class ISIXE_MODULES_API MeshAppearanceComponent : public Component {
		friend class modules::MeshComponent;

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

		/**
		 * \brief attaches a GameObject to a bone of this mesh
		 */
		void attachGameObjectToBone(const api::GOPtr & go, const std::string & boneName);

		/**
		 * \brief detaches a GameObject from a bone
		 */
		void detachGameObjectFromBone(const api::GOPtr & go, const std::string & boneName);

		/**
		 * \brief adds an event being triggered, when a specified frame time of the animation is reached
		 * note that all events are removed when another animation is played!
		 */
		void addAnimationFrameEvent(uint64_t frameTime, const std::function<void(void)> & func) const;

		/**
		 * \brief returns the Transform for one bone
		 */
		Transform getBoneTransform(const std::string & name) const;

		/**
		 * \brief enables or disables shadow casting for this mesh
		 */
		void setShadowCasting(bool enabled);

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

		bool _shadowCasting;

		mutable std::mutex _boneTransformLock;
		std::map<std::string, Transform> _boneTransforms;

		void Init() override;

		void Finalize() override;

		virtual std::pair<AddStrategy, int64_t> howToAdd(const ComPtr & comp) const override;

		/**
		 * \brief Sends message to MessagingController
		 * Sends message to MessagingController containing ObjectID, meshname, visibility and component
		 */
		void sendUpdateMessage() const;

		void updateBoneTransforms(const std::map<std::string, Transform> & boneTransformMap);
	};

} /* namespace api */
} /* namespace i6e */

#endif /* __I6ENGINE_API_MESHAPPEARANCECOMPONENT_H__ */

/**
 * @}
 */
