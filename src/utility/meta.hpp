#ifndef _UTILITY_META_HPP_
#define _UTILITY_META_HPP_

#include <opencv2/core/core.hpp>

#include <boost/preprocessor/arithmetic/inc.hpp>
#include <boost/preprocessor/comparison/not_equal.hpp>
#include <boost/preprocessor/list/adt.hpp>
#include <boost/preprocessor/list/at.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/repetition/repeat_from_to.hpp>

template <int N, typename PairType>
struct element
{};

template <typename T>
struct element<0, cv::Point_<T> >
{
    typedef T type;
};

template <typename T>
struct element<1, cv::Point_<T> >
{
    typedef T type;
};

template <typename T>
struct element<0, cv::Size_<T> >
{
    typedef T type;
};

template <typename T>
struct element<1, cv::Size_<T> >
{
    typedef T type;
};

//------------------------------------------------

#define GET_ITEM(z, n, data) boost::get<n>(BOOST_PP_LIST_AT(data, 0))BOOST_PP_COMMA_IF(BOOST_PP_NOT_EQUAL(n, BOOST_PP_LIST_AT(data, 1)))

#define UNPACK_TUPLE(tuple, first, last) \
    BOOST_PP_REPEAT_FROM_TO(first, BOOST_PP_INC(last), GET_ITEM, (tuple, (last, BOOST_PP_NIL)))

#endif /* _UTILITY_META_HPP_ */
