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

#include "i6engine/api/configs/GraphicsConfig.h"

namespace i6e {
namespace api {
namespace graphics {

	// Line
	Graphics_Line_Create::Graphics_Line_Create() : GameMessageStruct() {}
	Graphics_Line_Create::Graphics_Line_Create(const Vec3 & f, const Vec3 & t, const Vec3 & c) : GameMessageStruct(), from(f), to(t), colour(c) {
	}
	Graphics_Line_Create::Graphics_Line_Create(int64_t goid, int64_t coid, const Vec3 & f, const Vec3 & t, const Vec3 & c) : GameMessageStruct(coid, goid), from(f), to(t), colour(c) {
	}
	Graphics_Line_Delete::Graphics_Line_Delete(int64_t goid, int64_t coid) : GameMessageStruct(coid, goid) {
	}

	// Graphics
	Graphics_Node_Delete::Graphics_Node_Delete() : GameMessageStruct() {}
	Graphics_Node_Delete::Graphics_Node_Delete(const int64_t i) : GameMessageStruct(i, -1) {
	}

	// Graphics
	Graphics_Graphic_Delete::Graphics_Graphic_Delete() : GameMessageStruct() {}

	// Luminous
	Graphics_Luminous_Delete::Graphics_Luminous_Delete() : GameMessageStruct() {}
	Graphics_Luminous_Delete::Graphics_Luminous_Delete(const int64_t i, const int64_t cid) : GameMessageStruct(cid, i) {
	}

	Graphics_Luminous_Update::Graphics_Luminous_Update() : GameMessageStruct() {}
	Graphics_Luminous_Update::Graphics_Luminous_Update(const int64_t i, const int64_t cid, const uint16_t lt, const Vec3 & d, const Vec3 & s, const Vec4 & a, const Vec3 & dir, double si, double so, const Vec3 & p) : GameMessageStruct(cid, i), lightType(lt), diffuse(d), specular(s), attenuation(a), direction(dir), spotLightRangeInner(si), spotLightRangeOuter(so), position(p) {
	}

	// Camera
	Graphics_Camera_Create::Graphics_Camera_Create() : GameMessageStruct() {}
	Graphics_Camera_Create::Graphics_Camera_Create(const int64_t i) : GameMessageStruct(i, -1) {
	}
	Graphics_Camera_Create::Graphics_Camera_Create(const int64_t goID, const int64_t coID, const Vec3 & p, const Vec3 & l, const double nc, double a, double fo) : GameMessageStruct(coID, goID), pos(p), look(l), nearClip(nc), aspect(a), fov(fo) {
	}

	Graphics_Camera_Update::Graphics_Camera_Update() : GameMessageStruct() {}
	Graphics_Camera_Update::Graphics_Camera_Update(const int64_t goid, const int64_t cid, const Vec3 & p, const Vec3 & l, const double nc, double a, const double fo) : GameMessageStruct(cid, goid), pos(p), look(l), nearClip(nc), aspect(a), fov(fo) {
	}

	Graphics_Camera_Delete::Graphics_Camera_Delete() : GameMessageStruct() {}
	Graphics_Camera_Delete::Graphics_Camera_Delete(const int64_t i) : GameMessageStruct(i, -1) {
	}

	// Shader
	Graphics_Shader_Create::Graphics_Shader_Create() : GameMessageStruct() {}
	Graphics_Shader_Create::Graphics_Shader_Create(const int64_t i, const Shader s) : GameMessageStruct(i, -1), shader(s) {
	}

	Graphics_Shader_Delete::Graphics_Shader_Delete() : GameMessageStruct() {}
	Graphics_Shader_Delete::Graphics_Shader_Delete(const int64_t i, const Shader s) : GameMessageStruct(i, -1), shader(s) {
	}

	// MeshAppearance
	Graphics_Mesh_Create::Graphics_Mesh_Create() : GameMessageStruct() {}
	Graphics_Mesh_Create::Graphics_Mesh_Create(const int64_t goID, const int64_t coID, const std::string & m, const bool b, const Vec3 & p, const Quaternion & r, const Vec3 & s) : GameMessageStruct(coID, goID), mesh(m), visible(b), pos(p), rot(r), scale(s) {
	}

	Graphics_Mesh_Update::Graphics_Mesh_Update() : GameMessageStruct() {}
	Graphics_Mesh_Update::Graphics_Mesh_Update(const int64_t goID, const int64_t coID, const std::string & s, const bool b) : GameMessageStruct(coID, goID), mesh(s), visible(b) {
	}

