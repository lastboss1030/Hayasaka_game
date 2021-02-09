//=============================================================================
//
// 選択画面の処理 [select.cpp]
// Author:Taiki Hayasaka
//
//=============================================================================
#include "select.h"								//インクルードファイル
#include "input.h"
#include "fade.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MAX_TEX (3)									//テクスチャ最大数

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffSelect = NULL;	//頂点情報のポインタ
LPDIRECT3DTEXTURE9 g_apTextureSelect[MAX_TEX];	//テクスチャのポインタ
D3DXCOLOR g_colorSelect;							//色
int g_nCounterAnimS;								//アニメーションカウンター
int g_nPatternAnimS;								//アニメーションパターンNo
float g_nCntEnterS;

//=============================================================================
// 選択画面の初期化処理
//=============================================================================
HRESULT InitSelect(void)
{
	//変数宣言
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVtx;

	//デバイスの取得
	pDevice = GetDevice();

	//初期化
	g_colorSelect = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_nCntEnterS = 0;
	//アニメーションの初期化
	g_nCounterAnimS = 0;
	g_nPatternAnimS = 0;

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Select_BG.png", &g_apTextureSelect[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/press_enter001.png", &g_apTextureSelect[1]);

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_TEX,	//確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,		//頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffSelect,
		NULL)))
	{
		return E_FAIL;
	}

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffSelect->Lock(0, 0, (void**)&pVtx, 0);


	for (int nCntSelect = 0; nCntSelect < MAX_TEX; nCntSelect++)
	{
		//頂点情報の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);				//テクスチャ座標
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		//頂点座標の設定
		SetTextureSelect(nCntSelect);

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
	g_pVtxBuffSelect->Unlock();

	return S_OK;
}

//=============================================================================
// 選択画面の終了処理
//=============================================================================
void UninitSelect(void)
{
	//頂点バッファの開放
	if (g_pVtxBuffSelect != NULL)
	{
		g_pVtxBuffSelect->Release();
		g_pVtxBuffSelect = NULL;
	}

	//テクスチャの開放
	for (int nCntTexture = 0; nCntTexture < MAX_TEX; nCntTexture++)
	{
		if (g_apTextureSelect[nCntTexture] != NULL)
		{
			g_apTextureSelect[nCntTexture]->Release();
			g_apTextureSelect[nCntTexture] = NULL;
		}
	}
}

//=============================================================================
// 選択画面の更新処理
//=============================================================================
void UpdateSelect(void)
{
	//情報の取得
	int nFade = GetFade();
	g_nCounterAnimS++;		//アニメーションカウンター更新
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffSelect->Lock(0, 0, (void**)&pVtx, 0);

	if ((g_nCounterAnimS % 20) == 0)
	{
		//アニメーションパターンを更新
		g_nPatternAnimS = (g_nPatternAnimS + 1) % 2;
	}

	//テクスチャ座標を更新
	pVtx[8].tex = D3DXVECTOR2(0.0f + (0.5f*g_nPatternAnimS), 1.0f);		//テクスチャ座標
	pVtx[9].tex = D3DXVECTOR2(0.0f + (0.5f*g_nPatternAnimS), 0.0f);		//アニメーション
	pVtx[10].tex = D3DXVECTOR2(0.5f + (0.5f*g_nPatternAnimS), 1.0f);
	pVtx[11].tex = D3DXVECTOR2(0.5f + (0.5f*g_nPatternAnimS), 0.0f);

	//PRESS_ENTERの色更新
	pVtx[4].col = g_colorSelect;
	pVtx[5].col = g_colorSelect;
	pVtx[6].col = g_colorSelect;
	pVtx[7].col = g_colorSelect;

	//頂点バッファをアンロックする
	g_pVtxBuffSelect->Unlock();

	//PRESS_ENTERの点滅
	if (nFade == FADE_NONE)
	{
		g_nCntEnterS++;

		if (g_nCntEnterS == 20)
		{
			g_colorSelect.a = 1.0f;
		}
		else if (g_nCntEnterS == 40)
		{
			g_colorSelect.a = 0.0f;
			g_nCntEnterS = 0;
		}
	}

	//エンター押したら
	if (GetKeyboardTrigger(DIK_RETURN) == true)
	{
		if (nFade == FADE_NONE)
		{
			//効果音

			SetFade(FADE_OUT, MODE_GAME);	//ゲーム画面に切り替え
		}
	}
}

//=============================================================================
// 選択画面の描画処理
//=============================================================================
void DrawSelect(void)
{
	//変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffSelect, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	for (int nCntTexture = 0; nCntTexture < MAX_TEX; nCntTexture++)
	{
		pDevice->SetTexture(0, g_apTextureSelect[nCntTexture]);
	}

	//ポリゴンの描画
	for (int nCntTexture = 0; nCntTexture < MAX_TEX; nCntTexture++)
	{
		pDevice->SetTexture(0, g_apTextureSelect[nCntTexture]);
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTexture * 4, 2);
	}
}


//=============================================================================
// 選択画面のテクスチャの設定
//=============================================================================
void SetTextureSelect(int nCntSelect)
{
	VERTEX_2D *pVtx;

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffSelect->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += nCntSelect * 4;

	if (nCntSelect == 0)	//背景
	{
		//頂点座標
		pVtx[0].pos = D3DXVECTOR3(0, SCREEN_HEIGHT, 0.0f);	//Zは0.0固定
		pVtx[1].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, 0, 0.0f);
	}
	else if (nCntSelect == 1)	//PRESS_ENTER
	{
		//頂点座標
		pVtx[0].pos = D3DXVECTOR3(850, 200, 0.0f);	//Zは0.0固定
		pVtx[1].pos = D3DXVECTOR3(850, 50, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(1570, 200, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(1570, 50, 0.0f);
	}

	//頂点バッファをアンロックする
	g_pVtxBuffSelect->Unlock();
}