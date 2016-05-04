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

#include "LuaMathExport.h"

#include "i6engine/math/i6eMath.h"
#include "i6engine/math/i6eVector2.h"
#include "i6engine/math/i6eVector4.h"

#include "i6engine/luabind/operator.hpp"

using namespace luabind;

scope registerMath() {
	return
		def("binom", i6e::math::binom),
		def("disPointLine", i6e::math::disPointLine),
		def("rotateVector", i6e::math::rotateVector),

		class_<Quaternion>("Quaternion")
			.def(constructor<>())
			.def(constructor<double, double, double, double>())
			.def(constructor<i6e::math::i6eVector, double>())
			.def(constructor<const std::map<std::string, std::string> &, const std::string &>())
			.def(constructor<const std::string &>())
			.def("getX", &Quaternion::getX)
			.def("getY", &Quaternion::getY)
			.def("getZ", &Quaternion::getZ)
			.def("getW", &Quaternion::getW)
			.def("setX", &Quaternion::setX)
			.def("setY", &Quaternion::setY)
			.def("setZ", &Quaternion::setZ)
			.def("setW", &Quaternion::setW)
			.property("x", &Quaternion::getX, &Quaternion::setX)
			.property("y", &Quaternion::getY, &Quaternion::setY)
			.property("z", &Quaternion::getZ, &Quaternion::setZ)
			.property("w", &Quaternion::getW, &Quaternion::setW)
			.def(self + self)
			.def(self - self)
			.def(self * self)
			.def(self * double())
			.def(self / double())
			.def("insertInMap", &Quaternion::insertInMap)
			.def("inverse", &Quaternion::inverse)
			.def("length", &Quaternion::length)
			.def("normalize", &Quaternion::normalize)
			.def("equals", &Quaternion::equals)
			.def("toAxisAngle", &Quaternion::toAxisAngle)
			.def(self == self)
			.def("toVector", &Quaternion::toVector)
			.def("toString", &Quaternion::toString)
			.def(tostring(self))
			.def(self * Vec3()),

		def("dotProduct", i6e::math::dotProduct),
		def("scalProd", i6e::math::scalProd),

		class_<Vec2>("Vec2")
			.def(constructor<>())
			.def(constructor<double, double>())
			.def(constructor<std::string>())
			.def(constructor<const std::map<std::string, std::string> &, const std::string &>())
			.def("getX", &Vec2::getX)
			.def("getY", &Vec2::getY)
			.def("setX", &Vec2::setX)
			.def("setY", &Vec2::setY)
			.property("x", &Vec2::getX, &Vec2::setX)
			.property("y", &Vec2::getY, &Vec2::setY)
			.def(self + self)
			.def(self - self)
			.def(self * double())
			.def(self / double())
			.scope
			[
				def("scalProd", &Vec2::scalProd)
			]
			.def("normalize", &Vec2::normalize)
			.def("insertInMap", &Vec2::insertInMap)
			.def("mulComponents", &Vec2::mulComponents)
			.def("isValid", &Vec2::isValid)
			.def("setValid", &Vec2::setValid)
			.def("length", &Vec2::length)
			.def("toString", &Vec2::toString)
			.def(self == self)
			.def(tostring(self)),

		class_<Vec2f>("Vec2f")
			.def(constructor<>())
			.def(constructor<float, float>())
			.def(constructor<std::string>())
			.def(constructor<const std::map<std::string, std::string> &, const std::string &>())
			.def("getX", &Vec2f::getX)
			.def("getY", &Vec2f::getY)
			.def("setX", &Vec2f::setX)
			.def("setY", &Vec2f::setY)
			.property("x", &Vec2f::getX, &Vec2f::setX)
			.property("y", &Vec2f::getY, &Vec2f::setY)
			.def(self + self)
			.def(self - self)
			.def(self * float())
			.def(self / float())
			.scope
			[
				def("scalProd", &Vec2f::scalProd)
			]
			.def("normalize", &Vec2f::normalize)
			.def("insertInMap", &Vec2f::insertInMap)
			.def("mulComponents", &Vec2f::mulComponents)
			.def("isValid", &Vec2f::isValid)
			.def("setValid", &Vec2f::setValid)
			.def("length", &Vec2f::length)
			.def("toString", &Vec2f::toString)
			.def(self == self)
			.def(tostring(self)),

		class_<Vec2i>("Vec2i")
			.def(constructor<>())
			.def(constructor<int32_t, int32_t>())
			.def(constructor<std::string>())
			.def(constructor<const std::map<std::string, std::string> &, const std::string &>())
			.def("getX", &Vec2i::getX)
			.def("getY", &Vec2i::getY)
			.def("setX", &Vec2i::setX)
			.def("setY", &Vec2i::setY)
			.property("x", &Vec2i::getX, &Vec2i::setX)
			.property("y", &Vec2i::getY, &Vec2i::setY)
			.def(self + self)
			.def(self - self)
			.def(self * int32_t())
			.def(self / int32_t())
			.scope
			[
				def("scalProd", &Vec2i::scalProd)
			]
			.def("normalize", &Vec2i::normalize)
			.def("insertInMap", &Vec2i::insertInMap)
			.def("mulComponents", &Vec2i::mulComponents)
			.def("isValid", &Vec2i::isValid)
			.def("setValid", &Vec2i::setValid)
			.def("length", &Vec2i::length)
			.def("toString", &Vec2i::toString)
			.def(self == self)
			.def(tostring(self)),

		class_<Vec2ui>("Vec2ui")
			.def(constructor<>())
			.def(constructor<uint32_t, uint32_t>())
			.def(constructor<std::string>())
			.def(constructor<const std::map<std::string, std::string> &, const std::string &>())
			.def("getX", &Vec2ui::getX)
			.def("getY", &Vec2ui::getY)
			.def("setX", &Vec2ui::setX)
			.def("setY", &Vec2ui::setY)
			.property("x", &Vec2ui::getX, &Vec2ui::setX)
			.property("y", &Vec2ui::getY, &Vec2ui::setY)
			.def(self + self)
			.def(self - self)
			.def(self * uint32_t())
			.def(self / uint32_t())
			.scope
			[
				def("scalProd", &Vec2ui::scalProd)
			]
			.def("normalize", &Vec2ui::normalize)
			.def("insertInMap", &Vec2ui::insertInMap)
			.def("mulComponents", &Vec2ui::mulComponents)
			.def("isValid", &Vec2ui::isValid)
			.def("setValid", &Vec2ui::setValid)
			.def("length", &Vec2ui::length)
			.def("toString", &Vec2ui::toString)
			.def(self == self)
			.def(tostring(self)),

		class_<Vec3>("Vec3")
			.def(constructor<>())
			.def(constructor<double, double, double>())
			.def(constructor<std::string>())
			.def(constructor<const std::map<std::string, std::string> &, const std::string &>())
			.def("getX", &Vec3::getX)
			.def("getY", &Vec3::getY)
			.def("getZ", &Vec3::getZ)
			.def("setX", &Vec3::setX)
			.def("setY", &Vec3::setY)
			.def("setZ", &Vec3::setZ)
			.property("x", &Vec3::getX, &Vec3::setX)
			.property("y", &Vec3::getY, &Vec3::setZ)
			.property("z", &Vec3::getY, &Vec3::setZ)
			.def(self + self)
			.def(self - self)
			.def(self * self)
			.def(self * double())
			.def(self / double())
			.def(self == self)
			.def("normalize", &Vec3::normalize)
			.def("insertInMap", &Vec3::insertInMap)
			.def("mulComponents", &Vec3::mulComponents)
			.def("isValid", &Vec3::isValid)
			.def("setValid", &Vec3::setValid)
			.def("length", &Vec3::length)
			.def("toString", &Vec3::toString)
			.def(tostring(self))
			.scope
			[
				def("scalProd", &Vec3::scalProd),
				def("crossProd", &Vec3::crossProd),
				def("crossAngle", &Vec3::crossAngle)
			]
			.def(self * Quaternion()),

		class_<Vec4>("Vec4")
			.def(constructor<>())
			.def(constructor<double, double, double, double>())
			.def(constructor<const std::map<std::string, std::string> &, const std::string &>())
			.def(constructor<const std::string &>())
			.def("getX", &Vec4::getX)
			.def("getY", &Vec4::getY)
			.def("getZ", &Vec4::getZ)
			.def("getW", &Vec4::getW)
			.def("setX", &Vec4::setX)
			.def("setY", &Vec4::setY)
			.def("setZ", &Vec4::setZ)
			.def("setW", &Vec4::setW)
			.property("x", &Vec4::getX, &Vec4::setX)
			.property("y", &Vec4::getY, &Vec4::setY)
			.property("z", &Vec4::getZ, &Vec4::setZ)
			.property("w", &Vec4::getW, &Vec4::setW)
			.def(self == self)
			.def("insertInMap", &Vec4::insertInMap)
			.def("toString", &Vec4::toString)
			.def(tostring(self))
		;
}
