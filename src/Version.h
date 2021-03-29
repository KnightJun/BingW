#pragma once

#define VER_Major    0
#define VER_Minor    0
#define VER_Release    1 
#ifndef VER_Build
#define VER_Build    0
#endif
#define VER_Version    MAKELONG(MAKEWORD(VER_Major, VER_Minor), VER_Release)

#define _Stringizing(v)    #v
#define _VerJoin(a, b, c, d)  _Stringizing(a.b.c.d)

#define STR_BuildDate       __DATE__
#define STR_BuildTime       __TIME__
#define STR_BuilDateTime    __DATE__ " " __TIME__
#define STR_Version         _VerJoin(VER_Major, VER_Minor, VER_Release, VER_Build)

#define STR_AppName         "BingW"
#define STR_Author          "BingW"
#define STR_Corporation     "KnightJun"
#define STR_Web             ""
#define STR_Email           ""
#define STR_WebUrl          ""

#define STR_Description     "BingW"
#define STR_Copyright       "Copyright (C) 2021-2021 " STR_Corporation ". All rights reserved."
