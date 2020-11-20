//=============================================================================
//
// ボスバトルの処理 [BossBattle.cpp]
// Author:Taiki Hayasaka
//
//=============================================================================
#include "BossBattle.h"			//インクルードファイル
#include "player.h"
#include "bullet.h"
#include "explosion.h"
#include "sound.h"
#include "effect.h"
#include "Time.h"
#include "BossBG.h"
#include "Boss.h"
#include "enemy.h"

//=============================================================================
// ボスバトル画面の初期化処理
//=============================================================================
HRESULT InitBossBattle(void)
{
	//背景の初期化処理
	InitBossBG();

	//エフェクトの初期化処理
	InitEffect();

	//弾の初期化処理
	InitBullet();

	//ポリゴンの初期化処理
	InitPlayer();

	//ボスの初期化処理
	InitBoss();

	//爆発の初期化処理
	InitExplosion();

	//ボスの配置
	SetBoss(D3DXVECTOR3(960.0f, 200.0f, 0.0f), D3DXVECTOR3(8.0f, 0.0f, 0.0f));

	//BGM
	PlaySound(SOUND_LABEL_BGM_BOSS);

	return S_OK;
}

//=============================================================================
// ボスバトル画面の終了処理
//=============================================================================
void UninitBossBattle(void)
{
	//サウンドストップ
	StopSound();

	//背景の終了処理
	UninitBossBG();

	//エフェクトの終了処理
	UninitEffect();

	//弾の終了処理
	UninitBullet();

	//プレイヤーの終了処理
	UninitPlayer();

	//ボスの終了処理
	UninitBoss();

	//爆発の終了処理
	UninitExplosion();
}

//=============================================================================
// ボスバトル画面の更新処理
//=============================================================================
void UpdateBossBattle(void)
{
	//背景の更新処理
	UpdateBossBG();

	//エフェクトの更新処理
	UpdateEffect();

	//弾の更新処理
	UpdateBullet();

	//プレイヤーの更新処理
	UpdatePlayer();

	//ボスの更新処理
	UpdateBoss();

	//爆破の更新処理
	UpdateExplosion();
}

//=============================================================================
// ボスバトル画面の描画処理
//=============================================================================
void DrawBossBattle(void)
{
	//背景の描画処理
	DrawBossBG();

	//エフェクトの描画処理
	DrawEffect();

	//弾の描画処理
	DrawBullet();

	//プレイヤーの描画処理
	DrawPlayer();

	//ボスの描画処理
	DrawBoss();

	//爆発の描画処理
	DrawExplosion();
}
