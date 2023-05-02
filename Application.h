#pragma once

#include <windows.h>
#include <d3d11_4.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>
#include <vector>
#include "Loaders\DDSTextureLoader.h"
#include "Loaders\OBJLoader.h"
#include "resource.h"
#include "OrbitingCamera.h"
#include "Structures.h"
#include "GameObject.h"
#include "Vector3.h"
#include "Timer.h"

#include "SphereCollider.h"
#include "AABBCollider.h"
#include "PlaneCollider.h"
#include "Contact.h"
#include "StaticModel.h"

using namespace DirectX;

class Collider;

class Application
{
private:
#pragma region ID3D11
	/// <summary>Handle to an instance, i.e. the currently running instance of the program.</summary>
	HINSTANCE               _hInst;
	/// <summary>Handle to a window, i.e. the window the current instance is running in</summary>
	HWND                    _hWnd;
	/// <summary>Whether to use which type of drivers to use.
	/// <para>See https://learn.microsoft.com/en-us/windows/win32/api/d3dcommon/ne-d3dcommon-d3d_driver_type for each type.</para>
	/// <para>Used when calling D3D11CreateDevice.</para></summary>
	D3D_DRIVER_TYPE         _driverType;
	/// <summary>Describes the set of features targeted by a Direct3D device.</summary>
	D3D_FEATURE_LEVEL       _featureLevel;
	/// <summary>Used for creating resources, such as the many shaders in the pipeline.</summary>
	ID3D11Device*           _pd3dDevice;
	/// <summary>Used for binding ID3D11Device's resouces to the pipleline.</summary>
	ID3D11DeviceContext*    _pImmediateContext;
	/// <summary>An interface that implements one or more surfaces for storing rendered data before presenting it to an output.</summary>
	IDXGISwapChain*         _pSwapChain;
	/// <summary>This variable is a pointer to an object that holds all the information about the render target, i.e. the back buffer</summary>
	ID3D11RenderTargetView* _pRenderTargetView;
	/// <summary>Manages the executable program that controls vertex-shader stage of the pipeline.</summary>
	ID3D11VertexShader*     _pVertexShader;
	/// <summary>Manages the executable program that controls pixel-shader stage of the pipeline.</summary>
	ID3D11PixelShader*      _pPixelShader;
	/// <summary>Holds a definition of how to feed vertex data that is laid out in memory into the input-assembler stage of the graphics pipeline.</summary>
	ID3D11InputLayout*      _pVertexLayout;

	/// <summary>Acts as a way of passing constants to the GPU.</summary>
	ID3D11Buffer*           _pConstantBuffer;

	/// <summary>Accesses a texture during depth-stencil testing (stencil buffer/z buffer).</summary>
	ID3D11DepthStencilView* _depthStencilView = nullptr;
	/// <summary>The texture the depth stencil view accesses.</summary>
	ID3D11Texture2D* _depthStencilBuffer = nullptr;

	/// <summary>Specifies how textures are sampled. In this case, linear sampling is used.</summary>
	ID3D11SamplerState * _pSamplerLinear = nullptr;
#pragma endregion

	/// <summary>A pointer to the Timer class, which is used to limit frames per second.</summary>
	Timer* m_timer;

	/// <summary>A resource view specifies the subresouces a shader can access during rendering. 
	/// <para>In this calse, it allows access to a texture as a shader resource.</para>
	/// <para>This one stores the stone texture that appears on the cubes.</para></summary>
	ID3D11ShaderResourceView* m_stoneTextureRV = nullptr;
	/// <summary>Stores the texture of the tiled ground.</summary>
	ID3D11ShaderResourceView* m_groundTextureRV = nullptr;

	/// <summary>Stores the vertex and index buffers etc. of the donut object.</summary>
	MeshData m_donutMeshData;
	MeshData m_sphereMeshData;
	MeshData m_cubeMeshData;
	MeshData m_planeMeshData;

	/// <summary>Stores the ambient diffuse and specular component of the scene's directional light.</summary>
	Light m_directionalLight;

	/// <summary>A vector containing a collection of each of the game objects within the scene.</summary>
	vector<GameObject *> m_gameObjects;
	/// <summary>A pointer to the game object currently under player controlled.</summary>
	GameObject* m_controlledObject;

	OrbitingCamera* m_camera = nullptr;

	UINT _WindowHeight;
	UINT _WindowWidth;

	// Render dimensions - Change here to alter screen resolution
	UINT _renderHeight = 1080;
	UINT _renderWidth = 1920;

	ID3D11DepthStencilState* DSLessEqual;
	ID3D11RasterizerState* RSCullNone;

	ID3D11RasterizerState* CCWcullMode;
	ID3D11RasterizerState* CWcullMode;

private:
	HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow);
	HRESULT InitDevice();
	HRESULT InitWorld();
	void Cleanup();
	HRESULT CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);
	HRESULT InitShadersAndInputLayout();
	HRESULT InitVertexBuffer();
	HRESULT InitIndexBuffer();

	void UpdateWorld(float dt);
	void HandleCollisions(float dt);
	void HandleInput();

	void MoveForward();
	void MoveBackward();
	void MoveUp();
	void MoveDown();
	void MoveLeft();
	void MoveRight();

	void Select(unsigned int index);

public:
	Application();
	~Application();

	HRESULT Initialise(HINSTANCE hInstance, int nCmdShow);

	bool HandleKeyboard(MSG msg);

	void Update();
	void Draw();
};

