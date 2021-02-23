//=============================================================================
//
// クロスヘア処理 [Closshair.cpp]
// Author:Taiki Hayasaka
//
//=============================================================================
#include "closshair.h"							//インクルードファイル
#include "input.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MAX_TEX (1)									//テクスチャ最大数
#define MAX_VERTEX (4)								//頂点数

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffClosshair = NULL;	//頂点情報のポインタ
LPDIRECT3DTEXTURE9 g_apTextureClosshair[MAX_TEX];	//テクスチャのポインタ
D3DXCOLOR g_colorClosshair;							//色

//=============================================================================
// クロスヘアのの初期化処理
//=============================================================================
HRESULT InitClosshair(void)
{
	//変数宣言
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVtx;

	//デバイスの取得
	pDevice = GetDevice();

	//初期化
	g_colorClosshair = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/closs.png", &g_apTextureClosshair[0]);

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * MAX_VERTEX * MAX_TEX,	//確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,		//頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffClosshair,
		NULL)))
	{
		return E_FAIL;
	}

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffClosshair->Lock(0, 0, (void**)&pVtx, 0);


	for (int nCntClosshair = 0; nCntClosshair < MAX_TEX; nCntClosshair++)
	{
		//頂点情報の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);				//テクスチャ座標
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		//頂点座標の設定
		SetTextureClosshair(nCntClosshair);

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
	g_pVtxBuffClosshair->Unlock();

	return S_OK;
}

//=============================================================================
// クロスヘアの終了処理
//=============================================================================
void UninitClosshair(void)
{
	//頂点バッファの開放
	if (g_pVtxBuffClosshair != NULL)
	{
		g_pVtxBuffClosshair->Release();
		g_pVtxBuffClosshair = NULL;
	}

	//テクスチャの開放
	for (int nCntTexture = 0; nCntTexture < MAX_TEX; nCntTexture++)
	{
		if (g_apTextureClosshair[nCntTexture] != NULL)
		{
			g_apTextureClosshair[nCntTexture]->Release();
			g_apTextureClosshair[nCntTexture] = NULL;
		}
	}
}

//=============================================================================
// クロスヘアの更新処理
//=============================================================================
void UpdateClosshair(void)
{

}

//=============================================================================
// クロスヘアの描画処理
//=============================================================================
void DrawClosshair(void)
{
	//変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffClosshair, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	for (int nCntTexture = 0; nCntTexture < MAX_TEX; nCntTexture++)
	{
		pDevice->SetTexture(0, g_apTextureClosshair[nCntTexture]);
	}

	//ポリゴンの描画
	for (int nCntTexture = 0; nCntTexture < MAX_TEX; nCntTexture++)
	{
		pDevice->SetTexture(0, g_apTextureClosshair[nCntTexture]);
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTexture * MAX_VERTEX, 2);
	}
}


//=============================================================================
// クロスヘアのテクスチャの設定
//=============================================================================
void SetTextureClosshair(int nCntClosshair)
{
	VERTEX_2D *pVtx;

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffClosshair->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += nCntClosshair * 4;

	if (nCntClosshair == 0)	//ブーストゲージ
	{
		//頂点座標
		pVtx[0].pos = D3DXVECTOR3(920, 180, 0.0f);	//Zは0.0固定
		pVtx[1].pos = D3DXVECTOR3(920, 100, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(1000, 180, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(1000, 100, 0.0f);
	}

	//頂点バッファをアンロックする
	g_pVtxBuffClosshair->Unlock();
}