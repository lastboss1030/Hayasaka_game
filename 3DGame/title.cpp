//=============================================================================
//
// タイトルの処理 [title.cpp]
// Author:Taiki Hayasaka
//
//=============================================================================
#include "title.h"								//インクルードファイル
#include "input.h"
#include "fade.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MAX_TITLE_TEX (4)

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DTEXTURE9 g_pTextureTitle[MAX_TITLE_TEX] = {};	//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitle = NULL;			//頂点バッファへのポインタ
D3DXVECTOR3 g_posTitleLogo;								//タイトルロゴ座標
D3DXCOLOR g_color;										//色
float g_nCount;											//カウント
float g_nCountFade;										//自動フェードカウント
int g_nCntAnimTitle;									//アニメーションカウンター
float g_fPatternAnimT[MAX_TITLE_TEX];					//アニメーションパターン
float g_fMoveTitleX[MAX_TITLE_TEX];						//移動量X
float g_fMoveTitleY[MAX_TITLE_TEX];						//移動量Y

//=============================================================================
// タイトルの初期化処理
//=============================================================================
HRESULT InitTitle(void)
{
	//変数宣言
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVtx;

	//デバイスの取得
	pDevice = GetDevice();

	//カラーの初期
	g_color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_nCount = 0;

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/title.png", &g_pTextureTitle[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/press_enter001.png", &g_pTextureTitle[1]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/gameTitle.png", &g_pTextureTitle[2]);

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_TITLE_TEX,	//確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,		//頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffTitle,
		NULL)))
	{
		return E_FAIL;
	}

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTitle = 0; nCntTitle < MAX_TITLE_TEX; nCntTitle++)
	{
		//頂点情報の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);			//テクスチャ座標
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		//頂点座標の設定
		SetTextureTitle(nCntTitle);

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
	g_pVtxBuffTitle->Unlock();

	return S_OK;
}

//=============================================================================
// タイトルの終了処理
//=============================================================================
void UninitTitle(void)
{
	//頂点バッファの開放
	if (g_pVtxBuffTitle != NULL)
	{
		g_pVtxBuffTitle->Release();
		g_pVtxBuffTitle = NULL;
	}

	//テクスチャの開放
	for (int nCntTexture = 0; nCntTexture < MAX_TITLE_TEX; nCntTexture++)
	{
		if (g_pTextureTitle[nCntTexture] != NULL)
		{
			g_pTextureTitle[nCntTexture]->Release();
			g_pTextureTitle[nCntTexture] = NULL;
		}
	}
}

//=============================================================================
// タイトルの更新処理
//=============================================================================
void UpdateTitle(void)
{
	VERTEX_2D *pVtx;

	//情報の取得
	int nFade = GetFade();

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	//PRESS_ENTERの色更新
	pVtx[4].col = g_color;
	pVtx[5].col = g_color;
	pVtx[6].col = g_color;
	pVtx[7].col = g_color;

	// テクスチャの更新
	if ((g_nCntAnimTitle % 4) == 0)
	{
		// テクスチャのパターンの更新
		g_fPatternAnimT[0] += 0.0001f;

		// テクスチャの頂点座標の更新
		pVtx[0].tex = D3DXVECTOR2(0.0f + g_fPatternAnimT[0], 1.0f + g_fMoveTitleY[0]);
		pVtx[1].tex = D3DXVECTOR2(0.0f + g_fPatternAnimT[0], 0.0f + g_fMoveTitleY[0]);
		pVtx[2].tex = D3DXVECTOR2(1.0f + g_fPatternAnimT[0], 1.0f + g_fMoveTitleY[0]);
		pVtx[3].tex = D3DXVECTOR2(1.0f + g_fPatternAnimT[0], 0.0f + g_fMoveTitleY[0]);

	}

	//頂点バッファをアンロックする
	g_pVtxBuffTitle->Unlock();

	//PRESS_ENTERの点滅
	if (nFade == FADE_NONE)
	{
		g_nCount++;

		if (g_nCount == 20)
		{
			g_color.a = 1.0f;
		}
		else if (g_nCount == 40)
		{
			g_color.a = 0.0f;
			g_nCount = 0;
		}
	}

	//エンター押したら
	if (GetKeyboardTrigger(DIK_RETURN) == true)
	{
		if (nFade == FADE_NONE)
		{
			SetFade(FADE_OUT, MODE_TUTORIAL);	//ゲーム画面に切り替え
		}
	}
}

//=============================================================================
// タイトルの描画処理
//=============================================================================
void DrawTitle(void)
{
	//変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTitle, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	for (int nCntTexture = 0; nCntTexture < MAX_TITLE_TEX; nCntTexture++)
	{
		pDevice->SetTexture(0, g_pTextureTitle[nCntTexture]);
	}

	//ポリゴンの描画
	for (int nCntTexture = 0; nCntTexture < MAX_TITLE_TEX; nCntTexture++)
	{
		pDevice->SetTexture(0, g_pTextureTitle[nCntTexture]);
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTexture * 4, 2);
	}
}

//=============================================================================
// タイトルのテクスチャの設定
//=============================================================================
void SetTextureTitle(int nCntTitle)
{
	VERTEX_2D *pVtx;

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += nCntTitle * 4;

	if (nCntTitle == 0)	//背景
	{
		//頂点座標
		pVtx[0].pos = D3DXVECTOR3(0, SCREEN_HEIGHT, 0.0f);	//Zは0.0固定
		pVtx[1].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, 0, 0.0f);
	}
	else if (nCntTitle == 1)	//PRESS_ENTER
	{
		//頂点座標
		pVtx[0].pos = D3DXVECTOR3(650, 900, 0.0f);	//Zは0.0固定
		pVtx[1].pos = D3DXVECTOR3(650, 750, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(1290, 900, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(1290, 750, 0.0f);
	}
	else if (nCntTitle == 2)	//タイトルロゴ
	{
		//頂点座標
		pVtx[0].pos = D3DXVECTOR3(340, 550, 0.0f);	//Zは0.0固定
		pVtx[1].pos = D3DXVECTOR3(340, 100, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(1580, 550, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(1580, 100, 0.0f);
	}

	//頂点バッファをアンロックする
	g_pVtxBuffTitle->Unlock();
}