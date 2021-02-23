//=============================================================================
//
// クールダウンUI処理 [cooldownUI.cpp]
// Author:Taiki Hayasaka
//
//=============================================================================
#include "cooldown_UI.h"							//インクルードファイル
#include "input.h"
#include "player.h"
#include "enemy.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MAX_TEX (6)									//テクスチャ最大数
#define MAX_VERTEX (4)								//頂点数

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffCooldown = NULL;	//頂点情報のポインタ
LPDIRECT3DTEXTURE9 g_apTextureCooldown[MAX_TEX];	//テクスチャのポインタ
D3DXCOLOR g_colorCooldown;							//色
int g_nCounterAnimC;								//アニメーションカウンター
int g_nPatternAnimC;								//アニメーションパターンNo
bool bUse;

//=============================================================================
// クールダウンUIのの初期化処理
//=============================================================================
HRESULT InitCooldown(void)
{
	//変数宣言
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVtx;

	//デバイスの取得
	pDevice = GetDevice();

	//初期化
	g_colorCooldown = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	bUse = false;

	//アニメーションの初期化
	g_nCounterAnimC = 0;
	g_nPatternAnimC = 0;

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/cooldown001.png", &g_apTextureCooldown[0]);	//青
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/cooldown100.png", &g_apTextureCooldown[1]);	//ブースト枠線
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/cooldown002.png", &g_apTextureCooldown[2]);	//オレンジ
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/cooldown101.png", &g_apTextureCooldown[3]);	//Hp枠線
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/cooldown003.png", &g_apTextureCooldown[4]);	//ピンク
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/cooldown.png", &g_apTextureCooldown[5]);		//敵HP枠線
			
	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * MAX_VERTEX * MAX_TEX,	//確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,		//頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffCooldown,
		NULL)))
	{
		return E_FAIL;
	}

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffCooldown->Lock(0, 0, (void**)&pVtx, 0);


	for (int nCntCooldown = 0; nCntCooldown < MAX_TEX; nCntCooldown++)
	{
		//頂点情報の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);				//テクスチャ座標
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		//頂点座標の設定
		SetTextureCooldown(nCntCooldown);

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
	g_pVtxBuffCooldown->Unlock();

	return S_OK;
}

//=============================================================================
// クールダウンUIの終了処理
//=============================================================================
void UninitCooldown(void)
{
	//頂点バッファの開放
	if (g_pVtxBuffCooldown != NULL)
	{
		g_pVtxBuffCooldown->Release();
		g_pVtxBuffCooldown = NULL;
	}

	//テクスチャの開放
	for (int nCntTexture = 0; nCntTexture < MAX_TEX; nCntTexture++)
	{
		if (g_apTextureCooldown[nCntTexture] != NULL)
		{
			g_apTextureCooldown[nCntTexture]->Release();
			g_apTextureCooldown[nCntTexture] = NULL;
		}
	}
}

//=============================================================================
// クールダウンUIの更新処理
//=============================================================================
void UpdateCooldown(void)
{
	VERTEX_2D *pVtx;
	Player *pPlayer;
	Enemy *pEnemy;
	pPlayer = GetPlayer();
	pEnemy = GetEnemy();

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffCooldown->Lock(0, 0, (void**)&pVtx, 0);

	//ブースト
	pVtx[0].pos = D3DXVECTOR3(20, 80, 0.0f);	//Zは0.0固定
	pVtx[1].pos = D3DXVECTOR3(20, 20, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(pPlayer->boost * 3, 80, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(pPlayer->boost * 3, 20, 0.0f);

	//HP
	pVtx[8].pos = D3DXVECTOR3(20, 150, 0.0f);	//Zは0.0固定
	pVtx[9].pos = D3DXVECTOR3(20, 90, 0.0f);
	pVtx[10].pos = D3DXVECTOR3(pPlayer->Life * 3, 150, 0.0f);
	pVtx[11].pos = D3DXVECTOR3(pPlayer->Life * 3, 90, 0.0f);

	//敵HP
	pVtx[16].pos = D3DXVECTOR3(1120, 80, 0.0f);	//Zは0.0固定
	pVtx[17].pos = D3DXVECTOR3(1120, 20, 0.0f);
	pVtx[18].pos = D3DXVECTOR3(1120 +pEnemy->nLife * 3.7f, 80, 0.0f);
	pVtx[19].pos = D3DXVECTOR3(1120 +pEnemy->nLife * 3.7f, 20, 0.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffCooldown->Unlock();
}

//=============================================================================
// クールダウンUIの描画処理
//=============================================================================
void DrawCooldown(void)
{
	//変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffCooldown, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (bUse == true)
	{
		//テクスチャの設定
		for (int nCntTexture = 0; nCntTexture < MAX_TEX; nCntTexture++)
		{
			pDevice->SetTexture(0, g_apTextureCooldown[nCntTexture]);
		}

		//ポリゴンの描画
		for (int nCntTexture = 0; nCntTexture < MAX_TEX; nCntTexture++)
		{
			pDevice->SetTexture(0, g_apTextureCooldown[nCntTexture]);
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTexture * MAX_VERTEX, 2);
		}
	}
}


//=============================================================================
// クールダウンUIのテクスチャの設定
//=============================================================================
void SetTextureCooldown(int nCntCooldown)
{
	VERTEX_2D *pVtx;
	Player *pPlayer;
	pPlayer = GetPlayer();

	bUse = true;

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffCooldown->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += nCntCooldown * 4;

	if (nCntCooldown == 0)	//ブーストゲージ
	{
		//頂点座標
		pVtx[0].pos = D3DXVECTOR3(20, 80, 0.0f);	//Zは0.0固定
		pVtx[1].pos = D3DXVECTOR3(20, 20, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(600, 80, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(600, 20, 0.0f);
	}
	else if (nCntCooldown == 1)	//枠線
	{
		//頂点座標
		pVtx[0].pos = D3DXVECTOR3(20, 80, 0.0f);	//Zは0.0固定
		pVtx[1].pos = D3DXVECTOR3(20, 20, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(600, 80, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(600, 20, 0.0f);
	}
	else if (nCntCooldown == 2)	//HPゲージ
	{
		//頂点座標
		pVtx[0].pos = D3DXVECTOR3(20, 150, 0.0f);	//Zは0.0固定
		pVtx[1].pos = D3DXVECTOR3(20, 90, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(600, 150, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(600, 90, 0.0f);
	}
	else if (nCntCooldown == 3)	//枠線
	{
		//頂点座標
		pVtx[0].pos = D3DXVECTOR3(20, 150, 0.0f);	//Zは0.0固定
		pVtx[1].pos = D3DXVECTOR3(20, 90, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(600, 150, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(600, 90, 0.0f);
	}
	else if (nCntCooldown == 4)	//敵HPゲージ
	{
		//頂点座標
		pVtx[0].pos = D3DXVECTOR3(1120, 80, 0.0f);	//Zは0.0固定
		pVtx[1].pos = D3DXVECTOR3(1120, 20, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(1500, 80, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(1500, 20, 0.0f);
	}
	else if (nCntCooldown == 5)	//枠線
	{
		//頂点座標
		pVtx[0].pos = D3DXVECTOR3(1120, 80, 0.0f);	//Zは0.0固定
		pVtx[1].pos = D3DXVECTOR3(1120, 20, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(1500, 80, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(1500, 20, 0.0f);
	}

	//頂点バッファをアンロックする
	g_pVtxBuffCooldown->Unlock();
}