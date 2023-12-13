#include <boost/preprocessor/tuple.hpp>
#include <boost/preprocessor/inc.hpp>
#include <boost/preprocessor/for.hpp>
#include <stdio.h>
#include <stdlib.h>

#define _TU_ARR_ITER_PRED(r, state) \
	BOOST_PP_NOT_EQUAL(	\
		BOOST_PP_TUPLE_ELEM(2, 0, state), \
		BOOST_PP_TUPLE_SIZE( \
			BOOST_PP_TUPLE_ELEM(2, 1, state) \
		) \
	) \

#define _TU_ARR_INC(r, state) \
	( \
		BOOST_PP_INC(BOOST_PP_TUPLE_ELEM(2, 0, state)), \
		BOOST_PP_TUPLE_ELEM(2, 1, state) \
	) \

#define _TU_SETUP_ENUM(r, state) \
	BOOST_PP_TUPLE_ELEM( \
		0, \
		BOOST_PP_TUPLE_ELEM( \
			BOOST_PP_TUPLE_ELEM(0, state), \
			BOOST_PP_TUPLE_ELEM(1, state) \
		) \
	) , \

#define _TU_SETUP_UNION(r, state) \
	BOOST_PP_TUPLE_ELEM( \
		1, \
		BOOST_PP_TUPLE_ELEM( \
			BOOST_PP_TUPLE_ELEM(0, state), \
			BOOST_PP_TUPLE_ELEM(1, state) \
		) \
	) \
	BOOST_PP_TUPLE_ELEM( \
		0, \
		BOOST_PP_TUPLE_ELEM( \
			BOOST_PP_TUPLE_ELEM(0, state), \
			BOOST_PP_TUPLE_ELEM(1, state) \
		) \
	) \
	; \

/// Macro that expands to a tagged union declaration; which is 
/// a struct with an enum field containing one value for each field in 
/// the union, and the union itself, with the fields named after each
/// member of the enum.
#define tagged_union(name, pairs) \
	struct name { \
		enum { \
			BOOST_PP_FOR((0, pairs), _TU_ARR_ITER_PRED, _TU_ARR_INC, _TU_SETUP_ENUM) \
		} active; \
		union { \
			BOOST_PP_FOR((0, pairs), _TU_ARR_ITER_PRED, _TU_ARR_INC, _TU_SETUP_UNION)\
		}; \
	}

/// Like tagged_union, but it uses a given enum type as the backing enum.
#define tagged_union_from_enum(name, enumtype, pairs) \
	struct name { \
		enumtype active; \
		union { \
			BOOST_PP_FOR((0, pairs), _TU_ARR_ITER_PRED, _TU_ARR_INC, _TU_SETUP_UNION)\
		}; \
	}

static void t_union_check_error(const char *file, int line) {
	printf("%s:%d: Tagged union safety error: attempted to access inactive field.\n", file, line);
	exit(EXIT_FAILURE);
}

/// Gets the value in the tagged union. If it's an inactive field, an error is 
/// issued and the program exits.
#define TUnionGet(t_union, variant) \
	(((t_union).active == variant) \
		? (t_union).variant \
		: (t_union_check_error(__FILE__, __LINE__), (t_union).variant))

/// Expands to a compound literal of a tagged union. Use this instead of 
/// manually writing out compound literals to avoid silly mistakes.
/// literals to avoid silly mistakes.
#define TUnionLit(variant, literal) {.active = variant, .variant = literal}

/// Expands to the active field enum
#define TUnionActive(t_union) (t_union).active

/// Modifies the current variant's value. If the variant is different
/// than the one stored, it will be an error.
#define TUnionSet(t_union, variant, value) \
	if ((t_union).active != variant) { \
		_t_union_check_error(__FILE__, __LINE__) \
	} \
	(t_union).variant = value;
