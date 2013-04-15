#include "imgproc.hpp"

#include "imgproc/bilateral_filter.hpp"
#include "imgproc/blur.hpp"
#include "imgproc/box_filter.hpp"
#include "imgproc/cvt_color.hpp"

void imgproc::install()
{
    bilateralFilter::install();
    gimp_plugin_menu_register("bilateralFilter",
                              "<Image>/_OpenCV/_imgproc/");

    blur::install();
    gimp_plugin_menu_register("blur",
                              "<Image>/_OpenCV/_imgproc/");

    boxFilter::install();
    gimp_plugin_menu_register("boxFilter",
                              "<Image>/_OpenCV/_imgproc/");

    cvtColor::install();
    gimp_plugin_menu_register("cvtColor",
                              "<Image>/_OpenCV/_imgproc/");

}

void imgproc::registerNames(std::map<std::string, void(*)(GimpDrawable *drawable)>& runFunctions)
{
    bilateralFilter::registerName(runFunctions);
    blur::registerName(runFunctions);
    cvtColor::registerName(runFunctions);
    boxFilter::registerName(runFunctions);
}
