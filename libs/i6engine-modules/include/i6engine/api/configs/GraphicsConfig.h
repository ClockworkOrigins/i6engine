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

#ifndef __I6ENGINE_API_GRAPHICSCONFIG_H__
#define __I6ENGINE_API_GRAPHICSCONFIG_H__

#include "i6engine/math/i6eQuaternion.h"
#include "i6engine/math/i6eVector.h"
#include "i6engine/math/i6eVector4.h"

#include "i6engine/api/GameMessageStruct.h"

namespace i6engine {
namespace api {
namespace graphics {
	enum GraphicsMessageTypes {
		GraReset = 0,
		GraCamera,
		GraGraphic,
		GraLine,
		GraLuminous,
		GraMaterial,
		GraCusParam,
		GraMesh,
		GraParticle,
		GraShader,
		GraTerrain,
		GraViewport,
		GraFrustum,
		GraNode,
		GraAmbLight,
		GraViewports,
		GraSkyBox,
		GraSkyDome,
		GraSkyPlane,
		GraResolution,
		GraFullscreen,
		GraPlayAnimation,
		GraSetAnimationSpeed,
		GraStopAnimation,
		GraBillboardSet,
		GraBillboard,
		GraBillboardRemove,
		GraShadowTechnique,
		GraSetLinearFog,
		GraSetExponentialFog,
		GraSetExponentialFog2,
		GraMovableText,
		GraCompositor,
		GraScreenshot,
		GraFPS,
		GraParticleFadeOut
	};

	enum class ShadowTechnique {
		None,
		Stencil_Additive,
		Stencil_Modulative
	};

	struct ISIXE_MODULES_API Resolution {
		uint32_t width;
		uint32_t height;
	};

	enum class BillboardOrigin {
		CenterLeft
	};

	enum class Shader : uint16_t {
		GLOW
	};

	/**
	 * \brief creates a line between to given points, used by DebugDrawer for bounding boxes
	 */
	typedef struct ISIXE_MODULES_API Graphics_Line_Create : GameMessageStruct {
		Vec3 from, to;
		Graphics_Line_Create();
		Graphics_Line_Create(const Vec3 & f, const Vec3 & t);
		Graphics_Line_Create * copy() { return new Graphics_Line_Create(*this); }
	} Graphics_Line_Create;

	/**
	 * \brief creates a node in Graphics subsystem, used by PhysicsNode and MeshAppearanceComponent
	 */
	typedef struct Graphics_Node_Create : GameMessageStruct {
		Vec3 pos;
		Quaternion rot;
		Vec3 scale;
		Graphics_Node_Create();
		Graphics_Node_Create(const int64_t, const int64_t i, const Vec3 & p, const Quaternion & r, const Vec3 & s);
		Graphics_Node_Create * copy() { return new Graphics_Node_Create(*this); }
	} Graphics_Node_Create;

	/**
	 * \brief updates position, rotation and scale of a GraphicsNode
	 */
	typedef struct Graphics_Node_Update : GameMessageStruct {
		Vec3 pos;
		Quaternion rot;
		Vec3 scale;
		Graphics_Node_Update();
		Graphics_Node_Update(const int64_t goid, const int64_t coid, const Vec3 & p, const Quaternion & r, const Vec3 & s);
		Graphics_Node_Update * copy() { return new Graphics_Node_Update(*this); }
	} Graphics_Node_Update;

	/**
	 * \brief deletes a GraphicsNode
	 */
	typedef struct Graphics_Node_Delete : GameMessageStruct {
		Graphics_Node_Delete();
		Graphics_Node_Delete(const int64_t i);
		Graphics_Node_Delete * copy() { return new Graphics_Node_Delete(*this); }
	} Graphics_Node_Delete;

	/**
	 * \brief shuts graphics subsystem down
	 */
	typedef struct Graphics_Graphic_Delete : GameMessageStruct {
		Graphics_Graphic_Delete();
		Graphics_Graphic_Delete * copy() { return new Graphics_Graphic_Delete(*this); }
	} Graphics_Graphic_Delete;

