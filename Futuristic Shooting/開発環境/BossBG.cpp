//=============================================================================
//
// ボス背景処理 [BossBG.cpp]
// Author:Taiki Hayasaka
//
//=============================================================================
#include "BossBG.h"									//インクルードファイル

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBossBG = NULL;	//頂点情報のポインタ
LPDIRECT3DTEXTURE9 g_apTextureBossBG;				//テクスチャのポインタ

//=============================================================================
// ボス背景の初期化処理
//=============================================================================
HRESULT InitBossBG(void)
{
	//変数宣言
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVtx;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Boss_BG000.jpg", &g_apTextureBossBG);

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,	//確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,		//頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffBossBG,
		NULL)))
	{
		return E_FAIL;
	}

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffBossBG->Lock(0, 0, (void**)&pVtx, 0);

	//頂点情報の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);				//テクスチャ座標
	pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(0, SCREEN_HEIGHT, 0.0f);	//Zは0.0固定
	pVtx[1].pos = D3DXVECTOR3(0, 0, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, 0, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 200);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 200);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 200);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 200);

	//頂点バッファをアンロックする
	g_pVtxBuffBossBG->Unlock();

	return S_OK;
}

//=============================================================================
// ボス背景の終了処理
//=============================================================================
void UninitBossBG(void)
{
	//頂点バッファの開放
	if (g_pVtxBuffBossBG != NULL)
	{
		g_pVtxBuffBossBG->Release();
		g_pVtxBuffBossBG = NULL;
	}

	//テクスチャの開放
	if (g_apTextureBossBG != NULL)
	{
		g_apTextureBossBG->Release();
		g_apTextureBossBG = NULL;
	}
}

//=============================================================================
// ボス背景の更新処理
//=============================================================================
void UpdateBossBG(void)
{

}

//=============================================================================
// ボス背景の描画処理
//=============================================================================
void DrawBossBG(void)
{
	//変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBossBG, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_apTextureBossBG);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}