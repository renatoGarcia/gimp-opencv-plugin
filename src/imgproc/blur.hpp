#ifndef _IMGPROC_BLUR_HPP_
#define _IMGPROC_BLUR_HPP_

#include <libgimp/gimp.h>

#include <map>
#include <string>

namespace imgproc
{
    namespace blur
    {
        void install();

        void registerName(std::map<std::string, void(*)(GimpDrawable *drawable)>& runFunctions);

        void run(GimpDrawable* drawable);
    }
}


#endif /* _IMGPROC_BLUR_HPP_ */
