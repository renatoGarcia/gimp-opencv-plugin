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
#ifndef _WIDGET_NUMERIC_WIDGET_HPP_
#define _WIDGET_NUMERIC_WIDGET_HPP_

#include "utility/traits.hpp"

#include <gtk/gtk.h>

template <typename NumericType>
class NumericWidget
{
public:
    NumericWidget(NumericType const& defaultValue)
        : gtkSpinButton(GTK_SPIN_BUTTON(gtk_spin_button_new_with_range(SpinButtonTraits<NumericType>::min(),
                                                                       SpinButtonTraits<NumericType>::max(),
                                                                       SpinButtonTraits<NumericType>::step())))
    {
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(this->gtkSpinButton), defaultValue);
    }

    operator GtkWidget*()
    {
        return GTK_WIDGET(this->gtkSpinButton);
    }

    operator NumericType()
    {
        return gtk_spin_button_get_value(this->gtkSpinButton);
    }

private:
    GtkSpinButton* gtkSpinButton;
};


#endif /* _WIDGET_NUMERIC_WIDGET_HPP_ */
