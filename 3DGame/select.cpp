//=============================================================================
//
// 選択画面の処理 [select.cpp]
// Author:Taiki Hayasaka
//
//=============================================================================
#include "select.h"								//インクルードファイル
#include "input.h"
#include "fade.h"
#include "player.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MAX_TEX (4)									//テクスチャ最大数
#define MAX_VERTEX (4)								//頂点数

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffSelect = NULL;	//頂点情報のポインタ
LPDIRECT3DTEXTURE9 g_apTextureSelect[MAX_TEX];		//テクスチャのポインタ
D3DXCOLOR g_colorSelect[MAX_TEX];					//色
int g_nCounterAnimS;								//アニメーションカウンター
int g_nPatternAnimS;								//アニメーションパターンNo
int g_nState;										//ステート
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
	g_colorSelect[0] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_nCntEnterS = 0;

	//アニメーションの初期化
	g_nCounterAnimS = 0;
	g_nPatternAnimS = 0;

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Select_BG.png", &g_apTextureSelect[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/golem_select.png", &g_apTextureSelect[1]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/leo_select.png", &g_apTextureSelect[2]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/stalker_select.png", &g_apTextureSelect[3]);

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * MAX_VERTEX * MAX_TEX,	//確保するバッファサイズ
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
	VERTEX_2D *pVtx;
	int nFade = GetFade();
	Player *pPlayer;
	pPlayer = GetPlayer();

	g_nCounterAnimS++;

	if ((g_nCounterAnimS % 10) == 0)
	{
		//アニメーションパターンを更新
		g_nPatternAnimS = (g_nPatternAnimS + 1) % 8;
	}

	//セレクト
	if (GetKeyboardTrigger(DIK_D) || GetKeyboardTrigger(DIK_RIGHT) == true)
	{
		//効果音


		g_nState++;
		if (g_nState > 2)
		{
			g_nState = 0;
		}
	}

	if (GetKeyboardTrigger(DIK_A) || GetKeyboardTrigger(DIK_LEFT) == true)
	{
		//効果音


		g_nState--;
		if (g_nState < 0)
		{
			g_nState = 2;
		}
	}

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffSelect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntSelect = 0; nCntSelect < MAX_TEX; nCntSelect++, pVtx += 4)
	{
		switch (g_nState)
		{
		case SELECT_MENU_GOLEM:		 //ゴーレム選択時

			//pVtx[4].tex = D3DXVECTOR2(0.0f + (0.125f * g_nPatternAnimS), 1.0f);
			//pVtx[5].tex = D3DXVECTOR2(0.0f + (0.125f * g_nPatternAnimS), 0.0f);
			//pVtx[6].tex = D3DXVECTOR2(0.125f + (0.125f * g_nPatternAnimS), 1.0f);
			//pVtx[7].tex = D3DXVECTOR2(0.125f + (0.125f * g_nPatternAnimS), 0.0f);

			g_colorSelect[1] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			g_colorSelect[2] = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
			g_colorSelect[3] = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);

			break;

		case SELECT_MENU_LEO:		//レオ選択時
			g_colorSelect[1] = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
			g_colorSelect[2] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			g_colorSelect[3] = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);

			break;

		case SELECT_MENU_STALKER:	//ストーカー選択時
			g_colorSelect[1] = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
			g_colorSelect[2] = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
			g_colorSelect[3] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			break;
		}
		pVtx[0].col = g_colorSelect[nCntSelect];
		pVtx[1].col = g_colorSelect[nCntSelect];
		pVtx[2].col = g_colorSelect[nCntSelect];
		pVtx[3].col = g_colorSelect[nCntSelect];
	}

	//エンター押したら
	if (GetKeyboardTrigger(DIK_RETURN) == true)
	{
		if (nFade == FADE_NONE)
		{
			if (g_nState == SELECT_MENU_GOLEM)	//ゴーレム選択時
			{
				//決定音


				//キャラ決定
				pPlayer->playertype = PLAYERTYPE_GOLEM;

				//ゴーレム使用
				SetFade(FADE_OUT, MODE_GAME);

			}
			else if (g_nState == SELECT_MENU_LEO)	//レオ選択時
			{
				//決定音


				//キャラ決定
				pPlayer->playertype = PLAYERTYPE_LEO;

				//レオ使用
				SetFade(FADE_OUT, MODE_GAME);

			}
			else if (g_nState == SELECT_MENU_STALKER)	//ストーカー選択時
			{
				//決定音


				//キャラ決定
				pPlayer->playertype = PLAYERTYPE_STALKER;

				//ストーカー使用
				SetFade(FADE_OUT, MODE_GAME);

			}
		}
	}

	//頂点バッファをアンロックする
	g_pVtxBuffSelect->Unlock();
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

	//ポリゴンの描画
	for (int nCntTexture = 0; nCntTexture < MAX_TEX; nCntTexture++)
	{
		pDevice->SetTexture(0, g_apTextureSelect[nCntTexture]);
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTexture * MAX_VERTEX, 2);
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

		//カラー
		g_colorSelect[nCntSelect] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	}
	else if (nCntSelect == 1)	//ゴーレム
	{
		//頂点座標
		pVtx[0].pos = D3DXVECTOR3(120, 950, 0.0f);	//Zは0.0固定
		pVtx[1].pos = D3DXVECTOR3(120, 350, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(520, 950, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(520, 350, 0.0f);

		//カラー
		g_colorSelect[nCntSelect] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}
	else if (nCntSelect == 2)	//レオ
	{
		//頂点座標
		pVtx[0].pos = D3DXVECTOR3(750, 950, 0.0f);	//Zは0.0固定
		pVtx[1].pos = D3DXVECTOR3(750, 350, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(1150, 950, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(1150, 350, 0.0f);

		//カラー
		g_colorSelect[nCntSelect] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}
	else if (nCntSelect == 3)	//ストーカー
	{
		//頂点座標
		pVtx[0].pos = D3DXVECTOR3(1400, 950, 0.0f);	//Zは0.0固定
		pVtx[1].pos = D3DXVECTOR3(1400, 350, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(1800, 950, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(1800, 350, 0.0f);

		//カラー
		g_colorSelect[nCntSelect] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}

	pVtx[0].col = g_colorSelect[nCntSelect];
	pVtx[1].col = g_colorSelect[nCntSelect];
	pVtx[2].col = g_colorSelect[nCntSelect];
	pVtx[3].col = g_colorSelect[nCntSelect];


	//頂点バッファをアンロックする
	g_pVtxBuffSelect->Unlock();
}