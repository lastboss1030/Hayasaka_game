//=============================================================================
//
// DirectX01 [main.cpp]
// Author:Taiki Hayasaka
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS

//インクルードファイル
#include "main.h"	
#include "polygon.h"
#include "camera.h"
#include "light.h"
#include "input.h"
#include "player.h"
#include "shadow.h"
#include "wall.h"
#include "meshwall.h"
#include "Billboard.h"
#include "meshfield.h"
#include "bullet.h"
#include "explosion.h"
#include "effect.h"
#include "game.h"
#include "fade.h"
#include "title.h"
#include "tutorial.h"
#include "result.h"
#include "select.h"
#include "ranking.h"
#include "Sound.h"

#include <stdio.h>

//=============================================================================
// マクロ定義
//=============================================================================
#define CLASS_NAME "WindowClass"				//ウィンドウクラスの名前
#define WINDOW_NAME "Robots Battle"					//ウィンドウの名前
#define WINDOW_POS_X (100)						//ウィンドウの左上X
#define WINDOW_POS_Y (100)						//ウィンドウの左上Y
#define ID_TIMER (121)							//タイマーのID
#define TIMER_INTERVAL (1000/60)				//タイマーの発生間隔

//=============================================================================
// プロトタイプ宣言
//=============================================================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);
void DrawFPS(void);
void DrawPoint(void);

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3D9 g_pD3D = NULL;					//Direct3Dオブジェクトへのポインタ
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;		//Direct3Dデバイスへのポインタ
LPD3DXFONT g_pFont = NULL;					//フォントへのポインタ
int g_nCountFPS;							//FPSカウンタ
MODE g_mode = MODE_TITLE;					//モードの種類

//=============================================================================
// メイン関数
//=============================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WindowProc,
		0,
		0,
		hInstance,
		LoadIcon(NULL,IDI_APPLICATION),
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		CLASS_NAME,	//ウィンドウクラスの名前
		LoadIcon(NULL, IDI_APPLICATION)
	};
	HWND hWnd;	//ウィンドウハンドル(識別子)
	MSG msg;	//メッセージを格納する

	DWORD dwExecLastTime;	//処理実行最終時刻
	DWORD dwFPSLastTime;	//FPS計測最終時刻
	DWORD dwCurrentTime;	//現在時刻
	DWORD dwFrameCount;		//フレームカウント

	dwExecLastTime =
		dwFPSLastTime =
		dwCurrentTime = timeGetTime();	//現在時刻を設定
	dwFrameCount = 0;

	// ウィンドウクラスの登録
	RegisterClassEx(&wcex);

	//ウィンドウの大きさ調整
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };

	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

	//ウィンドウの作成
	hWnd = CreateWindowEx(0,
		CLASS_NAME,
		WINDOW_NAME,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,		//X座標
		CW_USEDEFAULT,		//Y座標
		rect.right,			//幅
		rect.bottom,		//高さ
		NULL,
		NULL,
		hInstance,
		NULL);

	//初期化処理
	if (FAILED(Init(hInstance, hWnd, TRUE)))
	{
		return -1;
	}
	//ウィンドウの表示
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	//メッセージループ
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{ //Windowsの処理
			if (msg.message == WM_QUIT)
			{
				break;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{ //DirectXの処理
			dwCurrentTime = timeGetTime();
			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{	//0.5秒
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);
				dwFPSLastTime = dwCurrentTime;
				dwFrameCount = 0;
			}

			if ((dwCurrentTime - dwExecLastTime) > (1000 / 60))
			{
				//更新処理
				Update();

				//描画処理
				Draw();

				dwFrameCount++;
			}
		}
	}
	//終了処理
	Uninit();

	//ウィンドウクラスの登録解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return(int)msg.wParam;
}

//=============================================================================
// ウィンドウプロシージャ
//=============================================================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

		//キーを押した場合
	case  WM_KEYDOWN:
		//ESCを押された場合
		switch (wParam)
		{
		case VK_ESCAPE:
			DestroyWindow(hWnd);
		}
		break;

		//×ボタンを押した場合
	case WM_CLOSE:
		DestroyWindow(hWnd);

	default:
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	//変数宣言
	D3DDISPLAYMODE d3ddm;
	D3DPRESENT_PARAMETERS d3dpp;

	//Direct3Dオブジェクトの生成
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}

	//ディスプレイモードを取得
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	//デバイスのプレゼントパラメーターの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferWidth = SCREEN_WIDTH;
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;
	d3dpp.BackBufferFormat = d3ddm.Format;
	d3dpp.BackBufferCount = 1;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp.Windowed = true;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

	//Direct3Dデバイスの生成
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&g_pD3DDevice)))
	{
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&g_pD3DDevice)))
		{
			if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&g_pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}

	//レンダーステートの設定
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				//カリングの設定
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				//αブレンド(α値の合成)の設定
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		//ソース(描画元)の合成方法の設定
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	//デスティネーション(描画先)の合成方法の設定

																			//サンプラーステートの設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	//縮小時　補間
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	//拡大時　補間
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	//U値　繰り返し
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	//V値　繰り返し

																			//テクスチャステージステートの設定
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);


	//フォントオブジェクトの生成
	D3DXCreateFont(g_pD3DDevice, 30, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &g_pFont);

	//キーボードの初期化処理
	InitKeyboard(hInstance, hWnd);

	//サウンドの初期化処理
	InitSound(hWnd);

	//モードの設定
	SetMode(g_mode);

	//フェードの初期化処理
	InitFade(g_mode);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void Uninit(void)
{
	//キーボードの終了処理
	UninitKeyboard();

	//ゲーム画面の終了処理
	UninitGame();

	//フェードの終了処理
	UninitFade();

	//Direct3Dデバイスの開放
	if (g_pD3DDevice != NULL)
	{
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	//Direct3Dオブジェクトの開放
	if (g_pD3D != NULL)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}

	//フォントオブジェクトの開放
	if (g_pFont != NULL)
	{
		g_pFont->Release();
		g_pFont = NULL;
	}

	//サウンドの終了処理
	UninitSound();
}

