//=============================================================================
//
// ボスの処理 [Boss.cpp]
// Author:Taiki Hayasaka
//
//=============================================================================
#include "Boss.h"				//インクルードファイル
#include "explosion.h"
#include "sound.h"
#include "bullet.h"
#include "input.h"
#include "fade.h"
#include "effect.h"
#include <stdlib.h>

//=============================================================================
// マクロ定義
//=============================================================================
#define MAX_TYPE_BOSS (1)	//敵の種類

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DTEXTURE9 g_apTextureBoss = NULL;				//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBoss = NULL;			//頂点バッファへのポインタ
D3DXVECTOR3 g_posBoss;									//ボスの位置
D3DXVECTOR3 g_rotBoss;									//ボスの回転角
D3DXVECTOR3 g_moveBoss;									//ボスの移動量
BOSS g_aBoss		;									//ボスの情報
float g_fLengthBoss;									//ボスの対角線の長さ
float g_fAngleBoss;										//ボスの対角線の角度
int g_nBossCnt;											//ボスの数
int g_nCntBossDeath;									//ボス死亡時のカウント
int g_nCounterAnimBoss;									//アニメーションカウンター
int g_nPatternAnimBoss;									//アニメーションパターンNo

//=============================================================================
// ボスの初期化処理
//=============================================================================
HRESULT InitBoss(void)
{
	//変数宣言
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVtx;
	int nCntBoss = NULL;

	//デバイスの取得
	pDevice = GetDevice();

	//アニメーションの初期化
	g_nCounterAnimBoss = 0;
	g_nPatternAnimBoss = 0;

	//ボスの数の初期化
	g_nBossCnt = 0;

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Boss000.png", &g_apTextureBoss);

	//初期化
	g_aBoss.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aBoss.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aBoss.bUse = false;
	g_aBoss.nCounterState = 0;
	g_aBoss.state = BOSSSTATE_NORMAL;
	g_aBoss.nLife = 0;
	

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BOSS,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBoss,
		NULL)))
	{
		return E_FAIL;
	}

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffBoss->Lock(0, 0, (void**)&pVtx, 0);

	//頂点情報の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);	//テクスチャ座標
	pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

	//頂点座標の設定
	SetVertexBoss(nCntBoss);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//頂点バッファをアンロックする
	g_pVtxBuffBoss->Unlock();

	return S_OK;
}

//=============================================================================
// ボスの終了処理
//=============================================================================
void UninitBoss(void)
{
	//頂点バッファの開放
	if (g_pVtxBuffBoss != NULL)
	{
		g_pVtxBuffBoss->Release();
		g_pVtxBuffBoss = NULL;
	}

	//テクスチャの開放
	if (g_apTextureBoss != NULL)
	{
		g_apTextureBoss->Release();
		g_apTextureBoss = NULL;
	}

	for (int nCnt = 0; nCnt < MAX_BOSS; nCnt++)
	{
		g_aBoss.bUse = false;
	}
}

//=============================================================================
// ボスの更新処理
//=============================================================================
void UpdateBoss(void)
{
	//変数宣言
	VERTEX_2D *pVtx;
	BOSS *pBoss;
	static int nCount;
	int nCntBoss1 = NULL;

	//pBossの初期化
	pBoss = &g_aBoss;

	//敵カウント
	nCount++;

	//アニメーションカウンター更新
	g_nCounterAnimBoss++;		

	if ((g_nCounterAnimBoss % 30) == 0)
	{
		//アニメーションパターンを更新
		g_nPatternAnimBoss = (g_nPatternAnimBoss + 1) % 2;
	}

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffBoss->Lock(0, 0, (void**)&pVtx, 0);

	if (pBoss->bUse == true)	//ボスが使用されているとき
	{
		//ボスの弾を出す
		if ((rand() % 15) == 0)
		{
			SetBullet(g_aBoss.pos, D3DXVECTOR3(0.0f, 20.0f, 0.0f), 40, BULLETTYPE_ENEMY);
			SetBullet(g_aBoss.pos, D3DXVECTOR3(1.0f, 20.0f, 0.0f), 40, BULLETTYPE_ENEMY);
			SetBullet(g_aBoss.pos, D3DXVECTOR3(-1.0f, 20.0f, 0.0f), 40, BULLETTYPE_ENEMY);
			SetBullet(g_aBoss.pos, D3DXVECTOR3(2.0f, 20.0f, 0.0f), 40, BULLETTYPE_ENEMY);
			SetBullet(g_aBoss.pos, D3DXVECTOR3(-2.0f, 20.0f, 0.0f), 40, BULLETTYPE_ENEMY);
		}

		//位置の更新
		pBoss->pos.x += pBoss->move.x;

		//敵の頂点座標の設定
		SetVertexBoss(nCntBoss1);

		//画面外チェック
		if (pBoss->pos.x < 200 || pBoss->pos.x > SCREEN_WIDTH - 200)
		{
			//端についたら逆へ
			g_aBoss.move *= -1.0f;
		}
	}

	//テクスチャ座標を更新
	pVtx[0].tex = D3DXVECTOR2(0.0f + (0.5f*g_nPatternAnimBoss), 1.0f);		//テクスチャ座標
	pVtx[1].tex = D3DXVECTOR2(0.0f + (0.5f*g_nPatternAnimBoss), 0.0f);		//アニメーション
	pVtx[2].tex = D3DXVECTOR2(0.5f + (0.5f*g_nPatternAnimBoss), 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.5f + (0.5f*g_nPatternAnimBoss), 0.0f);

		//ダメージの色を元に戻す
	switch (g_aBoss.state)
	{
	case BOSSSTATE_DAMAGE:

		//カウンターステートマイナス
		g_aBoss.nCounterState--;

		if (g_aBoss.nCounterState <= 0)
		{
			g_aBoss.state = BOSSSTATE_NORMAL;

			//頂点の色を戻す
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		}
		break;
	}

	//頂点バッファのアンロック
	g_pVtxBuffBoss->Unlock();

	//ボスを倒したら
	if (g_nBossCnt <= 0)
	{
		g_nCntBossDeath++;
		if (g_nCntBossDeath == 100)
		{
			SetFade(FADE_OUT, MODE_RESULT);	//リザルト画面に切り替え
			g_nBossCnt++;
		}
	}
}

