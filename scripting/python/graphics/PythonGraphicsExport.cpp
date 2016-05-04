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

#include "i6engine/math/i6eVector.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/facades/GraphicsFacade.h"

#include "boost/python.hpp"

namespace i6e {
namespace python {
namespace graphics {

	void closeViewports() {
		i6e::api::EngineController::GetSingleton().getGraphicsFacade()->closeViewports();
	}

	void setAmbientLight(double r, double g, double b) {
		i6e::api::EngineController::GetSingleton().getGraphicsFacade()->setAmbientLight(r, g, b);
	}

	void setSkyBox(const std::string & material, double distance, bool renderFirst) {
		i6e::api::EngineController::GetSingleton().getGraphicsFacade()->setSkyBox(material, distance, renderFirst);
	}

	void setSkyDome(const std::string & material, int32_t curvature, double tiles, double distance, bool renderFirst) {
		i6e::api::EngineController::GetSingleton().getGraphicsFacade()->setSkyDome(material, curvature, tiles, distance, renderFirst);
	}

	void setSkyPlane(const std::string & material, double distance, int32_t direction, int32_t size, double tiles, bool renderFirst, double curvature, int32_t xSegments, int32_t ySegments) {
		i6e::api::EngineController::GetSingleton().getGraphicsFacade()->setSkyPlane(material, distance, direction, size, tiles, renderFirst, curvature, xSegments, ySegments);
	}

	void removeSkyBox() {
		i6e::api::EngineController::GetSingleton().getGraphicsFacade()->removeSkyBox();
	}

	void removeSkyDome() {
		i6e::api::EngineController::GetSingleton().getGraphicsFacade()->removeSkyDome();
	}

	void removeSkyPlane() {
		i6e::api::EngineController::GetSingleton().getGraphicsFacade()->removeSkyPlane();
	}

	void setResolution(const i6e::api::graphics::Resolution & resolution) {
		i6e::api::EngineController::GetSingleton().getGraphicsFacade()->setResolution(resolution);
	}

	void setFullscreen(bool fullscreen) {
		i6e::api::EngineController::GetSingleton().getGraphicsFacade()->setFullscreen(fullscreen);
	}

	void setShadowTechnique(i6e::api::graphics::ShadowTechnique st) {
		i6e::api::EngineController::GetSingleton().getGraphicsFacade()->setShadowTechnique(st);
	}

	void resetGraphicsSubSystem() {
		i6e::api::EngineController::GetSingleton().getGraphicsFacade()->resetSubSystem();
	}

	std::vector<i6e::api::graphics::Resolution> getPossibleResolutions() {
		return i6e::api::EngineController::GetSingleton().getGraphicsFacade()->getPossibleResolutions();
	}

	i6e::api::graphics::Resolution getCurrentResolution() {
		return i6e::api::EngineController::GetSingleton().getGraphicsFacade()->getCurrentResolution();
	}

	std::vector<std::pair<int64_t, Vec3>> getSelectables() {
		return i6e::api::EngineController::GetSingleton().getGraphicsFacade()->getSelectables();
	}

	void setLinearFog(const Vec3 & colour, double start, double end) {
		i6e::api::EngineController::GetSingleton().getGraphicsFacade()->setLinearFog(colour, start, end);
	}

	void setExponentialFog(const Vec3 & colour, double density) {
		i6e::api::EngineController::GetSingleton().getGraphicsFacade()->setExponentialFog(colour, density);
	}

	void setExponentialFog2(const Vec3 & colour, double density) {
		i6e::api::EngineController::GetSingleton().getGraphicsFacade()->setExponentialFog2(colour, density);
	}

	void takeScreenshot(const std::string & prefix, const std::string & suffix) {
		i6e::api::EngineController::GetSingleton().getGraphicsFacade()->takeScreenshot(prefix, suffix);
	}

	void showFPS(double x, double y, const std::string & imageStyle, const std::string & printStyle, const std::string & imageset, const std::string & image) {
		i6e::api::EngineController::GetSingleton().getGraphicsFacade()->showFPS(x, y, imageStyle, printStyle, imageset, image);
	}

	void hideFPS() {
		i6e::api::EngineController::GetSingleton().getGraphicsFacade()->hideFPS();
	}

} /* namespace graphics */
} /* namespace python */
} /* namespace i6e */

BOOST_PYTHON_MODULE(ScriptingGraphicsPython) {
	using namespace boost::python;

	enum_<i6e::api::graphics::ShadowTechnique>("ShadowTechnique")
		.value("None", i6e::api::graphics::ShadowTechnique::None)
		.value("Stencil_Additive", i6e::api::graphics::ShadowTechnique::Stencil_Additive)
		.value("Stencil_Modulative", i6e::api::graphics::ShadowTechnique::Stencil_Modulative)
		.export_values();

	class_<i6e::api::graphics::Resolution>("Resolution")
		.def(init<>())
		.def_readwrite("width", &i6e::api::graphics::Resolution::width)
		.def_readwrite("height", &i6e::api::graphics::Resolution::height);

	def("closeViewports", &i6e::python::graphics::closeViewports);
	def("setAmbientLight", &i6e::python::graphics::setAmbientLight);
	def("setSkyBox", &i6e::python::graphics::setSkyBox);
	def("setSkyDome", &i6e::python::graphics::setSkyDome);
	def("setSkyPlane", &i6e::python::graphics::setSkyPlane);
	def("removeSkyBox", &i6e::python::graphics::removeSkyBox);
	def("removeSkyDome", &i6e::python::graphics::removeSkyDome);
	def("removeSkyPlane", &i6e::python::graphics::removeSkyPlane);
	def("setResolution", &i6e::python::graphics::setResolution);
	def("setFullscreen", &i6e::python::graphics::setFullscreen);
	def("setShadowTechnique", &i6e::python::graphics::setShadowTechnique);
	def("resetGraphicsSubSystem", &i6e::python::graphics::resetGraphicsSubSystem);
	def("getPossibleResolutions", &i6e::python::graphics::getPossibleResolutions);
	def("getCurrentResolution", &i6e::python::graphics::getCurrentResolution);
	def("getSelectables", &i6e::python::graphics::getSelectables);
	def("setLinearFog", &i6e::python::graphics::setLinearFog);
	def("setExponentialFog", &i6e::python::graphics::setExponentialFog);
	def("setExponentialFog2", &i6e::python::graphics::setExponentialFog2);
	def("takeScreenshot", &i6e::python::graphics::takeScreenshot);
	def("showFPS", &i6e::python::graphics::showFPS);
	def("hideFPS", &i6e::python::graphics::hideFPS);
}
