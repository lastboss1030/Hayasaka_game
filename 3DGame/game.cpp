//=============================================================================
//
// ゲームの処理 [game.cpp]
// Author:Taiki Hayasaka
//
//=============================================================================
#include "game.h"
#include "Billboard.h"
#include "bullet.h"
#include "camera.h"
#include "effect.h"
#include "explosion.h"
#include "light.h"
#include "meshfield.h"
#include "meshwall.h"
#include "player.h"
#include "shadow.h"
#include "fade.h"
#include "input.h"
#include "pause.h"
#include "object.h"
#include "cooldown_UI.h"
#include "item_boost.h"
#include "enemy.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MAX_WALLSIZE (800.0f)
#define HEIGHT_SIZE	 (100.0f)

//=============================================================================
// グローバル変数
//=============================================================================
bool g_bPause = false;		//ポーズ中かどうか

//=============================================================================
// ゲーム画面の初期化処理
//=============================================================================
HRESULT InitGame(void)
{
	//メッシュフィールドの初期化処理
	InitMeshfield();

	//壁(メッシュ)の初期化処理
	InitMeshwall();

	//ビルボードの初期化処理
	InitBillboard();

	//弾の初期化処理
	InitBullet();

	//爆発の初期化処理
	InitExplosion();

	//影の初期化処理
	InitShadow();

	//エフェクトの初期化処理
	InitEffect();

	//モデルの初期化処理
	InitPlayer();

	//オブジェクトの初期化処理
//	InitObject();

	//敵の初期化処理
	InitEnemy();

	//ブーストアイテムの初期化処理
	InitItemBoost();

	//ポーズの初期化処理
	InitPause();

	//カメラの初期化処理
	InitCamera();

	//ライトの初期化処理
	InitLight();

	//クールダウンUIの初期化処理
	InitCooldown();

	//------------------------------------

	//壁の配置
	SetWall();

	//------------------------------------
	return S_OK;
}

//=============================================================================
// ゲーム画面の終了処理
//=============================================================================
void UninitGame(void)
{
	//メッシュフィールドの終了処理
	UninitMeshfield();

	//壁(メッシュ)の終了処理
	UninitMeshwall();

	//ビルボードの終了処理
	UninitBillboard();

	//弾の終了処理
	UninitBullet();

	//爆発の終了処理
	UninitExplosion();

	//影の終了処理
	UninitShadow();

	//エフェクトの終了処理
	UninitEffect();

	//モデルの終了処理
	UninitPlayer();

	//オブジェクトの終了処理
//	UninitObject();

	//敵の終了処理
	UninitEnemy();

	//ブーストアイテムの終了処理
	UninitItemBoost();

	//ポーズの終了処理
	UninitPause();

	//カメラの終了処理
	UninitCamera();

	//ライトの終了処理
	UninitLight();

	//クールダウンUIの終了処理
	UninitCooldown();
}

//=============================================================================
// ゲーム画面の更新処理
//=============================================================================
void UpdateGame(void)
{
	//情報の取得
	int nFade = GetFade();

	//ポーズ
	if (GetKeyboardTrigger(DIK_P) == true)
	{
		//効果音

		g_bPause = g_bPause ? false : true;	//1行Ver.
	}

	if (g_bPause == true)
	{
		//ポーズの更新処理
		UpdatePause();
	}

	if (g_bPause == false)
	{
		//メッシュフィールドの更新処理
		UpdateMeshfield();

		//壁(メッシュ)の更新処理
		UpdateMeshwall();

		//ビルボードの更新処理
		UpdateBillboard();

		//弾の更新処理
		UpdateBullet();

		//爆発の更新処理
		UpdateExplosion();

		//影の更新処理
		UpdateShadow();

		//エフェクトの更新処理
		UpdateEffect();

		//モデルの更新処理
		UpdatePlayer();

		//オブジェクトの更新処理
//		UpdateObject();

		//敵の更新処理
		UpdateEnemy();

		//ブーストアイテムの更新処理
		UpdateItemBoost();

		//カメラの更新処理
		UpdateCamera();

		//ライトの更新処理
		UpdateLight();

		//クールダウンUIの更新処理
		UpdateCooldown();
	}

	//エンター押したら
	if (GetKeyboardTrigger(DIK_RETURN) == true)
	{
		if (nFade == FADE_NONE)
		{
			//効果音

			SetFade(FADE_OUT, MODE_RESULT);	//タイトル画面に切り替え
		}
	}
}

//=============================================================================
// ゲーム画面の描画処理
//=============================================================================
void DrawGame(void)
{
	//メッシュフィールドの描画処理
	DrawMeshfield();

	//壁(メッシュ)の描画処理
	DrawMeshwall();

	//ビルボードの描画処理
	DrawBillboard();

	//弾の描画処理
	DrawBullet();

	//爆発の描画処理
	DrawExplosion();

	//影の描画処理
	DrawShadow();

	//エフェクトの描画処理
	DrawEffect();

	//モデルの描画処理
	DrawPlayer();

	//オブジェクトの描画処理
//	DrawObject();

	//敵の描画処理
	DrawEnemy();

	//ブーストアイテムの描画処理
	DrawItemBoost();

	//クールダウンUIの描画処理
	DrawCooldown();


	//ポーズの描画処理
	if (g_bPause == true)
	{
		DrawPause();
	}
}

//=============================================================================
// 壁の配置
//=============================================================================
void SetWall(void)
{
	SetMeshwall(D3DXVECTOR3(0.0f, 0.0f, MAX_WALLSIZE), D3DXVECTOR3(0.0f, 0.0f, 0.0f), MAX_WALLSIZE, HEIGHT_SIZE);
	SetMeshwall(D3DXVECTOR3(0.0f, 0.0f, HEIGHT_SIZE), D3DXVECTOR3(0.0f, D3DX_PI / 2, 0.0f), MAX_WALLSIZE, HEIGHT_SIZE);
	SetMeshwall(D3DXVECTOR3(0.0f, 0.0f, HEIGHT_SIZE), D3DXVECTOR3(0.0f, D3DX_PI / -2, 0.0f), MAX_WALLSIZE, HEIGHT_SIZE);
	SetMeshwall(D3DXVECTOR3(0.0f, 0.0f, HEIGHT_SIZE), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), MAX_WALLSIZE, HEIGHT_SIZE);
}

//=============================================================================
// ポーズの設定
//=============================================================================
void SetPause(bool bPause)
{
	g_bPause = bPause;
}