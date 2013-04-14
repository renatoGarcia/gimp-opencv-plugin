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
