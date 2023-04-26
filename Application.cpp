#include "Application.h"
#include "Application.h"

#define NUMBEROFCUBES 2
///<summary>The amount of time each frame to be 60 FPS.</summary>
#define FPS60 1.0f/60.0f

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

bool Application::HandleKeyboard(MSG msg)
{
	XMFLOAT3 cameraPosition = m_camera->GetPosition();

	switch (msg.wParam)
	{
	case VK_UP:
		m_camera->ZoomIn();
		return true;
		break;

	case VK_DOWN:
		m_camera->ZoomOut();
		return true;
		break;

	case VK_RIGHT:
		m_camera->OrbitRight();
		return true;
		break;

	case VK_LEFT:
		m_camera->OrbitLeft();
		return true;
		break;
	}

	return false;
}

Application::Application()
{
	_hInst = nullptr;
	_hWnd = nullptr;
	_driverType = D3D_DRIVER_TYPE_NULL;
	_featureLevel = D3D_FEATURE_LEVEL_11_0;
	_pd3dDevice = nullptr;
	_pImmediateContext = nullptr;
	_pSwapChain = nullptr;
	_pRenderTargetView = nullptr;
	_pVertexShader = nullptr;
	_pPixelShader = nullptr;
	_pVertexLayout = nullptr;
	_pConstantBuffer = nullptr;
	m_timer = nullptr;
	CCWcullMode=nullptr;
	CWcullMode= nullptr;
	DSLessEqual = nullptr;
	RSCullNone = nullptr;
	 _WindowHeight = 0;
	 _WindowWidth = 0;
}

Application::~Application()
{
	Cleanup();
}

HRESULT Application::Initialise(HINSTANCE hInstance, int nCmdShow)
{

	HRESULT hr = 0;

    if (FAILED(InitWindow(hInstance, nCmdShow)))
	{
        return E_FAIL;
	}

    RECT rc;
	if (!GetClientRect(_hWnd, &rc)) { return E_FAIL; }
    _WindowWidth = rc.right - rc.left;
    _WindowHeight = rc.bottom - rc.top;

    if (FAILED(InitDevice()))
    {
        Cleanup();

        return E_FAIL;
    }

	//initialise timer
	m_timer = new Timer();
	
	//initialise Textures
	hr = CreateDDSTextureFromFile(_pd3dDevice, L"Assets\\Textures\\stone.dds", nullptr, &m_stoneTextureRV);
	hr = CreateDDSTextureFromFile(_pd3dDevice, L"Assets\\Textures\\floor.dds", nullptr, &m_groundTextureRV);
	
	if (FAILED(hr))
	{
		return E_FAIL;
	}

	InitWorld();

	return S_OK;
}

