#ifndef Engine_Include_h__
#define Engine_Include_h__

#include <map>
#include <list>
#include <vector>
#include <algorithm>
#include <time.h>
#include <d3dx9.h>
#include <d3d9.h>
#include <crtdbg.h>

#include <functional>
#include <iostream>
#include <io.h>
#include <atlimage.h>

#include <MMSystem.h>
#include <unordered_map.hpp>

#define DIRECTINPUT_VERSION		0x0800
#include <dinput.h>

#pragma comment(lib,"winmm.lib")

using namespace std;
using namespace boost;

#include "Engine_Enum.h"
#include "Engine_Macro.h"
#include "Engine_Struct.h"
#include "Engine_Function.h"
#include "Engine_Functor.h"


#pragma warning(disable : 4251)

#ifdef _DEBUG
#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif

#endif // Engine_Include_h__