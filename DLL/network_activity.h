#ifdef DO_EXPORTS
    #define EXPORTS_API __declspec(dllexport)
#else
    #define EXPORTS_API __declspec(dllimport)
#endif
extern "C" EXPORTS_API void __stdcall GetIpv4EnabledNics();
extern "C" EXPORTS_API DWORD __stdcall GetOutOctets();
extern "C" EXPORTS_API DWORD __stdcall GetInOctets();
extern "C" EXPORTS_API void __stdcall GetIpv4EnabledNicsLog();
extern "C" EXPORTS_API DWORD __stdcall GetOutOctetsLog();
extern "C" EXPORTS_API DWORD __stdcall GetInOctetsLog();
extern "C" EXPORTS_API void __stdcall CreateShortcutInStartUp();
extern "C" EXPORTS_API wchar_t* __stdcall GetStartupDirectory();
extern "C" EXPORTS_API void __stdcall WriteActiveNicsList();
extern "C" EXPORTS_API void __stdcall WriteActiveNicsListLog();