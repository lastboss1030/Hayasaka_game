//=============================================================================
//
// タイムの処理 [time.cpp]
// Author:Taiki Hayasaka
//
//=============================================================================
#include "time.h"
#include "Sound.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define LEFT_X			(895.0f)										//左X座標
#define RIGHT_X			(945.0f)										//右X座標
#define HIGH_Y			(10.0f)											//上Y座標
#define LOW_Y			(90.0f)											//下Y座標
#define WIDTH_X			(45.0f)											//スコアの幅

#define MAX_VERTEX		(4)
#define MAX_PATTERN     (10)											//アニメーションパターンNo.の最大数
#define TEXTURE_X		(10)											//アニメーションパターンNo.の横の最大数
#define TEXTURE_Y		(1)												//アニメーションパターンNo.の縦の最大数

#define PLAY_ANIM       ((1.0f / TEXTURE_X) * pExprosion->nPatternAnim)	//テクスチャの動作
#define UV_U			(1.0f / TEXTURE_X + PLAY_ANIM)					//Uの値
#define UV_V			(1.0f / TEXTURE_Y)								//Vの値

#define MAX_TIMER		(3)												//スコアテクスチャの最大値

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTime = NULL;	//タイマーのテクスチャのポインタ
LPDIRECT3DTEXTURE9 g_pTextureTime = NULL;		//タイマーの頂点バッファのポインタ
int g_nTime;									//タイマー
int g_nTimeCnt;									//タイマーカウント
int g_aNumber[3];								//残り時間を格納
D3DXCOLOR g_TimeCol;							//タイマーの色

//=============================================================================
// タイマーの初期化処理
//=============================================================================
HRESULT InitTime(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();

	// テクスチャの読み込み	
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/number001.png", &g_pTextureTime);

	// 変数の初期化
	g_nTime = 60;									//タイマー
	g_nTimeCnt = 60;								//タイマーカウント
	g_TimeCol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	//タイマーの色

													//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX * MAX_TIMER, //確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,					//頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffTime,
		NULL)))
	{
		return E_FAIL;
	}

	//構造体のポインタ変数
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	for (int nCntTime = 0; nCntTime < MAX_TIMER; nCntTime++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(LEFT_X + WIDTH_X * nCntTime, LOW_Y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(LEFT_X + WIDTH_X * nCntTime, HIGH_Y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(RIGHT_X + WIDTH_X * nCntTime, LOW_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(RIGHT_X + WIDTH_X * nCntTime, HIGH_Y, 0.0f);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = g_TimeCol;
		pVtx[1].col = g_TimeCol;
		pVtx[2].col = g_TimeCol;
		pVtx[3].col = g_TimeCol;

		//残り時間の計算
		g_aNumber[nCntTime] = (g_nTime / powf(10.0f, MAX_TIMER - (nCntTime + 1.0f)));

		//テクスチャの更新
		pVtx[0].tex = D3DXVECTOR2(0.0f + g_aNumber[nCntTime] * 0.1f, 1.0f / TEXTURE_Y);
		pVtx[1].tex = D3DXVECTOR2(0.0f + g_aNumber[nCntTime] * 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f / TEXTURE_X + g_aNumber[nCntTime] * 0.1f, 1.0f / TEXTURE_Y);
		pVtx[3].tex = D3DXVECTOR2(1.0f / TEXTURE_X + g_aNumber[nCntTime] * 0.1f, 0.0f);

		//スコアの情報を次にうつす
		pVtx += 4;
	}
	//頂点バッファをアンロックする
	g_pVtxBuffTime->Unlock();

	return S_OK;
}

//=============================================================================
// タイマーの終了処理
//=============================================================================
void UninitTime(void)
{
	//頂点バッファの開放
	if (g_pVtxBuffTime != NULL)
	{
		g_pVtxBuffTime->Release();
		g_pVtxBuffTime = NULL;
	}

	//テクスチャの開放
	if (g_pTextureTime != NULL)
	{
		g_pTextureTime->Release();
		g_pTextureTime = NULL;
	}
}

//=============================================================================
// タイマーの更新処理
//=============================================================================
void UpdateTime(void)
{
	//タイマーカウントの減少
	g_nTimeCnt--;

	if (g_nTimeCnt == 0 && g_nTime != 0)
	{
		//残り時間を減らす
		g_nTime -= 1;

		//初期化
		g_nTimeCnt = 60;

		if (g_nTime == 0)
		{
			g_nTimeCnt = 10;
		}
	}
	else if (g_nTimeCnt == 0 && g_nTime == 0)
	{
		//タイマーを点滅させる
		g_TimeCol.a = 0.0f;
		g_nTimeCnt = 20;

		//効果音
		PlaySound(SOUND_LABEL_SE_TIMEUP);
	}
	else if (g_nTimeCnt == 10 && g_nTime == 0)
	{
		g_TimeCol.a = 1.0f;
	}

	//構造体のポインタ変数
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);

	//残り時間を格納
	for (int nCntTime = 0; nCntTime < MAX_TIMER; nCntTime++)
	{
		//残り時間の計算
		g_aNumber[nCntTime] = (g_nTime / powf(10.0f, MAX_TIMER - (nCntTime + 1.0f)));

		//テクスチャの更新
		pVtx[0].tex = D3DXVECTOR2(0.0f + g_aNumber[nCntTime] * 0.1f, 1.0f / TEXTURE_Y);
		pVtx[1].tex = D3DXVECTOR2(0.0f + g_aNumber[nCntTime] * 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f / TEXTURE_X + g_aNumber[nCntTime] * 0.1f, 1.0f / TEXTURE_Y);
		pVtx[3].tex = D3DXVECTOR2(1.0f / TEXTURE_X + g_aNumber[nCntTime] * 0.1f, 0.0f);

		//頂点カラーの更新
		pVtx[0].col = g_TimeCol;
		pVtx[1].col = g_TimeCol;
		pVtx[2].col = g_TimeCol;
		pVtx[3].col = g_TimeCol;

		//スコアの情報を次にうつす
		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffTime->Unlock();
}

//=============================================================================
// タイマーの描画処理
//=============================================================================
void DrawTime(void)
{
	//変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	//変数の受け渡し
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTime, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定と描画
	for (int nCntTime = 0; nCntTime < MAX_TIMER; nCntTime++)
	{
		//表示
		pDevice->SetTexture(0, g_pTextureTime);
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTime * 4, 2);
	}
}

//=============================================================================
// タイマー情報の取得
//=============================================================================
int GetTime(void)
{
	return g_nTime;
}