// Copyright (c) 2003 Daniel Wallin and Arvid Norberg

// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF
// ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
// PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT
// SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR
// ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
// ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE
// OR OTHER DEALINGS IN THE SOFTWARE.


#ifndef __LUABIND_DETAIL_TYPETRAITS_HPP__
#define __LUABIND_DETAIL_TYPETRAITS_HPP__

#include "i6engine/luabind/config.hpp"

#include "i6engine/luabind/detail/primitives.hpp"

#include "boost/mpl/if.hpp"
#include "boost/type_traits/is_reference.hpp"
#include "boost/type_traits/is_const.hpp"


namespace luabind {
namespace detail {

#ifdef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
	template<class T>
	struct is_const_type {
		typedef typename boost::mpl::if_<boost::is_const<T>, yes_t, no_t>::type type;
	};

	template<bool is_Reference = false>
	struct is_const_reference_helper {
		template<class>
		struct apply {
			enum {
				value = false
			};
		};
	};

	template<class T>
	typename is_const_type<T>::type is_const_reference_tester(T &);
	no_t is_const_reference_tester(...);

	template<>
	struct is_const_reference_helper<true> {
		template<class T>
		struct apply {
			static T getT();

			enum {
				value = sizeof(is_const_reference_tester(getT())) == sizeof(yes_t)
			};
		};
	};

	template<class T>
	struct is_const_reference : is_const_reference_helper<boost::is_reference<T>::value>::template apply<T> {
		typedef boost::mpl::bool_<value> type;
	};
#else /* BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION */
	template<class T>
	struct is_const_reference {
		enum {
			value = false
		};
		typedef boost::mpl::bool_<value> type;
	};

	template<class T>
	struct is_const_reference<const T &> {
		enum {
			value = true
		};
		typedef boost::mpl::bool_<value> type;
	};
#endif /* BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION */

	template<class T>
	struct is_nonconst_reference {
		enum {
			value = boost::is_reference<T>::value && !is_const_reference<T>::value
		};
		typedef boost::mpl::bool_<value> type;
	};

	template<class A>
	yes_t is_const_pointer_helper(void(*)(const A *));
	no_t is_const_pointer_helper(...);

	template<class T>
	struct is_const_pointer {
		enum {
			value = sizeof(is_const_pointer_helper((void(*)(T))0)) == sizeof(yes_t)
		};
		typedef boost::mpl::bool_<value> type;
	};

	template<class A>
	yes_t is_nonconst_pointer_helper(void(*)(A *));
	no_t is_nonconst_pointer_helper(...);

	template<class T>
	struct is_nonconst_pointer {
		enum {
			value = sizeof(is_nonconst_pointer_helper((void(*)(T))0)) == sizeof(yes_t) && !is_const_pointer<T>::value
		};
		typedef boost::mpl::bool_<value> type;
	};

	template<int v1, int v2>
	struct max_c {
		enum {
			value = (v1 > v2) ? v1 : v2
		};
	};

} /* namespace detail */
} /* namespace luabind */

#endif /* __LUABIND_DETAIL_TYPETRAITS_HPP__ */
