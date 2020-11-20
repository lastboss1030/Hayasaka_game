//=============================================================================
//
// チュートリアル処理 [Tutorial.cpp]
// Author:Taiki Hayasaka
//
//=============================================================================
#include "tutorial.h"									//インクルードファイル
#include "input.h"
#include "fade.h"
#include "sound.h"

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTutorial = NULL;	//頂点情報のポインタ
LPDIRECT3DTEXTURE9 g_apTextureTutorial[2];			//テクスチャのポインタ
D3DXCOLOR g_colorTutorial;									//色
float g_nCntEnter;

//=============================================================================
// チュートリアルの初期化処理
//=============================================================================
HRESULT InitTutorial(void)
{
	//変数宣言
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVtx;

	//デバイスの取得
	pDevice = GetDevice();

	//初期化
	g_colorTutorial = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_nCntEnter = 0;

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/TUTORIAL.png", &g_apTextureTutorial[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/press_enter001.png", &g_apTextureTutorial[1]);

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * 2,	//確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,		//頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffTutorial,
		NULL)))
	{
		return E_FAIL;
	}

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffTutorial->Lock(0, 0, (void**)&pVtx, 0);


	for (int nCntTutorial = 0; nCntTutorial < 2; nCntTutorial++)
	{
		//頂点情報の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);				//テクスチャ座標
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		//頂点座標の設定
		SetTextureTutorial(nCntTutorial);

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
	g_pVtxBuffTutorial->Unlock();

	PlaySound(SOUND_LABEL_TUTORIAL);

	return S_OK;
}

//=============================================================================
// チュートリアルの終了処理
//=============================================================================
void UninitTutorial(void)
{
	//サウンド停止
	StopSound();

	//頂点バッファの開放
	if (g_pVtxBuffTutorial != NULL)
	{
		g_pVtxBuffTutorial->Release();
		g_pVtxBuffTutorial = NULL;
	}

	//テクスチャの開放
	for (int nCntTexture = 0; nCntTexture < 2; nCntTexture++)
	{
		if (g_apTextureTutorial[nCntTexture] != NULL)
		{
			g_apTextureTutorial[nCntTexture]->Release();
			g_apTextureTutorial[nCntTexture] = NULL;
		}
	}
}

//=============================================================================
// チュートリアルの更新処理
//=============================================================================
void UpdateTutorial(void)
{
	//情報の取得
	int nFade = GetFade();

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTutorial->Lock(0, 0, (void**)&pVtx, 0);

	//PRESS_ENTERの色更新
	pVtx[4].col = g_colorTutorial;
	pVtx[5].col = g_colorTutorial;
	pVtx[6].col = g_colorTutorial;
	pVtx[7].col = g_colorTutorial;

	//頂点バッファをアンロックする
	g_pVtxBuffTutorial->Unlock();

	//PRESS_ENTERの点滅
	if (nFade == FADE_NONE)
	{
		g_nCntEnter++;

		if (g_nCntEnter == 20)
		{
			g_colorTutorial.a = 1.0f;
		}
		else if (g_nCntEnter == 40)
		{
			g_colorTutorial.a = 0.0f;
			g_nCntEnter = 0;
		}
	}

	//エンター押したら
	if (GetKeyboardTrigger(DIK_RETURN) == true)
	{
		if (nFade == FADE_NONE)
		{
			PlaySound(SOUND_LABEL_SE_START);
			SetFade(FADE_OUT, MODE_GAME);	//ゲーム画面に切り替え
		}
	}
}

//=============================================================================
// チュートリアルの描画処理
//=============================================================================
void DrawTutorial(void)
{
	//変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTutorial, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	for (int nCntTexture = 0; nCntTexture < 2; nCntTexture++)
	{
		pDevice->SetTexture(0, g_apTextureTutorial[nCntTexture]);
	}

	//ポリゴンの描画
	for (int nCntTexture = 0; nCntTexture < 2; nCntTexture++)
	{
		pDevice->SetTexture(0, g_apTextureTutorial[nCntTexture]);
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTexture * 4, 2);
	}
}


//=============================================================================
// チュートリアルのテクスチャの設定
//=============================================================================
void SetTextureTutorial(int nCntTutorial)
{
	VERTEX_2D *pVtx;

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffTutorial->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += nCntTutorial * 4;

	if (nCntTutorial == 0)	//背景
	{
		//頂点座標
		pVtx[0].pos = D3DXVECTOR3(0, SCREEN_HEIGHT, 0.0f);	//Zは0.0固定
		pVtx[1].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, 0, 0.0f);
	}
	else if (nCntTutorial == 1)	//PRESS_ENTER
	{
		//頂点座標
		pVtx[0].pos = D3DXVECTOR3(1000, 200, 0.0f);	//Zは0.0固定
		pVtx[1].pos = D3DXVECTOR3(1000, 50, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(1720, 200, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(1720, 50, 0.0f);
	}

	//頂点バッファをアンロックする
	g_pVtxBuffTutorial->Unlock();
}