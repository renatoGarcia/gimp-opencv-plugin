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
#ifndef _WIDGET_PAIR_WIDGET_HPP_
#define _WIDGET_PAIR_WIDGET_HPP_

#include "utility/meta.hpp"
#include "utility/traits.hpp"

#include <gtk/gtk.h>

template <typename PairType>
class PairWidget
{
    typedef typename element<0, PairType>::type Element0;
    typedef typename element<1, PairType>::type Element1;

public:
    PairWidget(Element0 const& element0, Element1 const& element1)
        : gtkHBox(GTK_HBOX(gtk_hbox_new(FALSE, 0)))
        , gtkSpinButton_0(GTK_SPIN_BUTTON(gtk_spin_button_new_with_range(SpinButtonTraits<Element0>::min(),
                                                                         SpinButtonTraits<Element0>::max(),
                                                                         SpinButtonTraits<Element0>::step())))
        , gtkSpinButton_1(GTK_SPIN_BUTTON(gtk_spin_button_new_with_range(SpinButtonTraits<Element1>::min(),
                                                                         SpinButtonTraits<Element1>::max(),
                                                                         SpinButtonTraits<Element1>::step())))
    {
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(this->gtkSpinButton_0), element0);
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(this->gtkSpinButton_1), element1);

        this->addLabel("(");
        gtk_container_add(GTK_CONTAINER(this->gtkHBox), GTK_WIDGET(this->gtkSpinButton_0));
        this->addLabel(", ");
        gtk_container_add(GTK_CONTAINER(this->gtkHBox), GTK_WIDGET(this->gtkSpinButton_1));
        this->addLabel(")");
    }

    operator GtkWidget*()
    {
        return GTK_WIDGET(this->gtkHBox);
    }

    operator PairType()
    {
        return PairType(gtk_spin_button_get_value(this->gtkSpinButton_0),
                        gtk_spin_button_get_value(this->gtkSpinButton_1));
    }

private:
    GtkHBox* gtkHBox;
    GtkSpinButton* gtkSpinButton_0;
    GtkSpinButton* gtkSpinButton_1;

    void addLabel(char const* const label)
    {
        GtkLabel* gtkLabel = GTK_LABEL(gtk_label_new(label));
        gtk_container_add(GTK_CONTAINER(this->gtkHBox), GTK_WIDGET(gtkLabel));
    }
};


#endif /* _WIDGET_PAIR_WIDGET_HPP_ */
