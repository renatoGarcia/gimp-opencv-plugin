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
#ifndef _UTILITY_INTERFACE_IPP_
#define _UTILITY_INTERFACE_IPP_

#include <gdk-pixbuf/gdk-pixbuf.h>

template <typename T>
struct toCvTuple<cv::Size_<T> >
{
    cv::Size_<T> operator()(boost::array<GtkSpinButton*, 2> gtkSpinButtons)
    {
        return cv::Size_<T>(gtk_spin_button_get_value(gtkSpinButtons.at(0)),
                            gtk_spin_button_get_value(gtkSpinButtons.at(1)));
    }
};

template <typename T>
struct toCvTuple<cv::Point_<T> >
{
    cv::Point_<T> operator()(boost::array<GtkSpinButton*, 2> gtkSpinButtons)
    {
        return cv::Point_<T>(gtk_spin_button_get_value(gtkSpinButtons.at(0)),
                             gtk_spin_button_get_value(gtkSpinButtons.at(1)));
    }
};

template <typename T>
struct toCvTuple<cv::Scalar_<T> >
{
    cv::Scalar_<T> operator()(boost::array<GtkSpinButton*, 4> gtkSpinButtons)
    {
        return cv::Scalar_<T>(gtk_spin_button_get_value(gtkSpinButtons.at(0)),
                              gtk_spin_button_get_value(gtkSpinButtons.at(1)),
                              gtk_spin_button_get_value(gtkSpinButtons.at(2)),
                              gtk_spin_button_get_value(gtkSpinButtons.at(3)));
    }
};

#endif /* _UTILITY_INTERFACE_IPP_ */
