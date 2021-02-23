//=============================================================================
//
// 壁の処理 [meshwall.cpp]
// Author:Taiki Hayasaka
//
//=============================================================================
#include "meshwall.h"
#include "input.h"

//==============================================================================
// マクロ定義
//==============================================================================
#define MAX_WALL	(4)
#define WIDTH_WALL	(2)		// 列数(幅)
#define HEIGHT_WALL	(2)		// 行数(高さ)
#define WALL_SIZE (800.0f)

//==============================================================================
// グローバル変数
//==============================================================================
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshwall = NULL;	// バッファへのポインタ
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshwall = NULL;	// インデックスバッファへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureMeshwall = NULL;		// テクスチャへのポインタ
Meshwall g_aMeshwall[MAX_WALL];
int g_nAllPointWall;								// 総頂点数
int g_nPolygonWall;									// ポリゴン数
int g_nIdxPointWall;								// インデックスバッファの必要な確保数

//==============================================================================
// 初期化処理
//==============================================================================
HRESULT InitMeshwall(void)
{
	// ローカル変数宣言
	VERTEX_3D *pVtx;
	LPDIRECT3DDEVICE9 pDevice;
	WORD *pIdx;
	int nCount = 0;

	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(	pDevice,"data/TEXTURE/tokyo.jpg",	&g_pTextureMeshwall);

	// 変数の初期化
	for (int nCnt = 0; nCnt < MAX_WALL; nCnt++)
	{
		g_aMeshwall[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aMeshwall[nCnt].posMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aMeshwall[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aMeshwall[nCnt].fWidth = 0.0f;
		g_aMeshwall[nCnt].fHeight = 0.0f;
		g_aMeshwall[nCnt].nCntX = WIDTH_WALL + 1;
		g_aMeshwall[nCnt].nCntY = HEIGHT_WALL + 1;
		g_aMeshwall[nCnt].bUse = false;
	}

	g_nAllPointWall = HEIGHT_WALL * 2 * (WIDTH_WALL + 2) - 2;					// 辿る総頂点数
	g_nPolygonWall = WIDTH_WALL * HEIGHT_WALL * 2 + (4 * (HEIGHT_WALL - 1));	// 描画するポリゴン数
	g_nIdxPointWall = (WIDTH_WALL + 1) * (HEIGHT_WALL + 1);						// インデックスバッファでの総頂点数

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * g_nIdxPointWall * MAX_WALL,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMeshwall,
		NULL);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffMeshwall->Lock(0, 0, (void**)&pVtx, 0);


	for (int nCnt = 0; nCnt < MAX_WALL; nCnt++, pVtx += 9)
	{
		// ポリゴンの各頂点座標
		pVtx[0].pos = D3DXVECTOR3(g_aMeshwall[nCnt].pos.x - WALL_SIZE, g_aMeshwall[nCnt].pos.y + WALL_SIZE, g_aMeshwall[nCnt].pos.z);
		pVtx[1].pos = D3DXVECTOR3(g_aMeshwall[nCnt].pos.x, g_aMeshwall[nCnt].pos.y + 200.0f, g_aMeshwall[nCnt].pos.z);
		pVtx[2].pos = D3DXVECTOR3(g_aMeshwall[nCnt].pos.x + WALL_SIZE, g_aMeshwall[nCnt].pos.y + WALL_SIZE, g_aMeshwall[nCnt].pos.z);
		pVtx[3].pos = D3DXVECTOR3(g_aMeshwall[nCnt].pos.x - WALL_SIZE, g_aMeshwall[nCnt].pos.y + WALL_SIZE, g_aMeshwall[nCnt].pos.z);
		pVtx[4].pos = D3DXVECTOR3(g_aMeshwall[nCnt].pos.x, g_aMeshwall[nCnt].pos.y + 100.0f, g_aMeshwall[nCnt].pos.z);
		pVtx[5].pos = D3DXVECTOR3(g_aMeshwall[nCnt].pos.x + WALL_SIZE, g_aMeshwall[nCnt].pos.y + WALL_SIZE, g_aMeshwall[nCnt].pos.z);
		pVtx[6].pos = D3DXVECTOR3(g_aMeshwall[nCnt].pos.x - WALL_SIZE, g_aMeshwall[nCnt].pos.y, g_aMeshwall[nCnt].pos.z);
		pVtx[7].pos = D3DXVECTOR3(g_aMeshwall[nCnt].pos.x, g_aMeshwall[nCnt].pos.y,	g_aMeshwall[nCnt].pos.z);
		pVtx[8].pos = D3DXVECTOR3(g_aMeshwall[nCnt].pos.x + WALL_SIZE, g_aMeshwall[nCnt].pos.y, g_aMeshwall[nCnt].pos.z);

		// 法線ベクトルの設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[4].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[5].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[6].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[7].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[8].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		// 各頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[4].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[5].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[6].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[7].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[8].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// テクスチャ頂点情報の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(2.0f, 0.0f);
		pVtx[3].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[4].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[5].tex = D3DXVECTOR2(2.0f, 1.0f);
		pVtx[6].tex = D3DXVECTOR2(0.0f, 2.0f);
		pVtx[7].tex = D3DXVECTOR2(1.0f, 2.0f);
		pVtx[8].tex = D3DXVECTOR2(2.0f, 2.0f);
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffMeshwall->Unlock();

	// インデックスバッファの生成
	pDevice->CreateIndexBuffer(
		sizeof(WORD) * g_nAllPointWall * MAX_WALL,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffMeshwall,
		NULL);

	// インデックスバッファをロックし、番号データへのポインタを取得
	g_pIdxBuffMeshwall->Lock(0, 0, (void**)&pIdx, 0);

	pIdx[0] = 3;
	pIdx[1] = 0;
	pIdx[2] = 4;
	pIdx[3] = 1;
	pIdx[4] = 5;
	pIdx[5] = 2;
	pIdx[6] = 2;
	pIdx[7] = 6;
	pIdx[8] = 6;
	pIdx[9] = 3;
	pIdx[10] = 7;
	pIdx[11] = 4;
	pIdx[12] = 8;
	pIdx[13] = 5;

	// 壁の設定


	// インデックスバッファをアンロックする
	g_pIdxBuffMeshwall->Unlock();

	return S_OK;
}

//==============================================================================
// 終了処理
//==============================================================================
void UninitMeshwall(void)
{
	// 頂点バッファの開放
	if (g_pVtxBuffMeshwall != NULL)
	{
		g_pVtxBuffMeshwall->Release();
		g_pVtxBuffMeshwall = NULL;
	}

	// テクスチャの開放
	if (g_pTextureMeshwall != NULL)
	{
		g_pTextureMeshwall->Release();
		g_pTextureMeshwall = NULL;
	}

	// インデックスバッファの開放
	if (g_pIdxBuffMeshwall != NULL)
	{
		g_pIdxBuffMeshwall->Release();
		g_pIdxBuffMeshwall = NULL;
	}
}

//==============================================================================
// 更新処理
//==============================================================================
void UpdateMeshwall(void)
{
	// ローカル変数宣言
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffMeshwall->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntMax = 0; nCntMax < MAX_WALL; nCntMax++)
	{
		for (int nCntWall = 0; nCntWall < g_aMeshwall[nCntMax].nCntY; nCntWall++)
		{
			for (int nCnt = 0; nCnt < g_aMeshwall[nCntMax].nCntX; nCnt++, pVtx++)
			{
				if (g_aMeshwall[nCntMax].bUse == true)
				{
					// ポリゴンの各頂点座標
					pVtx[0].pos = D3DXVECTOR3(
						g_aMeshwall[nCntMax].posMove.x - g_aMeshwall[nCntMax].fWidth + g_aMeshwall[nCntMax].fWidth * nCnt * 2 / WIDTH_WALL,
						g_aMeshwall[nCntMax].posMove.y + g_aMeshwall[nCntMax].fHeight * 2 - g_aMeshwall[nCntMax].fHeight * nCntWall * 2 / HEIGHT_WALL,
						g_aMeshwall[nCntMax].posMove.z);

					// テクスチャ頂点情報の設定
					pVtx[0].tex = D3DXVECTOR2(0.5f * nCnt, 0.5f * nCntWall);
				}
			}
		}
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffMeshwall->Unlock();
}

//==============================================================================
// 描画処理
//==============================================================================
void DrawMeshwall(void)
{
	// ローカル変数宣言
	LPDIRECT3DDEVICE9 pDevice;		// デバイスのポインタ
	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス

	// デバイスの取得
	pDevice = GetDevice();

	for (int nCnt = 0; nCnt < MAX_WALL; nCnt++)
	{
		if (g_aMeshwall[nCnt].bUse == true)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aMeshwall[nCnt].mtxWorld);

			//ライティングを無効にする
			pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

			// 向きの反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot,g_aMeshwall[nCnt].rot.y,g_aMeshwall[nCnt].rot.x,g_aMeshwall[nCnt].rot.z);
			D3DXMatrixMultiply(&g_aMeshwall[nCnt].mtxWorld,&g_aMeshwall[nCnt].mtxWorld,&mtxRot);

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans,g_aMeshwall[nCnt].pos.x,g_aMeshwall[nCnt].pos.y,g_aMeshwall[nCnt].pos.z);
			D3DXMatrixMultiply(&g_aMeshwall[nCnt].mtxWorld,&g_aMeshwall[nCnt].mtxWorld,&mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aMeshwall[nCnt].mtxWorld);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffMeshwall, 0, sizeof(VERTEX_3D));

			// インデックスバッファをデータストリームに設定
			pDevice->SetIndices(g_pIdxBuffMeshwall);

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureMeshwall);

			// ポリゴンの描画
			pDevice->DrawIndexedPrimitive(
				D3DPT_TRIANGLESTRIP,		// プリミティブの種類
				0,
				0,
				g_nIdxPointWall,				// 頂点数
				0,
				g_nPolygonWall);			// プリミティブ数

			//ライティングを無効にする
			pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
		}
	}
}

//==============================================================================
//	メッシュ壁の設定
//==============================================================================
void SetMeshwall(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight)
{
	// ローカル変数宣言
	Meshwall *pMeshwall;
	pMeshwall = &g_aMeshwall[0];

	// 壁の設定
	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++, pMeshwall++)
	{
		if (pMeshwall->bUse == false)
		{
			pMeshwall->posMove = pos;		//位置

			pMeshwall->rot = rot;			//向き

			pMeshwall->fWidth = fWidth;		//幅

			pMeshwall->fHeight = fHeight;	//高さ

			pMeshwall->bUse = true;			//使用状態

			break;
		}
	}
}