HRESULT Application::InitWorld()
{
	// Setup Camera
	XMFLOAT3 eye = XMFLOAT3(0.0f, 2.0f, -1.0f);
	XMFLOAT3 at = XMFLOAT3(0.0f, 2.0f, 0.0f);
	XMFLOAT3 up = XMFLOAT3(0.0f, 1.0f, 0.0f);

	m_camera = new OrbitingCamera(eye, at, up, (float)_renderWidth, (float)_renderHeight, 0.01f, 200.0f);

	// Setup the scene's light
	m_directionalLight.AmbientLight = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	m_directionalLight.DiffuseLight = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_directionalLight.SpecularLight = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	m_directionalLight.SpecularPower = 20.0f;
	m_directionalLight.LightVecW = XMFLOAT3(0.0f, 1.0f, -1.0f);

	Geometry donutGeometry;
	m_donutMeshData = OBJLoader::Load("Assets/3DModels/donut.obj", _pd3dDevice);
	donutGeometry.indexBuffer = m_donutMeshData.IndexBuffer;
	donutGeometry.numberOfIndices = m_donutMeshData.IndexCount;
	donutGeometry.vertexBuffer = m_donutMeshData.VertexBuffer;
	donutGeometry.vertexBufferOffset = m_donutMeshData.VBOffset;
	donutGeometry.vertexBufferStride = m_donutMeshData.VBStride;

	Geometry cubeGeometry;
	cubeGeometry.indexBuffer = m_cubeMeshData.IndexBuffer;
	cubeGeometry.numberOfIndices = m_cubeMeshData.IndexCount = 36;
	cubeGeometry.vertexBuffer = m_cubeMeshData.VertexBuffer;
	cubeGeometry.vertexBufferOffset = m_cubeMeshData.VBOffset = 0;
	cubeGeometry.vertexBufferStride = m_cubeMeshData.VBStride = sizeof(SimpleVertex);

	Geometry planeGeometry;
	planeGeometry.indexBuffer = m_planeMeshData.IndexBuffer;
	planeGeometry.numberOfIndices = m_planeMeshData.IndexCount = 6;
	planeGeometry.vertexBuffer = m_planeMeshData.VertexBuffer;
	planeGeometry.vertexBufferOffset = m_planeMeshData.VBOffset = 0;
	planeGeometry.vertexBufferStride = m_planeMeshData.VBStride = sizeof(SimpleVertex);

	Material shinyMaterial;
	shinyMaterial.ambient = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	shinyMaterial.diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	shinyMaterial.specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	shinyMaterial.specularPower = 10.0f;

	Material noSpecMaterial;
	noSpecMaterial.ambient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	noSpecMaterial.diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	noSpecMaterial.specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	noSpecMaterial.specularPower = 0.0f;

	Transform* transform = new Transform();
	GameObject* gameObject = new GameObject("Floor", new Appearance(planeGeometry, noSpecMaterial), transform, new StaticModel(transform, 1.0f, 0.5f));
	gameObject->GetTransform()->SetPosition(0.0f, 0.0f, 0.0f);
	gameObject->GetTransform()->SetScale(15.0f, 15.0f, 15.0f);
	gameObject->GetTransform()->SetRotation(XMConvertToRadians(90.0f), 0.0f, 0.0f);
	gameObject->GetAppearance()->SetTextureRV(m_groundTextureRV);
	gameObject->GetPhysicsModel()->EnableGravity(false);
	gameObject->GetPhysicsModel()->SetCollider(new PlaneCollider(gameObject->GetTransform(), Vector3(0.0f, 1.0f, 0.0f), 0.0f));

	m_gameObjects.push_back(gameObject);

	for (auto i = 0; i < NUMBEROFCUBES; i++)
	{
		transform = new Transform();
		gameObject = new GameObject("Cube " + to_string(i), new Appearance(cubeGeometry, shinyMaterial), transform, new RigidBodyModel(transform, 1.0f, 0.0f));
		gameObject->GetTransform()->SetPosition(-3.0f + (i * 2.5f), 1.0f, 10.0f);
		gameObject->GetTransform()->SetScale(1.0f, 1.0f, 1.0f);
		gameObject->GetAppearance()->SetTextureRV(m_stoneTextureRV);
		gameObject->GetPhysicsModel()->EnableGravity(true);
		gameObject->GetPhysicsModel()->SetCollider(new AABBCollider(gameObject->GetTransform(), 1.0f, 1.0f, 1.0f));
		//gameObject->GetPhysicsModel()->SetCollider(new SphereCollider(gameObject->GetTransform(), 1.0f));

		m_gameObjects.push_back(gameObject);
	}

	transform = new Transform();
	gameObject = new GameObject("Donut", new Appearance(donutGeometry, shinyMaterial), transform, new RigidBodyModel(transform, 1.0f, 0.5f));
	gameObject->GetTransform()->SetPosition(-6.0f, 0.5f, 10.0f);
	gameObject->GetTransform()->SetScale(0.5f, 0.5f, 0.5f);
	gameObject->GetAppearance()->SetTextureRV(m_stoneTextureRV);
	gameObject->GetPhysicsModel()->EnableGravity(false);

	m_gameObjects.push_back(gameObject);

	return S_OK;
}