//=============================================================================
// ボスの描画処理
//=============================================================================
void DrawBoss(void)
{
	//変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBoss, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_apTextureBoss);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//=============================================================================
// ボスの設定
//=============================================================================
void SetBoss(D3DXVECTOR3 pos, D3DXVECTOR3 move)
{
	//変数宣言
	BOSS *pBoss;
	VERTEX_2D *pVtx;

	pBoss = &g_aBoss;

	//敵数プラス
	g_nBossCnt++;

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffBoss->Lock(0, 0, (void**)&pVtx, 0);

	if (pBoss->bUse == false)
	{
		//ボスの体力
		g_aBoss.nLife = 180;

		//位置を設定
		pBoss->pos = pos;

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(pBoss->pos.x - 30.0f, pBoss->pos.y + 30.0f, 0.0f);	//Zは0.0固定
		pVtx[1].pos = D3DXVECTOR3(pBoss->pos.x - 30.0f, pBoss->pos.y - 30.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(pBoss->pos.x + 30.0f, pBoss->pos.y + 30.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(pBoss->pos.x + 30.0f, pBoss->pos.y - 30.0f, 0.0f);

		//ボスの使用の設定
		pBoss->bUse = true;

		//ボスの移動の設定
		pBoss->move = move;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffBoss->Unlock();
}

//=============================================================================
// ボスの頂点座標の設定
//=============================================================================
void SetVertexBoss(int nIdx)
{
	//変数宣言
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBoss->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += nIdx * 4;

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_aBoss.pos.x - 200.0f, g_aBoss.pos.y + 200.0f, 0.0f);	//大きさ変更はここ
	pVtx[1].pos = D3DXVECTOR3(g_aBoss.pos.x - 200.0f, g_aBoss.pos.y - 200.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aBoss.pos.x + 200.0f, g_aBoss.pos.y + 200.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aBoss.pos.x + 200.0f, g_aBoss.pos.y - 200.0f, 0.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffBoss->Unlock();
}

//=============================================================================
// ボスの当たり判定
//=============================================================================
bool HitBoss(int nDamage)
{
	//変数宣言
	VERTEX_2D *pVtx;

	//ボスライフのマイナス
	g_aBoss.nLife -= nDamage;

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffBoss->Lock(0, 0, (void**)&pVtx, 0);

	if (g_aBoss.nLife <= 0)
	{
		//爆発をボスの座標に出す
		SetExplosion(g_aBoss.pos, D3DXCOLOR(255.0f, 255.0f, 255.0f, 100.0f));

		//爆発効果音(爆発)
		PlaySound(SOUND_LABEL_SE_EXPLOSION3);

		//ヒット音
		PlaySound(SOUND_LABEL_SE_HIT);

		//ボスを使った判定
		g_aBoss.bUse = false;

		//敵の数カウントを減らす
		g_nBossCnt--;

		return true;
	}
	else
	{
		g_aBoss.state = BOSSSTATE_DAMAGE;
		g_aBoss.nCounterState = 5;

		pVtx[0].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 0, 0, 255);

		//ヒット音
		PlaySound(SOUND_LABEL_SE_HIT);

		return false;
	}
}
//=============================================================================
// ボスの取得
//=============================================================================
BOSS *GetBoss(void)
{
	return &g_aBoss;
}