	/**
	 * \brief deletes a light from the GraphicsNode
	 */
	typedef struct Graphics_Luminous_Delete : public GameMessageStruct {
		Graphics_Luminous_Delete();
		Graphics_Luminous_Delete(const int64_t i, const int64_t cid);
		Graphics_Luminous_Delete * copy() { return new Graphics_Luminous_Delete(*this); }
	} Graphics_Luminous_Delete;

	/**
	 * \brief updates the light on a GraphicsNode
	 */
	typedef struct Graphics_Luminous_Update : GameMessageStruct {
		uint16_t lightType;
		Vec3 diffuse;
		Vec3 specular;
		Vec4 attenuation;
		Vec3 direction;
		double spotLightRangeInner;
		double spotLightRangeOuter;
		Vec3 position;
		Graphics_Luminous_Update();
		Graphics_Luminous_Update(const int64_t i, const int64_t cid, const uint16_t lt, const Vec3 & d, const Vec3 & s, const Vec4 & a, const Vec3 & dir, double si, double so, const Vec3 & p);
		Graphics_Luminous_Update * copy() { return new Graphics_Luminous_Update(*this); }
	} Graphics_Luminous_Update;

	/**
	 * \brief creates a camera node on a GraphicsNode
	 */
	typedef struct Graphics_Camera_Create : GameMessageStruct {
		Vec3 pos;
		Vec3 look;
		double nearClip;
		double fov;
		Graphics_Camera_Create();
		Graphics_Camera_Create(const int64_t i);
		Graphics_Camera_Create(const int64_t i, const int64_t, const Vec3 & p, const Vec3 & l, const double nc, double fo);
		Graphics_Camera_Create * copy() { return new Graphics_Camera_Create(*this); }
	} Graphics_Camera_Create;

	/**
	 * \brief updates a camera node on a GraphicsNode
	 */
	typedef struct Graphics_Camera_Update : GameMessageStruct {
		Vec3 pos;
		Vec3 look;
		double nearClip;
		double fov;
		Graphics_Camera_Update();
		Graphics_Camera_Update(const int64_t goid, const int64_t cid, const Vec3 & p, const Vec3 & l, const double nc, const double fo);
		Graphics_Camera_Update * copy() { return new Graphics_Camera_Update(*this); }
	} Graphics_Camera_Update;

	/**
	 * \brief sets frustum for a camera node on a GraphicsNode
	 */
	typedef struct Graphics_CameraFrustum_Update : GameMessageStruct {
		double left;
		double right;
		double top;
		double bottom;
		Graphics_CameraFrustum_Update() {}
		Graphics_CameraFrustum_Update(const int64_t goid, const int64_t cid, double l, double r, double t, double b) : GameMessageStruct(cid, goid), left(l), right(r), top(t), bottom(b) {
		}
		Graphics_CameraFrustum_Update * copy() { return new Graphics_CameraFrustum_Update(*this); }
	} Graphics_CameraFrustum_Update;

	/**
	 * \brief deletes a camera node from a GraphicsNode
	 */
	typedef struct Graphics_Camera_Delete : GameMessageStruct {
		Graphics_Camera_Delete();
		Graphics_Camera_Delete(const int64_t i);
		Graphics_Camera_Delete * copy() { return new Graphics_Camera_Delete(*this); }
	} Graphics_Camera_Delete;

	/**
	 * \brief creates a shader for a GraphicsNode (deprecated)
	 */
	typedef struct Graphics_Shader_Create : GameMessageStruct {
		Shader shader;
		Graphics_Shader_Create();
		Graphics_Shader_Create(const int64_t i, const Shader s);
		Graphics_Shader_Create * copy() { return new Graphics_Shader_Create(*this); }
	} Graphics_Shader_Create;

