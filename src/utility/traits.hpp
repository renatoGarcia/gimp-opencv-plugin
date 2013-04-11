#ifndef _UTILITY_TRAITS_HPP_
#define _UTILITY_TRAITS_HPP_

#include <glib.h>

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

#endif /* _UTILITY_TRAITS_HPP_ */