//=============================================================================
// 更新処理
//=============================================================================
void Update(void)
{
	//キーボードの更新処理
	UpdateKeyboard();

	// フェードの更新処理
	UpdateFade();

	//更新処理
	switch (g_mode)
	{
	case MODE_TITLE:		//タイトル画面
		UpdateTitle();
		break;

	case MODE_TUTORIAL:		//チュートリアル画面
		UpdateTutorial();
		break;

	case MODE_SELECT:		//選択画面
		UpdateSelect();
		break;

	case MODE_GAME:			//ゲーム画面
		UpdateGame();
		break;

	case MODE_RESULT:		//リザルト画面
		UpdateResult();
		break;

	case MODE_RANKING:		//ランキング画面
		UpdateRanking();
		break;
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void Draw(void)
{
	//バックバッファ＆Zバッファのクリア（画面のクリア）
	g_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);	//ウィンドウの色

	//描画の開始
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{
//		D3DVIEWPORT9 viewportDef;

		//現在のビューポートの取得
//		g_pD3DDevice->GetViewport(&viewportDef);

		//カメラの設定処理
		SetCamera();

		switch (g_mode)
		{
		case MODE_TITLE:		//タイトル画面
			DrawTitle();
			break;

		case MODE_TUTORIAL:		//チュートリアル画面
			DrawTutorial();
			break;

		case MODE_SELECT:		//選択画面
			DrawSelect();
			break;

		case MODE_GAME:			//ゲーム画面
			DrawGame();
			break;

		case MODE_RESULT:		//リザルト画面
			DrawResult();
			break;

		case MODE_RANKING:		//ランキング画面
			DrawRanking();
			break;
		}

		//ビューポートを元に戻す
//		g_pD3DDevice->SetViewport(&viewportDef);

#ifdef _DEBUG

		//FPSの描画処理
		DrawFPS();

		//座標
		DrawPoint();

#endif

		// フェードの描画処理
		DrawFade();

		//描画の終了
		g_pD3DDevice->EndScene();
	}

	//バックバッファとフロントバッファの入れ替え
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//=============================================================================
// FPSの描画
//=============================================================================
void DrawFPS(void)
{
	RECT rect = { 0,0,SCREEN_WIDTH -30, SCREEN_HEIGHT };
	char aStr[256];

	wsprintf(&aStr[0], "FPS:%d\n", g_nCountFPS);

	//テキストの描画
	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_RIGHT, D3DCOLOR_RGBA(0, 255, 255, 255));
}

//=============================================================================
// 座標の表示
//=============================================================================
void DrawPoint(void)
{
	Camera *pCamera;
	Player *pPlayer;
	pCamera = GetCamera();
	pPlayer = GetPlayer();

#if 0
	if (g_mode == MODE_GAME)
	{
		RECT rect = { 0,0,SCREEN_WIDTH, SCREEN_HEIGHT };
		char aStr[512];
		int nNum = sprintf(&aStr[0], "[カメラの視点  :(%.2f : %.2f : %.2f)]\n", pCamera->posV.x, pCamera->posV.y, pCamera->posV.z);
		nNum += sprintf(&aStr[nNum], "[カメラの注視点:(%.2f : %.2f : %.2f)]\n", pCamera->posR.x, pCamera->posR.y, pCamera->posR.z);
		nNum += sprintf(&aStr[nNum], "[カメラの向き  :(%.2f)]\n", pCamera->rot.y);

		nNum += sprintf(&aStr[nNum], "\n[モデルの位置  :(%.2f : %.2f : %.2f)]\n", pPlayer->pos.x, pPlayer->pos.y,pPlayer->pos.z);
		nNum += sprintf(&aStr[nNum], "[モデルの向き  :(%.2f)]\n", pPlayer->rot.y);
		nNum += sprintf(&aStr[nNum], "[ブースト残量  :(%.0f)]\n", pPlayer->boost);

		//	wsprintf(&aStr[0], "FPS:%d\n", g_nCountFPS);

			//テキストの描画
		g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
	}
#endif
}

//=============================================================================
// デバイスの取得
//=============================================================================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}

//=============================================================================
// モードの設定
//=============================================================================
void SetMode(MODE mode)
{
	//終了処理
	switch (g_mode)
	{
	case MODE_TITLE:		//タイトル画面
		UninitTitle();
		break;

	case MODE_TUTORIAL:		//チュートリアル画面
		UninitTutorial();
		break;

	case MODE_SELECT:		//選択画面
		UninitSelect();
		break;

	case MODE_GAME:			//ゲーム画面
		UninitGame();
		break;

	case MODE_RESULT:		//リザルト画面
		UninitResult();
		break;

	case MODE_RANKING:		//ランキング画面
		UninitResult();
		break;
	}
	g_mode = mode;


	//初期化処理
	switch (g_mode)		
	{
	case MODE_TITLE:	//タイトル画面
		InitTitle();
		break;

	case MODE_TUTORIAL:	//チュートリアル画面
		InitTutorial();
		break;

	case MODE_SELECT:		//選択画面
		InitSelect();
		break;

	case MODE_GAME:		//ゲーム画面
		InitGame();
		break;

	case MODE_RESULT:	//リザルト画面
		InitResult();
		break;

	case MODE_RANKING:	//ランキング画面
		InitRanking();
		break;
	}

	g_mode = mode;

}

//=============================================================================
// モードの取得
//=============================================================================
MODE *GetMode(void)
{
	return &g_mode;
}