	/**
	 * \brief deletes a shader of a GraphicsNode (deprecated)
	 */
	typedef struct Graphics_Shader_Delete : GameMessageStruct {
		Shader shader;
		Graphics_Shader_Delete();
		Graphics_Shader_Delete(const int64_t i, const Shader s);
		Graphics_Shader_Delete * copy() { return new Graphics_Shader_Delete(*this); }
	} Graphics_Shader_Delete;

	/**
	 * \brief creates a mesh on a GraphicsNode
	 */
	typedef struct Graphics_Mesh_Create : GameMessageStruct {
		std::string mesh;
		bool visible;
		Vec3 pos;
		Quaternion rot;
		Vec3 scale;
		Graphics_Mesh_Create();
		Graphics_Mesh_Create(const int64_t i, const int64_t, const std::string & m, const bool b, const Vec3 & p, const Quaternion & r, const Vec3 & s);
		Graphics_Mesh_Create * copy() { return new Graphics_Mesh_Create(*this); }
	} Graphics_Mesh_Create;

	/**
	 * \brief updates the mesh on a GraphicsNode
	 */
	typedef struct Graphics_Mesh_Update : GameMessageStruct {
		std::string mesh;
		bool visible;
		Graphics_Mesh_Update();
		Graphics_Mesh_Update(const int64_t, const int64_t, const std::string &, const bool);
		Graphics_Mesh_Update * copy() { return new Graphics_Mesh_Update(*this); }
	} Graphics_Mesh_Update;

	/**
	 * \brief deletes mesh on a GraphicsNode
	 */
	typedef struct Graphics_Mesh_Delete : GameMessageStruct {
		Graphics_Mesh_Delete();
		Graphics_Mesh_Delete(const int64_t goid, const int64_t cid);
		Graphics_Mesh_Delete * copy() { return new Graphics_Mesh_Delete(*this); }
	} Graphics_Mesh_Delete;

	/**
	 * \brief changes material of a mesh
	 */
	typedef struct Graphics_Material_Update : GameMessageStruct {
		std::string material;
		Graphics_Material_Update();
		Graphics_Material_Update(const int64_t, const int64_t i, const std::string & mat);
		Graphics_Material_Update * copy() { return new Graphics_Material_Update(*this); }
	} Graphics_Material_Update;

	/**
	 * \brief Message to change a custom parameter of a shader script
	 * \sa GraphicsNode::setCustomParameter()
	 */
	typedef struct Graphics_CusParam_Update : GameMessageStruct {
		uint32_t num;
		Vec4 value;
		Graphics_CusParam_Update() : GameMessageStruct(), num(0), value() {}
		Graphics_CusParam_Update(const int64_t cid, const int64_t goid, const uint32_t n, const Vec4 & v) : GameMessageStruct(cid, goid), num(n), value(v) {
		}
		Graphics_CusParam_Update * copy() { return new Graphics_CusParam_Update(*this); }
	} Graphics_CusParam_Update;

	/**
	 * \brief creates a Terrain in graphics subsystem
	 */
	typedef struct Graphics_Terrain_Create : GameMessageStruct {
		std::string heightmap;
		double size;
		double inputScale;
		std::vector<std::tuple<double, std::string, std::string, double, double>> layers;
		int64_t minX;
		int64_t minY;
		int64_t maxX;
		int64_t maxY;
		Graphics_Terrain_Create(const int64_t, const int64_t, const std::string & h, const double s, double i, const std::vector<std::tuple<double, std::string, std::string, double, double>> & l, int64_t miX, int64_t miY, int64_t maX, int64_t maY);
		Graphics_Terrain_Create * copy() { return new Graphics_Terrain_Create(*this); }
	} Graphics_Terrain_Create;

