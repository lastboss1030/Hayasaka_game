//=============================================================================
//
// リザルトの処理 [result.cpp]
// Author:Taiki Hayasaka
//
//=============================================================================
#include "result.h"								//インクルードファイル
#include "input.h"
#include "fade.h"
#include "player.h"
#include "enemy.h"
#include "Sound.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MAX_RESULT_TEX	(5)		//テクスチャ数
#define MAX_VERTEX		(4)		//頂点数

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DTEXTURE9 g_pTextureResult[MAX_RESULT_TEX] = {};	//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResult = NULL;			//頂点バッファへのポインタ
D3DXCOLOR g_colorR;											//色
int g_nCounterAnimR;										//アニメーションカウンター
int g_nPatternAnimR;										//アニメーションパターンNo
float g_fPatternAnimR[MAX_RESULT_TEX];						//アニメーションパターン
float g_nCountR;											//カウント
int g_nResultState;											//リザルトモード
D3DXCOLOR ResultCol[MAX_RESULT_TEX];						//リザルトカラー
float g_fMoveResultY[MAX_RESULT_TEX];						//移動量Y

//=============================================================================
// リザルトの初期化処理
//=============================================================================
HRESULT InitResult(void)
{
	//変数宣言
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVtx;

	//デバイスの取得
	pDevice = GetDevice();

	//カラーの初期
//	g_colorR = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_nCountR = 0;

	//初期化
	g_nResultState = RESULT_GAMEOVER;
	ResultCol[0] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/result.png", &g_pTextureResult[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/press_enter001.png", &g_pTextureResult[1]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/gameover_logo001.png", &g_pTextureResult[2]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/gameclear_logo001.png", &g_pTextureResult[3]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/haguruma.png", &g_pTextureResult[4]);

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * MAX_VERTEX * MAX_RESULT_TEX,	//確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,		//頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffResult,
		NULL)))
	{
		return E_FAIL;
	}

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntResult = 0; nCntResult < MAX_RESULT_TEX; nCntResult++)
	{
		//頂点情報の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);			//テクスチャ座標
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		//頂点座標の設定
		SetTextureResult(nCntResult);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		////頂点カラーの設定
		//pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		//pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		//pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		//pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		//４つの頂点を進める
		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffResult->Unlock();

	//BGM
	PlaySound(SOUND_LABEL_BGM_RESULT);

	return S_OK;
}

//=============================================================================
// リザルトの終了処理
//=============================================================================
void UninitResult(void)
{
	//サウンド停止
	StopSound();

	//頂点バッファの開放
	if (g_pVtxBuffResult != NULL)
	{
		g_pVtxBuffResult->Release();
		g_pVtxBuffResult = NULL;
	}

	//テクスチャの開放
	for (int nCntTexture = 0; nCntTexture < MAX_RESULT_TEX; nCntTexture++)
	{
		if (g_pTextureResult[nCntTexture] != NULL)
		{
			g_pTextureResult[nCntTexture]->Release();
			g_pTextureResult[nCntTexture] = NULL;
		}
	}
}

