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
#include "bundle_widgets.hpp"

GtkTable* bundleWidgets(std::vector<boost::tuple<std::string, GtkWidget*> > const& argumentPairs)
{
    std::size_t const nPairs = argumentPairs.size();

    GtkTable* const gtkTable = GTK_TABLE(gtk_table_new(nPairs, 2, FALSE));

    for (int i = 0; i < nPairs; ++i)
    {
        boost::tuple<std::string, GtkWidget*> const& pair = argumentPairs.at(i);

        GtkLabel* const gtkLabel = GTK_LABEL(gtk_label_new(boost::get<0>(pair).c_str()));
        gtk_table_attach_defaults(GTK_TABLE(gtkTable), GTK_WIDGET(gtkLabel), 0, 1, i, i+1);

        gtk_table_attach_defaults(GTK_TABLE(gtkTable), boost::get<1>(pair), 1, 2, i, i+1);
    }

    return gtkTable;
}
