//=============================================================================
//
// 敵の処理 [enemy.cpp]
// Author:Taiki Hayasaka
//
//=============================================================================
#include "enemy.h"				//インクルードファイル
#include "explosion.h"
#include "score.h"
#include "sound.h"
#include "bullet.h"
#include "input.h"
#include "fade.h"
#include "effect.h"
#include <stdlib.h>

//=============================================================================
// マクロ定義
//=============================================================================
#define MAX_TYPE_ENEMY (256)	//敵の種類

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DTEXTURE9 g_apTextureEnemy[MAX_TYPE_ENEMY] = {};	//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEnemy = NULL;				//頂点バッファへのポインタ
D3DXVECTOR3 g_posEnemy;										//敵の位置
D3DXVECTOR3 g_rotEnemy;										//敵の回転角
D3DXVECTOR3 g_moveEnemy;									//敵の移動量
ENEMY g_aEnemy[MAX_ENEMY];									//敵の情報
float g_fLengthEnemy;										//敵の対角線の長さ
float g_fAngleEnemy;										//敵の対角線の角度
int g_nCounterAnimEnemy;									//アニメーションカウンター
int g_nPatternAnimEnemy;									//アニメーションパターンNo
int g_nEnemyCnt;											//敵の数
int g_nCntEnemyDeath;										//敵死亡時のカウント

//=============================================================================
// 敵の初期化処理
//=============================================================================
HRESULT InitEnemy(void)
{
	//変数宣言
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVtx;

	//デバイスの取得
	pDevice = GetDevice();

	//アニメーションの初期化
	g_nCounterAnimEnemy = 0;
	g_nPatternAnimEnemy = 0;

	//敵の数の初期化
	g_nEnemyCnt = 0;

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/enemy100.png", &g_apTextureEnemy[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/enemy101.png", &g_apTextureEnemy[1]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/enemy102.png", &g_apTextureEnemy[2]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/enemy103.png", &g_apTextureEnemy[3]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/enemy004.png", &g_apTextureEnemy[4]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/enemy005.png", &g_apTextureEnemy[5]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/enemy006.png", &g_apTextureEnemy[6]);

	//初期化
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		g_aEnemy[nCntEnemy].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].bUse = false;
		g_aEnemy[nCntEnemy].nType = 0;
		g_aEnemy[nCntEnemy].nCounterState = 0;
		g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;
		g_aEnemy[nCntEnemy].nLife = 0;
	}

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_ENEMY,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEnemy,
		NULL)))
	{
		return E_FAIL;
	}

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		//頂点情報の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);		//テクスチャ座標
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		//頂点座標の設定
		SetVertexEnemy(nCntEnemy);

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

		//４つの頂点を進める
		pVtx += 4;
	}
	//頂点バッファをアンロックする
	g_pVtxBuffEnemy->Unlock();

	return S_OK;
}

//=============================================================================
// 敵の終了処理
//=============================================================================
void UninitEnemy(void)
{
	//頂点バッファの開放
	if (g_pVtxBuffEnemy != NULL)
	{
		g_pVtxBuffEnemy->Release();
		g_pVtxBuffEnemy = NULL;
	}

	//テクスチャの開放
	for (int nCntEnemy = 0; nCntEnemy < MAX_TYPE_ENEMY; nCntEnemy++)
	{
		if (g_apTextureEnemy[nCntEnemy] != NULL)
		{
			g_apTextureEnemy[nCntEnemy]->Release();
			g_apTextureEnemy[nCntEnemy] = NULL;
		}
	}

	for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++)
	{
		g_aEnemy[nCnt].bUse = false;
	}
}

//=============================================================================
// 敵の更新処理
//=============================================================================
void UpdateEnemy(void)
{
	//変数宣言
	g_nCounterAnimEnemy++;		//アニメーションカウンター更新
	VERTEX_2D *pVtx;
	ENEMY *pEnemy;
	static int nCount;

	//pEnemyの初期化
	pEnemy = &g_aEnemy[0];

	//敵カウント
	nCount++;

	if ((g_nCounterAnimEnemy % 20) == 0)
	{
		//アニメーションパターンを更新
		g_nPatternAnimEnemy = (g_nPatternAnimEnemy + 1) % 2;
	}

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bUse == true)	//敵が使用されているとき
		{
			//敵の弾を出す
			if ((rand() % 120) == 0)
			{
				SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(0.0f, 10.0f, 0.0f), 60, BULLETTYPE_ENEMY);
			}

			//位置の更新
			pEnemy->pos.x += pEnemy->move.x;

			//敵の頂点座標の設定
			SetVertexEnemy(nCntEnemy);

			//画面外チェック
			if (pEnemy->pos.x < 20 || pEnemy->pos.x > SCREEN_WIDTH - 20)
			{
				for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++)
				{
					//端についたら逆へ
					g_aEnemy[nCnt].move *= -1.0f;
				}
			}
		}

		//テクスチャ座標を更新
		pVtx[0].tex = D3DXVECTOR2(0.0f + (0.5f*g_nPatternAnimEnemy), 1.0f);		//テクスチャ座標
		pVtx[1].tex = D3DXVECTOR2(0.0f + (0.5f*g_nPatternAnimEnemy), 0.0f);		//アニメーション
		pVtx[2].tex = D3DXVECTOR2(0.5f + (0.5f*g_nPatternAnimEnemy), 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.5f + (0.5f*g_nPatternAnimEnemy), 0.0f);

		//ダメージの色を元に戻す
		switch (g_aEnemy[nCntEnemy].state)
		{
		case ENEMYSTATE_DAMAGE:

			//カウンターステートマイナス
			g_aEnemy[nCntEnemy].nCounterState--;

			if (g_aEnemy[nCntEnemy].nCounterState <= 0)
			{
				g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;

				//頂点の色を戻す
				pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			}
			break;
		}
		//４つの頂点を進める
		pVtx += 4;
	}
	//頂点バッファのアンロック
	g_pVtxBuffEnemy->Unlock();

	//敵を全部倒したら
	if (g_nEnemyCnt <= 0)
	{
		g_nCntEnemyDeath++;
		if (g_nCntEnemyDeath == 40)
		{
			SetFade(FADE_OUT, MODE_BOSSBATTLE);	//リザルト画面に切り替え
			g_nEnemyCnt++;
		}
	}
}

