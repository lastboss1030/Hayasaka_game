//=============================================================================
//
// カメラの処理 [camera.cpp]
// Author:Taiki Hayasaka
//
//=============================================================================
#include "camera.h"
#include "input.h"
#include "player.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MOVE_CAMERA (4.0f)			//移動量
#define CAMERA_DISTANCE (600.0f)	//距離

//=============================================================================
// グローバル変数
//=============================================================================
Camera g_camera;	//カメラの情報

//=============================================================================
// 初期化処理
//=============================================================================
void InitCamera(void)
{
	//初期化
	g_camera.posV = D3DXVECTOR3(0.0f, 300.0f, -CAMERA_DISTANCE);
	g_camera.posVDest = g_camera.posV;
	g_camera.posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_camera.posRDest = g_camera.posR;
	g_camera.rot  = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_camera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	g_camera.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	g_camera.viewport.X = 0;
	g_camera.viewport.Y = 0;
	g_camera.viewport.Width = 1080;
	g_camera.viewport.Height = 1920;
	g_camera.viewport.MinZ = 0.0f;
	g_camera.viewport.MaxZ = 1.0f;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitCamera(void)
{

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateCamera(void)
{
	Player *pPlayer = GetPlayer();

	if (GetKeyboardPress(DIK_Z) == true)	//Zを押した
	{
		g_camera.rot.y -= 0.03f;

		if (g_camera.rot.y < -D3DX_PI)
		{
			g_camera.rot.y += D3DX_PI * 2.0f;
		}
	}
	if (GetKeyboardPress(DIK_C) == true)	//Cを押した
	{
		g_camera.rot.y += 0.03f;

		if (g_camera.rot.y > D3DX_PI)
		{
			g_camera.rot.y -= D3DX_PI * 2.0f;
		}
	}

	//追従
	FollowCamera();
}

//=============================================================================
// 設定処理
//=============================================================================
void SetCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&g_camera.mtxProjection);

	//ビューポートの設定
//	pDevice->SetViewport(&g_camera.viewport);

	//プロジェクションマトリックスを作成
	D3DXMatrixPerspectiveFovLH(&g_camera.mtxProjection,
		D3DXToRadian(45.0f),								//画角
		(float) SCREEN_WIDTH / (float) SCREEN_HEIGHT,
		10.0f,												//手前の描画
		2000.0f);											//奥の描画

	//プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &g_camera.mtxProjection);

	//ビューマトリックスの初期化
	D3DXMatrixIdentity(&g_camera.mtxView);

	//ビューマトリックスの作成
	D3DXMatrixLookAtLH(&g_camera.mtxView,
		&g_camera.posV,
		&g_camera.posR,
		&g_camera.vecU);

	//ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &g_camera.mtxView);
}

//=============================================================================
// モデルの追従
//=============================================================================
void FollowCamera(void)
{
	Player *pPlayer = GetPlayer();

	g_camera.posR = pPlayer->pos;

	// 注視点の更新
	g_camera.posR.x = pPlayer->pos.x;	// 注視点のX軸座標の決定
	g_camera.posR.z = pPlayer->pos.z;	// 注視点のZ軸座標の決定
	g_camera.posR.y = pPlayer->pos.y;	// 注視点のY軸座標の決定

	// 視点の更新
	g_camera.posV.x = g_camera.posR.x - sinf(g_camera.rot.y) * 400;		//視点のX軸座標の決定
	g_camera.posV.z = g_camera.posR.z - cosf(g_camera.rot.y) * 400;		//視点のZ軸座標の決定
	g_camera.posV.y = g_camera.posR.y + 200.0f;							//視点のY軸座標の決定
}

//=============================================================================
// デバイスの取得
//=============================================================================
Camera *GetCamera(void)
{
	return &g_camera;
}