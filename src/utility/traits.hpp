/* Gimp OpenCV Plugin
 *
 * Copyright (c) 2013 see AUTHORS file.
 *
 * Gimp OpenCV Plugin is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or (at
 * your option) any later version.
 *
 * Gimp OpenCV Plugin is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with Gimp OpenCV Plugin. If not, see <http://www.gnu.org/licenses/>.
 */
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