	Graphics_MeshVisibility_Update::Graphics_MeshVisibility_Update(const int64_t goID, const int64_t coID, bool v) : GameMessageStruct(coID, goID), visible(v) {
	}

	Graphics_Mesh_Delete::Graphics_Mesh_Delete() : GameMessageStruct() {}
	Graphics_Mesh_Delete::Graphics_Mesh_Delete(const int64_t goid, const int64_t cid) : GameMessageStruct(cid, goid) {
	}

	Graphics_Node_Update::Graphics_Node_Update() : GameMessageStruct() {}
	Graphics_Node_Update::Graphics_Node_Update(const int64_t goID, const int64_t coID, const Vec3 & p, const Quaternion & r, const Vec3 & s) : GameMessageStruct(coID, goID), pos(p), rot(r), scale(s) {
	}

	Graphics_Node_Create::Graphics_Node_Create() : GameMessageStruct() {}
	Graphics_Node_Create::Graphics_Node_Create(const int64_t goID, int64_t cid, const Vec3 & p, const Quaternion & r, const Vec3 & s) : GameMessageStruct(goID, -1), pos(p), rot(r), scale(s), coid(cid) {
	}

	// Material
	Graphics_Material_Update::Graphics_Material_Update() : GameMessageStruct() {}
	Graphics_Material_Update::Graphics_Material_Update(const int64_t goID, const int64_t coID, const std::string & mat) : GameMessageStruct(goID, coID), material(mat) {
	}

	// Terrain
	Graphics_TerrainHeightmap_Create::Graphics_TerrainHeightmap_Create(const int64_t coID, const std::string & h, const double s, double i, uint32_t v, const std::vector<std::tuple<double, std::string, std::string, double, double>> & l, int64_t miX, int64_t miY, int64_t maX, int64_t maY) : GameMessageStruct(coID, -1), heightmap(h), size(s), inputScale(i), vertices(v), layers(l), minX(miX), minY(miY), maxX(maX), maxY(maY) {
	}
	Graphics_TerrainHeightdata_Create::Graphics_TerrainHeightdata_Create(const int64_t coID, const std::vector<std::vector<double>> & h, const double s, double i, uint32_t v, const std::vector<std::tuple<double, std::string, std::string, double, double>> & l, int64_t miX, int64_t miY, int64_t maX, int64_t maY) : GameMessageStruct(coID, -1), heightdata(h), size(s), inputScale(i), vertices(v), layers(l), minX(miX), minY(miY), maxX(maX), maxY(maY) {
	}

	Graphics_TerrainHeightmap_Update::Graphics_TerrainHeightmap_Update(const int64_t coID, const std::string & h, const double s, double i, uint32_t v, const std::vector<std::tuple<double, std::string, std::string, double, double>> & l, int64_t miX, int64_t miY, int64_t maX, int64_t maY) : GameMessageStruct(coID, -1), heightmap(h), size(s), inputScale(i), vertices(v), layers(l), minX(miX), minY(miY), maxX(maX), maxY(maY) {
	}
	Graphics_TerrainHeightdata_Update::Graphics_TerrainHeightdata_Update(const int64_t coID, const std::vector<std::vector<double>> & h, const double s, double i, uint32_t v, const std::vector<std::tuple<double, std::string, std::string, double, double>> & l, int64_t miX, int64_t miY, int64_t maX, int64_t maY) : GameMessageStruct(coID, -1), heightdata(h), size(s), inputScale(i), vertices(v), layers(l), minX(miX), minY(miY), maxX(maX), maxY(maY) {
	}

	Graphics_TerrainSetHeight_Update::Graphics_TerrainSetHeight_Update(const int64_t coID, uint64_t xPos, uint64_t zPos, double h) : GameMessageStruct(coID, -1), x(xPos), z(zPos), height(h) {
	}

	Graphics_Terrain_Delete::Graphics_Terrain_Delete() : GameMessageStruct() {}
	Graphics_Terrain_Delete::Graphics_Terrain_Delete(const int64_t c) : GameMessageStruct(c, -1) {
	}

	// Viewport
	Graphics_Viewport_Update::Graphics_Viewport_Update() : GameMessageStruct() {}
	Graphics_Viewport_Update::Graphics_Viewport_Update(const int64_t goID, const int64_t coID, int z, const double l, const double t, const double w, const double h, const double re, const double gr, const double bl, const double al) : GameMessageStruct(coID, goID), zOrder(z), left(l), top(t), width(w), height(h), red(re), green(gr), blue(bl), alpha(al) {
	}

	// Particle
	Graphics_Particle_Create::Graphics_Particle_Create() : GameMessageStruct() {}
	Graphics_Particle_Create::Graphics_Particle_Create(const int64_t goID, const int64_t coID, const std::string & s, const Vec3 & p) : GameMessageStruct(coID, goID), emitterName(s), pos(p) {
	}

