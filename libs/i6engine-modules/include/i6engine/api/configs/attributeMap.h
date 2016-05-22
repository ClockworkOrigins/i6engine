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
 * \addtogroup api
 * @{
 */

#ifndef __I6ENGINE_API_CONFIGS_ATTRIBUTEMAP_H__
#define __I6ENGINE_API_CONFIGS_ATTRIBUTEMAP_H__

#include <map>
#include <string>
#include <type_traits>

#include "i6engine/utils/Exceptions.h"

#include "boost/lexical_cast.hpp"

#include "boost/archive/text_iarchive.hpp"
#include "boost/archive/text_oarchive.hpp"

namespace i6e {
namespace api {

	typedef std::map<std::string, std::string> attributeMap;

namespace detail {

	template<typename T>
	struct hasInsertInMap {
		template<typename U, void(U::*)(const std::string &, attributeMap &)const>
		struct SFINAE {
		};
		template<typename U>
		static char Test(SFINAE<U, &U::insertInMap>*);
		template<typename U>
		static int Test(...);
		static const bool value = sizeof(Test<T>(0)) == sizeof(char);
	};

	template<class type, class...Args>
	struct constructible_from {
		template<class C>
		static C arg();

		template<typename U>
		static char constructible_test(U *, decltype(U(arg<Args>()...)) * = 0);
		static int constructible_test(...);

		static const bool value = sizeof(decltype(constructible_test(static_cast<type *>(nullptr)))) == sizeof(char);
	};

} /* namespace detail */

	/**
	 * \brief converts a string to enum value
	 */
	template<typename T>
	typename std::enable_if<std::is_enum<T>::value && !std::is_fundamental<T>::value, void>::type parseAttribute(attributeMap::const_iterator it, T & value) {
		value = T(std::stoul(it->second));
	}
	
	/**
	 * \brief converts a string to a class using T(const std::string &) constructor
	 */
	template<typename T>
	typename std::enable_if<!std::is_enum<T>::value && !std::is_fundamental<T>::value && detail::constructible_from<T, const std::string &>::value, void>::type parseAttribute(attributeMap::const_iterator it, T & value) {
		value = T(it->second);
	}
	
	/**
	 * \brief converts a string to a class using boost::serialization (used if no T(const std::string &) constructor is available)
	 */
	template<typename T>
	typename std::enable_if<!std::is_enum<T>::value && !std::is_fundamental<T>::value && !detail::constructible_from<T, const std::string &>::value, void>::type parseAttribute(attributeMap::const_iterator it, T & value) {
		std::stringstream ss(it->second);
		boost::archive::text_iarchive arch(ss, boost::archive::no_header | boost::archive::no_codecvt | boost::archive::no_xml_tag_checking | boost::archive::archive_flags::no_tracking);
		arch >> value;
	}
	
	/**
	 * \brief converts a string to a fundamental type using boost::lexical_cast
	 */
	template<typename T>
	typename std::enable_if<!std::is_enum<T>::value && std::is_fundamental<T>::value, void>::type parseAttribute(attributeMap::const_iterator it, T & value) {
		value = boost::lexical_cast<T>(it->second);
	}

	/**
	 * \brief writes into an attributeMap
	 */
	template<typename T>
	typename std::enable_if<std::is_enum<T>::value && !std::is_fundamental<T>::value, void>::type writeAttribute(attributeMap & params, const std::string & entry, const T & value) {
		params.insert(std::make_pair(entry, std::to_string(int(value))));
	}

	/**
	 * \brief parses a value from attribute map into a variable with possibility to throw exception, if entry not available
	 */
	template<bool Required, typename T>
	typename std::enable_if<Required, void>::type parseAttribute(const attributeMap & params, const std::string & entry, T & value) {
		auto it = params.find(entry);
		if (it == params.end()) {
			ISIXE_THROW_API("parseAttribute", entry + " not set!");
		} else {
			parseAttribute(it, value);
		}
	}
	
	/**
	 * \brief parses an optional value from attribute map into a variable
	 */
	template<bool Required, typename T>
	typename std::enable_if<!Required, void>::type parseAttribute(const attributeMap & params, const std::string & entry, T & value) {
		auto it = params.find(entry);
		if (it != params.end()) {
			parseAttribute(it, value);
		}
	}
	
	/**
	 * \brief writes a class providing the method insertInMap
	 */
	template<typename T>
	typename std::enable_if<!std::is_enum<T>::value && !std::is_fundamental<T>::value && !std::is_same<T, std::string>::value && detail::hasInsertInMap<T>::value, void>::type writeAttribute(attributeMap & params, const std::string & entry, T value) {
		value.insertInMap(entry, params);
	}
	
	/**
	 * \brief writes a string as is
	 */
	template<typename T>
	typename std::enable_if<!std::is_enum<T>::value && !std::is_fundamental<T>::value && (std::is_same<T, std::string>::value || std::is_same<const char, typename std::remove_pointer<T>::type>::value), void>::type writeAttribute(attributeMap & params, const std::string & entry, T value) {
		params.insert(std::make_pair(entry, value));
	}
	
	/**
	 * \brief writes a fundamental type to the map using std::to_string
	 */
	template<typename T>
	typename std::enable_if<!std::is_enum<T>::value && std::is_fundamental<T>::value, void>::type writeAttribute(attributeMap & params, const std::string & entry, T value) {
		writeAttribute(params, entry, std::to_string(value));
	}
	
	/**
	 * \brief writes a class not providing the method insertInMap, so it uses boost::serialization to convert to a string
	 */
	template<typename T>
	typename std::enable_if<!std::is_enum<T>::value && !std::is_fundamental<T>::value && !std::is_same<T, std::string>::value && !std::is_same<const char, typename std::remove_pointer<T>::type>::value && !detail::hasInsertInMap<T>::value, void>::type writeAttribute(attributeMap & params, const std::string & entry, T value) {
		std::stringstream ss;
		boost::archive::text_oarchive arch(ss, boost::archive::no_header | boost::archive::no_codecvt | boost::archive::no_xml_tag_checking | boost::archive::archive_flags::no_tracking);
		arch << value;
		writeAttribute(params, entry, ss.str());
	}

} /* namespace api */
} /* namespace i6e */

#endif /* __I6ENGINE_API_CONFIGS_ATTRIBUTEMAP_H__ */

/**
 * @}
 */
