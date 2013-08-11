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
#ifndef _WIDGET_BOOLEAN_WIDGET_HPP_
#define _WIDGET_BOOLEAN_WIDGET_HPP_

#include <gtk/gtk.h>

#include <boost/noncopyable.hpp>

class BooleanWidget
    : private boost::noncopyable
{
public:
    explicit BooleanWidget(bool const& defaultValue)
        : gtkCheckButton(GTK_CHECK_BUTTON(gtk_check_button_new()))
    {
        g_object_ref_sink(G_OBJECT(this->gtkCheckButton));
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(this->gtkCheckButton), defaultValue);
    }

    ~BooleanWidget()
    {
        g_object_unref(G_OBJECT(this->gtkCheckButton));
    }

    BooleanWidget& operator=(bool const& value)
    {
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(this->gtkCheckButton), value);
        return *this;
    }

    operator GtkWidget*() const
    {
        return GTK_WIDGET(this->gtkCheckButton);
    }

    operator bool() const
    {
        return gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(this->gtkCheckButton));
    }

private:
    GtkCheckButton* const gtkCheckButton;
};

#endif /* _WIDGET_BOOLEAN_WIDGET_HPP_ */
