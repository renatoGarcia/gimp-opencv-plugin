#include "bilateral_filter.hpp"

#include "imgproc/enums.hpp"
#include "utility/bundle_widgets.hpp"
#include "utility/conversions.hpp"
#include "utility/meta.hpp"
#include "widget/numeric_widget.hpp"
#include "widget/enum_widget.hpp"

#include <gtk/gtk.h>
#include <libgimp/gimpui.h>
#include <opencv2/imgproc/imgproc.hpp>

#include <boost/optional/optional.hpp>
#include <boost/tuple/tuple.hpp>
#include <vector>

namespace
{
    typedef boost::tuple<int, double, double, int> Arguments;

    boost::optional<Arguments> presentDialog()
    {
        gimp_ui_init("opencv", FALSE);

        GimpDialog* dialog =
            GIMP_DIALOG(gimp_dialog_new("bilateralFilter",
                                        "bilateralFilter_role",
                                        NULL,
                                        GTK_DIALOG_MODAL,
                                        gimp_standard_help_func,
                                        "opencv_gimp",

                                        GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                        GTK_STOCK_OK,     GTK_RESPONSE_OK,

                                        NULL));

        std::vector<boost::tuple<std::string, GtkWidget*> > argumentPairs;

        NumericWidget<int> dWidget(0);
        argumentPairs.push_back(makeArgumentPair("d:", dWidget));

        NumericWidget<double> sigmaColorWidget(0.0);
        argumentPairs.push_back(makeArgumentPair("sigmaColor:", sigmaColorWidget));

        NumericWidget<double> sigmaSpaceWidget(0.0);
        argumentPairs.push_back(makeArgumentPair("sigmaSpace:", sigmaSpaceWidget));

        EnumWidget borderTypeWidget(TYPE_BORDER_ENUM, cv::BORDER_DEFAULT);
        argumentPairs.push_back(makeArgumentPair("borderType:", borderTypeWidget));


        gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG(dialog))),
                          GTK_WIDGET(bundleWidgets(argumentPairs)));
        gtk_widget_show_all(GTK_WIDGET(dialog));

        boost::optional<Arguments> arguments;
        if (gimp_dialog_run(GIMP_DIALOG(dialog)) == GTK_RESPONSE_OK)
        {
            arguments = Arguments(dWidget, sigmaColorWidget, sigmaSpaceWidget, borderTypeWidget);
        }

        gtk_widget_destroy(GTK_WIDGET(dialog));

        return arguments;
    }

} // namespace

void imgproc::bilateralFilter::install()
{
    static GimpParamDef args[] =
    {
        {GIMP_PDB_INT32,    (gchar*)"run-mode", (gchar*)"Run mode"},
        {GIMP_PDB_IMAGE,    (gchar*)"image",    (gchar*)"Input image"},
        {GIMP_PDB_DRAWABLE, (gchar*)"drawable", (gchar*)"Input drawable"}
    };

    gimp_install_procedure("bilateralFilter",
        "OpenCV color converter",
        "OpenCV color converter",
        "Renato Florentino Garcia",
        "Renato Florentino Garcia",
        "2013",
        "_bilateralFilter",
        "RGB*, GRAY*",
        GIMP_PLUGIN,
        G_N_ELEMENTS(args), 0,
        args, NULL);
}

void imgproc::bilateralFilter::registerName(std::map<std::string, void(*)(GimpDrawable *drawable)>& runFunctions)
{
    runFunctions["bilateralFilter"] = imgproc::bilateralFilter::run;
}

void imgproc::bilateralFilter::run(GimpDrawable* drawable)
{
    boost::optional<Arguments> arguments = presentDialog();
    if (!arguments)
    {
        return;
    }

    cv::Mat src = drawableToMat(drawable);
    cv::Mat dst;
    cv::bilateralFilter(src, dst, UNPACK_TUPLE(*arguments, 0, 3));
    setMatToDrawable(dst, drawable);
}
