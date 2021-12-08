#ifndef _WXCHARTS_EXPORT_H_
#define _WXCHARTS_EXPORT_H_

#ifdef WXCHARTS_BUILD_DLL
    #define WXCHARTS_EXPORT __declspec(dllexport)
#elif defined(WXCHARTS_USE_DLL)
    #define WXCHARTS_EXPORT __declspec(dllimport)
#else
    #define WXCHARTS_EXPORT
#endif

#endif // _WXCHARTS_EXPORT_H_