	/**
	 * \brief updates a terrain (sense?)
	 */
	typedef struct Graphics_Terrain_Update : GameMessageStruct {
		std::string heightmap;
		double size;
		double inputScale;
		std::vector<std::tuple<double, std::string, std::string, double, double>> layers;
		int64_t minX;
		int64_t minY;
		int64_t maxX;
		int64_t maxY;
		Graphics_Terrain_Update(const int64_t goID, const std::string & h, const double s, double i, const std::vector<std::tuple<double, std::string, std::string, double, double>> & l, int64_t miX, int64_t miY, int64_t maX, int64_t maY);
		Graphics_Terrain_Update * copy() { return new Graphics_Terrain_Update(*this); }
	} Graphics_Terrain_Update;

	/**
	 * \brief deletes a terrain
	 */
	typedef struct Graphics_Terrain_Delete : GameMessageStruct {
		int64_t comp;
		Graphics_Terrain_Delete();
		Graphics_Terrain_Delete(const int64_t i, const int64_t c);
		Graphics_Terrain_Delete * copy() { return new Graphics_Terrain_Delete(*this); }
	} Graphics_Terrain_Delete;

	/**
	 * \brief creates a viewport
	 */
	typedef struct Graphics_Viewport_Create : GameMessageStruct {
		double left, top, width, height;
		double red, green, blue, alpha;
		Graphics_Viewport_Create();
		Graphics_Viewport_Create(const int64_t i, const int64_t, const double l, const double t, const double w, const double h, const double re, const double gr, const double bl, const double al);
		Graphics_Viewport_Create * copy() { return new Graphics_Viewport_Create(*this); }
	} Graphics_Viewport_Create;

	/**
	 * \brief updates dimensions and color of a viewport
	 */
	typedef struct Graphics_Viewport_Update : GameMessageStruct {
		double left, top, width, height;
		double red, green, blue, alpha;
		Graphics_Viewport_Update();
		Graphics_Viewport_Update(const int64_t i, const int64_t, const double l, const double t, const double w, const double h, const double re, const double gr, const double bl, const double al);
		Graphics_Viewport_Update * copy() { return new Graphics_Viewport_Update(*this); }
	} Graphics_Viewport_Update;

	/**
	 * \brief deletes a viewport
	 */
	typedef struct Graphics_Viewports_Delete : GameMessageStruct {
		Graphics_Viewports_Delete();
		Graphics_Viewports_Delete * copy() { return new Graphics_Viewports_Delete(*this); }
	} Graphics_Viewports_Delete;

	/**
	 * \brief creates a particle effect on given position
	 */
	typedef struct Graphics_Particle_Create : GameMessageStruct {
		std::string emitterName;
		Vec3 pos;
		Graphics_Particle_Create();
		Graphics_Particle_Create(const int64_t i, const int64_t, const std::string & s, const Vec3 & p);
		Graphics_Particle_Create * copy() { return new Graphics_Particle_Create(*this); }
	} Graphics_Particle_Create;

	/**
	 * \brief updates a particle effect
	 */
	typedef struct Graphics_Particle_Update : GameMessageStruct {
		Graphics_Particle_Update();
		Graphics_Particle_Update(const int64_t, const int64_t);
		Graphics_Particle_Update * copy() { return new Graphics_Particle_Update(*this); }
	} Graphics_Particle_Update;

	/**
	 * \brief lets a particle fade out
	 */
	typedef struct Graphics_ParticleFadeOut_Update : GameMessageStruct {
		Graphics_ParticleFadeOut_Update(const int64_t goid, const int64_t coid) : GameMessageStruct(coid, goid) {
		}
		Graphics_ParticleFadeOut_Update * copy() {
			return new Graphics_ParticleFadeOut_Update(*this);
		}
	} Graphics_ParticleFadeOut_Update;

	/**
	 * \brief deletes a particle effect
	 */
	typedef struct Graphics_Particle_Delete : GameMessageStruct {
		Graphics_Particle_Delete();
		Graphics_Particle_Delete(const int64_t goid, const int64_t cid);
		Graphics_Particle_Delete * copy() { return new Graphics_Particle_Delete(*this); }
	} Graphics_Particle_Delete;

