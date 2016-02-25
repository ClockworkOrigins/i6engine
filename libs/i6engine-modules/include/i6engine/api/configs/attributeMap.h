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
 * \addtogroup api
 * @{
 */

#ifndef __I6ENGINE_API_CONFIGS_ATTRIBUTEMAP_H__
#define __I6ENGINE_API_CONFIGS_ATTRIBUTEMAP_H__

#include <map>
#include <string>

#include "i6engine/utils/Exceptions.h"

#include "boost/lexical_cast.hpp"

namespace i6engine {
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
	class constructible_from {
		template<class C>
		static C arg();

		template<typename U>
		static std::true_type constructible_test(U *, decltype(U(arg<Args>()...)) * = 0);
		static std::false_type constructible_test(...);

	public:
		typedef decltype(constructible_test(static_cast<type*>(nullptr))) result;
	};

} /* namespace detail */

	template<class type>
	struct constructible {
		template<class...Args>
		struct from :
			detail::constructible_from<type, Args...>::result {
		};
	};

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
	typename std::enable_if<!std::is_enum<T>::value && !std::is_fundamental<T>::value && constructible<T>::from<const std::string &>::value, void>::type parseAttribute(attributeMap::const_iterator it, T & value) {
		value = T(it->second);
	}
	
	/**
	 * \brief converts a string to a class using boost::serialization (used if no T(const std::string &) constructor is available)
	 */
	template<typename T>
	typename std::enable_if<!std::is_enum<T>::value && !std::is_fundamental<T>::value && !constructible<T>::from<const std::string &>::value, void>::type parseAttribute(attributeMap::const_iterator it, T & value) {
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
	 * \brief writes a class not providing the method insertInMap, so it uses boost::serialization to convert to a string
	 */
	template<typename T>
	typename std::enable_if<!std::is_enum<T>::value && !std::is_fundamental<T>::value && !std::is_same<T, std::string>::value && !std::is_same<const char, typename std::remove_pointer<T>::type>::value && !detail::hasInsertInMap<T>::value, void>::type writeAttribute(attributeMap & params, const std::string & entry, T value) {
		std::stringstream ss;
		boost::archive::text_oarchive arch(ss, boost::archive::no_header | boost::archive::no_codecvt | boost::archive::no_xml_tag_checking | boost::archive::archive_flags::no_tracking);
		arch << value;
		writeAttribute(params, entry, ss.str());
	}
	
	/**
	 * \brief writes a fundamental type to the map using std::to_string
	 */
	template<typename T>
	typename std::enable_if<!std::is_enum<T>::value && std::is_fundamental<T>::value, void>::type writeAttribute(attributeMap & params, const std::string & entry, T value) {
		writeAttribute(params, entry, std::to_string(value));
	}
	
	/**
	 * \brief writes a string as is
	 */
	template<typename T>
	typename std::enable_if<!std::is_enum<T>::value && !std::is_fundamental<T>::value && (std::is_same<T, std::string>::value || std::is_same<const char, typename std::remove_pointer<T>::type>::value), void>::type writeAttribute(attributeMap & params, const std::string & entry, T value) {
		params.insert(std::make_pair(entry, value));
	}

} /* namespace api */
} /* namespace i6engine */

#endif /* __I6ENGINE_API_CONFIGS_ATTRIBUTEMAP_H__ */

/**
 * @}
 */
