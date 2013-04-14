#include "blur.hpp"

#include "imgproc/enums.hpp"
#include "utility/bundle_widgets.hpp"
#include "utility/conversions.hpp"
#include "widget/enum_widget.hpp"
#include "widget/pair_widget.hpp"

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

        PairWidget<cv::Size_<int> > ksizeWidget(0, 0);
        argumentPairs.push_back(makeArgumentPair("ksize:", ksizeWidget));

        PairWidget<cv::Point_<int> > anchorWidget(-1, -1);
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
        "Renato Florentino Garcia",
        "Renato Florentino Garcia",
        "2013",
        "_blur",
        "RGB*, GRAY*",
        GIMP_PLUGIN,
        G_N_ELEMENTS(args), 0,
        args, NULL);
}

void imgproc::blur::registerName(std::map<std::string, void(*)(GimpDrawable *drawable)>& runFunctions)
{
    runFunctions["blur"] = imgproc::blur::run;
}

void imgproc::blur::run(GimpDrawable *drawable)
{
    boost::optional<Arguments> arguments = presentDialog();
    if (!arguments)
    {
        return;
    }

    cv::Mat src = drawableToMat(drawable);
    cv::Mat dst;
    cv::blur(src, dst, boost::get<0>(*arguments), boost::get<1>(*arguments), boost::get<2>(*arguments));
    setMatToDrawable(dst, drawable);
}