#pragma region D3D11

HRESULT Application::InitShadersAndInputLayout()
{
	HRESULT hr;

    // Compile the vertex shader
    ID3DBlob* pVSBlob = nullptr;
    hr = CompileShaderFromFile(L"DX11 Framework.hlsl", "VS", "vs_4_0", &pVSBlob);

    if (FAILED(hr))
    {
        MessageBox(nullptr,
                   L"The HLSL file cannot be compiled.  Error output to Visual Studio Console.", L"Error", MB_OK);
        return hr;
    }

	// Create the vertex shader
	hr = _pd3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &_pVertexShader);

	if (FAILED(hr))
	{	
		pVSBlob->Release();
        return hr;
	}

	// Compile the pixel shader
	ID3DBlob* pPSBlob = nullptr;
    hr = CompileShaderFromFile(L"DX11 Framework.hlsl", "PS", "ps_4_0", &pPSBlob);

    if (FAILED(hr))
    {
        MessageBox(nullptr,
                   L"The HLSL file cannot be compiled.  Error output to Visual Studio Console.", L"Error", MB_OK);
        return hr;
    }

	// Create the pixel shader
	hr = _pd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &_pPixelShader);
	pPSBlob->Release();

    if (FAILED(hr))
        return hr;
	
    // Define the input layout
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	UINT numElements = ARRAYSIZE(layout);

    // Create the input layout
	hr = _pd3dDevice->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(),
                                        pVSBlob->GetBufferSize(), &_pVertexLayout);
	pVSBlob->Release();

	if (FAILED(hr))
        return hr;

    // Set the input layout
    _pImmediateContext->IASetInputLayout(_pVertexLayout);

	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	hr = _pd3dDevice->CreateSamplerState(&sampDesc, &_pSamplerLinear);

	return hr;
}

HRESULT Application::InitVertexBuffer()
{
	HRESULT hr;

    // Create vertex buffer
    SimpleVertex vertices[] =
    {
		{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) },

		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },

		{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },

		{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 0.0f) },

		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) },

		{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 0.0f) },
    };

    D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(SimpleVertex) * 24;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
    InitData.pSysMem = vertices;

    hr = _pd3dDevice->CreateBuffer(&bd, &InitData, &m_cubeMeshData.VertexBuffer);

    if (FAILED(hr))
        return hr;

	// Create vertex buffer
	SimpleVertex planeVertices[] =
	{
		{ XMFLOAT3(-1.0f, -1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(0.0f, 5.0f) },
		{ XMFLOAT3(1.0f, -1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(5.0f, 5.0f) },
		{ XMFLOAT3(1.0f, 1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(5.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) },
	};

	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SimpleVertex) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = planeVertices;

	hr = _pd3dDevice->CreateBuffer(&bd, &InitData, &m_planeMeshData.VertexBuffer);

	if (FAILED(hr))
		return hr;

	return S_OK;
}

HRESULT Application::InitIndexBuffer()
{
	HRESULT hr;

    // Create index buffer
    WORD indices[] =
    {
		3, 1, 0,
		2, 1, 3,

		6, 4, 5,
		7, 4, 6,

		11, 9, 8,
		10, 9, 11,

		14, 12, 13,
		15, 12, 14,

		19, 17, 16,
		18, 17, 19,

		22, 20, 21,
		23, 20, 22
    };

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(WORD) * 36;     
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
    InitData.pSysMem = indices;
    hr = _pd3dDevice->CreateBuffer(&bd, &InitData, &m_cubeMeshData.IndexBuffer);

    if (FAILED(hr))
        return hr;

	// Create plane index buffer
	WORD planeIndices[] =
	{
		0, 3, 1,
		3, 2, 1,
	};

	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(WORD) * 6;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;

	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = planeIndices;
	hr = _pd3dDevice->CreateBuffer(&bd, &InitData, &m_planeMeshData.IndexBuffer);

	if (FAILED(hr))
		return hr;

	return S_OK;
}

HRESULT Application::InitWindow(HINSTANCE hInstance, int nCmdShow)
{
    // Register class
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, (LPCTSTR)IDI_TUTORIAL1);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW );
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = L"TutorialWindowClass";
    wcex.hIconSm = LoadIcon(wcex.hInstance, (LPCTSTR)IDI_TUTORIAL1);
    if (!RegisterClassEx(&wcex))
        return E_FAIL;

    // Create window
    _hInst = hInstance;
    RECT rc = {0, 0, 960, 540};
    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
    _hWnd = CreateWindow(L"TutorialWindowClass", L"FGGC Semester 2 Framework", WS_OVERLAPPEDWINDOW,
                         CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, hInstance,
                         nullptr);
    if (!_hWnd)
		return E_FAIL;

    ShowWindow(_hWnd, nCmdShow);

    return S_OK;
}

