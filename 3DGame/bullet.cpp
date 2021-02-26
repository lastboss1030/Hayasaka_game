//=============================================================================
//
// 弾の処理 [bullet.cpp]
// Author:Taiki Hayasaka
//
//=============================================================================
#include "bullet.h"									//インクルードファイル
#include "player.h"
#include "explosion.h"
#include "effect.h"
#include "input.h"
#include "enemy.h"
#include "player.h"
#include "score.h"
#include "Sound.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MAX_BULLET		(256)	//弾の最大数
#define HIT_WALL		(1200)
#define MAX_VERTEX		(4)
#define BULLET_SIZE_P	(70.0f)
#define BULLET_SIZE_E	(30.0f)

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DTEXTURE9 g_pTextureBullet = NULL;			//弾のテクスチャのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;	//弾の頂点バッファのポインタ
BULLET g_aBullet[MAX_BULLET];						//弾の情報
int g_nCntEffect;

//=============================================================================
// 弾の初期化処理
//=============================================================================
HRESULT InitBullet(void)
{
	//変数宣言
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_3D *pVtx;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bullet000.png", &g_pTextureBullet);

	//初期化
	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		g_aBullet[nCntBullet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].type = BULLETTYPE_PLAYER;
		g_aBullet[nCntBullet].fWidth = 5.0f;
		g_aBullet[nCntBullet].fHeight = 5.0f;
		g_aBullet[nCntBullet].bUse = false;
	}

	//初期化
	g_nCntEffect = 0;

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * MAX_VERTEX * MAX_BULLET,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBullet,
		NULL)))
	{
		return E_FAIL;
	}

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - g_aBullet[nCntBullet].fWidth, g_aBullet[nCntBullet].pos.y + g_aBullet[nCntBullet].fHeight, g_aBullet[nCntBullet].pos.z);
		pVtx[1].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + g_aBullet[nCntBullet].fWidth, g_aBullet[nCntBullet].pos.y + g_aBullet[nCntBullet].fHeight, g_aBullet[nCntBullet].pos.z);
		pVtx[2].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - g_aBullet[nCntBullet].fWidth, g_aBullet[nCntBullet].pos.y - g_aBullet[nCntBullet].fHeight, g_aBullet[nCntBullet].pos.z);
		pVtx[3].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + g_aBullet[nCntBullet].fWidth, g_aBullet[nCntBullet].pos.y - g_aBullet[nCntBullet].fHeight, g_aBullet[nCntBullet].pos.z);

		//法線ベクトルの設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 0.6f, 0.2f, 0.5f);
		pVtx[1].col = D3DXCOLOR(1.0f, 0.6f, 0.2f, 0.5f);
		pVtx[2].col = D3DXCOLOR(1.0f, 0.6f, 0.2f, 0.5f);
		pVtx[3].col = D3DXCOLOR(1.0f, 0.6f, 0.2f, 0.5f);

		//頂点情報の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

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
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;
	}

	//テクスチャの開放
	if (g_pTextureBullet != NULL)
	{
		g_pTextureBullet->Release();
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
	Enemy *pEnemy = GetEnemy();
	Player *pPlayer = GetPlayer();
	D3DXVECTOR3 BulletV;

	//pBulletの初期化
	pBullet = &g_aBullet[0];

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++, pBullet++)
	{
		if (pBullet->type == BULLETTYPE_PLAYER)	//プレイヤーの弾だったら
		{

			if (pBullet->bUse == true)	//弾が使用されていたら
			{
				//位置の更新
				pBullet->pos.x += pBullet->move.x;
				pBullet->pos.z += pBullet->move.z;

				//エフェクト
				SetEffect(pBullet->pos,
					D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f),
					0.0f,
					8.0f,
					0.05f,
					0.0f);

				//エフェクト
				SetEffect(pBullet->pos,
					D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(0.52f, 0.811f, 1.0f, 1.0f),
					0.0f,
					8.0f,
					0.07f,
					0.0f);

				// 画面外チェック
				if (pBullet->pos.z <= -HIT_WALL || pBullet->pos.z >= HIT_WALL ||
					pBullet->pos.x <= -HIT_WALL || pBullet->pos.x >= HIT_WALL)
				{
					//// 爆発アニメーションの配置
					//SetExplosion(pBullet->pos);

					g_nCntEffect++;

					if (g_nCntEffect % 1 == 0)
					{
						for (int g_nCntEffect = 0; g_nCntEffect < 10; g_nCntEffect++)
						{
							// 角度の設定
							float fAngle = ((float)(rand() % 800)) / 100.0f;
							float fmove = (float)(rand() % 1 + 1);

							// エフェクトの設定
							SetEffect(pBullet->pos,
								D3DXVECTOR3(sinf(fAngle) * fmove, 5, cosf(fAngle) * fmove),
								D3DXCOLOR(1.0f, 0.5f, 0.2f, 1.0f),
								1.0f,
								5.0f,
								0.01f,
								0.3f);
						}
					}

					// 弾の状態をfalseにする
					pBullet->bUse = false;
				}
			}
		}


		if (pBullet->type == BULLETTYPE_ENEMY)	//プレイヤーの弾だったら
		{
			if (pBullet->bUse == true)	//弾が使用されていたら
			{
				//位置の更新
				pBullet->pos.x += pBullet->move.x;
				pBullet->pos.z += pBullet->move.z;

				//エフェクト
				SetEffect(pBullet->pos,
					D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 0.6f, 0.2f, 1.0f),
					0.0f,
					10.0f,
					0.05f,
					0.0f);

				//エフェクト
				SetEffect(pBullet->pos,
					D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),
					0.0f,
					8.0f,
					0.05f,
					0.0f);

				// 画面外チェック
				if (pBullet->pos.z <= -HIT_WALL || pBullet->pos.z >= HIT_WALL ||
					pBullet->pos.x <= -HIT_WALL || pBullet->pos.x >= HIT_WALL)
				{
					//// 爆発アニメーションの配置
					//SetExplosion(pBullet->pos);

					g_nCntEffect++;

					if (g_nCntEffect % 1 == 0)
					{
						for (int g_nCntEffect = 0; g_nCntEffect < 10; g_nCntEffect++)
						{
							// 角度の設定
							float fAngle = ((float)(rand() % 800)) / 100.0f;
							float fmove = (float)(rand() % 1 + 1);

							// エフェクトの設定
							SetEffect(pBullet->pos,
								D3DXVECTOR3(sinf(fAngle) * fmove, 5, cosf(fAngle) * fmove),
								D3DXCOLOR(1.0f, 0.5f, 0.2f, 1.0f),
								1.0f,
								5.0f,
								0.01f,
								0.3f);
						}
					}

					// 弾の状態をfalseにする
					pBullet->bUse = false;
				}
			}
		}
		if (pBullet->type == BULLETTYPE_EFFECT)	//エフェクト用
		{

			if (pBullet->bUse == true)	//弾が使用されていたら
			{
				//位置の更新
				pBullet->pos.x += pBullet->move.x;
				pBullet->pos.z += pBullet->move.z;

				//エフェクト
				SetEffect(pBullet->pos,
					D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
					0.0f,
					5.0f,
					0.05f,
					0.0f);

				//エフェクト
				SetEffect(pBullet->pos,
					D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
					0.0f,
					8.0f,
					0.05f,
					0.0f);

				// 画面外チェック
				if (pBullet->pos.z <= -HIT_WALL || pBullet->pos.z >= HIT_WALL ||
					pBullet->pos.x <= -HIT_WALL || pBullet->pos.x >= HIT_WALL)
				{
					// 弾の状態をfalseにする
					pBullet->bUse = false;
				}
			}
		}

		for (int nCntEnemy = 0; nCntEnemy < 1; nCntEnemy++, pEnemy++)
		{
			if (pBullet->type == BULLETTYPE_PLAYER)
			{
				if (pBullet->bUse == true)
				{
					if (pEnemy->bUse == true)
					{
						if (pBullet->pos.x + BULLET_SIZE_P > pEnemy->pos.x - pEnemy->vtxMinEnemy.x &&
							pBullet->pos.x - BULLET_SIZE_P < pEnemy->pos.x + pEnemy->vtxMaxEnemy.x &&
 							pBullet->pos.z + BULLET_SIZE_P > pEnemy->pos.z - pEnemy->vtxMinEnemy.z &&
							pBullet->pos.z - BULLET_SIZE_P < pEnemy->pos.z + pEnemy->vtxMaxEnemy.z)
						{

							if (pPlayer->playertype == PLAYERTYPE_GOLEM)
							{
								HitEnemy(5);					//ゴーレム時
								pBullet->bUse = false;			//使った判定
								PlaySound(SOUND_LABEL_SE_HIT);	//効果音
							}
							if (pPlayer->playertype == PLAYERTYPE_LEO)
							{
								HitEnemy(10);					//レオ選択時
								pBullet->bUse = false;			//使った判定
								PlaySound(SOUND_LABEL_SE_HIT);	//効果音
							}
							if (pPlayer->playertype == PLAYERTYPE_STALKER)
							{
								HitEnemy(3);					//ストーカー選択時
								pBullet->bUse = false;			//使った判定
								PlaySound(SOUND_LABEL_SE_HIT);	//効果音
							}

							g_nCntEffect++;

							if (g_nCntEffect % 10 == 0)
							{
								for (int g_nCntEffect = 0; g_nCntEffect < 10; g_nCntEffect++)
								{
									// 角度の設定
									float fAngle = ((float)(rand() % 800)) / 100.0f;
									float fmove = (float)(rand() % 1 + 1);

									// エフェクトの設定
									SetEffect(pEnemy->pos,
										D3DXVECTOR3(sinf(fAngle) * fmove, 5, cosf(fAngle) * fmove),
										D3DXCOLOR(1.0f, 0.5f, 0.2f, 1.0f),
										1.0f,
										5.0f,
										0.01f,
										0.3f);
								}
							}
						}
					}
				}
			}

			if (pBullet->type == BULLETTYPE_ENEMY)
			{
				if (pBullet->bUse == true)
				{
					if (pPlayer->bUse == true)
					{
						if (pBullet->pos.x + BULLET_SIZE_E > pPlayer->pos.x - pPlayer->minVecPlayer.x &&
							pBullet->pos.x - BULLET_SIZE_E < pPlayer->pos.x + pPlayer->maxVecPlayer.x &&
							pBullet->pos.z + BULLET_SIZE_E > pPlayer->pos.z - pPlayer->minVecPlayer.z &&
							pBullet->pos.z - BULLET_SIZE_E < pPlayer->pos.z + pPlayer->maxVecPlayer.z)
						{
							g_nCntEffect++;

							if (g_nCntEffect % 10 == 0)
							{
								//プレイヤーにダメージ
								HitPlayer(10);

								//効果音
								PlaySound(SOUND_LABEL_SE_HIT);	

								//スコアマイナス
								AddScore(-100);

								//使った判定
								pBullet->bUse = false;

								for (int g_nCntEffect = 0; g_nCntEffect < 10; g_nCntEffect++)
								{
									// 角度の設定
									float fAngle = ((float)(rand() % 800)) / 100.0f;
									float fmove = (float)(rand() % 1 + 1);

									// エフェクトの設定
									SetEffect(pBullet->pos,
										D3DXVECTOR3(sinf(fAngle) * fmove, 5, cosf(fAngle) * fmove),
										D3DXCOLOR(1.0f, 0.5f, 0.2f, 1.0f),
										1.0f,
										5.0f,
										0.01f,
										0.3f);
								}
							}
						}
					}
				}
			}
		}
	}
}

