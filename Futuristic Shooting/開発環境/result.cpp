//=============================================================================
//
// リザルトの処理 [result.cpp]
// Author:Taiki Hayasaka
//
//=============================================================================
#include "result.h"									//インクルードファイル
#include "input.h"
#include "score.h"
#include "fade.h"
#include "player.h"
#include "enemy.h"
#include "sound.h"

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DTEXTURE9 g_pTextureResult[3] = {};		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResult = NULL;	//頂点バッファへのポインタ
int g_nResultState;									//リザルトモード
D3DXCOLOR ResultCol[3];								//リザルトカラー

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

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/BG_Result.jpg", &g_pTextureResult[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/gameclear_logo001.png", &g_pTextureResult[1]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/gameover_logo001.png", &g_pTextureResult[2]);
	

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * 3,	//確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,		//頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffResult,
		NULL)))
	{
		return E_FAIL;
	}

	//初期化
	g_nResultState = RESULT_GAMECLEAR;
	ResultCol[0] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntResult = 0; nCntResult < 3; nCntResult++)
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

		//４つの頂点を進める
		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffResult->Unlock();

	PlaySound(SOUND_LABEL_BGM_END);

	return S_OK;
}

//=============================================================================
// リザルトの終了処理
//=============================================================================
void UninitResult(void)
{
	StopSound();

	//頂点バッファの開放
	if (g_pVtxBuffResult != NULL)
	{
		g_pVtxBuffResult->Release();
		g_pVtxBuffResult = NULL;
	}

	//テクスチャの開放
	for (int nCntTexture = 0; nCntTexture < 3; nCntTexture++)
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
	VERTEX_2D *pVtx;
	PLAYER *pPlayer;

	//情報の取得
	pPlayer = GetPlayer();
	int nFade = GetFade();
	
	if (pPlayer->nRemaining <= 0)
	{
		g_nResultState = RESULT_GAMEOVER;
	}

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntResult = 0; nCntResult < 3; nCntResult++, pVtx += 4)
	{
		switch (g_nResultState)
		{
		case RESULT_GAMECLEAR:	//ゲームクリア

			ResultCol[0] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			ResultCol[1] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			ResultCol[2] = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

			break;

		case RESULT_GAMEOVER:	//ゲームオーバー
			ResultCol[0] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			ResultCol[1] = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
			ResultCol[2] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			break;
		}
		pVtx[0].col = ResultCol[nCntResult];
		pVtx[1].col = ResultCol[nCntResult];
		pVtx[2].col = ResultCol[nCntResult];
		pVtx[3].col = ResultCol[nCntResult];
	}

	//エンター押したら
	if (GetKeyboardTrigger(DIK_RETURN) == true)
	{
		if (nFade == FADE_NONE)
		{
			SetFade(FADE_OUT, MODE_TITLE);	//タイトル画面に切り替え
		}
	}

	//頂点バッファをアンロックする
	g_pVtxBuffResult->Unlock();
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
	for (int nCntTexture = 0; nCntTexture < 3; nCntTexture++)
	{
		pDevice->SetTexture(0, g_pTextureResult[nCntTexture]);
	}

	//ポリゴンの描画
	for (int nCntTexture = 0; nCntTexture < 3; nCntTexture++)
	{
		pDevice->SetTexture(0, g_pTextureResult[nCntTexture]);
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTexture * 4, 2);
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

	if (nCntResult == 0)		//背景
	{
		//頂点座標
		pVtx[0].pos = D3DXVECTOR3(0, SCREEN_HEIGHT, 0.0f);	//Zは0.0固定
		pVtx[1].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, 0, 0.0f);

		//カラー
		ResultCol[nCntResult] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
	}
	else if (nCntResult == 1)	//GAME_CLEAR
	{
		//頂点座標
		pVtx[0].pos = D3DXVECTOR3(480, 400, 0.0f);	//Zは0.0固定
		pVtx[1].pos = D3DXVECTOR3(480, 250, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(1440, 400, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(1440, 250, 0.0f);

		//カラー
		ResultCol[nCntResult] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}
	else if (nCntResult == 2)	//GAME_OVER
	{
		//頂点座標
		pVtx[0].pos = D3DXVECTOR3(480, 400, 0.0f);	//Zは0.0固定
		pVtx[1].pos = D3DXVECTOR3(480, 250, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(1440, 400, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(1440, 250, 0.0f);

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