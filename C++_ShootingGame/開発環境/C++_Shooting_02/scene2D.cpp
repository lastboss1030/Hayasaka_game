//=============================================================================
//
// オブジェクト処理 [scene.cpp]
// Author : Taiki Hayasaka
//
//=============================================================================
#include "scene2D.h"
#include "manager.h"
#include "renderer.h"
#include "Input_Keyboard.h"

#include "scene.h"
#include "bg.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define FVF_VERTEX_2D (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)	// テクスチャの座標

//=============================================================================
// コンストラクタ
//=============================================================================
CScene2D::CScene2D(PRIORITY nPriority) : CScene(nPriority)
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CScene2D::~CScene2D()
{

}

//=============================================================================
// 初期化
//=============================================================================
HRESULT CScene2D::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	//変数宣言
	VERTEX_2D *pVtx;

	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL)))
	{
		return E_FAIL;
	}

	// 頂点バッファをロックし頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	m_posPolygon = pos;
	m_size = size;

	// 頂点座標の更新
	pVtx[0].pos = D3DXVECTOR3(m_posPolygon.x - size.x / 2.0f, m_posPolygon.y + size.y / 2.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_posPolygon.x + size.x / 2.0f, m_posPolygon.y + size.y / 2.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_posPolygon.x - size.x / 2.0f, m_posPolygon.y - size.y / 2.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_posPolygon.x + size.x / 2.0f, m_posPolygon.y - size.y / 2.0f, 0.0f);

	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CScene2D::Uninit(void)
{
	// 頂点バッファの開放
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// オブジェクトの破棄
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CScene2D::Update(void)
{

}

//=============================================================================
// 描画処理
//=============================================================================
void CScene2D::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//=============================================================================
// 生成
//=============================================================================
CScene2D *CScene2D::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	CScene2D* pScene2D;
	pScene2D = new CScene2D;

	if (pScene2D != NULL)
	{
		pScene2D->Init(pos, size);
	}
	
	return pScene2D;
}

//=============================================================================
// セットポジション
//=============================================================================
void CScene2D::SetPosition(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// 代入
	m_posPolygon = pos;
	m_size = size;

	// 変数宣言
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = D3DXVECTOR3(m_posPolygon.x - size.x / 2.0f, m_posPolygon.y + size.y / 2.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_posPolygon.x - size.x / 2.0f, m_posPolygon.y - size.y / 2.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_posPolygon.x + size.x / 2.0f, m_posPolygon.y + size.y / 2.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_posPolygon.x + size.x / 2.0f, m_posPolygon.y - size.y / 2.0f, 0.0f);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	// 呼び出し
	SetPos(pos);
	SetSize(size);
}

//-------------------------------------------------------------------------------
// プレイヤーの位置を取得
//-------------------------------------------------------------------------------
D3DXVECTOR3 CScene2D::GetPosition(void)
{
	return m_posPolygon;
}

//=============================================================================
// テクスチャ
//=============================================================================
void CScene2D::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	// テクスチャの割り当て
	m_pTexture = pTexture;
}

//=============================================================================
// セットテクスチャ
//=============================================================================
void CScene2D::SetTex(int m_nPatternAnim, int PatterMax)
{
	// 変数宣言
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f + m_nPatternAnim*(1.0f / PatterMax), 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f + m_nPatternAnim*(1.0f / PatterMax), 0.0f);
	pVtx[2].tex = D3DXVECTOR2((1.0f / PatterMax) + m_nPatternAnim*(1.0f / PatterMax), 1.0f);
	pVtx[3].tex = D3DXVECTOR2((1.0f / PatterMax) + m_nPatternAnim*(1.0f / PatterMax), 0.0f);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
// テクスチャ(スクロール)
//=============================================================================
void CScene2D::SetTexBg(float TexU, float TexV)
{
	// 変数宣言
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// スクロールの処理
	pVtx[0].tex = D3DXVECTOR2(0.0f + TexU, 1.0f + TexV);
	pVtx[1].tex = D3DXVECTOR2(0.0f + TexU, 0.0f + TexV);
	pVtx[2].tex = D3DXVECTOR2(1.0f + TexU, 1.0f + TexV);
	pVtx[3].tex = D3DXVECTOR2(1.0f + TexU, 0.0f + TexV);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
// カラーの更新
//=============================================================================
void CScene2D::SetCol(D3DXCOLOR col)
{
	// 変数宣言
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点カラー設定
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
// ポリゴンの透明度(effect)
//=============================================================================
void CScene2D::SetEffect(D3DXCOLOR col, float nData)
{
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(col.r, col.g, col.b, nData / 100);
	pVtx[1].col = D3DXCOLOR(col.r, col.g, col.b, nData / 100);
	pVtx[2].col = D3DXCOLOR(col.r, col.g, col.b, nData / 100);
	pVtx[3].col = D3DXCOLOR(col.r, col.g, col.b, nData / 100);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
//衝突判定
//=============================================================================
bool CScene2D::Collision(D3DXVECTOR3 pos, D3DXVECTOR3 fsize, OBJTYPE Type, CScene **pColScene)
{
	bool bCol = false;
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		for (int nCntScene = 0; nCntScene < MAX_POLYGON; nCntScene++)
		{
			//sceneを取得
			CScene *pScene;
			pScene = CScene::GetScene(nCntPriority, nCntScene);

			if (pScene != NULL)
			{
				//オブジェクトタイプを取得
				OBJTYPE objType;
				objType = pScene->GetObjType();

				if (objType == Type)
				{

					//位置を取得
					D3DXVECTOR3 Epos = pScene->GetPosition();
					//取得した大きさを入れる変数
					D3DXVECTOR3 fSize;

					//サイズを取得
					fSize = pScene->GetSize();

					//当たり判定処理
					if (Epos.y + fSize.x >= pos.y - fsize.y &&  Epos.y - fSize.x <= pos.y + fsize.y&&
						pos.x - fsize.x <= Epos.x + fSize.x && pos.x + fsize.x >= Epos.x - fSize.y)
					{
						*pColScene = pScene;

						bCol = true;
					}
				}
			}
		}
	}
	return bCol;
}