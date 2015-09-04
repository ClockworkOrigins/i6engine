/*
 * Copyright 2011-2012 Prof. Dr. Richard Lenz, Thomas Fischer
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
 * \addtogroup utils
 * @{
 */

#ifndef __I6ENGINE_UTILS_ENUMCLASSSTREAM_H__
#define __I6ENGINE_UTILS_ENUMCLASSSTREAM_H__

/**
 * \brief method is used when using an enum class in an ostream
 */
template<typename T>
std::ostream & operator<<(typename std::enable_if<std::is_enum<T>::value, std::ostream>::type & stream, const T & e) {
	return stream << static_cast<typename std::underlying_type<T>::type>(e);
}

#endif /* __I6ENGINE_UTILS_ENUMCLASSSTREAM_H__ */

/**
 * @}
 */
