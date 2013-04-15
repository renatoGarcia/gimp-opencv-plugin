#ifndef _WIDGET_BOOLEAN_WIDGET_HPP_
#define _WIDGET_BOOLEAN_WIDGET_HPP_

#include <gtk/gtk.h>

class BooleanWidget
{
public:
    BooleanWidget(bool const& defaultValue)
        : gtkCheckButton(GTK_CHECK_BUTTON(gtk_check_button_new()))
    {
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(this->gtkCheckButton), defaultValue);
    }

    operator GtkWidget*()
    {
        return GTK_WIDGET(this->gtkCheckButton);
    }

    operator bool()
    {
        return gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(this->gtkCheckButton));
    }

private:
    GtkCheckButton* gtkCheckButton;
};

#endif /* _WIDGET_BOOLEAN_WIDGET_HPP_ */
