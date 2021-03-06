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

#include "i6engine/math/i6eMath.h"
#include "i6engine/math/i6eVector2.h"
#include "i6engine/math/i6eVector4.h"

#include "boost/python.hpp"

BOOST_PYTHON_MODULE(ScriptingMathPython) {
	using namespace boost::python;

	def("binom", &i6e::math::binom);
	def("disPointLine", &i6e::math::disPointLine);
	def("rotateVector", &i6e::math::rotateVector);

	class_<Quaternion>("Quaternion")
		.def(init<>())
		.def(init<double, double, double, double>())
		.def(init<i6e::math::i6eVector, double>())
		.def(init<const std::map<std::string, std::string> &, const std::string &>())
		.def(init<const std::string &>())
		.def("getX", &Quaternion::getX)
		.def("getY", &Quaternion::getY)
		.def("getZ", &Quaternion::getZ)
		.def("getW", &Quaternion::getW)
		.def("setX", &Quaternion::setX)
		.def("setY", &Quaternion::setY)
		.def("setZ", &Quaternion::setZ)
		.def("setW", &Quaternion::setW)
		.add_property("x", &Quaternion::getX, &Quaternion::setX)
		.add_property("y", &Quaternion::getY, &Quaternion::setY)
		.add_property("z", &Quaternion::getZ, &Quaternion::setZ)
		.add_property("w", &Quaternion::getW, &Quaternion::setW)
		.def(self + self)
		.def(self - self)
		.def(self * self)
		.def(self * double())
		.def(self / double())
		.def(self += self)
		.def("insertInMap", &Quaternion::insertInMap)
		.def("inverse", &Quaternion::inverse)
		.def("length", &Quaternion::length)
		.def("normalize", &Quaternion::normalize)
		.def("equals", &Quaternion::equals)
		.def("toAxisAngle", &Quaternion::toAxisAngle)
		.def(self == self)
		.def(self != self)
		.def("toVector", &Quaternion::toVector)
		.def("toString", &Quaternion::toString)
		.def(self_ns::str(self_ns::self))
		.def(self * Vec3());

	def("dotProduct", i6e::math::dotProduct);
	def("scalProd", i6e::math::scalProd);

	class_<Vec2>("Vec2")
		.def(init<>())
		.def(init<double, double>())
		.def(init<std::string>())
		.def(init<const std::map<std::string, std::string> &, const std::string &>())
		.def("getX", &Vec2::getX)
		.def("getY", &Vec2::getY)
		.def("setX", &Vec2::setX)
		.def("setY", &Vec2::setY)
		.add_property("x", &Vec2::getX, &Vec2::setX)
		.add_property("y", &Vec2::getY, &Vec2::setY)
		.def(self + self)
		.def(self - self)
		.def(self * double())
		.def(self / double())
		.def("scalProd", &Vec2::scalProd)
		.staticmethod("scalProd")
		.def("normalize", &Vec2::normalize)
		.def("insertInMap", &Vec2::insertInMap)
		.def("mulComponents", &Vec2::mulComponents)
		.def("isValid", &Vec2::isValid)
		.def("setValid", &Vec2::setValid)
		.def("length", &Vec2::length)
		.def("toString", &Vec2::toString)
		.def(self == self)
		.def(self != self);

	class_<Vec2f>("Vec2f")
		.def(init<>())
		.def(init<float, float>())
		.def(init<std::string>())
		.def(init<const std::map<std::string, std::string> &, const std::string &>())
		.def("getX", &Vec2f::getX)
		.def("getY", &Vec2f::getY)
		.def("setX", &Vec2f::setX)
		.def("setY", &Vec2f::setY)
		.add_property("x", &Vec2f::getX, &Vec2f::setX)
		.add_property("y", &Vec2f::getY, &Vec2f::setY)
		.def(self + self)
		.def(self - self)
		.def(self * float())
		.def(self / float())
		.def("scalProd", &Vec2f::scalProd)
		.staticmethod("scalProd")
		.def("normalize", &Vec2f::normalize)
		.def("insertInMap", &Vec2f::insertInMap)
		.def("mulComponents", &Vec2f::mulComponents)
		.def("isValid", &Vec2f::isValid)
		.def("setValid", &Vec2f::setValid)
		.def("length", &Vec2f::length)
		.def("toString", &Vec2f::toString)
		.def(self == self)
		.def(self != self);

	class_<Vec2i>("Vec2i")
		.def(init<>())
		.def(init<int32_t, int32_t>())
		.def(init<std::string>())
		.def(init<const std::map<std::string, std::string> &, const std::string &>())
		.def("getX", &Vec2i::getX)
		.def("getY", &Vec2i::getY)
		.def("setX", &Vec2i::setX)
		.def("setY", &Vec2i::setY)
		.add_property("x", &Vec2i::getX, &Vec2i::setX)
		.add_property("y", &Vec2i::getY, &Vec2i::setY)
		.def(self + self)
		.def(self - self)
		.def(self * int32_t())
		.def(self / int32_t())
		.def("scalProd", &Vec2i::scalProd)
		.staticmethod("scalProd")
		.def("normalize", &Vec2i::normalize)
		.def("insertInMap", &Vec2i::insertInMap)
		.def("mulComponents", &Vec2i::mulComponents)
		.def("isValid", &Vec2i::isValid)
		.def("setValid", &Vec2i::setValid)
		.def("length", &Vec2i::length)
		.def("toString", &Vec2i::toString)
		.def(self == self)
		.def(self != self);

	class_<Vec2ui>("Vec2ui")
		.def(init<>())
		.def(init<uint32_t, uint32_t>())
		.def(init<std::string>())
		.def(init<const std::map<std::string, std::string> &, const std::string &>())
		.def("getX", &Vec2ui::getX)
		.def("getY", &Vec2ui::getY)
		.def("setX", &Vec2ui::setX)
		.def("setY", &Vec2ui::setY)
		.add_property("x", &Vec2ui::getX, &Vec2ui::setX)
		.add_property("y", &Vec2ui::getY, &Vec2ui::setY)
		.def(self + self)
		.def(self - self)
		.def(self * uint32_t())
		.def(self / uint32_t())
		.def(self += self)
		.def(self -= self)
		.def("scalProd", &Vec2ui::scalProd)
		.staticmethod("scalProd")
		.def("normalize", &Vec2ui::normalize)
		.def("insertInMap", &Vec2ui::insertInMap)
		.def("mulComponents", &Vec2ui::mulComponents)
		.def("isValid", &Vec2ui::isValid)
		.def("setValid", &Vec2ui::setValid)
		.def("length", &Vec2ui::length)
		.def("toString", &Vec2ui::toString)
		.def(self == self)
		.def(self != self);

	class_<Vec3>("Vec3")
		.def(init<>())
		.def(init<double, double, double>())
		.def(init<std::string>())
		.def(init<const std::map<std::string, std::string> &, const std::string &>())
		.def("getX", &Vec3::getX)
		.def("getY", &Vec3::getY)
		.def("getZ", &Vec3::getZ)
		.def("setX", &Vec3::setX)
		.def("setY", &Vec3::setY)
		.def("setZ", &Vec3::setZ)
		.add_property("x", &Vec3::getX, &Vec3::setX)
		.add_property("y", &Vec3::getY, &Vec3::setZ)
		.add_property("z", &Vec3::getY, &Vec3::setZ)
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
		.def("insertInMap", &Vec3::insertInMap)
		.def("mulComponents", &Vec3::mulComponents)
		.def("isValid", &Vec3::isValid)
		.def("setValid", &Vec3::setValid)
		.def("length", &Vec3::length)
		.def("toString", &Vec3::toString)
		.def(self * Quaternion())
		.def(self_ns::str(self_ns::self));

	class_<Vec4>("Vec4")
		.def(init<>())
		.def(init<double, double, double, double>())
		.def(init<const std::map<std::string, std::string> &, const std::string &>())
		.def(init<const std::string &>())
		.def("getX", &Vec4::getX)
		.def("getY", &Vec4::getY)
		.def("getZ", &Vec4::getZ)
		.def("getW", &Vec4::getW)
		.def("setX", &Vec4::setX)
		.def("setY", &Vec4::setY)
		.def("setZ", &Vec4::setZ)
		.def("setW", &Vec4::setW)
		.add_property("x", &Vec4::getX, &Vec4::setX)
		.add_property("y", &Vec4::getY, &Vec4::setY)
		.add_property("z", &Vec4::getZ, &Vec4::setZ)
		.add_property("w", &Vec4::getW, &Vec4::setW)
		.def(self == self)
		.def(self != self)
		.def("insertInMap", &Vec4::insertInMap)
		.def("toString", &Vec4::toString)
		.def(self_ns::str(self_ns::self));
}
