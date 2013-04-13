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
