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
#ifndef _UTILITY_BUNDLE_WIDGETS_HPP_
#define _UTILITY_BUNDLE_WIDGETS_HPP_

#include <gtk/gtk.h>

#include <boost/tuple/tuple.hpp>
#include <string>
#include <vector>

template <typename T1, typename T2>
boost::tuple<std::string, GtkWidget*> makeArgumentPair(T1 const& label, T2& widget)
{
    return boost::make_tuple(std::string(label), (GtkWidget*)widget);
}

/**
 * @brief Bundle the pairs of (<argument label>, <argument input widget>) in a GtkTable.
 *
 * @param argumentPairs A vector of tuples with the argument pairs.
 * @return The GtkTable just created.
 */
GtkTable* bundleWidgets(std::vector<boost::tuple<std::string, GtkWidget*> > const& argumentPairs);

#endif /* _UTILITY_BUNDLE_WIDGETS_HPP_ */
