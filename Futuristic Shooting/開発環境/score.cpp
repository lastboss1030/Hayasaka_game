//=============================================================================
//
// スコアの処理 [score.cpp]
// Author:Taiki Hayasaka
//
//=============================================================================
#include "score.h"									//インクルードファイル

//=============================================================================
// マクロ定義
//=============================================================================
#define MAX_SCORE (8)								//スコアポリゴンの最大数

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DTEXTURE9 g_pTextureScore = NULL;			//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffScore = NULL;		//頂点バッファへのポインタ
D3DXVECTOR3 g_posScore[MAX_SCORE];					//スコアの位置
float g_fLengthScore;								//スコアの対角線の長さ
float g_fAngleScore;								//スコアの対角線の角度
int g_nScore;										//スコア

//=============================================================================
// スコアの初期化処理
//=============================================================================
HRESULT InitScore(void)
{
	//変数宣言
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVtx;

	//デバイスの取得
	pDevice = GetDevice();

	//スコアの初期化
	g_nScore = 0;

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/number001.png", &g_pTextureScore);

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_SCORE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffScore,
		NULL)))
	{
		return E_FAIL;
	}
	
	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);


	for (int nCntScore = 0; nCntScore < MAX_SCORE; nCntScore++, pVtx += 4)
	{
		//スコアの位置
		g_posScore[nCntScore] = D3DXVECTOR3(1460.0f + nCntScore * 60, 60, 0.0f);

		//対角線の長さ
		g_fLengthScore = sqrtf(30 * 30 + 50 * 50);

		//対角線の角度
		g_fAngleScore = atan2f(30, 50);

		//頂点情報の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);		//テクスチャ座標
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 0.0f);

		//頂点座標の設定
		pVtx[0].pos.x = (g_posScore[nCntScore].x - sinf(g_fAngleScore) * g_fLengthScore);
		pVtx[0].pos.y = (g_posScore[nCntScore].y + cosf(g_fAngleScore) * g_fLengthScore);
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = (g_posScore[nCntScore].x - sinf(g_fAngleScore) * g_fLengthScore);
		pVtx[1].pos.y = (g_posScore[nCntScore].y - cosf(g_fAngleScore) * g_fLengthScore);
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = (g_posScore[nCntScore].x + sinf(g_fAngleScore) * g_fLengthScore);
		pVtx[2].pos.y = (g_posScore[nCntScore].y + cosf(g_fAngleScore) * g_fLengthScore);
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = (g_posScore[nCntScore].x + sinf(g_fAngleScore) * g_fLengthScore);
		pVtx[3].pos.y = (g_posScore[nCntScore].y - cosf(g_fAngleScore) * g_fLengthScore);
		pVtx[3].pos.z = 0.0f;

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = D3DCOLOR_RGBA(0, 200, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(0, 200, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(0, 200, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(0, 200, 255, 255);
	}

	//頂点バッファをアンロックする
	g_pVtxBuffScore->Unlock();

	return S_OK;
}

//=============================================================================
// スコアの終了処理
//=============================================================================
void UninitScore(void)
{
	//頂点バッファの開放
	if (g_pVtxBuffScore != NULL)
	{
		g_pVtxBuffScore->Release();
		g_pVtxBuffScore = NULL;
	}

	//テクスチャの開放
	if (g_pTextureScore != NULL)
	{
		g_pTextureScore->Release();
		g_pTextureScore = NULL;
	}
}

//=============================================================================
// スコアの更新処理
//=============================================================================
void UpdateScore(void)
{
	
}

//=============================================================================
// スコアの描画処理
//=============================================================================
void DrawScore(void)
{
	//変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffScore, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureScore);

	//スコアの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 32);
}

//=============================================================================
// スコア計算
//=============================================================================
void AddScore(int nValue)
{
	int aNumber[MAX_SCORE];
	VERTEX_2D *pVtx;

	g_nScore += nValue;

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	aNumber[0] = g_nScore % 100000000 / 10000000;
	aNumber[1] = g_nScore % 10000000 / 1000000;
	aNumber[2] = g_nScore % 1000000 / 100000;
	aNumber[3] = g_nScore % 100000 / 10000;
	aNumber[4] = g_nScore % 10000 / 1000;
	aNumber[5] = g_nScore % 1000 / 100;
	aNumber[6] = g_nScore % 100 / 10;
	aNumber[7] = g_nScore % 10 / 1;

	//テクスチャ座標の更新
	for (int nCntScore = 0; nCntScore < MAX_SCORE; nCntScore++)
	{
		SetTextureScore(nCntScore,aNumber[nCntScore]);
	}

	//頂点バッファをアンロックする
	g_pVtxBuffScore->Unlock();
}

//=============================================================================
// テクスチャ座標の更新
//=============================================================================
void SetTextureScore(int nIdx, int nNumber)
{
	VERTEX_2D *pVtx;

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);
	
	pVtx += (nIdx * 4);

	pVtx[0].tex = D3DXVECTOR2(0.0f + (0.1f*nNumber), 1.0f);		//テクスチャ座標
	pVtx[1].tex = D3DXVECTOR2(0.0f + (0.1f*nNumber), 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.1f + (0.1f*nNumber), 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.1f + (0.1f*nNumber), 0.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffScore->Unlock();
}