HRESULT Application::CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
{
    HRESULT hr = S_OK;

    DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
    // Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
    // Setting this flag improves the shader debugging experience, but still allows 
    // the shaders to be optimized and to run exactly the way they will run in 
    // the release configuration of this program.
    dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

    ID3DBlob* pErrorBlob;
    hr = D3DCompileFromFile(szFileName, nullptr, nullptr, szEntryPoint, szShaderModel, 
        dwShaderFlags, 0, ppBlobOut, &pErrorBlob);

    if (FAILED(hr))
    {
        if (pErrorBlob != nullptr)
            OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());

        if (pErrorBlob) pErrorBlob->Release();

        return hr;
    }

    if (pErrorBlob) pErrorBlob->Release();

    return S_OK;
}

HRESULT Application::InitDevice()
{
    HRESULT hr = S_OK;

    UINT createDeviceFlags = 0;

#ifdef _DEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_DRIVER_TYPE driverTypes[] =
    {
        D3D_DRIVER_TYPE_HARDWARE,
        D3D_DRIVER_TYPE_WARP,
        D3D_DRIVER_TYPE_REFERENCE,
    };

    UINT numDriverTypes = ARRAYSIZE(driverTypes);

    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
    };

	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	UINT sampleCount = 4;

    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 1;
    sd.BufferDesc.Width = _renderWidth;
    sd.BufferDesc.Height = _renderHeight;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = _hWnd;
	sd.SampleDesc.Count = sampleCount;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;

    for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
    {
        _driverType = driverTypes[driverTypeIndex];
        hr = D3D11CreateDeviceAndSwapChain(nullptr, _driverType, nullptr, createDeviceFlags, featureLevels, numFeatureLevels,
                                           D3D11_SDK_VERSION, &sd, &_pSwapChain, &_pd3dDevice, &_featureLevel, &_pImmediateContext);
        if (SUCCEEDED(hr))
            break;
    }

    if (FAILED(hr))
        return hr;

    // Create a render target view
    ID3D11Texture2D* pBackBuffer = nullptr;
    hr = _pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

    if (FAILED(hr))
        return hr;

    hr = _pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &_pRenderTargetView);
    pBackBuffer->Release();

    if (FAILED(hr))
        return hr;

    // Setup the viewport
    D3D11_VIEWPORT vp;
    vp.Width = (FLOAT)_renderWidth;
    vp.Height = (FLOAT)_renderHeight;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    _pImmediateContext->RSSetViewports(1, &vp);

	InitShadersAndInputLayout();

	InitVertexBuffer();
	InitIndexBuffer();

    // Set primitive topology
    _pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Create the constant buffer
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstantBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
    hr = _pd3dDevice->CreateBuffer(&bd, nullptr, &_pConstantBuffer);

    if (FAILED(hr))
        return hr;

	D3D11_TEXTURE2D_DESC depthStencilDesc;

	depthStencilDesc.Width = _renderWidth;
	depthStencilDesc.Height = _renderHeight;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = sampleCount;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	_pd3dDevice->CreateTexture2D(&depthStencilDesc, nullptr, &_depthStencilBuffer);
	_pd3dDevice->CreateDepthStencilView(_depthStencilBuffer, nullptr, &_depthStencilView);

	_pImmediateContext->OMSetRenderTargets(1, &_pRenderTargetView, _depthStencilView);

	// Rasterizer
	D3D11_RASTERIZER_DESC cmdesc;

	ZeroMemory(&cmdesc, sizeof(D3D11_RASTERIZER_DESC));
	cmdesc.FillMode = D3D11_FILL_SOLID;
	cmdesc.CullMode = D3D11_CULL_NONE;
	hr = _pd3dDevice->CreateRasterizerState(&cmdesc, &RSCullNone);

	D3D11_DEPTH_STENCIL_DESC dssDesc;
	ZeroMemory(&dssDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	dssDesc.DepthEnable = true;
	dssDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dssDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	_pd3dDevice->CreateDepthStencilState(&dssDesc, &DSLessEqual);

	ZeroMemory(&cmdesc, sizeof(D3D11_RASTERIZER_DESC));

	cmdesc.FillMode = D3D11_FILL_SOLID;
	cmdesc.CullMode = D3D11_CULL_BACK;

	cmdesc.FrontCounterClockwise = true;
	hr = _pd3dDevice->CreateRasterizerState(&cmdesc, &CCWcullMode);

	cmdesc.FrontCounterClockwise = false;
	hr = _pd3dDevice->CreateRasterizerState(&cmdesc, &CWcullMode);

    return S_OK;
}

