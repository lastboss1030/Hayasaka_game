//=============================================================================
//
// オブジェクトの処理 [object.h]
// Author:Taiki Hayasaka
//
//=============================================================================
#include "object.h"
#include "input.h"
#include "camera.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define TEX (10)

//==============================================================================
// グローバル変数
//==============================================================================
LPD3DXMESH g_pMeshObject = NULL;			// メッシュ(頂点情報)へのポインタ
LPD3DXBUFFER g_pBuffMatObject = NULL;		// マテリアル(材質情報)へのポインタ
DWORD g_nNumMatObject = 0;					// マテリアルの数					
LPDIRECT3DTEXTURE9 g_apTextureObject[TEX] = {};	//テクスチャへのポインタ
Object g_object;

//==============================================================================
// 初期化処理
//==============================================================================
HRESULT InitObject(void)
{
	//ローカル変数宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		// デバイスの取得

	//Xファイルの読み込み
	D3DXLoadMeshFromX(
		"data/MODEL/robot000.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_pBuffMatObject,
		NULL,
		&g_nNumMatObject,
		&g_pMeshObject);

	//変数宣言
	int nNumVtx;	// 頂点数
	DWORD sizeFVF;	// 頂点フォーマットのサイズ
	BYTE *pVtxBuff;	// 頂点バッファへのポインタ

	//頂点数を取得
	nNumVtx = g_pMeshObject->GetNumVertices();

	//頂点フォーマットのサイズの取得
	sizeFVF = D3DXGetFVFVertexSize(g_pMeshObject->GetFVF());

	//変数の初期化
	g_object.pos = D3DXVECTOR3(100.0f, 0.0f, 100.0f);
	g_object.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_object.rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_object.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//頂点バッファをロック
	g_pMeshObject->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	//頂点座標の比較
	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;	// 頂点座標の代入

		// 各座標の最大値の比較
		if (g_object.vtxMaxObject.x < vtx.x)
		{
			g_object.vtxMaxObject.x = vtx.x;
		}
		if (g_object.vtxMaxObject.y < vtx.y)
		{
			g_object.vtxMaxObject.y = vtx.y;
		}
		if (g_object.vtxMaxObject.z < vtx.z)
		{
			g_object.vtxMaxObject.z = vtx.z;
		}

		//各座標の最小値の比較
		if (g_object.vtxMinObject.x > vtx.x)
		{
			g_object.vtxMinObject.x = vtx.x;
		}
		if (g_object.vtxMinObject.y > vtx.y)
		{
			g_object.vtxMinObject.y = vtx.y;
		}
		if (g_object.vtxMinObject.z > vtx.z)
		{
			g_object.vtxMinObject.z = vtx.z;
		}

		pVtxBuff += sizeFVF;	//頂点フォーマットのサイズ分ポインタを進める
	}

	//頂点バッファをアンロック
	g_pMeshObject->UnlockVertexBuffer();

	return S_OK;
}

//==============================================================================
// 終了処理
//==============================================================================
void UninitObject(void)
{
	//メッシュの破棄
	if (g_pMeshObject != NULL)
	{
		g_pMeshObject->Release();
		g_pMeshObject = NULL;
	}

	//マテリアルの破棄
	if (g_pBuffMatObject != NULL)
	{
		g_pBuffMatObject->Release();
		g_pBuffMatObject = NULL;
	}
}

//==============================================================================
// 更新処理
//==============================================================================
void UpdateObject(void)
{

}

//==============================================================================
// 描画処理
//==============================================================================
void DrawObject(void)
{
	//ローカル変数宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;				// 計算用マトリックス
	D3DMATERIAL9 matDef;						// 現在のマテリアル保存用
	D3DXMATERIAL *pMat;							// マテリアルデータへのポインタ

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_object.mtxWorld);

	//向きの反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_object.rot.y, g_object.rot.x, g_object.rot.z);
	D3DXMatrixMultiply(&g_object.mtxWorld, &g_object.mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_object.pos.x, g_object.pos.y, g_object.pos.z);
	D3DXMatrixMultiply(&g_object.mtxWorld, &g_object.mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_object.mtxWorld);

	//現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	//マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)g_pBuffMatObject->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_nNumMatObject; nCntMat++)
	{
		//マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		//テクスチャの設定
		pDevice->SetTexture(0, NULL);

		//モデル(パーツ)の描画
		g_pMeshObject->DrawSubset(nCntMat);
	}

	//保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}
//=============================================================================
// オブジェクトの取得
//=============================================================================
Object *GetObject(void)
{
	return &g_object;
}