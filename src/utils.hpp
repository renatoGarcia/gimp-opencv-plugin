#ifndef _UTILS_HPP_
#define _UTILS_HPP_

#include <glib.h>
#include <libgimp/gimp.h>
#include <opencv2/core/core.hpp>

template <typename T>
struct SpinButtonTraits
{};

template <>
struct SpinButtonTraits<int>
{
    static gdouble min()
    {
        return G_MININT;
    }
    static gdouble max()
    {
        return G_MAXINT;
    }
    static gdouble step()
    {
        return 1.0;
    }
};

template <>
struct SpinButtonTraits<double>
{
    static gdouble min()
    {
        return -G_MAXDOUBLE;
    }
    static gdouble max()
    {
        return G_MAXDOUBLE;
    }
    static gdouble step()
    {
        return 1.0E-7;
    }
};

cv::Mat drawableToMat(GimpDrawable* drawable);

void setMatToDrawable(cv::Mat& mat, GimpDrawable* drawable);

#endif /* _UTILS_HPP_ */
