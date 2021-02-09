//=============================================================================
//
// フェードの処理 [fade.cpp]
// Author:Taiki Hayasaka
//
//=============================================================================
#include "fade.h"								//インクルードファイル

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffFade = NULL;	//頂点情報へのポインタ
LPDIRECT3DTEXTURE9 g_pTextureFade = NULL;		//ポリゴンのテクスチャへのポインタ
FADE g_fade;									//フェードの状態
MODE g_modeNext;								//次のモード(画面)
D3DXCOLOR g_colorFade;							//フェード色

//=============================================================================
// フェードの初期化処理
//=============================================================================
HRESULT InitFade(MODE modeNext)
{
	//変数宣言
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVtx;

	//デバイスの取得
	pDevice = GetDevice();

	//初期化
	g_fade = FADE_IN;
	g_modeNext = modeNext;
	g_colorFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,	//確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,		//頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffFade,
		NULL)))
	{
		return E_FAIL;
	}

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);

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

	// 頂点カラーの設定
	pVtx[0].col = g_colorFade;
	pVtx[1].col = g_colorFade;
	pVtx[2].col = g_colorFade;
	pVtx[3].col = g_colorFade;

	//頂点バッファをアンロックする
	g_pVtxBuffFade->Unlock();

	return S_OK;
}

//=============================================================================
// フェードの終了処理
//=============================================================================
void UninitFade(void)
{
	//頂点バッファの開放
	if (g_pVtxBuffFade != NULL)
	{
		g_pVtxBuffFade->Release();
		g_pVtxBuffFade = NULL;
	}
}

//=============================================================================
// フェードの更新処理
//=============================================================================
void UpdateFade(void)
{
	//変数宣言
	VERTEX_2D *pVtx;

	//フェードの処理
	if (g_fade == FADE_IN)			//フェードイン
	{
		g_colorFade.a -= 0.03f;

		if (g_colorFade.a <= 0.0f)	//0.0fになったら何もしない
		{
			g_colorFade.a = 0.0f;
			g_fade = FADE_NONE;
		}
	}
	else if (g_fade == FADE_OUT)	//フェードアウト
	{
		g_colorFade.a += 0.03f;

		if (g_colorFade.a >= 1.0f)	//1.0fになったらフェードイン
		{
			g_colorFade.a = 1.0f;
			g_fade = FADE_IN;

			SetMode(g_modeNext);
		}
	}
	else if (g_fade == FADE_NONE)	//何もしてないとき
	{
		g_colorFade.a = 0.0f;
	}

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点カラーの設定
	pVtx[0].col = g_colorFade;
	pVtx[1].col = g_colorFade;
	pVtx[2].col = g_colorFade;
	pVtx[3].col = g_colorFade;

	// 頂点バッファをアンロックする
	g_pVtxBuffFade->Unlock();
}

//=============================================================================
// フェードの描画処理
//=============================================================================
void DrawFade(void)
{
	//変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffFade, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, NULL);	//使わない場合NULL

									//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}
//=============================================================================
// フェードの設定
//=============================================================================
void SetFade(FADE fade, MODE modeNext)
{
	g_fade = fade;
	g_modeNext = modeNext;
	g_colorFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
}

//=============================================================================
// フェードの取得
//=============================================================================
FADE GetFade(void)
{
	return g_fade;
}