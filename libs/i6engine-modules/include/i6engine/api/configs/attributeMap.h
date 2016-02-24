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

} // detail::

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

	template<bool Required, typename T>
	typename std::enable_if<!Required, void>::type parseAttribute(const attributeMap & params, const std::string & entry, T & value) {
		auto it = params.find(entry);
		if (it != params.end()) {
			parseAttribute(it, value);
		}
	}

	template<typename T>
	typename std::enable_if<std::is_enum<T>::value && !std::is_fundamental<T>::value, void>::type parseAttribute(attributeMap::const_iterator it, T & value) {
		value = T(std::stoul(it->second));
	}

	template<typename T>
	typename std::enable_if<!std::is_enum<T>::value && !std::is_fundamental<T>::value, void>::type parseAttribute(attributeMap::const_iterator it, T & value) {
		value = T(it->second);
	}

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

	template<typename T>
	typename std::enable_if<!std::is_enum<T>::value && !std::is_fundamental<T>::value && !std::is_same<T, std::string>::value && detail::hasInsertInMap<T>::value, void>::type writeAttribute(attributeMap & params, const std::string & entry, const T & value) {
		value.insertInMap(entry, params);
	}

	template<typename T>
	typename std::enable_if<!std::is_enum<T>::value && !std::is_fundamental<T>::value && !std::is_same<T, std::string>::value && !detail::hasInsertInMap<T>::value, void>::type writeAttribute(attributeMap & params, const std::string & entry, const T & value) {
		static_assert(false, "class has no method insertInMap");
	}

	template<typename T>
	typename std::enable_if<!std::is_enum<T>::value && std::is_fundamental<T>::value, void>::type writeAttribute(attributeMap & params, const std::string & entry, const T & value) {
		params.insert(std::make_pair(entry, std::to_string(value)));
	}

	template<typename T>
	typename std::enable_if<!std::is_enum<T>::value && !std::is_fundamental<T>::value && std::is_same<T, std::string>::value, void>::type writeAttribute(attributeMap & params, const std::string & entry, const T & value) {
		params.insert(std::make_pair(entry, value));
	}

} /* namespace api */
} /* namespace i6engine */

#endif /* __I6ENGINE_API_CONFIGS_ATTRIBUTEMAP_H__ */

/**
 * @}
 */