//=============================================================================
// リザルトの更新処理
//=============================================================================
void UpdateResult(void)
{
	//変数宣言
	VERTEX_2D *pVtx;
	Player *pPlayer = GetPlayer();
	Enemy *pEnemy = GetEnemy();
	int nFade = GetFade();

	//g_nCounterAnimR++;		//アニメーションカウンター更新

	if (pEnemy->nLife <= 0)
	{
		g_nResultState = RESULT_GAMECLEAR;
	}

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);

	// テクスチャの更新
	if ((g_nCounterAnimR % 4) == 0)
	{
		// テクスチャのパターンの更新
		g_fPatternAnimR[0] += 0.0005f;

		// テクスチャの頂点座標の更新
		pVtx[16].tex = D3DXVECTOR2(0.0f + g_fPatternAnimR[0], 1.0f + g_fMoveResultY[0]);
		pVtx[17].tex = D3DXVECTOR2(0.0f + g_fPatternAnimR[0], 0.0f + g_fMoveResultY[0]);
		pVtx[18].tex = D3DXVECTOR2(1.0f + g_fPatternAnimR[0], 1.0f + g_fMoveResultY[0]);
		pVtx[19].tex = D3DXVECTOR2(1.0f + g_fPatternAnimR[0], 0.0f + g_fMoveResultY[0]);
	}

	for (int nCntResult = 0; nCntResult < MAX_RESULT_TEX; nCntResult++, pVtx += 4)
	{
		switch (g_nResultState)
		{
		case RESULT_GAMECLEAR:	//ゲームクリア

			ResultCol[0] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			ResultCol[1] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			ResultCol[2] = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
			ResultCol[3] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			break;

		case RESULT_GAMEOVER:	//ゲームオーバー
			ResultCol[0] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			ResultCol[1] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			ResultCol[2] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			ResultCol[3] = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

			break;
		}
		pVtx[0].col = ResultCol[nCntResult];
		pVtx[1].col = ResultCol[nCntResult];
		pVtx[2].col = ResultCol[nCntResult];
		pVtx[3].col = ResultCol[nCntResult];
	}

	//PRESS_ENTERの色更新
	pVtx[4].col = g_colorR;
	pVtx[5].col = g_colorR;
	pVtx[6].col = g_colorR;
	pVtx[7].col = g_colorR;

	//頂点バッファをアンロックする
	g_pVtxBuffResult->Unlock();

	//PRESS_ENTERの点滅
	if (nFade == FADE_NONE)
	{
		g_nCountR++;

		if (g_nCountR == 20)
		{
			g_colorR.a = 1.0f;
		}
		else if (g_nCountR == 40)
		{
			g_colorR.a = 0.0f;
			g_nCountR = 0;
		}
	}

	//エンター押したら
	if (GetKeyboardTrigger(DIK_RETURN) == true)
	{
		if (nFade == FADE_NONE)
		{
			//効果音
			PlaySound(SOUND_LABEL_SE_ENTER);

			//クリアだったら
			if (g_nResultState == RESULT_GAMECLEAR)
			{
				SetFade(FADE_OUT, MODE_RANKING);	//ランキング画面に切り替え
			}

			//ゲームオーバーだったら
			if (g_nResultState == RESULT_GAMEOVER)
			{
				SetFade(FADE_OUT, MODE_TITLE);	//ランキング画面に切り替え
			}
		}
	}
}

//=============================================================================
// リザルトの描画処理
//=============================================================================
void DrawResult(void)
{
	//変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffResult, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	for (int nCntTexture = 0; nCntTexture < MAX_RESULT_TEX; nCntTexture++)
	{
		pDevice->SetTexture(0, g_pTextureResult[nCntTexture]);
	}

	//ポリゴンの描画
	for (int nCntTexture = 0; nCntTexture < MAX_RESULT_TEX; nCntTexture++)
	{
		pDevice->SetTexture(0, g_pTextureResult[nCntTexture]);
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTexture * MAX_VERTEX, 2);
	}
}

//=============================================================================
// リザルトのテクスチャの設定
//=============================================================================
void SetTextureResult(int nCntResult)
{
	VERTEX_2D *pVtx;

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += nCntResult * 4;

	if (nCntResult == 0)	//背景
	{
		//頂点座標
		pVtx[0].pos = D3DXVECTOR3(0, SCREEN_HEIGHT, 0.0f);	//Zは0.0固定
		pVtx[1].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, 0, 0.0f);

		//カラー
		ResultCol[nCntResult] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
	}
	else if (nCntResult == 1)	//PRESS_ENTER
	{
		//頂点座標
		pVtx[0].pos = D3DXVECTOR3(650, 950, 0.0f);	//Zは0.0固定
		pVtx[1].pos = D3DXVECTOR3(650, 800, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(1290, 950, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(1290, 800, 0.0f);

		//カラー
		ResultCol[nCntResult] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}
	else if (nCntResult == 2)	//ゲームオーバー
	{
		//頂点座標
		pVtx[0].pos = D3DXVECTOR3(480, 400, 0.0f);	//Zは0.0固定
		pVtx[1].pos = D3DXVECTOR3(480, 200, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(1440, 400, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(1440, 200, 0.0f);

		//カラー
		ResultCol[nCntResult] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}
	else if (nCntResult == 3)	//ゲームクリア
	{
		//頂点座標
		pVtx[0].pos = D3DXVECTOR3(480, 400, 0.0f);	//Zは0.0固定
		pVtx[1].pos = D3DXVECTOR3(480, 200, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(1440, 400, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(1440, 200, 0.0f);

		//カラー
		ResultCol[nCntResult] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}
	else if (nCntResult == 4)	//歯車
	{
		//頂点座標
		pVtx[0].pos = D3DXVECTOR3(0, SCREEN_HEIGHT, 0.0f);	//Zは0.0固定
		pVtx[1].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, 0, 0.0f);

		//カラー
		ResultCol[nCntResult] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}

	pVtx[0].col = ResultCol[nCntResult];
	pVtx[1].col = ResultCol[nCntResult];
	pVtx[2].col = ResultCol[nCntResult];
	pVtx[3].col = ResultCol[nCntResult];

	//頂点バッファをアンロックする
	g_pVtxBuffResult->Unlock();
}