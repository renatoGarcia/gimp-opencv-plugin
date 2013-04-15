#ifndef _IMGPROC_BOX_FILTER_HPP_
#define _IMGPROC_BOX_FILTER_HPP_

#include <libgimp/gimp.h>

#include <map>
#include <string>

namespace imgproc
{
    namespace boxFilter
    {
        void install();

        void registerName(std::map<std::string, void(*)(GimpDrawable *drawable)>& runFunctions);

        void run(GimpDrawable* drawable);
    }
}

#endif /* _IMGPROC_BOX_FILTER_HPP_ */
