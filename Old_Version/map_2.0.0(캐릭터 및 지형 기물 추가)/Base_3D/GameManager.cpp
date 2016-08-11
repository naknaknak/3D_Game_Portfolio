#include "stdafx.h"
#include "GameManager.h"

GameManager::GameManager()
{
}


GameManager::~GameManager()
{
}

void GameManager::Initialize(HWND handle)
{
	windowHandle = handle;

	direct3d = Direct3DCreate9(D3D_SDK_VERSION);

	D3DCAPS9 caps;	//�ϵ������ �ɷ�ġ
	int	vertexProcessing;
	direct3d->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);
	if ( caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT )
	{
		vertexProcessing = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	}
	else
	{
		vertexProcessing = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}

	D3DPRESENT_PARAMETERS d3dPP;		//ȭ�� ǥ�� �Ķ����
	ZeroMemory(&d3dPP, sizeof(D3DPRESENT_PARAMETERS));
	d3dPP.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dPP.Windowed = true;
	d3dPP.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dPP.EnableAutoDepthStencil = true;
	d3dPP.AutoDepthStencilFormat = D3DFMT_D16;

	HRESULT result = direct3d->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		windowHandle,
		vertexProcessing,
		&d3dPP,
		&direct3dDevice);

	/*
	�������� ����,D3D_OK �� �����ش�.
	�������� ����, ������ ��� ���� �����ش�.

	D3DERR_INVALIDCALL �޼����� ȣ���� ��ȿ�̴�. ���� ���, �޼����� �Ķ���Ϳ� ��ȿ�� ���� �����Ǿ� �ִ� ��� ���̴�.
	D3DERR_NOTAVAILABLE �� ��ġ��, ������ ��ũ���� ���� �ϰ� ���� �ʴ�.
	D3DERR_OUTOFVIDEOMEMORY Direct3D �� ó���� �ǽ��ϴµ� ����� ���÷��� �޸𸮰� ����.

	*/
	if ( result != D3D_OK )
	{
		MessageBoxA(windowHandle, "CreateDevice Fail!!", "D3D Critical Error", MB_OK);
		return;
	}
		
	//FPS �ʱ�ȭ
	fpsCount = 0;
	fpsTimeCheck = 0.0f;
	strcpy_s(fpsText, "FPS : 0");

	//���� �ʱ�ȭ �� ����
	ZeroMemory(&mainLight, sizeof(D3DLIGHT9));
	mainLight.Type = D3DLIGHTTYPE::D3DLIGHT_DIRECTIONAL;
	mainLight.Ambient = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);
	mainLight.Diffuse = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);
	mainLight.Specular = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);
	D3DXVECTOR3 direction(1.0f, -2.0f, 1.0f);
	D3DXVec3Normalize(&direction, &direction);
	mainLight.Direction = direction;

	direct3dDevice->SetLight(0, &mainLight);
	direct3dDevice->LightEnable(0, true);

	camera = new Camera();
	camera->Initialize();

	//���� �ʱ�ȭ
	RandomUtil::Initialize();
	
	UI_Manager::Get().Initialize();

	//GameState�� �ʱ�ȭ
	GameStateManager::Get().Initialize();
	
}

void GameManager::Destroy()
{
	//GameState�� ����
	GameStateManager::Get().Destroy();	

	UI_Manager::Get().Destroy();

	SAFE_DELETE(camera);

	TextureManager::Get().Destroy();
	
	SkinnedMeshManager::Get().Destroy();
	//����̽� ������
	ULONG result = 0;
	result = SAFE_RELEASE(direct3dDevice);
#ifdef _DEBUG
	assert(result == 0 && "����̽��� �̿��� ������ ��ü �� �������� ���� ��ü�� �ֽ��ϴ�.");
#endif

	SAFE_RELEASE(direct3d);
}

void GameManager::Loop(double tick)
{
	currentTickTime = tick;

	Update();
	Render();
}

void GameManager::Update()
{
	//FPS ������Ʈ
	fpsCount++;
	fpsTimeCheck += (float)currentTickTime;
	if (fpsTimeCheck > 1.0f)
	{
		sprintf_s(fpsText, "FPS : %d", fpsCount);
		fpsCount = 0;
		fpsTimeCheck = 0.0f;
	}

	//GameState ������Ʈ
	GameStateManager::Get().Update();

	if (camera)
	{
		camera->Update();
	}

	UI_Manager::Get().Update();

}

void GameManager::Render()
{
	direct3dDevice->Clear(
		0, nullptr,	//ȭ����ü Ŭ����
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB(0, 0, 50),
		1.0f,	//z�� 0~1. 1�̸� ���� �ִ�
		0);		//���ٽ� ���ۿ� ����

	//direct3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);	//���̾� ������ ���� �׸���

	direct3dDevice->BeginScene();

	// �׸� �׸��� -------------------------------------------------

	//GameState �׸���
	GameStateManager::Get().Render();

	UI_Manager::Get().Render();

	//FPS �׸���
	RECT rc = { 10, 10, 11, 11 };
	UI_Manager::GetFont()->DrawTextA(NULL, fpsText, strlen(fpsText), &rc, DT_TOP | DT_LEFT | DT_NOCLIP, D3DCOLOR_XRGB(255, 255, 255));

	if ( camera )
	{
		camera->Render();
	}
	//--------------------------------------------------------------

	direct3dDevice->EndScene();

	direct3dDevice->Present(NULL, NULL, NULL, NULL);
}

LRESULT GameManager::GameInputProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	//�����̵� �� �޼����� ���� ó���� �־����� �� ������ ����
	LRESULT result = 0;
	if (result == 0 && camera)
	{
		camera->CameraInputProc(hWnd, message, wParam, lParam);
	}
	if (result == 0)
	{
		result = UI_Manager::Get().InputProc(hWnd, message, wParam, lParam);	//UI�� �ֿ켱 ó��
	}
	if ( result == 0 )
	{
		result = GameStateManager::Get().InputProc(hWnd, message, wParam, lParam);
	}
	
	return result;
}