//=============================================================================
//
// 影の処理 [shadow.cpp]
// Author:Taiki Hayasaka
//
//=============================================================================
#include "shadow.h"
#include "input.h"
#include "player.h"

//=============================================================================
//マクロ定義
//=============================================================================
#define MAX_SHADOW (64)		//影の最大数

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffShadow = NULL;	//頂点バッファへのポインタ
LPDIRECT3DTEXTURE9	g_pTextureShadow = NULL;		//テクスチャへのポインタ
Shadow g_aShadow[MAX_SHADOW];						//影の情報

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitShadow(void)
{
	LPDIRECT3DDEVICE9 pDevice;	//デバイスへのポインタ
	VERTEX_3D *pVtx;			//頂点情報へのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/shadow000.jpg", &g_pTextureShadow);

	//初期化
	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		g_aShadow[nCntShadow].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aShadow[nCntShadow].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aShadow[nCntShadow].fWidth = 1.0f;
		g_aShadow[nCntShadow].fDepth = 1.0f;
		g_aShadow[nCntShadow].bUse = false;
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_SHADOW,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffShadow,
		NULL);

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		//頂点座標の設定(Z型)
		pVtx[0].pos = D3DXVECTOR3(g_aShadow[nCntShadow].pos.x - 30.0f, g_aShadow[nCntShadow].pos.y, g_aShadow[nCntShadow].pos.z + 30.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aShadow[nCntShadow].pos.x + 30.0f, g_aShadow[nCntShadow].pos.y, g_aShadow[nCntShadow].pos.z + 30.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aShadow[nCntShadow].pos.x - 30.0f, g_aShadow[nCntShadow].pos.y, g_aShadow[nCntShadow].pos.z - 30.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aShadow[nCntShadow].pos.x + 30.0f, g_aShadow[nCntShadow].pos.y, g_aShadow[nCntShadow].pos.z - 30.0f);

		//法線ベクトルの設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(0.8f, 0.8f, 0.8f, 0.8f);
		pVtx[1].col = D3DXCOLOR(0.8f, 0.8f, 0.8f, 0.8f);
		pVtx[2].col = D3DXCOLOR(0.8f, 0.8f, 0.8f, 0.8f);
		pVtx[3].col = D3DXCOLOR(0.8f, 0.8f, 0.8f, 0.8f);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		//４つ進める
		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffShadow->Unlock();


	Player *pPlayer;
	pPlayer = GetPlayer();

	//影の配置
	SetShadow(pPlayer->pos, 10.0f, 10.0f);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitShadow(void)
{
	//頂点バッファの開放
	if (g_pVtxBuffShadow != NULL)
	{
		g_pVtxBuffShadow->Release();
		g_pVtxBuffShadow = NULL;
	}

	//テクスチャの開放
	if (g_pTextureShadow != NULL)
	{
		g_pTextureShadow->Release();
		g_pTextureShadow = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateShadow(void)
{

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawShadow(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスへのポインタ
	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス

	//デバイスの取得
	pDevice = GetDevice();

	//減算合成の設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		if (g_aShadow[nCntShadow].bUse == true)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aShadow[nCntShadow].mtxWorld);

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aShadow[nCntShadow].rot.y, g_aShadow[nCntShadow].rot.x, g_aShadow[nCntShadow].rot.z);
			D3DXMatrixMultiply(&g_aShadow[nCntShadow].mtxWorld, &g_aShadow[nCntShadow].mtxWorld, &mtxRot);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aShadow[nCntShadow].pos.x, g_aShadow[nCntShadow].pos.y, g_aShadow[nCntShadow].pos.z);
			D3DXMatrixMultiply(&g_aShadow[nCntShadow].mtxWorld, &g_aShadow[nCntShadow].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aShadow[nCntShadow].mtxWorld);

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffShadow, 0, sizeof(VERTEX_3D));

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureShadow);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntShadow * 4, 2);
		}
	}

	//通常の合成に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//==============================================================================
// 影の設定
//==============================================================================
int SetShadow(D3DXVECTOR3 pos, float fWidth, float fDepth)
{
	//変数宣言
	int nShadow = -1;

	//影の設定
	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		if (g_aShadow[nCntShadow].bUse == false)
		{
			//位置の設定
			g_aShadow[nCntShadow].pos = pos;

			//幅の設定
			g_aShadow[nCntShadow].fWidth = fWidth;

			//奥行の設定
			g_aShadow[nCntShadow].fDepth = fDepth;

			//使っているか
			g_aShadow[nCntShadow].bUse = true;

			//影に代入
			nShadow = nCntShadow;

			break;
		}
	}

	return nShadow;	//返す
}

//==============================================================================
// 影の追従
//==============================================================================
void SetPositionShadow(int nIdx, D3DXVECTOR3 pos)
{
	g_aShadow[nIdx].pos = pos;
}

//==============================================================================
// 影の削除
//==============================================================================
void DeleteShadow(int nIdx)
{
	// 影の構造体のポインタ変数
	Shadow *pShadow = &g_aShadow[0];

	// 何番目の影か
	pShadow += nIdx;

	// 使用状態をfalseにする
	pShadow->bUse = false;
}