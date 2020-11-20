//=============================================================================
//
// 弾の処理 [bullet.cpp]
// Author:Taiki Hayasaka
//
//=============================================================================
#include "bullet.h"									//インクルードファイル
#include "player.h"
#include "enemy.h"
#include "explosion.h"
#include "score.h"
#include "sound.h"
#include "effect.h"
#include "Boss.h"

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DTEXTURE9 g_pTextureBullet = NULL;			//弾のテクスチャのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;	//弾の頂点バッファのポインタ
BULLET g_aBullet[MAX_BALLET];						//弾の情報

//=============================================================================
// 弾の初期化処理
//=============================================================================
HRESULT InitBullet(void)
{
	//変数宣言
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVtx;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Bullet000.png", &g_pTextureBullet);

	//初期化
	for (int nCntBullet = 0; nCntBullet < MAX_BALLET; nCntBullet++)
	{
		g_aBullet[nCntBullet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].nLife = 0;
		g_aBullet[nCntBullet].bUse = false;
		g_aBullet[nCntBullet].type = BULLETTYPE_PLAYER;
	}

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BALLET,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBullet,
		NULL)))
	{
		return E_FAIL;
	}

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBullet = 0; nCntBullet < MAX_BALLET; nCntBullet++)
	{
		//頂点情報の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);		//テクスチャ座標
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(100, 200, 0.0f);	//Zは0.0固定
		pVtx[1].pos = D3DXVECTOR3(100, 400, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(200, 200, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(200, 400, 0.0f);

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
	g_pVtxBuffBullet->Unlock();

	return S_OK;
}

//=============================================================================
// 弾の終了処理
//=============================================================================
void UninitBullet(void)
{
	//頂点バッファの開放
	if (g_pVtxBuffBullet != NULL)
	{
		g_pVtxBuffBullet ->Release();
		g_pVtxBuffBullet = NULL;
	}

	//テクスチャの開放
	if (g_pTextureBullet != NULL)
	{
		g_pTextureBullet ->Release();
		g_pTextureBullet = NULL;
	}
}

//=============================================================================
// 弾の更新処理
//=============================================================================
void UpdateBullet(void)
{
	//変数宣言
	BULLET *pBullet;
	ENEMY *pEnemy;
	BOSS *pBoss;
	PLAYER *pPlayer;
	VERTEX_2D *pVtx;

	//pBulletの初期化
	pBullet = &g_aBullet[0];

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBullet = 0; nCntBullet < MAX_BALLET; nCntBullet++, pBullet++)
	{
		if (pBullet->bUse == true)	//弾が使用されていたら
		{
			//位置の更新
			pBullet->pos.x += pBullet->move.x;
			pBullet->pos.y += pBullet->move.y;

			//頂点座標の更新
			pVtx[0].pos = D3DXVECTOR3(pBullet->pos.x - 15.0f, pBullet->pos.y + 15.0f, 0.0f);	//Zは0.0固定
			pVtx[1].pos = D3DXVECTOR3(pBullet->pos.x - 15.0f, pBullet->pos.y - 15.0f, 0.0f);	//大きさ変更はここ
			pVtx[2].pos = D3DXVECTOR3(pBullet->pos.x + 15.0f, pBullet->pos.y + 15.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(pBullet->pos.x + 15.0f, pBullet->pos.y - 15.0f, 0.0f);

			// 画面外チェック
			if (pBullet->pos.y <= 0)			//画面上
			{
				pBullet->bUse = false;
			}
			if (pBullet->pos.y > SCREEN_HEIGHT)	//画面下
			{
				pBullet->bUse = false;
			}

			if (pBullet->type == BULLETTYPE_PLAYER)	//プレイヤーの弾だったら
			{
				//エフェクト
				SetEffect(pBullet->pos, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f));
				SetEffect(pBullet->pos, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
				SetEffect(pBullet->pos, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f));

				//敵との当たり判定
				pEnemy = GetEnemy();
				pBoss = GetBoss();

				for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
				{
					if (pEnemy->bUse == true)	//敵が使用されていたら
					{
						//弾と敵が重なった
						if (pEnemy->pos.x - 40.0f < g_aBullet[nCntBullet].pos.x &&
							pEnemy->pos.x + 40.0f > g_aBullet[nCntBullet].pos.x &&
							pEnemy->pos.y - 40.0f < g_aBullet[nCntBullet].pos.y &&
							pEnemy->pos.y + 40.0f > g_aBullet[nCntBullet].pos.y)
						{
							//敵の当たり判定
							HitEnemy(nCntEnemy, 1);

							//弾を使った判定
							pBullet->bUse = false;
						}

						//寿命チェック
						g_aBullet[nCntBullet].nLife--;
						if (g_aBullet[nCntBullet].nLife < 0)
						{
							//寿命で爆発
							SetExplosion(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));

							//爆発効果音(爆発)
							PlaySound(SOUND_LABEL_SE_EXPLOSION2);

							//弾をfalseに
							g_aBullet[nCntBullet].bUse = false;
						}
					}
				}

				if (pBoss->bUse == true)
				{
					//弾と敵が重なった
					if (pBoss->pos.x - 180.0f < g_aBullet[nCntBullet].pos.x &&
						pBoss->pos.x + 180.0f > g_aBullet[nCntBullet].pos.x &&
						pBoss->pos.y - 180.0f < g_aBullet[nCntBullet].pos.y &&
						pBoss->pos.y + 180.0f > g_aBullet[nCntBullet].pos.y)
					{
						//敵の当たり判定
						HitBoss(1);

						//弾を使った判定
						pBullet->bUse = false;
					}
				}
			}

			if (pBullet->type == BULLETTYPE_ENEMY)	//敵の弾だったら
			{
				//エフェクト
				SetEffect(pBullet->pos, D3DXCOLOR(0.0f, 1.0f, 1.0f, 0.5f));
				SetEffect(pBullet->pos, D3DXCOLOR(0.0f, 1.0f, 1.0f, 0.5f));

				//プレイヤーとの当たり判定
				pPlayer = GetPlayer();

				if (pPlayer->state == PLAYERSTATE_NORMAL)	//プレイヤーが使用されていたら
				{
					//弾とプレイヤーが重なった
					if (pPlayer->pos.x - 40.0f < g_aBullet[nCntBullet].pos.x &&
						pPlayer->pos.x + 40.0f > g_aBullet[nCntBullet].pos.x &&
						pPlayer->pos.y - 40.0f < g_aBullet[nCntBullet].pos.y &&
						pPlayer->pos.y + 40.0f > g_aBullet[nCntBullet].pos.y)
					{
						//プレイヤーの当たり判定
						HitPlayer(1);

						//弾を使った判定
						pBullet->bUse = false;
					}
				}

				//寿命チェック
				g_aBullet[nCntBullet].nLife--;
				if (g_aBullet[nCntBullet].nLife < 0)
				{
					//寿命で爆発
					SetExplosion(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f,1.0f,1.0f,0.5f));

					//爆発効果音(爆発)
					PlaySound(SOUND_LABEL_SE_EXPLOSION2);

					//弾をfalseに
					g_aBullet[nCntBullet].bUse = false;
				}
			}
		}
		//４つの頂点を進める
		pVtx += 4;
	}
	//頂点バッファをアンロックする
	g_pVtxBuffBullet->Unlock();
}

