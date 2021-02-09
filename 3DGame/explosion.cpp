//=============================================================================
//
// 爆発の処理 [explosion.cpp]
// Author:Taiki Hayasaka
//
//=============================================================================
#include "explosion.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MAX_PATTERN     (8)					// アニメーションパターンNo.の最大数
#define TEXTURE_X		(8)					// アニメーションパターンNo.の横の最大数
#define TEXTURE_Y		(1)					// アニメーションパターンNo.の縦の最大数
#define MAX_EXPLOSION	(64)				// 弾の最大値

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffExplosion = NULL;	// 弾のテクスチャのポインタ
LPDIRECT3DTEXTURE9 g_pTextureExplosion = NULL;		// 弾の頂点バッファのポインタ
Explosion g_aExplosion[MAX_EXPLOSION];				// 弾の情報
int g_nCounterAnimE;									//アニメーションカウンター
int g_nPatternAnimE;									//アニメーションパターンNo

//=============================================================================
// 爆発の初期化処理
//=============================================================================
HRESULT InitExplosion(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/explosion001.png", &g_pTextureExplosion);

	// 爆発の情報の初期化
	for (int nCntExprosion = 0; nCntExprosion < MAX_EXPLOSION; nCntExprosion++)
	{
		g_aExplosion[nCntExprosion].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aExplosion[nCntExprosion].nCounterAnimE = 0;
		g_aExplosion[nCntExprosion].nPatternAnimE = 0;
		g_aExplosion[nCntExprosion].bUse = false;
	}

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * 4 * MAX_EXPLOSION, // 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,					// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffExplosion,
		NULL)))
	{
		return E_FAIL;
	}

	// 構造体のポインタ変数
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	for (int nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x - 20.0f, g_aExplosion[nCntExplosion].pos.y + 20.0f, g_aExplosion[nCntExplosion].pos.z);
		pVtx[1].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x + 20.0f, g_aExplosion[nCntExplosion].pos.y + 20.0f, g_aExplosion[nCntExplosion].pos.z);
		pVtx[2].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x - 20.0f, g_aExplosion[nCntExplosion].pos.y - 20.0f, g_aExplosion[nCntExplosion].pos.z);
		pVtx[3].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x + 20.0f, g_aExplosion[nCntExplosion].pos.y - 20.0f, g_aExplosion[nCntExplosion].pos.z);

		// 法線ベクトルの設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		// 頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f);

		// テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f / TEXTURE_Y);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f / TEXTURE_X, 1.0f / TEXTURE_Y);
		pVtx[3].tex = D3DXVECTOR2(1.0f / TEXTURE_X, 0.0f);

		// 弾の情報を次にうつす
		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffExplosion->Unlock();

	return S_OK;
}

//=============================================================================
// 爆発の終了処理
//=============================================================================
void UninitExplosion(void)
{
	// 頂点バッファの開放
	if (g_pVtxBuffExplosion != NULL)
	{
		g_pVtxBuffExplosion->Release();
		g_pVtxBuffExplosion = NULL;
	}

	// テクスチャの開放
	if (g_pTextureExplosion != NULL)
	{
		g_pTextureExplosion->Release();
		g_pTextureExplosion = NULL;
	}
}

//=============================================================================
// 爆発の更新処理
//=============================================================================
void UpdateExplosion(void)
{
	g_nCounterAnimE++;		//アニメーションカウンター更新
	Explosion *pExplosion;
	VERTEX_3D *pVtx;

	pExplosion = &g_aExplosion[0];

	if ((g_nCounterAnimE % 5) == 0)
	{
		//アニメーションパターンNoを更新
		g_nPatternAnimE = (g_nPatternAnimE + 1) % 8;
	}

	if (g_pVtxBuffExplosion != NULL)
	{
		//頂点バッファをロックし頂点情報へのポインタを取得
		g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

		//テクスチャ座標を更新
		for (int nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
		{
			g_aExplosion[nCntExplosion].nCounterAnimE++;

			if (g_aExplosion[nCntExplosion].bUse == true)
			{
				if (g_aExplosion[nCntExplosion].nCounterAnimE > 4)
				{
					g_aExplosion[nCntExplosion].nCounterAnimE = 0;						//爆発アニメーション
					g_aExplosion[nCntExplosion].nPatternAnimE++;

					//頂点情報の設定
					pVtx[0].tex = D3DXVECTOR2(0.0f + (0.125f*g_nPatternAnimE), 1.0f);	//テクスチャ座標
					pVtx[1].tex = D3DXVECTOR2(0.0f + (0.125f*g_nPatternAnimE), 0.0f);	//アニメーション
					pVtx[2].tex = D3DXVECTOR2(0.125f + (0.125f*g_nPatternAnimE), 1.0f);
					pVtx[3].tex = D3DXVECTOR2(0.125f + (0.125f*g_nPatternAnimE), 0.0f);

					if (g_aExplosion[nCntExplosion].nPatternAnimE == 8)
					{
						g_aExplosion[nCntExplosion].nPatternAnimE = 0;
						g_aExplosion[nCntExplosion].bUse = false;						//使ったら捨てる
					}
				}
			}
			//４つの頂点を進める
			pVtx += 4;
		}
		//頂点バッファをアンロックする
		g_pVtxBuffExplosion->Unlock();
	}
}

//=============================================================================
// 爆発の描画処理
//=============================================================================
void DrawExplosion(void)
{
	// 変数宣言
	Explosion *pExplosion = &g_aExplosion[0];
	LPDIRECT3DDEVICE9 pDevice;
	D3DXMATRIX mtxView;							// ビューマトリックス取得用
	D3DXMATRIX mtxScaling, mtxTrans;			// 計算用マトリックス

	// 変数の受け渡し
	pDevice = GetDevice();

	// ライトをオフにする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// Zバッファを適用
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	for (int nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++, pExplosion++)
	{
		if (pExplosion->bUse == true)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&pExplosion->mtxWorld);

			// ビューマトリックスの取得
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			// ポリゴンをカメラに対して正面に向ける
			D3DXMatrixInverse(&g_aExplosion->mtxWorld, NULL, &mtxView);
			g_aExplosion->mtxWorld._41 = 0.0f;
			g_aExplosion->mtxWorld._42 = 0.0f;
			g_aExplosion->mtxWorld._43 = 0.0f;

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, pExplosion->pos.x, pExplosion->pos.y, pExplosion->pos.z);
			D3DXMatrixMultiply(&pExplosion->mtxWorld, &pExplosion->mtxWorld, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &pExplosion->mtxWorld);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffExplosion, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// ポリゴンの描画
			pDevice->SetTexture(0, g_pTextureExplosion);
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntExplosion * 4, 2);
		}
	}
	// 標準に戻す
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// ライトをオンにする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//=============================================================================
// 爆発の設定
//=============================================================================
void SetExplosion(D3DXVECTOR3 pos)
{
	//構造体のポインタ
	Explosion *pExprosion = &g_aExplosion[0];

	//爆発の設定
	for (int nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++, pExprosion++)
	{
		if (pExprosion->bUse == false)
		{
			//位置の設定
			pExprosion->pos = pos;

			//使用しているかどうか
			pExprosion->bUse = true;

			break;
		}
	}
}