	/**
	 * \brief creates an ambient light
	 */
	typedef struct Graphics_AmbLight_Update : GameMessageStruct {
		double red;
		double green;
		double blue;
		Graphics_AmbLight_Update(double r, double g, double b);
		Graphics_AmbLight_Update * copy() { return new Graphics_AmbLight_Update(*this); }
	} Graphics_AmbLight_Update;

	/**
	 * \brief plays an animation
	 */
	typedef struct Graphics_PlayAnimation_Update : GameMessageStruct {
		std::string anim;
		bool looping;
		double offsetPercent;
		Graphics_PlayAnimation_Update(const int64_t, const int64_t, const std::string & a, bool l, double o);
		Graphics_PlayAnimation_Update * copy() {
			return new Graphics_PlayAnimation_Update(*this);
		}
	} Graphics_PlayAnimation_Update;

	/**
	 * \brief sets animation speed
	 */
	typedef struct Graphics_SetAnimationSpeed_Update : GameMessageStruct {
		double speed;
		Graphics_SetAnimationSpeed_Update(const int64_t, const int64_t, double s);
		Graphics_SetAnimationSpeed_Update * copy() {
			return new Graphics_SetAnimationSpeed_Update(*this);
		}
	} Graphics_SetAnimationSpeed_Update;

	/**
	* \brief stop an animation
	*/
	typedef struct Graphics_StopAnimation_Update : GameMessageStruct {
		Graphics_StopAnimation_Update(const int64_t, const int64_t);
		Graphics_StopAnimation_Update * copy() {
			return new Graphics_StopAnimation_Update(*this);
		}
	} Graphics_StopAnimation_Update;

	/**
	 * \brief creates a billboard set
	 */
	typedef struct Graphics_BillboardSet_Create : GameMessageStruct {
		std::string material;
		double width;
		double height;
		BillboardOrigin origin;
		Graphics_BillboardSet_Create(const int64_t goid, const int64_t coid, const std::string & m, double w, double h, BillboardOrigin o);
		Graphics_BillboardSet_Create * copy() {
			return new Graphics_BillboardSet_Create(*this);
		}
	} Graphics_BillboardSet_Create;

	/**
	 * \brief creates a billboard
	 */
	typedef struct Graphics_Billboard_Update : GameMessageStruct {
		std::string identifier;
		Vec3 offset;
		double width;
		double height;
		double u0;
		double v0;
		double u1;
		double v1;
		Graphics_Billboard_Update(const int64_t goid, const int64_t coid, const std::string & i, const Vec3 & o, double w, double h, double u_0, double v_0, double u_1, double v_1);
		Graphics_Billboard_Update * copy() {
			return new Graphics_Billboard_Update(*this);
		}
	} Graphics_Billboard_Update;

	/**
	 * \brief removes a billboard
	 */
	typedef struct Graphics_BillboardRemove_Update : GameMessageStruct {
		std::string identifier;
		Graphics_BillboardRemove_Update(const int64_t goid, const int64_t coid, const std::string & i);
		Graphics_BillboardRemove_Update * copy() {
			return new Graphics_BillboardRemove_Update(*this);
		}
	} Graphics_BillboardRemove_Update;

	/**
	 * \brief removes a billboard set
	 */
	typedef struct Graphics_BillboardSet_Delete : GameMessageStruct {
		Graphics_BillboardSet_Delete(const int64_t goid, const int64_t coid);
		Graphics_BillboardSet_Delete * copy() {
			return new Graphics_BillboardSet_Delete(*this);
		}
	} Graphics_BillboardSet_Delete;

	/**
	 * \brief creates sky box
	 */
	typedef struct Graphics_SkyBox_Create : GameMessageStruct {
		std::string material;
		double distance;
		bool renderFirst;
		Graphics_SkyBox_Create(const std::string & m, double d, bool r);
		Graphics_SkyBox_Create * copy() {
			return new Graphics_SkyBox_Create(*this);
		}
	} Graphics_SkyBox_Create;

