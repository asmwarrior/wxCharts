#ifndef _WXCHARTS_API_MACRO_H_
#define _WXCHARTS_API_MACRO_H_

#ifdef WXCHARTS_BUILD_DLL
#define WXCHARTS_API __declspec(dllexport)
#elif defined(WXCHARTS_USE_DLL)
#define WXCHARTS_API __declspec(dllimport)
#else
#define WXCHARTS_API
#endif

#endif // _WXCHARTS_API_MACRO_H_