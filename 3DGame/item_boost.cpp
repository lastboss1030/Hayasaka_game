//=============================================================================
//
// ブーストアイテムの処理 [item_boost.cpp]
// Author:Taiki Hayasaka
//
//=============================================================================
#include "item_boost.h"
#include "player.h"
#include "camera.h"
#include "shadow.h"
#include "input.h"
#include "effect.h"
#include "Sound.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MAX_ITEMBOOST	(8)

//=============================================================================
// グローバル変数
//=============================================================================
ItemBoost g_aItemBoost[MAX_ITEMBOOST];							// モデル情報
D3DXVECTOR3 g_vtxMinItemBoost, g_vtxMaxItemBoost;				// 各頂点座標の最小値、最大値
LPDIRECT3DTEXTURE9 g_apTextureItemBoost[MAX_ITEMBOOST] = {};	// テクスチャへのポインタ
int g_nCntEffectGet = 0;

//=============================================================================
// ブーストアイテムの初期化処理
//=============================================================================
HRESULT InitItemBoost(void)
{
	//変数宣言
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();

	for (int nCnt = 0; nCnt < MAX_ITEMBOOST; nCnt++)
	{
		//初期化
		g_aItemBoost[nCnt].pMeshItemBoost = NULL;
		g_aItemBoost[nCnt].pBuffMatItemBoost = NULL;
		g_aItemBoost[nCnt].nNumMatItemBoost = NULL;
		g_aItemBoost[nCnt].pos = D3DXVECTOR3(50.0f, 10.0f, 0.0f);
		g_aItemBoost[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aItemBoost[nCnt].rotDest = g_aItemBoost[nCnt].rot;
		g_aItemBoost[nCnt].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aItemBoost[nCnt].moverot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aItemBoost[nCnt].nIdx = -1;
		g_aItemBoost[nCnt].bUse = false;

		//Xファイルの読み込み
		if (FAILED(D3DXLoadMeshFromX("data/MODEL/healdrone.x",
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_aItemBoost[nCnt].pBuffMatItemBoost,
			NULL,
			&g_aItemBoost[nCnt].nNumMatItemBoost,
			&g_aItemBoost[nCnt].pMeshItemBoost)))
		{
			return E_FAIL;
		}
	}
	//アイテムの設置
	SetItemBoost(D3DXVECTOR3(200.0f, 0.0f, 0.0f));
	SetItemBoost(D3DXVECTOR3(-200.0f, 0.0f, 0.0f));
	SetItemBoost(D3DXVECTOR3(0.0f, 0.0f, 200.0f));
	SetItemBoost(D3DXVECTOR3(0.0f, 0.0f, -200.0f));
	SetItemBoost(D3DXVECTOR3(100.0f, 0.0f, 100.0f));
	SetItemBoost(D3DXVECTOR3(-100.0f, 0.0f, 100.0f));
	SetItemBoost(D3DXVECTOR3(100.0f, 0.0f, -100.0f));
	SetItemBoost(D3DXVECTOR3(-100.0f, 0.0f, -100.0f));

	return S_OK;
}

//=============================================================================
// ブーストアイテムの終了処理
//=============================================================================
void UninitItemBoost(void)
{
	for (int nCnt = 0; nCnt < MAX_ITEMBOOST; nCnt++)
	{
		//メッシュの破棄
		if (g_aItemBoost[nCnt].pBuffMatItemBoost != NULL)
		{
			g_aItemBoost[nCnt].pBuffMatItemBoost->Release();
			g_aItemBoost[nCnt].pBuffMatItemBoost = NULL;
		}

		//マテリアルの破棄
		if (g_aItemBoost[nCnt].pMeshItemBoost != NULL)
		{
			g_aItemBoost[nCnt].pMeshItemBoost->Release();
			g_aItemBoost[nCnt].pMeshItemBoost = NULL;
		}
	}
}

//=============================================================================
// ブーストアイテムの更新処理
//=============================================================================
void UpdateItemBoost(void)
{
	Player *pPlayer = GetPlayer();
	ItemBoost *pItemBoost = &g_aItemBoost[0];

	// 変数宣言
	int nCntTrue = 0;

	for (int nCntItemBoost = 0; nCntItemBoost < MAX_ITEMBOOST; nCntItemBoost++, pItemBoost++)
	{
		if (pItemBoost->bUse == true)
		{
			//プレイヤーが当たったら消す
			if ((pPlayer->pos.x - pPlayer->minVecPlayer.x) < (pItemBoost->pos.x + pItemBoost->vtxMaxItemBoost.x) &&
				(pPlayer->pos.x + pPlayer->maxVecPlayer.x) > (pItemBoost->pos.x - pItemBoost->vtxMinItemBoost.x) &&
				(pPlayer->pos.z - pPlayer->minVecPlayer.z) < (pItemBoost->pos.z + pItemBoost->vtxMaxItemBoost.z) &&
				(pPlayer->pos.z + pPlayer->maxVecPlayer.z) > (pItemBoost->pos.z - pItemBoost->vtxMinItemBoost.z))
			{
				if (pPlayer->boost < 200)
				{
					//ブースト残量増加
					pPlayer->boost = 200;

					//効果音
					PlaySound(SOUND_LABEL_SE_ITEM);

					//使った判定
					pItemBoost->bUse = false;

					//影を消す
					DeleteShadow(pItemBoost->nIdx);

					g_nCntEffectGet++;

					if (g_nCntEffectGet % 1 == 0)
					{
						for (int g_nCntEffect = 0; g_nCntEffect < 10; g_nCntEffect++)
						{
							//角度の設定
							float fAngle = ((float)(rand() % 800)) / 100.0f;
							float fmove = (float)(rand() % 1 + 1);

							//エフェクトの設定
							SetEffect(pItemBoost->pos,
								D3DXVECTOR3(sinf(fAngle) * fmove, 5, cosf(fAngle) * fmove),
								D3DXCOLOR(0.0f, 0.5f, 1.0f, 1.0f),
								1.0f,
								5.0f,
								0.01f,
								0.2f);
						}
					}
				}
			}
			//回転させる
			pItemBoost->rot.y += 0.1f;

			//使われているカウント
			nCntTrue++;
		}
	}
}

//=============================================================================
//ブーストアイテムの描画処理
//=============================================================================
void DrawItemBoost(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;		//デバイスへのポインタ
	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス
	D3DMATERIAL9 matDef;			//現在のマテリアル保存用
	D3DXMATERIAL *pMat;				//マテリアルデータへのポインタ
	pDevice = GetDevice();

	for (int nCntItemBoost = 0; nCntItemBoost < MAX_ITEMBOOST; nCntItemBoost++)
	{
		if (g_aItemBoost[nCntItemBoost].bUse == true)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aItemBoost[nCntItemBoost].mtxWorld);

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aItemBoost[nCntItemBoost].rot.y, g_aItemBoost[nCntItemBoost].rot.x, g_aItemBoost[nCntItemBoost].rot.z);
			D3DXMatrixMultiply(&g_aItemBoost[nCntItemBoost].mtxWorld, &g_aItemBoost[nCntItemBoost].mtxWorld, &mtxRot);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aItemBoost[nCntItemBoost].pos.x, g_aItemBoost[nCntItemBoost].pos.y, g_aItemBoost[nCntItemBoost].pos.z);
			D3DXMatrixMultiply(&g_aItemBoost[nCntItemBoost].mtxWorld, &g_aItemBoost[nCntItemBoost].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aItemBoost[nCntItemBoost].mtxWorld);

			//現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			//マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_aItemBoost[nCntItemBoost].pBuffMatItemBoost->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_aItemBoost[nCntItemBoost].nNumMatItemBoost; nCntMat++)
			{
				//マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				//テクスチャの設定
				pDevice->SetTexture(0, g_apTextureItemBoost[nCntMat]);

				//モデル(パーツ)の描画
				g_aItemBoost[nCntItemBoost].pMeshItemBoost->DrawSubset(nCntMat);
			}
			//保存していたマテリアルを戻す
			pDevice->SetMaterial(&matDef);
		}
	}
}