void Application::Cleanup()
{
    if (_pImmediateContext) _pImmediateContext->ClearState();
	if (_pSamplerLinear) _pSamplerLinear->Release();

	if (m_stoneTextureRV) m_stoneTextureRV->Release();

	if (m_groundTextureRV) m_groundTextureRV->Release();

    if (_pConstantBuffer) _pConstantBuffer->Release();

    if (_pVertexLayout) _pVertexLayout->Release();
    if (_pVertexShader) _pVertexShader->Release();
    if (_pPixelShader) _pPixelShader->Release();
    if (_pRenderTargetView) _pRenderTargetView->Release();
    if (_pSwapChain) _pSwapChain->Release();
    if (_pImmediateContext) _pImmediateContext->Release();
    if (_pd3dDevice) _pd3dDevice->Release();
	if (_depthStencilView) _depthStencilView->Release();
	if (_depthStencilBuffer) _depthStencilBuffer->Release();

	if (DSLessEqual) DSLessEqual->Release();
	if (RSCullNone) RSCullNone->Release();

	if (CCWcullMode) CCWcullMode->Release();
	if (CWcullMode) CWcullMode->Release();

	if (m_camera)
	{
		delete m_camera;
	}

	for (auto gameObject : m_gameObjects)
	{
		if (gameObject)
		{
			delete gameObject;
		}
	}
}

#pragma endregion


#pragma region MovingCubes

void Application::moveForward(int objectNumber)
{
	m_gameObjects[objectNumber]->GetPhysicsModel()->AddForce(0.0f, 0.0f, -1.0f);
}

void Application::moveBackward(int objectNumber)
{
	m_gameObjects[objectNumber]->GetPhysicsModel()->AddForce(0.0f, 0.0f, 1.0f);
}

void Application::moveUp(int objectNumber)
{
	m_gameObjects[objectNumber]->GetPhysicsModel()->AddForce(0.0f, 1.0f, 0.0f);
}

void Application::moveDown(int objectNumber)
{
	m_gameObjects[objectNumber]->GetPhysicsModel()->AddForce(0.0f, -1.0f, 0.0f);
}