//=============================================================================
// 弾の描画処理
//=============================================================================
void DrawBullet(void)
{
	//変数宣言
	LPDIRECT3DDEVICE9 pDevice;
	D3DXMATRIX mtxView;			//ビューマトリックス取得用
	D3DXMATRIX mtxTrans;		//計算用マトリックス

	//デバイスの取得
	pDevice = GetDevice();

	//Zテスト
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

	//ライティングを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// アルファテスト
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 120);

	for (int nCnt = 0; nCnt < MAX_BULLET; nCnt++)
	{
		if (g_aBullet[nCnt].bUse == true)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aBullet[nCnt].mtxWorld);

			//ビューマトリックスを取得
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//ポリゴンをカメラに対して正面に向ける
			D3DXMatrixInverse(&g_aBullet[nCnt].mtxWorld, NULL, &mtxView);	//逆行列を求める
			g_aBullet[nCnt].mtxWorld._41 = 0.0f;
			g_aBullet[nCnt].mtxWorld._42 = 0.0f;
			g_aBullet[nCnt].mtxWorld._43 = 0.0f;

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aBullet[nCnt].pos.x, g_aBullet[nCnt].pos.y, g_aBullet[nCnt].pos.z);
			D3DXMatrixMultiply(&g_aBullet[nCnt].mtxWorld, &g_aBullet[nCnt].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aBullet[nCnt].mtxWorld);

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_3D));

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureBullet);
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * MAX_VERTEX, 2);
		}
	}

	// アルファテストを元の設定に戻す
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0x00);

	//ライティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	//Zテスト戻す
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
}

//=============================================================================
// 弾の設定
//=============================================================================
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fWidth, float fHeight, BULLETTYPE type)
{
	//変数宣言
	BULLET *pBullet;
	pBullet = &g_aBullet[0];
	VERTEX_3D *pVtx;

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++, pBullet++)
	{
		if (pBullet->bUse == false)
		{
			//位置を設定
			pBullet->pos = pos;

			//移動量を設定
			pBullet->move = move;

			//幅の設定
			pBullet->fWidth = fWidth;

			//高さの設定
			pBullet->fHeight = fHeight;

			//弾の種類
			pBullet->type = type;

			//弾の使用
			pBullet->bUse = true;

			break;
		}
	}
	//頂点バッファをアンロックする
	g_pVtxBuffBullet->Unlock();
}

//=============================================================================
// 弾の位置情報の取得
//=============================================================================
BULLET *GetBullet(void)
{
	return &g_aBullet[0];
}