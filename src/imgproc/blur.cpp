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
#include "blur.hpp"

#include "exception.hpp"
#include "imgproc/enums.hpp"
#include "utility/bundle_widgets.hpp"
#include "utility/interface.hpp"
#include "utility/meta.hpp"
#include "widget/enum_widget.hpp"
#include "widget/message_dialog.hpp"
#include "widget/tuple_widget.hpp"

#include <gtk/gtk.h>
#include <libgimp/gimpui.h>
#include <opencv2/imgproc/imgproc.hpp>

#include <boost/optional/optional.hpp>
#include <boost/tuple/tuple.hpp>
#include <vector>

namespace
{
    typedef boost::tuple<cv::Size, cv::Point, int> Arguments;

    boost::optional<Arguments> presentDialog()
    {
        gimp_ui_init("opencv", FALSE);

        GimpDialog* dialog =
            GIMP_DIALOG(gimp_dialog_new("blur",
                                        "blur_role",
                                        NULL,
                                        GTK_DIALOG_MODAL,
                                        gimp_standard_help_func,
                                        "opencv_gimp",

                                        GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                        GTK_STOCK_OK,     GTK_RESPONSE_OK,

                                        NULL));

        std::vector<boost::tuple<std::string, GtkWidget*> > argumentPairs;

        TupleWidget<cv::Size> ksizeWidget(cv::Size(0, 0));
        argumentPairs.push_back(makeArgumentPair("ksize:", ksizeWidget));

        TupleWidget<cv::Point> anchorWidget(cv::Point(-1, -1));
        argumentPairs.push_back(makeArgumentPair("anchor:", anchorWidget));

        EnumWidget borderTypeWidget(TYPE_BORDER_ENUM, cv::BORDER_DEFAULT);
        argumentPairs.push_back(makeArgumentPair("borderType:", borderTypeWidget));


        gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG(dialog))),
                          GTK_WIDGET(bundleWidgets(argumentPairs)));
        gtk_widget_show_all(GTK_WIDGET(dialog));

        boost::optional<Arguments> arguments;
        if (gimp_dialog_run(GIMP_DIALOG(dialog)) == GTK_RESPONSE_OK)
        {
            arguments = Arguments(ksizeWidget, anchorWidget, borderTypeWidget);
        }

        gtk_widget_destroy(GTK_WIDGET(dialog));

        return arguments;
    }

} // namespace

void imgproc::blur::install()
{
    static GimpParamDef args[] =
    {
        {GIMP_PDB_INT32,    (gchar*)"run-mode", (gchar*)"Run mode"},
        {GIMP_PDB_IMAGE,    (gchar*)"image",    (gchar*)"Input image"},
        {GIMP_PDB_DRAWABLE, (gchar*)"drawable", (gchar*)"Input drawable"}
    };

    gimp_install_procedure("blur",
        "Blurs an image using the normalized box filter",
        "Blurs an image using the normalized box filter",
        "Gimp OpenCV Plugin Team",
        "Gimp OpenCV Plugin Team",
        "2013",
        "_blur",
        "RGB*, GRAY*",
        GIMP_PLUGIN,
        G_N_ELEMENTS(args), 0,
        args, NULL);
}

void imgproc::blur::registerName(std::map<std::string, void(*)(GimpRunMode, gint32, gint32)>& runFunctions)
{
    runFunctions["blur"] = imgproc::blur::run;
}

void imgproc::blur::run(GimpRunMode, gint32, gint32 drawableId)
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
        cv::Mat dst;
        cv::blur(src, dst, UNPACK_TUPLE(*arguments, 0, 2));
        setMatToDrawable(dst, drawable);
    }
    catch (cv::Exception const& e)
    {
        messageDialog(e.what());
    }
    catch (IncompatibleMat const& e)
    {
        messageDialog(e.what());
    }

    gimp_displays_flush();
    gimp_drawable_detach(drawable);
}
