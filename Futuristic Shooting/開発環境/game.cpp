//=============================================================================
//
// ゲームの処理 [game.cpp]
// Author:Taiki Hayasaka
//
//=============================================================================
#include "game.h"			//インクルードファイル
#include "BG.h"
#include "player.h"
#include "enemy.h"
#include "bullet.h"
#include "explosion.h"
#include "score.h"
#include "sound.h"
#include "fade.h"
#include "effect.h"
#include "pause.h"
#include "Time.h"

//=============================================================================
// グローバル変数
//=============================================================================
bool g_bPause = false;		//ポーズ中かどうか

//=============================================================================
// ゲーム画面の初期化処理
//=============================================================================
HRESULT InitGame(void)
{
	//背景の初期化処理
	InitBG();

	//エフェクトの初期化処理
	InitEffect();

	//ポリゴンの初期化処理
	InitPlayer();

	//敵の初期化処理
	InitEnemy();

	//弾の初期化処理
	InitBullet();

	//爆発の初期化処理
	InitExplosion();

	//スコアの初期化処理
	InitScore();

	//ポーズの初期化処理
	InitPause();

	//敵の設定
	SetEnemy(D3DXVECTOR3(600.0f, 150.0f, 0.0f), 0, D3DXVECTOR3(2.0f, 0.0f, 0.0f));	//1列目
	SetEnemy(D3DXVECTOR3(720.0f, 150.0f, 0.0f), 1, D3DXVECTOR3(2.0f, 0.0f, 0.0f));
	SetEnemy(D3DXVECTOR3(840.0f, 150.0f, 0.0f), 2, D3DXVECTOR3(2.0f, 0.0f, 0.0f));
	SetEnemy(D3DXVECTOR3(960.0f, 150.0f, 0.0f), 3, D3DXVECTOR3(2.0f, 0.0f, 0.0f));
	SetEnemy(D3DXVECTOR3(1080.0f, 150.0f, 0.0f), 2, D3DXVECTOR3(2.0f, 0.0f, 0.0f));
	SetEnemy(D3DXVECTOR3(1200.0f, 150.0f, 0.0f), 1, D3DXVECTOR3(2.0f, 0.0f, 0.0f));
	SetEnemy(D3DXVECTOR3(1320.0f, 150.0f, 0.0f), 0, D3DXVECTOR3(2.0f, 0.0f, 0.0f));

	SetEnemy(D3DXVECTOR3(600.0f, 250.0f, 0.0f), 0, D3DXVECTOR3(-2.0f, 0.0f, 0.0f));	//2列目
	SetEnemy(D3DXVECTOR3(720.0f, 250.0f, 0.0f), 1, D3DXVECTOR3(-2.0f, 0.0f, 0.0f));
	SetEnemy(D3DXVECTOR3(840.0f, 250.0f, 0.0f), 2, D3DXVECTOR3(-2.0f, 0.0f, 0.0f));
	SetEnemy(D3DXVECTOR3(960.0f, 250.0f, 0.0f), 3, D3DXVECTOR3(-2.0f, 0.0f, 0.0f));
	SetEnemy(D3DXVECTOR3(1080.0f, 250.0f, 0.0f), 2, D3DXVECTOR3(-2.0f, 0.0f, 0.0f));
	SetEnemy(D3DXVECTOR3(1200.0f, 250.0f, 0.0f), 1, D3DXVECTOR3(-2.0f, 0.0f, 0.0f));
	SetEnemy(D3DXVECTOR3(1320.0f, 250.0f, 0.0f), 0, D3DXVECTOR3(-2.0f, 0.0f, 0.0f));

	SetEnemy(D3DXVECTOR3(600.0f, 350.0f, 0.0f), 0, D3DXVECTOR3(2.0f, 0.0f, 0.0f));	//3列目
	SetEnemy(D3DXVECTOR3(720.0f, 350.0f, 0.0f), 1, D3DXVECTOR3(2.0f, 0.0f, 0.0f));
	SetEnemy(D3DXVECTOR3(840.0f, 350.0f, 0.0f), 2, D3DXVECTOR3(2.0f, 0.0f, 0.0f));
	SetEnemy(D3DXVECTOR3(960.0f, 350.0f, 0.0f), 3, D3DXVECTOR3(2.0f, 0.0f, 0.0f));
	SetEnemy(D3DXVECTOR3(1080.0f, 350.0f, 0.0f), 2, D3DXVECTOR3(2.0f, 0.0f, 0.0f));
	SetEnemy(D3DXVECTOR3(1200.0f, 350.0f, 0.0f), 1, D3DXVECTOR3(2.0f, 0.0f, 0.0f));
	SetEnemy(D3DXVECTOR3(1320.0f, 350.0f, 0.0f), 0, D3DXVECTOR3(2.0f, 0.0f, 0.0f));

	//BGM
	PlaySound(SOUND_LABEL_BGM003);

	return S_OK;
}

//=============================================================================
// ゲーム画面の終了処理
//=============================================================================
void UninitGame(void)
{
	//サウンドストップ
	StopSound();

	//背景の終了処理
	UninitBG();

	//エフェクトの終了処理
	UninitEffect();

	//プレイヤーの終了処理
	UninitPlayer();

	//敵の終了処理
	UninitEnemy();

	//弾の終了処理
	UninitBullet();

	//爆発の終了処理
	UninitExplosion();

	//スコアの終了処理
	UninitScore();

	//ポーズの終了処理
	UninitPause();
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
		g_bPause = g_bPause ? false : true;	//1行Ver.

		//ポーズ効果音
		PlaySound(SOUND_LABEL_SE_PAUSE);
	}

	if (g_bPause == true)
	{
		//ポーズの更新処理
		UpdatePause();
	}

	if (g_bPause == false)
	{
		//背景の更新処理
		UpdateBG();

		//エフェクトの更新処理
		UpdateEffect();

		//プレイヤーの更新処理
		UpdatePlayer();

		//敵の更新処理
		UpdateEnemy();

		//弾の更新処理
		UpdateBullet();

		//爆破の更新処理
		UpdateExplosion();

		//スコアの更新処理
		UpdateScore();
	}
}

//=============================================================================
// ゲーム画面の描画処理
//=============================================================================
void DrawGame(void)
{
	//背景の描画処理
	DrawBG();

	//エフェクトの描画処理
	DrawEffect();

	//プレイヤーの描画処理
	DrawPlayer();

	//敵の描画処理
	DrawEnemy();

	//弾の描画処理
	DrawBullet();

	//爆発の描画処理
	DrawExplosion();

	//スコアの描画処理
	DrawScore();

	//ポーズの描画処理
	if (g_bPause ==true)
	{
		DrawPause();
	}
}

//=============================================================================
// ポーズの設定
//=============================================================================
void SetPause(bool bPause)
{
	g_bPause = bPause;
}