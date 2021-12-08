//=============================================================================
//
//	ポリゴン処理 [polygon.cpp]
//	Author:Taiki Haysaka
//
//=============================================================================
#include "polygon.h"
#include "manager.h"
#include "renderer.h"
#include "scene2D.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define FVF_VERTEX_2D (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)

//=============================================================================
// コンストラクタ
//=============================================================================
CPolygon::CPolygon()
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CPolygon::~CPolygon()
{

}

//=============================================================================
// ナンバーの生成
//=============================================================================
CPolygon *CPolygon::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	CPolygon *pPolygon = NULL;

	if (pPolygon == NULL)
	{
		 // 動的確保
		pPolygon = new CPolygon;

		if (pPolygon != NULL)
		{
			pPolygon->Init(pos, size);
		}
	}

	return pPolygon;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CPolygon::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// 変数宣言
	VERTEX_2D *pVtx;

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer
	(sizeof(VERTEX_2D) * 4,					// 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,						// 頂点フォーマット
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL)))
	{
		return E_FAIL;
	}

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標
	pVtx[0].pos = D3DXVECTOR3(pos.x - size.x / 2.0f, pos.y + size.y / 2.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(pos.x + size.x / 2.0f, pos.y + size.y / 2.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(pos.x - size.x / 2.0f, pos.y - size.y / 2.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(pos.x + size.x / 2.0f, pos.y - size.y / 2.0f, 0.0f);

	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラー設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	// 頂点情報の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// ポリゴンの終了処理
//=============================================================================
void CPolygon::Uninit(void)
{
	// 頂点バッファの開放
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

//=============================================================================
// ポリゴンの更新処理
//=============================================================================
void CPolygon::Update(void)
{
}

//=============================================================================
// ポリゴンの描画処理
//=============================================================================
void CPolygon::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// 頂点バッファをデータストーリに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	// テクスチャの描画
	pDevice->DrawPrimitive
	(D3DPT_TRIANGLESTRIP,		// プリミティブの種類
		0,						// 描画を開始する頂点インデックス
		2);						// 描画するプリミティブ数
}

//=============================================================================
// ポリゴンの設定
//=============================================================================
void CPolygon::SetPolygon(int nNumber)
{
	// 変数宣言
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ナンバーの処理
	pVtx[0].tex = D3DXVECTOR2(0.0f + (0.1f*nNumber), 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f + (0.1f*nNumber), 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.1f + (0.1f*nNumber), 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.1f + (0.1f*nNumber), 0.0f);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
// テクスチャ割り当て処理
//=============================================================================
void CPolygon::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}