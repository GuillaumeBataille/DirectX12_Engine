#pragma once 

#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>

#include <windows.h>			// Window,message,threads
#include <WindowsX.h>			// Inputs and interactions

#include <d3d12.h>				// Direct X 12 API
#include <dxgi1_4.h>			// Direct X 12 API Graphic stuff  
#include <D3Dcompiler.h>		// Direct X Compiler for shaders
#include <DirectXMath.h>		// Direct X Maths functions and classes
#include <DirectXCollision.h>	// Direct X Collision functions and classes
#include <DirectXColors.h>		// Direct X Colors ready to use with functions (works on shaders)

#include <wrl.h>				//Wrapper for using COM pointers easily
#include <comdef.h>				//Functions to use COM for errors


/* Local dependencies */
#include "../Depedencies/d3dx12.h"		// Toolkit for microsoft to make DX12 smoother to use
#include "../Depedencies/MathHelper.h"	// Math functions if needed
#include "../resource.h"				// Ressource manager
#include "GlobalVar.h"

//#include <DirectXPackedVector.h>

//#include <memory>
//#include <algorithm>
//#include <vector>
//#include <array>
//#include <cstdint>
//#include <sstream>
//#include <cassert>
//#include "DDSTextureLoader.h"

// Link necessary d3d12 libraries.
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "dxgi.lib")

/* If debug mod is enable, map memory to identify memory leaks */
#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

#define HInstance() GetModuleHandle(NULL)
