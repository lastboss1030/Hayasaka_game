//=============================================================================
//
// 爆発処理 [explosion.cpp]
// Author:Taiki Hayasaka
//
//=============================================================================
#include "explosion.h"									//インクルードファイル

//=============================================================================
// マクロ定義
//=============================================================================
#define MAX_EXPLOSION (256)								//爆発最大数

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DTEXTURE9 g_pTextureExplosion = NULL;			//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffExplosion = NULL; 	//頂点バッファへのポインタ
int g_nCounterAnim;										//アニメーションカウンター
int g_nPatternAnim;										//アニメーションパターンNo
EXPLOSION g_aExplosion[MAX_EXPLOSION];					//爆発の情報
D3DXVECTOR3 g_posExplosion;								//爆発の位置
D3DXVECTOR3 g_rotExplosion;								//爆発の回転角
D3DXVECTOR3 g_moveExplosion;							//爆発の移動量
float g_fLengthExplosion;								//爆発の対角線の長さ
float g_fAngleExplosion;								//爆発の対角線の角度

//=============================================================================
// 爆発の初期化処理
//=============================================================================
HRESULT InitExplosion(void)
{
	//変数宣言
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVtx;

	//デバイスの取得
	pDevice = GetDevice();

	//爆発の初期化
	for (int nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		g_aExplosion[nCntExplosion].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aExplosion[nCntExplosion].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		g_aExplosion[nCntExplosion].nCounterAnim = 0;
		g_aExplosion[nCntExplosion].nPattenAnim = 0;
		g_aExplosion[nCntExplosion].bUse = false;
	}

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/explosion000.png", &g_pTextureExplosion);

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_EXPLOSION,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffExplosion,
		NULL)))
	{
		return E_FAIL;
	}

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	//対角線の長さ
	g_fLengthExplosion = sqrtf(50 * 50 + 60 * 60);

	//対角線の角度
	g_fAngleExplosion = atan2f(50, 60);

	//初期位置
	g_posExplosion = D3DXVECTOR3(200, 200, 0.0f);

	for (int nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		//頂点情報の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);		//テクスチャ座標
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.125f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.125f, 0.0f);

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
		pVtx[0].col = D3DXCOLOR(1.0f, 0.5f, 0.0f, 0.5f);
		pVtx[1].col = D3DXCOLOR(1.0f, 0.5f, 0.0f, 0.5f);
		pVtx[2].col = D3DXCOLOR(1.0f, 0.5f, 0.0f, 0.5f);
		pVtx[3].col = D3DXCOLOR(1.0f, 0.5f, 0.0f, 0.5f);

		//４つの頂点を進める
		pVtx += 4;
	}
	//頂点バッファをアンロックする
	g_pVtxBuffExplosion->Unlock();

	return S_OK;
}

//=============================================================================
// 爆発の終了処理
//=============================================================================
void UninitExplosion(void)
{
	//頂点バッファの開放
	if (g_pVtxBuffExplosion != NULL)
	{
		g_pVtxBuffExplosion->Release();
		g_pVtxBuffExplosion = NULL;
	}

	//テクスチャの開放
	if (g_pTextureExplosion != NULL)
	{
		g_pTextureExplosion -> Release();
		g_pTextureExplosion = NULL;
	}
}

//=============================================================================
// 爆発の更新処理
//=============================================================================
void UpdateExplosion(void)
{
	g_nCounterAnim++;		//アニメーションカウンター更新
	EXPLOSION *pExplosion;
	VERTEX_2D *pVtx;

	pExplosion = &g_aExplosion[0];

	if ((g_nCounterAnim % 4) == 0)
	{
		//アニメーションパターンNoを更新
		g_nPatternAnim = (g_nPatternAnim + 1) % 8;
	}

	if (g_pVtxBuffExplosion != NULL)
	{
		//頂点バッファをロックし頂点情報へのポインタを取得
		g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

		//テクスチャ座標を更新
		for (int nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
		{
			g_aExplosion[nCntExplosion].nCounterAnim++;

			if (g_aExplosion[nCntExplosion].bUse == true)
			{
				if (g_aExplosion[nCntExplosion].nCounterAnim > 6)
				{
					g_aExplosion[nCntExplosion].nCounterAnim = 0;						//爆発アニメーション
					g_aExplosion[nCntExplosion].nPattenAnim++;

					//頂点情報の設定
					pVtx[0].tex = D3DXVECTOR2(0.0f + (0.125f*g_nPatternAnim), 1.0f);	//テクスチャ座標
					pVtx[1].tex = D3DXVECTOR2(0.0f + (0.125f*g_nPatternAnim), 0.0f);	//アニメーション
					pVtx[2].tex = D3DXVECTOR2(0.125f + (0.125f*g_nPatternAnim), 1.0f);
					pVtx[3].tex = D3DXVECTOR2(0.125f + (0.125f*g_nPatternAnim), 0.0f);

					if (g_aExplosion[nCntExplosion].nPattenAnim == 8)
					{
						g_aExplosion[nCntExplosion].nPattenAnim = 0;
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
	//変数宣言
	LPDIRECT3DDEVICE9 pDevice;
	int nCntVertex = 0;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffExplosion, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureExplosion);

	for (int nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		if (g_aExplosion[nCntExplosion].bUse == true)
		{
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntVertex, 2);
		}
		nCntVertex += 4;
	}
}

//=============================================================================
// 爆発の設定
//=============================================================================
void SetExplosion(D3DXVECTOR3 pos, D3DXCOLOR col)
{
	//変数宣言
	VERTEX_2D *pVtx;

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		if (g_aExplosion[nCntExplosion].bUse == false)
		{
			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(pos.x - 40.0f, pos.y + 40.0f, 0.0f);	//Zは0.0固定
			pVtx[1].pos = D3DXVECTOR3(pos.x - 40.0f, pos.y - 40.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(pos.x + 40.0f, pos.y + 40.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(pos.x + 40.0f, pos.y - 40.0f, 0.0f);

			g_aExplosion[nCntExplosion].pos = pos;
			g_aExplosion[nCntExplosion].col = col;
			g_aExplosion[nCntExplosion].nCounterAnim = 0;
			g_aExplosion[nCntExplosion].nPattenAnim = 0;
			g_aExplosion[nCntExplosion].bUse = true;

			break;
		}
		//４つの頂点を進める
		pVtx += 4;
	}
	//頂点バッファをアンロックする
	g_pVtxBuffExplosion->Unlock();
}