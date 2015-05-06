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

#include "i6engine/math/i6eMath.h"

#include "boost/python.hpp"

BOOST_PYTHON_MODULE(ScriptingMathPython) {
	using namespace boost::python;

	def("rotateVector", &i6engine::math::rotateVector);

	class_<Quaternion>("i6eQuaternion")
			.def(init<i6engine::math::i6eVector, double>())
			.def("toVector", &Quaternion::toVector);

	class_<Vec3>("i6eVector")
			.def(init<double, double, double>())
			.def(init<std::string>())
			.def("getX", &Vec3::getX)
			.def("getY", &Vec3::getY)
			.def("getZ", &Vec3::getZ)
			.def("setX", &Vec3::setX)
			.def("setY", &Vec3::setY)
			.def("setZ", &Vec3::setZ)
			.def(self + self)
			.def(self - self)
			.def(self * self)
			.def(self * double())
			.def("scalProd", &Vec3::scalProd)
			.staticmethod("scalProd")
			.def(self / double())
			.def(self += self)
			.def(self -= self)
			.def(self == self)
			.def(self != self)
			.def("normalize", &Vec3::normalize)
			.def("crossProd", &Vec3::crossProd)
			.staticmethod("crossProd")
			.def("crossAngle", &Vec3::crossAngle)
			.staticmethod("crossAngle")
			.def("mulComponents", &Vec3::mulComponents)
			.def("length", &Vec3::length);
}