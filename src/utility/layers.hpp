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
#ifndef _UTILITY_LAYERS_HPP_
#define _UTILITY_LAYERS_HPP_

#include <gtk/gtk.h>

#include <boost/optional/optional.hpp>
#include <string>

class Layers
{
public:
    static void setCurrentImageId(gint32 imageId);

    // Presents the tree of layers and return the drawable id of the selected layer, if any.
    // Can throw std::runtime_error.
    static boost::optional<int> selectLayer();

    template <typename SinglePassRange>
    static void insertLayersGroup(std::string const& layerGroupName, SinglePassRange const& range);

private:
    enum
    {
        ID_COLUMN,
        IMAGE_COLUMN,
        NAME_COLUMN,
        N_COLUMNS
    };

    static GtkTreeView* buildTreeView(GtkTreeStore* store);
    static void populateTree(gint32 itemId, GtkTreeStore* store, GtkTreeIter* parent);

    Layers();

    static gint32 imageId;
};

#include "layers.ipp"

#endif /* _UTILITY_LAYERS_HPP_ */