	// Particle
	Graphics_Particle_Update::Graphics_Particle_Update() : GameMessageStruct() {}
	Graphics_Particle_Update::Graphics_Particle_Update(const int64_t goID, const int64_t coID) : GameMessageStruct(coID, goID) {
	}

	// Particle
	Graphics_Particle_Delete::Graphics_Particle_Delete() : GameMessageStruct() {}
	Graphics_Particle_Delete::Graphics_Particle_Delete(const int64_t goid, const int64_t cid) : GameMessageStruct(cid, goid) {
	}

	Graphics_PlayAnimation_Update::Graphics_PlayAnimation_Update(const int64_t goID, const int64_t coID, const std::string & a, bool l, double o) : GameMessageStruct(goID, coID), anim(a), looping(l), offsetPercent(o) {
	}

	Graphics_SetAnimationSpeed_Update::Graphics_SetAnimationSpeed_Update(const int64_t goID, const int64_t coID, double s) : GameMessageStruct(goID, coID), speed(s) {
	}

	Graphics_StopAnimation_Update::Graphics_StopAnimation_Update(const int64_t goID, const int64_t coID) : GameMessageStruct(goID, coID) {
	}

	Graphics_BillboardSet_Create::Graphics_BillboardSet_Create(const int64_t goID, const int64_t coID, const std::string & m, double w, double h, BillboardOrigin o) : GameMessageStruct(coID, goID), material(m), width(w), height(h), origin(o) {
	}

	Graphics_Billboard_Update::Graphics_Billboard_Update(const int64_t goid, const int64_t coid, const std::string & i, const Vec3 & o, double w, double h, double u_0, double v_0, double u_1, double v_1) : GameMessageStruct(coid, goid), identifier(i), offset(o), width(w), height(h), u0(u_0), v0(v_0), u1(u_1), v1(v_1) {
	}

	Graphics_BillboardRemove_Update::Graphics_BillboardRemove_Update(const int64_t goid, const int64_t coid, const std::string & i) : GameMessageStruct(coid, goid), identifier(i) {
	}

	Graphics_BillboardSet_Delete::Graphics_BillboardSet_Delete(const int64_t goid, const int64_t coid) : GameMessageStruct(coid, goid) {
	}

	Graphics_AmbLight_Update::Graphics_AmbLight_Update(double r, double g, double b) : GameMessageStruct(), red(r), green(g), blue(b) {
	}

	Graphics_Viewports_Delete::Graphics_Viewports_Delete() : GameMessageStruct() {
	}

	Graphics_SkyBox_Create::Graphics_SkyBox_Create(const std::string & m, double d, bool r) : GameMessageStruct(), material(m), distance(d), renderFirst(r) {
	}

	Graphics_SkyDome_Create::Graphics_SkyDome_Create(const std::string & m, int32_t c, double t, double d, bool r) : GameMessageStruct(), material(m), curvature(c), tiles(t), distance(d), renderFirst(r) {
	}

	Graphics_SkyPlane_Create::Graphics_SkyPlane_Create(const std::string & m, double d, int32_t dir, int32_t s, double t, bool r, double c, int32_t x, int32_t y) : GameMessageStruct(), material(m), distance(d), direction(dir), size(s), tiles(t), renderFirst(r), curvature(c), xSegments(x), ySegments(y) {
	}

	Graphics_SkyBox_Delete::Graphics_SkyBox_Delete() : GameMessageStruct() {
	}

	Graphics_SkyDome_Delete::Graphics_SkyDome_Delete() : GameMessageStruct() {
	}

	Graphics_SkyPlane_Delete::Graphics_SkyPlane_Delete() : GameMessageStruct() {
	}

	Graphics_Resolution_Update::Graphics_Resolution_Update(const graphics::Resolution & r) : GameMessageStruct(), resolution(r) {
	}

	Graphics_Fullscreen_Update::Graphics_Fullscreen_Update(bool fs) : GameMessageStruct(), fullscreen(fs) {
	}

	Graphics_ShadowTechnique_Update::Graphics_ShadowTechnique_Update(ShadowTechnique st) : GameMessageStruct(), shadowTechnique(st) {
	}

	Graphics_SaveTerrainShape_Create::Graphics_SaveTerrainShape_Create(const int64_t goID, const std::string & f, const std::function<void(void)> & cb) : GameMessageStruct(-1, goID), file(f), callback(cb) {
	}

} /* namespace graphics */
} /* namespace api */
} /* namespace i6e */
