#ifndef _IMGPROC_BILATERAL_FILTER_HPP_
#define _IMGPROC_BILATERAL_FILTER_HPP_

#include <libgimp/gimp.h>

#include <map>
#include <string>

namespace imgproc
{
    namespace bilateralFilter
    {
        void install();

        void registerName(std::map<std::string, void(*)(GimpDrawable *drawable)>& runFunctions);

        void run(GimpDrawable* drawable);
    }
}

#endif /* _IMGPROC_BILATERAL_FILTER_HPP_ */