//=============================================================================
// ブーストアイテムの設置
//=============================================================================
void SetItemBoost(D3DXVECTOR3 pos)
{
	ItemBoost *pItemBoost = &g_aItemBoost[0];

	for (int nCntItemBoost = 0; nCntItemBoost < MAX_ITEMBOOST; nCntItemBoost++)
	{
		if (g_aItemBoost[nCntItemBoost].bUse == false)
		{
			//アイテムの頂点
			VecItemBoost(nCntItemBoost);

			//座標の設定
			g_aItemBoost[nCntItemBoost].pos = pos;

			//使っているかどうか
			g_aItemBoost[nCntItemBoost].bUse = true;

			//影の配置
			g_aItemBoost[nCntItemBoost].nIdx = SetShadow(D3DXVECTOR3(g_aItemBoost[nCntItemBoost].pos.x, 0.0f, g_aItemBoost[nCntItemBoost].pos.z), 20.0f, 20.0f);

			break;
		}
	}
}

//=============================================================================
// ブーストアイテムの頂点座標
//=============================================================================
void VecItemBoost(int nCntItemBoost)
{
	//変数宣言
	int nNumVtx;			//頂点数
	DWORD sizeFVF;			//頂点フォーマットのサイズ
	BYTE *pVtxBuff;			//頂点バッファへのポインタ

	//頂点数の取得
	nNumVtx = g_aItemBoost[nCntItemBoost].pMeshItemBoost->GetNumVertices();

	//頂点フォーマットのサイズを取得
	sizeFVF = D3DXGetFVFVertexSize(g_aItemBoost[nCntItemBoost].pMeshItemBoost->GetFVF());

	//頂点バッファのロック
	g_aItemBoost[nCntItemBoost].pMeshItemBoost->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		//頂点座標の代入
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

		//全ての頂点を比較しモデルの最小値、最大値を抜き出す
		if (g_aItemBoost[nCntItemBoost].vtxMinItemBoost.x > vtx.x)
		{
			g_aItemBoost[nCntItemBoost].vtxMinItemBoost.x = vtx.x;
		}
		else if (g_aItemBoost[nCntItemBoost].vtxMaxItemBoost.x < vtx.x)
		{
			g_aItemBoost[nCntItemBoost].vtxMaxItemBoost.x = vtx.x;
		}

		if (g_aItemBoost[nCntItemBoost].vtxMinItemBoost.y > vtx.y)
		{
			g_aItemBoost[nCntItemBoost].vtxMinItemBoost.y = vtx.y;
		}
		else if (g_aItemBoost[nCntItemBoost].vtxMaxItemBoost.y < vtx.y)
		{
			g_aItemBoost[nCntItemBoost].vtxMaxItemBoost.y = vtx.y;
		}

		if (g_aItemBoost[nCntItemBoost].vtxMinItemBoost.z > vtx.z)
		{
			g_aItemBoost[nCntItemBoost].vtxMinItemBoost.z = vtx.z;
		}
		else if (g_aItemBoost[nCntItemBoost].vtxMaxItemBoost.z < vtx.z)
		{
			g_aItemBoost[nCntItemBoost].vtxMaxItemBoost.z = vtx.z;
		}
		
		pVtxBuff += sizeFVF;
	}
	//頂点バッファのアンロック
	g_aItemBoost[0].pMeshItemBoost->UnlockVertexBuffer();

	//0.0fより小さいとき-1.0倍する
	if (g_aItemBoost[nCntItemBoost].vtxMaxItemBoost.x < 0.0f)
	{
		g_aItemBoost[nCntItemBoost].vtxMaxItemBoost.x *= -1.0f;
	{
	}
	if (g_aItemBoost[nCntItemBoost].vtxMaxItemBoost.y < 0.0f)
		g_aItemBoost[nCntItemBoost].vtxMaxItemBoost.y *= -1.0f;
	}
	if (g_aItemBoost[nCntItemBoost].vtxMaxItemBoost.z < 0.0f)
	{
		g_aItemBoost[nCntItemBoost].vtxMaxItemBoost.z *= -1.0f;
	}
	if (g_aItemBoost[nCntItemBoost].vtxMaxItemBoost.x < 0.0f)
	{
		g_aItemBoost[nCntItemBoost].vtxMaxItemBoost.x *= -1.0f;
	}
	if (g_aItemBoost[nCntItemBoost].vtxMaxItemBoost.y < 0.0f)
	{
		g_aItemBoost[nCntItemBoost].vtxMaxItemBoost.y *= -1.0f;
	}
	if (g_aItemBoost[nCntItemBoost].vtxMaxItemBoost.z < 0.0f)
	{
		g_aItemBoost[nCntItemBoost].vtxMaxItemBoost.z *= -1.0f;
	}
}