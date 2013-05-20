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
#ifndef _WIDGET_ENUM_WIDGET_HPP_
#define _WIDGET_ENUM_WIDGET_HPP_

#include <glib-object.h>
#include <libgimp/gimpui.h>

#include <boost/noncopyable.hpp>

class EnumWidget
    : private boost::noncopyable
{
public:
    EnumWidget(GType enumType, gint defaultItem)
        : gimpEnumComboBox(GIMP_ENUM_COMBO_BOX(gimp_enum_combo_box_new(enumType)))
    {
        g_object_ref_sink(G_OBJECT(this->gimpEnumComboBox));
        gimp_int_combo_box_set_active(GIMP_INT_COMBO_BOX(this->gimpEnumComboBox), defaultItem);
    }

    ~EnumWidget()
    {
        g_object_unref(G_OBJECT(this->gimpEnumComboBox));
    }

    operator GtkWidget*() const
    {
        return GTK_WIDGET(this->gimpEnumComboBox);
    }

    operator int() const
    {
        gint value;
        gimp_int_combo_box_get_active(GIMP_INT_COMBO_BOX(this->gimpEnumComboBox), &value);

        return value;
    }

private:
    GimpEnumComboBox* const gimpEnumComboBox;
};

#endif /* _WIDGET_ENUM_WIDGET_HPP_ */
