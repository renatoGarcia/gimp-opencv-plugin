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
#include "build_pyramid.hpp"

#include "exception.hpp"
#include "imgproc/enums.hpp"
#include "utility/bundle_widgets.hpp"
#include "utility/interface.hpp"
#include "utility/meta.hpp"
#include "widget/enum_widget.hpp"
#include "widget/message_dialog.hpp"
#include "widget/numeric_widget.hpp"

#include <gtk/gtk.h>
#include <libgimp/gimpui.h>
#include <opencv2/imgproc/imgproc.hpp>

#include <boost/optional/optional.hpp>
#include <boost/tuple/tuple.hpp>
#include <vector>

namespace
{
    typedef boost::tuple<int, int> Arguments;

    boost::optional<Arguments> presentDialog()
    {
        gimp_ui_init("opencv", FALSE);

        GimpDialog* dialog =
            GIMP_DIALOG(gimp_dialog_new("buildPyramid",
                                        "buildPyramid_role",
                                        NULL,
                                        GTK_DIALOG_MODAL,
                                        gimp_standard_help_func,
                                        "opencv_gimp",

                                        GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                        GTK_STOCK_OK,     GTK_RESPONSE_OK,

                                        NULL));

        std::vector<boost::tuple<std::string, GtkWidget*> > argumentPairs;

        NumericWidget<int> maxlevelWidget(0);
        argumentPairs.push_back(makeArgumentPair("maxlevel:", maxlevelWidget));

        EnumWidget borderTypeWidget(TYPE_BORDER_ENUM, cv::BORDER_DEFAULT);
        argumentPairs.push_back(makeArgumentPair("borderType:", borderTypeWidget));

        gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG(dialog))),
                          GTK_WIDGET(bundleWidgets(argumentPairs)));
        gtk_widget_show_all(GTK_WIDGET(dialog));

        boost::optional<Arguments> arguments;
        if (gimp_dialog_run(GIMP_DIALOG(dialog)) == GTK_RESPONSE_OK)
        {
            arguments = Arguments(maxlevelWidget, borderTypeWidget);
        }

        gtk_widget_destroy(GTK_WIDGET(dialog));

        return arguments;
    }

} // namespace

void imgproc::buildPyramid::install()
{
    static GimpParamDef args[] =
        {
            {GIMP_PDB_INT32,    (gchar*)"run-mode", (gchar*)"Run mode"},
            {GIMP_PDB_IMAGE,    (gchar*)"image",    (gchar*)"Input image"},
            {GIMP_PDB_DRAWABLE, (gchar*)"drawable", (gchar*)"Input drawable"}
        };

    gimp_install_procedure("buildPyramid",
                           "Constructs the Gaussian pyramid for an image",
                           "Constructs the Gaussian pyramid for an image",
                           "Gimp OpenCV Plugin Team",
                           "Gimp OpenCV Plugin Team",
                           "2013",
                           "_buildPyramid",
                           "RGB*, GRAY*",
                           GIMP_PLUGIN,
                           G_N_ELEMENTS(args), 0,
                           args, NULL);
}

void imgproc::buildPyramid::registerName(std::map<std::string, void(*)(GimpRunMode, gint32, gint32)>& runFunctions)
{
    runFunctions["buildPyramid"] = imgproc::buildPyramid::run;
}

void imgproc::buildPyramid::run(GimpRunMode, gint32 imageId, gint32 drawableId)
{
    boost::optional<Arguments> arguments = presentDialog();
    if (!arguments)
    {
        return;
    }

    GimpDrawable* const drawable = gimp_drawable_get(drawableId);

    try
    {
        cv::Mat src = drawableToMat(drawable);
        std::vector<cv::Mat> dst;
        for (int i = 0; i < boost::get<0>(*arguments); ++i)
        {
            dst.push_back(cv::Mat());
        }

        cv::buildPyramid(src, dst, UNPACK_TUPLE(*arguments, 0, 1));

        insertLayersGroup(imageId, "pyramid", dst);
    }
    catch (cv::Exception const& e)
    {
        messageDialog(e.what());
    }
    catch (IncompatibleMat const& e)
    {
        messageDialog(e.what());
    }
}
