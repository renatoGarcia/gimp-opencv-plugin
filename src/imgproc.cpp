#include "imgproc.hpp"

void imgproc::install()
{
    cvtColor::install();
    gimp_plugin_menu_register("cvtColor",
                              "<Image>/_OpenCV/_imgproc/");

    bilateralFilter::install();
    gimp_plugin_menu_register("bilateralFilter",
                              "<Image>/_OpenCV/_imgproc/");
}
