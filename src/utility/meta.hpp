#ifndef _UTILITY_META_HPP_
#define _UTILITY_META_HPP_

#include <opencv2/core/core.hpp>

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

#endif /* _UTILITY_META_HPP_ */