void Application::moveLeft(int objectNumber)
{
	m_gameObjects[objectNumber]->GetPhysicsModel()->AddForce(1.0f, 0.0f, 0.0f);
}

void Application::moveRight(int objectNumber)
{
	m_gameObjects[objectNumber]->GetPhysicsModel()->AddForce(-1.0f, 0.0f, 0.0f);
}

#pragma endregion

void Application::Update()
{
    // Update our time
	static float accumulator = 0.0f;
	accumulator += m_timer->GetDeltaTime();

	while (accumulator >= FPS60)
	{
		HandleInput();
		UpdateWorld(FPS60);
		HandleCollisions(FPS60);
		//DebugPrintF("Accumulator: %f\n", accumulator);
		m_timer->Tick();
		accumulator -= FPS60;
	}

}

void Application::UpdateWorld(float dt)
{
	// Update camera
	float angleAroundZ = XMConvertToRadians(m_camera->GetAngleXZ());

	float x = m_camera->GetRadius() * cos(angleAroundZ);
	float z = m_camera->GetRadius() * sin(angleAroundZ);

	XMFLOAT3 cameraPos = m_camera->GetPosition();
	cameraPos.x = x;
	cameraPos.z = z;

	m_camera->SetPosition(cameraPos);
	m_camera->Update(dt);

	// Update objects
	for (auto gameObject : m_gameObjects)
	{
		gameObject->Update(dt);
	}
}

void Application::HandleCollisions(float dt)
{
	//For each collidable object...
	for (GameObject* gameObject : m_gameObjects)
	{
		if (!gameObject->GetPhysicsModel()->IsCollidable())
		{
			break;
		}

		//Cache the object's aspects that'll be used in the collision response
		float inverseMass = 1 / gameObject->GetPhysicsModel()->GetMass();
		float restitution = -(1+gameObject->GetPhysicsModel()->GetRestitution());
		Collider* collider = gameObject->GetPhysicsModel()->GetCollider();
		Vector3 velocity = gameObject->GetPhysicsModel()->GetVelocity();

		//for each other collidable object...
		for (GameObject* other : m_gameObjects)	
		{
			if (gameObject == other)
			{
				break;
			}
			if (!other->GetPhysicsModel()->IsCollidable())
			{
				break;
			}

			//If there is a collision between the two,
			Collider* colliderOther = other->GetPhysicsModel()->GetCollider();
			Collision collision = collider->CollidesWith(*colliderOther);
			if (!collision.collided)
			{
				break;
			}
			//Cache the second object's aspects that'll be used in the collision response
			float inverseMassOther = 1 / other->GetPhysicsModel()->GetMass();
			float restitutionOther = -(1+other->GetPhysicsModel()->GetRestitution());
			Vector3 velocityOther = other->GetPhysicsModel()->GetVelocity();
			Vector3 relativeVelocity = velocity - velocityOther;

			//For each point of contact in the collision...
			for (Contact* contact : collision.contacts)
			{
				//Check the objects are approaching eachother by dotting the relative velocity onto the normal
				if (contact->normal * relativeVelocity > 0.0f)
				{
					break;
				}

				//total velocity = -(1 + restitution) * Dot(relative velocity, collision normal)
				float totalVelocity = (relativeVelocity * contact->normal);
				//momentum = total velocity * (inverse mass + inverse mass)
				float momentum = totalVelocity * (inverseMass + inverseMassOther);
				//impulse = momentum in direction normal to collision
				Vector3 impulse = (-(1 + restitution) * momentum) * contact->normal;
				Vector3 impulseOther = (-(1 + restitutionOther) * momentum) * contact->normal;

				//split the impulse evenly across the collision surface by dividing by the number of collision points
				impulse /= collision.contacts.size();
				impulseOther /= collision.contacts.size();

				//multiply the impulse by the inverse mass for each object, be sure to put them in opposite directions
				gameObject->GetPhysicsModel()->ApplyImpulse(impulse * inverseMass);
				other->GetPhysicsModel()->ApplyImpulse(-impulseOther * inverseMassOther);
			}
			collision.contacts.clear();
		}
	}
}

