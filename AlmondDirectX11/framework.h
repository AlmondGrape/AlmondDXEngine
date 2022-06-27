#pragma once

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
// Windows 헤더 파일
#include <windows.h>

// C++ STL
#include <iostream>
#include <string>
#include <map>
#include <vector>

// DirectX 11
#include <d3d11.h>
#include <dxgi.h>
#include <wrl.h>

// DiretcX 헬퍼
#include "D3DUtil.h"

// DirectX Simple Math
#include "SimpleMath.h"


// 각종 lib
#pragma comment(lib, "d3d11")
#pragma comment(lib, "dxgi")
#pragma comment(lib, "dxguid")
#pragma comment(lib, "d3dcompiler")

// 각종 using typedef
using int8 = __int8;
using int16 = __int16;
using int32 = __int32;
using int64 = __int64;
using uint8 = unsigned __int8;
using uint16 = unsigned __int16;
using uint32 = unsigned __int32;
using uint64 = unsigned __int64;

// 임시

