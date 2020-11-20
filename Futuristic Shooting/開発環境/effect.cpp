//=============================================================================
//
// エフェクトの処理 [effect.cpp]
// Author:Taiki Hayasaka
//
//=============================================================================
#include "effect.h"									//インクルードファイル
#include "fade.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MAX_EFFECT (4096)							//エフェクト最大数

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DTEXTURE9 g_pTextureEffect = NULL;			//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect = NULL; 	//頂点バッファへのポインタ
EFFECT g_aEffect[MAX_EFFECT];
D3DXVECTOR3 g_posEffect;							//エフェクトの位置
D3DXVECTOR3 g_rotEffect;							//エフェクトの回転角
D3DXVECTOR3 g_moveEffect;							//エフェクトの移動量
float g_fLengthEffect;								//エフェクトの対角線の長さ
float g_fAngleEffect;								//エフェクトの対角線の角度

//=============================================================================
// エフェクトの初期化処理
//=============================================================================
HRESULT InitEffect(void)
{
	//変数宣言
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVtx;

	//デバイスの取得
	pDevice = GetDevice();

	//エフェクトの初期化
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		g_aEffect[nCntEffect].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].bUse = false;
	}

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect000.jpg", &g_pTextureEffect);

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_EFFECT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEffect,
		NULL)))
	{
		return E_FAIL;
	}

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	//対角線の長さ
	g_fLengthEffect = sqrtf(40 * 40 + 50 * 50);

	//対角線の角度
	g_fAngleEffect = atan2f(40, 50);

	//初期位置
	g_posEffect = D3DXVECTOR3(0, 0, 0.0f);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		//頂点情報の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);		//テクスチャ座標
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(100, 200, 0.0f);	//Zは0.0固定
		pVtx[1].pos = D3DXVECTOR3(100, 400, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(200, 200, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(200, 400, 0.0f);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = g_aEffect[nCntEffect].col;
		pVtx[1].col = g_aEffect[nCntEffect].col;
		pVtx[2].col = g_aEffect[nCntEffect].col;
		pVtx[3].col = g_aEffect[nCntEffect].col;

		//４つの頂点を進める
		pVtx += 4;
	}
	//頂点バッファをアンロックする
	g_pVtxBuffEffect->Unlock();

	return S_OK;
}

//=============================================================================
// エフェクトの終了処理
//=============================================================================
void UninitEffect(void)
{
	//頂点バッファの開放
	if (g_pVtxBuffEffect != NULL)
	{
		g_pVtxBuffEffect->Release();
		g_pVtxBuffEffect = NULL;
	}

	//テクスチャの開放
	if (g_pTextureEffect != NULL)
	{
		g_pTextureEffect->Release();
		g_pTextureEffect = NULL;
	}
}

//=============================================================================
// エフェクトの更新処理
//=============================================================================
void UpdateEffect(void)
{
	VERTEX_2D *pVtx;

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		g_aEffect[nCntEffect].col.a -= 0.04f;

		if (g_aEffect[nCntEffect].col.a <= 0.0f)	//0.0fになったら何もしない
		{
			g_aEffect[nCntEffect].bUse = false;
		}

		//頂点カラーの設定
		pVtx[0].col = g_aEffect[nCntEffect].col;
		pVtx[1].col = g_aEffect[nCntEffect].col;
		pVtx[2].col = g_aEffect[nCntEffect].col;
		pVtx[3].col = g_aEffect[nCntEffect].col;

		//４つの頂点を進める
		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffEffect->Unlock();
}

//=============================================================================
// エフェクトの描画処理
//=============================================================================
void DrawEffect(void)
{
	//変数宣言
	LPDIRECT3DDEVICE9 pDevice;
	int nCntVertex = 0;

	//デバイスの取得
	pDevice = GetDevice();

	//レンダーステートの設定
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);	//ソース(描画元)の合成方法の設定
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);		//デスティネーション(描画先)の合成方法の設定

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffEffect, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureEffect);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == true)
		{
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntVertex, 2);
		}
		nCntVertex += 4;
	}

	//レンダーステートの設定
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		//ソース(描画元)の合成方法の設定
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	//デスティネーション(描画先)の合成方法の設定
}

//=============================================================================
// エフェクトの設定
//=============================================================================
void SetEffect(D3DXVECTOR3 pos, D3DXCOLOR col)
{
	//変数宣言
	VERTEX_2D *pVtx;

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == false)
		{
			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(pos.x - 20.0f, pos.y + 20.0f, 0.0f);	//Zは0.0固定
			pVtx[1].pos = D3DXVECTOR3(pos.x - 20.0f, pos.y - 20.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(pos.x + 20.0f, pos.y + 20.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(pos.x + 20.0f, pos.y - 20.0f, 0.0f);

			g_aEffect[nCntEffect].pos = pos;
			g_aEffect[nCntEffect].col = col;
			g_aEffect[nCntEffect].bUse = true;

			break;
		}
		//４つの頂点を進める
		pVtx += 4;
	}
	//頂点バッファをアンロックする
	g_pVtxBuffEffect->Unlock();
}