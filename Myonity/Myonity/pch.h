// pch.h: �̸� �����ϵ� ��� �����Դϴ�.
#pragma once

#ifndef PCH_H
#define PCH_H

//-----------------------------------------------------------------
//                              Windows
//-----------------------------------------------------------------
#include <windows.h>
#include <windowsx.h>
#include <SDKDDKVer.h>

#define NOMINMAX // Windows ��� ���Ͽ��� ���ǵǾ� �ִ� min �� max ��ũ�θ� ��Ȱ��ȭ.

//-----------------------------------------------------------------
//                              DirectX
//-----------------------------------------------------------------
#include <d3d11.h>
#pragma comment (lib, "d3d11.lib")

#include <dwrite.h> //�ؽ�Ʈ
#pragma comment(lib, "dwrite")

#include <wincodec.h>	//�̹��� ���ڵ�
#pragma comment(lib, "windowscodecs")

#include <d3dcompiler.h>
#pragma comment (lib, "D3DCompiler.lib")

#include <dxgi.h>
#pragma comment (lib, "dxgi.lib")

#include <d3dcommon.h>
#pragma comment (lib, "User32.lib")

#include <wrl/client.h> //COM ����Ʈ������ ���
using namespace Microsoft::WRL;
namespace wrl = Microsoft::WRL;

#include <d2d1.h>
#include <d2d1_1.h>
#pragma comment(lib, "d2d1.lib")

//#include <D3DX11async.h>



//-----------------------------------------------------------------
//                           DirectXTK
//-----------------------------------------------------------------

//#include "dxtk/Audio.h"
//#include "dxtk/BufferHelpers.h"
//#include "dxtk/CommonStates.h"
#include "dxtk/DDSTextureLoader.h"
#include "dxtk/DirectXHelpers.h"
#include "dxtk/Effects.h"
//#include "dxtk/GamePad.h"
#include "dxtk/GeometricPrimitive.h"
#include "dxtk/GraphicsMemory.h"
//#include "dxtk/Keyboard.h"
#include "dxtk/Model.h"
//#include "dxtk/Mouse.h"
//#include "dxtk/PostProcess.h"
//#include "dxtk/PrimitiveBatch.h"
//#include "dxtk/ScreenGrab.h"
#include "dxtk/SimpleMath.h"
//#include "dxtk/SpriteBatch.h"
//#include "dxtk/SpriteFont.h"
//#include "dxtk/VertexTypes.h"
//#include "dxtk/XboxDDSTextureLoader.h"
//#include "dxtk/WICTextureLoader.h"

namespace dsm = DirectX::SimpleMath;

//-----------------------------------------------------------------
//                              STL
//-----------------------------------------------------------------
#include <vector>	
using std::vector;

#include <map>
using std::map;

#include <unordered_map>
using std::unordered_map;

#include <string>
using std::string;
using std::wstring;

#include  <utility>
using std::make_pair;
using std::pair;

#include <cmath>
#define _USE_MATH_DEFINES

#include <limits>
using std::numeric_limits; //�ڷ����� �ּ� �ִ� ��

#include <array>
#include <random>
#include <cassert>
#include <algorithm>
#include <tuple>
#include <locale>
#include <stdlib.h>
#include <memory>
#include <iterator>
#include <thread>
#include <future>
#include <stdexcept>
#include <sstream>

//-----------------------------------------------------------------
//                             Defines
//-----------------------------------------------------------------
#include "EnginePreferences.h"
#include "EngineDefine.h"

//-----------------------------------------------------------------
//                             Defines
//-----------------------------------------------------------------

// Windows API���� ������� ���� ��ɵ��� ��Ȱ��ȭ
// ���α׷� ���� �ÿ� ���ʿ��� ���ҽ� ����� ���̰� ���� �ð��� ���� ����.
#ifndef FULL_WINTARD
#define WIN32_LEAN_AND_MEAN 
//GDI�� Campmasks���� ���
#define NOGDICAPMASKS 
// �ý����� ���÷��� �� ��Ʈ ������ ���õ� ������ �����ϴ°�
#define NOSYSMETRICS
// ������ �޴��� ���õ� ���
#define NOMENUS
// �������� �ٷ�� �Ϻ� ���
#define NOICONS
// �ý��۸�ɰ� ���õ� ��� (â�� �ൿ ����)
#define NOSYSCOMMANDS
// ������ �۾�(Raster Operations) ���� (��Ʈ���� �ٷ� ��)
#define NORASTEROPS
// Ư�� �ϵ���� �Ǵ� ������ü�� ���õ� ���ҽ� 
#define OEMRESOURCE
// ���ڿ��� ������ ������ ������ �ڿ�
#define NOATOM
// ������ ���� �� �ٿ��ֱ� Ŭ������
#define NOCLIPBOARD
// ���� ���� ���
#define NOCOLOR
//��Ʈ�� ������(GUI ��Ʈ�� ~ ��ư, �ؽ�Ʈ ����)
#define NOCTLMGR
// �ؽ��� �׸���
#define NODRAWTEXT
// Ŀ�θ�� �Լ��� ���õ� ���(�ý��� ���ҽ��� ����)
#define NOKERNEL
// �޸� ������
#define NOMEMMGR
// ��Ÿ����(�׷��� �׸��� ���õ� Ư�� ���� ����)
#define NOMETAFILE
// ���� ���� ���� ���)
#define NOOPENFILE
// ��ũ�� �� ����
#define NOSCROLL
// ���� (��׶��� �۾�)
#define NOSERVICE
// ���� ���
#define NOSOUND
// �ؽ�Ʈ ��� ���� ����
#define NOTEXTMETRIC
// ������ �ڵ�
#define NOWH
// �ø��� ��� 
#define NOCOMM
// �Ϻ��� �Է� �� ����
#define NOKANJI
// ���� �ý���
#define NOHELP
// �������Ϸ�(�ڵ� ���� �м�)
#define NOPROFILER
// ������ ������ ����
#define NODEFERWINDOWPOS
// Window Media Center
#define NOMCX
// ���� ���ν��� ȣ�� (�л� �ý��� �� ���� ���� ȣ��)
#define NORPC
// ���Ͻ� ����(COM�� ���õ� ���)
#define NOPROXYSTUB
// �̹��� ���� ���(�̹��� ó�� �� ���÷���)
#define NOIMAGE
// ������ ����̺� ���� ��� 
#define NOTAPE

#endif //FULL_WINTARD
#endif //PCH_H
