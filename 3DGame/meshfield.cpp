//=============================================================================
//
// ポリゴンの処理 [meshfield.cpp]
// Author:Taiki Hayasaka
//
//=============================================================================
#include "meshfield.h"
#include "input.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MAX_SIZE (800.0f)							//フィールドサイズ
#define MAX_VERTEX (14)								//頂点数
#define MAX_SUEFACE (9)								//面の数

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshfield = NULL;	//頂点バッファへのポインタ
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshfield = NULL;	//インデックスバッファへのポインタ
LPDIRECT3DTEXTURE9	g_pTextureMeshfield = NULL;		//テクスチャへのポインタ
D3DXVECTOR3 g_posMeshfield;							//位置
D3DXVECTOR3 g_rotMeshfield;							//向き
D3DXMATRIX g_mtxWorldMeshfield;						//ワールドマトリックス

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitMeshfield(void)
{
	LPDIRECT3DDEVICE9 pDevice;	//デバイスへのポインタ
	VERTEX_3D *pVtx;			//頂点情報へのポインタ
	WORD *pIdx;					//インデックス情報へのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/block000.jpg", &g_pTextureMeshfield);

	g_posMeshfield = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_rotMeshfield = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * MAX_SUEFACE, 
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMeshfield,
		NULL);

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffMeshfield->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定(Z型)
	pVtx[0].pos = D3DXVECTOR3(g_posMeshfield.x - MAX_SIZE, g_posMeshfield.y, g_posMeshfield.z + MAX_SIZE);
	pVtx[1].pos = D3DXVECTOR3(g_posMeshfield.x, g_posMeshfield.y, g_posMeshfield.z + MAX_SIZE);
	pVtx[2].pos = D3DXVECTOR3(g_posMeshfield.x + MAX_SIZE, g_posMeshfield.y, g_posMeshfield.z + MAX_SIZE);
	pVtx[3].pos = D3DXVECTOR3(g_posMeshfield.x - MAX_SIZE, g_posMeshfield.y, g_posMeshfield.z);
	pVtx[4].pos = D3DXVECTOR3(g_posMeshfield.x, g_posMeshfield.y, g_posMeshfield.z);
	pVtx[5].pos = D3DXVECTOR3(g_posMeshfield.x + MAX_SIZE, g_posMeshfield.y, g_posMeshfield.z);
	pVtx[6].pos = D3DXVECTOR3(g_posMeshfield.x - MAX_SIZE, g_posMeshfield.y, g_posMeshfield.z - MAX_SIZE);
	pVtx[7].pos = D3DXVECTOR3(g_posMeshfield.x, g_posMeshfield.y, g_posMeshfield.z - MAX_SIZE);
	pVtx[8].pos = D3DXVECTOR3(g_posMeshfield.x + MAX_SIZE, g_posMeshfield.y, g_posMeshfield.z - MAX_SIZE);

	//法線ベクトルの設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[4].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[5].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[6].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[7].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[8].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[4].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[5].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[6].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[7].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[8].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(6.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(12.0f, 0.0f);
	pVtx[3].tex = D3DXVECTOR2(0.0f, 6.0f);
	pVtx[4].tex = D3DXVECTOR2(6.0f, 6.0f);
	pVtx[5].tex = D3DXVECTOR2(12.0f, 6.0f);
	pVtx[6].tex = D3DXVECTOR2(0.0f, 12.0f);
	pVtx[7].tex = D3DXVECTOR2(6.0f, 12.0f);
	pVtx[8].tex = D3DXVECTOR2(12.0f, 12.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffMeshfield->Unlock();

	//インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * MAX_VERTEX,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffMeshfield,
		NULL);

	//インデックスバッファをロックし番号データへのポインタを取得
	g_pIdxBuffMeshfield->Lock(0, 0, (void**)&pIdx, 0);

	//番号データの設定
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

	//インデックスバッファをアンロックする
	g_pIdxBuffMeshfield->Unlock();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitMeshfield(void)
{
	//頂点バッファの開放
	if (g_pVtxBuffMeshfield != NULL)
	{
		g_pVtxBuffMeshfield->Release();
		g_pVtxBuffMeshfield = NULL;
	}

	//テクスチャの開放
	if (g_pTextureMeshfield != NULL)
	{
		g_pTextureMeshfield->Release();
		g_pTextureMeshfield = NULL;
	}

	//インデックスバッファの開放
	if (g_pIdxBuffMeshfield != NULL)
	{
		g_pIdxBuffMeshfield->Release();
		g_pIdxBuffMeshfield = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateMeshfield(void)
{

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawMeshfield(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスへのポインタ
	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス

	//デバイスの取得
	pDevice = GetDevice();

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorldMeshfield);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotMeshfield.y, g_rotMeshfield.x, g_rotMeshfield.z);
	D3DXMatrixMultiply(&g_mtxWorldMeshfield, &g_mtxWorldMeshfield, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_posMeshfield.x, g_posMeshfield.y, g_posMeshfield.z);
	D3DXMatrixMultiply(&g_mtxWorldMeshfield, &g_mtxWorldMeshfield, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldMeshfield);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffMeshfield, 0, sizeof(VERTEX_3D));

	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(g_pIdxBuffMeshfield);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureMeshfield);

	//ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, MAX_SUEFACE, 0, 12);
}