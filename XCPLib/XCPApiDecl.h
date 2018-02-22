#ifdef _WIN32
  #ifdef XCPLIB_EXPORTS //XCPLIB_EXPORTS macro is defined in the project settings
  #define XCP_API    __declspec(dllexport)
  #else
  #define XCP_API    __declspec(dllimport)
  #endif
#else
  #define XCP_API
#endif