//=============================================================================
// 敵の描画処理
//=============================================================================
void DrawEnemy(void)
{
	//変数宣言
	LPDIRECT3DDEVICE9 pDevice;
	int nCntVertex = 0;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffEnemy, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true)
		{
			pDevice->SetTexture(0, g_apTextureEnemy[g_aEnemy[nCntEnemy].nType]);
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntVertex, 2);
		}
		//４つ進める
		nCntVertex += 4;
	}
}

//=============================================================================
// 敵の設定
//=============================================================================
void SetEnemy(D3DXVECTOR3 pos, int nType, D3DXVECTOR3 move)
{
	//変数宣言
	ENEMY *pEnemy;
	VERTEX_2D *pVtx;

	pEnemy = &g_aEnemy[0];

	//敵数プラス
	g_nEnemyCnt++;

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bUse == false)
		{
			//敵の体力
			g_aEnemy[nCntEnemy].nLife = 3;

			//位置を設定
			pEnemy->pos = pos;

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(pEnemy->pos.x - 30.0f, pEnemy->pos.y + 30.0f, 0.0f);	//Zは0.0固定
			pVtx[1].pos = D3DXVECTOR3(pEnemy->pos.x - 30.0f, pEnemy->pos.y - 30.0f, 0.0f);	
			pVtx[2].pos = D3DXVECTOR3(pEnemy->pos.x + 30.0f, pEnemy->pos.y + 30.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(pEnemy->pos.x + 30.0f, pEnemy->pos.y - 30.0f, 0.0f);

			//タイプの設定
			pEnemy->nType = nType;

			//敵の使用の設定
			pEnemy->bUse = true;

			//敵の移動の設定
			pEnemy->move = move;

			break;
		}
		//４つの頂点を進める
		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffEnemy->Unlock();
}

//=============================================================================
// 敵の頂点座標の設定
//=============================================================================
void SetVertexEnemy(int nIdx)
{
	//変数宣言
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += nIdx * 4;

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nIdx].pos.x - 50.0f, g_aEnemy[nIdx].pos.y + 50.0f, 0.0f);	//大きさ変更はここ
	pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nIdx].pos.x - 50.0f, g_aEnemy[nIdx].pos.y - 50.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nIdx].pos.x + 50.0f, g_aEnemy[nIdx].pos.y + 50.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nIdx].pos.x + 50.0f, g_aEnemy[nIdx].pos.y - 50.0f, 0.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffEnemy->Unlock();
}

//=============================================================================
// 敵の当たり判定
//=============================================================================
bool HitEnemy(int nIdx, int nDamage)
{
	//変数宣言
	VERTEX_2D *pVtx;

	//敵ライフのマイナス
	g_aEnemy[nIdx].nLife -= nDamage;

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	if (g_aEnemy[nIdx].nLife <= 0)
	{
		//爆発を敵の座標に出す
		SetExplosion(g_aEnemy[nIdx].pos, D3DXCOLOR(255.0f, 255.0f, 255.0f, 100.0f));

		//爆発効果音(爆発)
		PlaySound(SOUND_LABEL_SE_EXPLOSION);

		//ヒット音
		PlaySound(SOUND_LABEL_SE_HIT);

		//敵を使った判定
		g_aEnemy[nIdx].bUse = false;

		//敵を倒した得点
		AddScore(200);

		//敵の数カウントを減らす
		g_nEnemyCnt--;

		return true;
	}
	else
	{
		g_aEnemy[nIdx].state = ENEMYSTATE_DAMAGE;
		g_aEnemy[nIdx].nCounterState = 5;

		//４つの頂点を進める
		pVtx += nIdx * 4;
		
		for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
		{
			pVtx[0].col = D3DCOLOR_RGBA(255, 0, 0, 255);
			pVtx[1].col = D3DCOLOR_RGBA(255, 0, 0, 255);
			pVtx[2].col = D3DCOLOR_RGBA(255, 0, 0, 255);
			pVtx[3].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		}

		//ヒット音
		PlaySound(SOUND_LABEL_SE_HIT);

		return false;
	}
}
//=============================================================================
// 敵の取得
//=============================================================================
ENEMY *GetEnemy(void)
{
	return &g_aEnemy[0];
}