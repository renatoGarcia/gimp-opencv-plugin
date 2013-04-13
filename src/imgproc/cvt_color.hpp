#ifndef _IMGPROC_CVT_COLOR_HPP_
#define _IMGPROC_CVT_COLOR_HPP_

#include <libgimp/gimp.h>

#include <map>
#include <string>

namespace imgproc
{
    namespace cvtColor
    {
        void install();

        void registerName(std::map<std::string, void(*)(GimpDrawable *drawable)>& runFunctions);

        void run(GimpDrawable* drawable);
    }
}

#endif /* _IMGPROC_CVTCOLOR_HPP_ */