	/**
	 * \brief creates sky dome
	 */
	typedef struct Graphics_SkyDome_Create : GameMessageStruct {
		std::string material;
		int32_t curvature;
		double tiles;
		double distance;
		bool renderFirst;
		Graphics_SkyDome_Create(const std::string & m, int32_t c, double t, double d, bool r);
		Graphics_SkyDome_Create * copy() {
			return new Graphics_SkyDome_Create(*this);
		}
	} Graphics_SkyDome_Create;

	/**
	* \brief creates sky plane
	*/
	typedef struct Graphics_SkyPlane_Create : GameMessageStruct {
		std::string material;
		double distance;
		int32_t direction;
		int32_t size;
		double tiles;
		bool renderFirst;
		double curvature;
		int32_t xSegments;
		int32_t ySegments;
		Graphics_SkyPlane_Create(const std::string & m, double d, int32_t dir, int32_t s, double t, bool r, double c, int32_t x, int32_t y);
		Graphics_SkyPlane_Create * copy() {
			return new Graphics_SkyPlane_Create(*this);
		}
	} Graphics_SkyPlane_Create;

	/**
	* \brief deletes sky box
	*/
	typedef struct Graphics_SkyBox_Delete : GameMessageStruct {
		Graphics_SkyBox_Delete();
		Graphics_SkyBox_Delete * copy() {
			return new Graphics_SkyBox_Delete(*this);
		}
	} Graphics_SkyBox_Delete;

	/**
	* \brief deletes sky dome
	*/
	typedef struct Graphics_SkyDome_Delete : GameMessageStruct {
		Graphics_SkyDome_Delete();
		Graphics_SkyDome_Delete * copy() {
			return new Graphics_SkyDome_Delete(*this);
		}
	} Graphics_SkyDome_Delete;

	/**
	 * \brief deletes sky plane
	 */
	typedef struct Graphics_SkyPlane_Delete : GameMessageStruct {
		Graphics_SkyPlane_Delete();
		Graphics_SkyPlane_Delete * copy() {
			return new Graphics_SkyPlane_Delete(*this);
		}
	} Graphics_SkyPlane_Delete;

	/**
	 * \brief updates resolution of render window
	 */
	typedef struct Graphics_Resolution_Update : GameMessageStruct {
		graphics::Resolution resolution;
		Graphics_Resolution_Update(const graphics::Resolution & r);
		Graphics_Resolution_Update * copy() {
			return new Graphics_Resolution_Update(*this);
		}
	} Graphics_Resolution_Update;

	/**
	 * \brief updates fullscreen mode of render window
	 */
	typedef struct Graphics_Fullscreen_Update : GameMessageStruct {
		bool fullscreen;
		Graphics_Fullscreen_Update(bool fs);
		Graphics_Fullscreen_Update * copy() {
			return new Graphics_Fullscreen_Update(*this);
		}
	} Graphics_Fullscreen_Update;

	/**
	 * \brief updates shadow technique
	 */
	typedef struct Graphics_ShadowTechnique_Update : GameMessageStruct {
		ShadowTechnique shadowTechnique;
		Graphics_ShadowTechnique_Update(ShadowTechnique st);
		Graphics_ShadowTechnique_Update * copy() {
			return new Graphics_ShadowTechnique_Update(*this);
		}
	} Graphics_ShadowTechnique_Update;

	/**
	 * \brief sets linear fog
	 */
	typedef struct Graphics_SetLinearFog_Update : GameMessageStruct {
		Vec3 colour;
		double start;
		double end;
		Graphics_SetLinearFog_Update(const Vec3 & c, double s, double e) : GameMessageStruct(), colour(c), start(s), end(e) {
		}
		Graphics_SetLinearFog_Update * copy() {
			return new Graphics_SetLinearFog_Update(*this);
		}
	} Graphics_SetLinearFog_Update;