void Application::HandleInput()
{
	// Move gameobject
	if (GetAsyncKeyState('1'))
	{
		moveForward(1);
	}
	if (GetAsyncKeyState('2'))
	{
		moveForward(2);
	}
	if (GetAsyncKeyState('3'))
	{
		moveBackward(1);
	}
	if (GetAsyncKeyState('4'))
	{
		moveBackward(2);
	}
	if (GetAsyncKeyState('5'))
	{
		moveUp(1);
	}
	if (GetAsyncKeyState('6'))
	{
		moveUp(2);
	}
	if (GetAsyncKeyState('7'))
	{
		moveDown(1);
	}
	if (GetAsyncKeyState('8'))
	{
		moveDown(2);
	}
	if (GetAsyncKeyState('9'))
	{
		moveLeft(1);
	}
	if (GetAsyncKeyState('0'))
	{
		moveLeft(2);
	}
	if (GetAsyncKeyState('O'))
	{
		moveRight(1);
	}
	if (GetAsyncKeyState('P'))
	{
		moveRight(2);
	}
}

void Application::Draw()
{
    //
    // Clear buffers
    //

	float ClearColor[4] = { 0.5f, 0.5f, 0.5f, 1.0f }; // red,green,blue,alpha
    _pImmediateContext->ClearRenderTargetView(_pRenderTargetView, ClearColor);
	_pImmediateContext->ClearDepthStencilView(_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    //
    // Setup buffers and render scene
    //
	_pImmediateContext->IASetInputLayout(_pVertexLayout);

	_pImmediateContext->VSSetShader(_pVertexShader, nullptr, 0);
	_pImmediateContext->PSSetShader(_pPixelShader, nullptr, 0);

	_pImmediateContext->VSSetConstantBuffers(0, 1, &_pConstantBuffer);
	_pImmediateContext->PSSetConstantBuffers(0, 1, &_pConstantBuffer);
	_pImmediateContext->PSSetSamplers(0, 1, &_pSamplerLinear);

    ConstantBuffer cb;

	XMFLOAT4X4 viewAsFloats = m_camera->GetView();
	XMFLOAT4X4 projectionAsFloats = m_camera->GetProjection();

	XMMATRIX view = XMLoadFloat4x4(&viewAsFloats);
	XMMATRIX projection = XMLoadFloat4x4(&projectionAsFloats);

	cb.View = XMMatrixTranspose(view);
	cb.Projection = XMMatrixTranspose(projection);
	
	cb.light = m_directionalLight;
	cb.EyePosW = m_camera->GetPosition();

	// Render all scene objects
	for (auto gameObject : m_gameObjects)
	{
		// Get render material
		Material material = gameObject->GetAppearance()->GetMaterial();

		// Copy material to shader
		cb.surface.AmbientMtrl = material.ambient;
		cb.surface.DiffuseMtrl = material.diffuse;
		cb.surface.SpecularMtrl = material.specular;

		// Set world matrix
		cb.World = XMMatrixTranspose(gameObject->GetTransform()->GetWorldMatrix());

		// Set texture
		if (gameObject->GetAppearance()->HasTexture())
		{
			ID3D11ShaderResourceView * textureRV = gameObject->GetAppearance()->GetTextureRV();
			_pImmediateContext->PSSetShaderResources(0, 1, &textureRV);
			cb.HasTexture = 1.0f;
		}
		else
		{
			cb.HasTexture = 0.0f;
		}

		// Update constant buffer
		_pImmediateContext->UpdateSubresource(_pConstantBuffer, 0, nullptr, &cb, 0, 0);

		// Draw object
		gameObject->Draw(_pImmediateContext);
	}

    //
    // Present our back buffer to our front buffer
    //
    _pSwapChain->Present(0, 0);
}