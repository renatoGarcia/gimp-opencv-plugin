#ifndef _CVT_COLOR_HPP_
#define _CVT_COLOR_HPP_

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

#endif /* _CVTCOLOR_HPP_ */