//=============================================================================
// 弾の描画処理
//=============================================================================
void DrawBullet(void)
{
	//変数宣言
	LPDIRECT3DDEVICE9 pDevice;
	int nCntVertex = 0;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureBullet);

	for (int nCntBullet = 0; nCntBullet < MAX_BALLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,nCntVertex, 2);
		}
		//４つ進める
		nCntVertex += 4;
	}
}

//=============================================================================
// 弾の設定
//=============================================================================
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nLife,BULLETTYPE type)
{
	//変数宣言
	BULLET *pBullet;
	pBullet = &g_aBullet[0];
	VERTEX_2D *pVtx;

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBullet = 0; nCntBullet < MAX_BALLET; nCntBullet++, pBullet++)
	{
		if (pBullet->bUse == false)
		{
			//位置を設定
			pBullet->pos = pos;

			//頂点座標の設定
			SetVertexBullet(nCntBullet);

			//４つの頂点を進める
			pVtx += 4;

			//移動量を設定
			pBullet->move = move;

			//寿命を設定
			pBullet->nLife = nLife;

			//弾の使用
			pBullet->bUse = true;

			//弾のタイプ
			pBullet->type = type;

			break;
		}
	}
	//頂点バッファをアンロックする
	g_pVtxBuffBullet->Unlock();
}

//=============================================================================
// 弾の頂点座標の設定
//=============================================================================
void SetVertexBullet(int nIdx)
{
	//変数宣言
	BULLET *pBullet;
	VERTEX_2D *pVtx;
	pBullet = &g_aBullet[0];

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	//４つの頂点を進める
	pVtx += nIdx * 4;

	pVtx[0].pos = D3DXVECTOR3(pBullet->pos.x - 10.0f, pBullet->pos.y + 10.0f, 0.0f);	//Zは0.0固定
	pVtx[1].pos = D3DXVECTOR3(pBullet->pos.x - 10.0f, pBullet->pos.y - 10.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(pBullet->pos.x + 10.0f, pBullet->pos.y + 10.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(pBullet->pos.x + 10.0f, pBullet->pos.y - 10.0f, 0.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffBullet->Unlock();
}