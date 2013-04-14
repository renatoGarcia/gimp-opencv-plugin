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