	/**
	 * \brief sets exponential fog
	 */
	typedef struct Graphics_SetExponentialFog_Update : GameMessageStruct {
		Vec3 colour;
		double density;
		Graphics_SetExponentialFog_Update(const Vec3 & c, double d) : GameMessageStruct(), colour(c), density(d) {
		}
		Graphics_SetExponentialFog_Update * copy() {
			return new Graphics_SetExponentialFog_Update(*this);
		}
	} Graphics_SetExponentialFog_Update;

	/**
	 * \brief creates a MovableText on a GraphicsNode
	 */
	typedef struct Graphics_MovableText_Create : GameMessageStruct {
		int64_t targetID;
		std::string font;
		std::string text;
		uint16_t size;
		Vec3 colour;
		Graphics_MovableText_Create(int64_t goid, int64_t coid, int64_t tid, const std::string & f, const std::string & t, uint16_t s, const Vec3 & c) : GameMessageStruct(coid, goid), targetID(tid), font(f), text(t), size(s), colour(c) {
		}
		Graphics_MovableText_Create * copy() {
			return new Graphics_MovableText_Create(*this);
		}
	} Graphics_MovableText_Create;

	/**
	 * \brief updates a MovableText on a GraphicsNode
	 */
	typedef struct Graphics_MovableText_Update : GameMessageStruct {
		std::string font;
		std::string text;
		uint16_t size;
		Vec3 colour;
		Graphics_MovableText_Update(int64_t goid, int64_t coid, const std::string & f, const std::string & t, uint16_t s, const Vec3 & c) : GameMessageStruct(coid, goid), font(f), text(t), size(s), colour(c) {
		}
		Graphics_MovableText_Update * copy() {
			return new Graphics_MovableText_Update(*this);
		}
	} Graphics_MovableText_Update;

	/**
	 * \brief deletes a MovableText on a GraphicsNode
	 */
	typedef struct Graphics_MovableText_Delete : GameMessageStruct {
		Graphics_MovableText_Delete(int64_t goid, int64_t coid) : GameMessageStruct(coid, goid) {
		}
		Graphics_MovableText_Delete * copy() {
			return new Graphics_MovableText_Delete(*this);
		}
	} Graphics_MovableText_Delete;

	/**
	 * \brief enables/disables a compositor for a viewport
	 */
	typedef struct Graphics_Compositor_Update : GameMessageStruct {
		std::string compositor;
		bool enabled;
		Graphics_Compositor_Update(int64_t goid, int64_t coid, const std::string & c, bool e) : GameMessageStruct(coid, goid), compositor(c), enabled(e) {
		}
		Graphics_Compositor_Update * copy() {
			return new Graphics_Compositor_Update(*this);
		}
	} Graphics_Compositor_Update;

	/**
	 * \brief takes a screenshot
	 */
	typedef struct Graphics_Screenshot_Create : GameMessageStruct {
		std::string prefix;
		std::string suffix;
		Graphics_Screenshot_Create(const std::string & p, const std::string & s) : GameMessageStruct(), prefix(p), suffix(s) {
		}
		Graphics_Screenshot_Create * copy() {
			return new Graphics_Screenshot_Create(*this);
		}
	} Graphics_Screenshot_Create;

	/**
	 * \brief shows FPS
	 */
	typedef struct Graphics_FPS_Create : GameMessageStruct {
		Graphics_FPS_Create() : GameMessageStruct() {
		}
		Graphics_FPS_Create * copy() {
			return new Graphics_FPS_Create(*this);
		}
	} Graphics_FPS_Create;

	/**
	 * \brief hides FPS
	 */
	typedef struct Graphics_FPS_Delete : GameMessageStruct {
		Graphics_FPS_Delete() : GameMessageStruct() {
		}
		Graphics_FPS_Delete * copy() {
			return new Graphics_FPS_Delete(*this);
		}
	} Graphics_FPS_Delete;

} /* namespace graphics */
} /* namespace api */
} /* namespace i6engine */

#endif /* __I6ENGINE_API_GRAPHICSCONFIG_H__ */

/**
 * @}
 */
