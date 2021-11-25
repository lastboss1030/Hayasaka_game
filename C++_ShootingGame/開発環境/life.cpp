//=============================================================================
//
// ライフの処理 [life.cpp]
// Author : taiki hayasaka
//
//=============================================================================
#include "life.h"
#include "manager.h"
#include "renderer.h"
#include "polygon.h"
#include "game.h"
#include "Input_Keyboard.h"
#include "fade.h"

//=============================================================================
// 静的メンバ変数
//=============================================================================
LPDIRECT3DTEXTURE9 CLife::m_pTexture = NULL;

//=============================================================================
// コンストラクタ
//=============================================================================
CLife::CLife(PRIORITY nPriority) : CScene(nPriority)
{
	// 初期化
	int m_nLife = 0;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//=============================================================================
// デストラクタ
//=============================================================================
CLife::~CLife()
{

}

//=============================================================================
// 生成処理
//=============================================================================
CLife *CLife::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	CLife *pLife = NULL;

	if (pLife == NULL)
	{
		 // 動的確保
		pLife = new CLife;

		if (pLife != NULL)
		{
			 // 初期化処理を呼び出す
			pLife->Init(pos, size);
		}
	}
	
	return pLife;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CLife::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// 座標
	m_pos = pos;

	CLife *pLife = NULL;

	// 5つ分並べる処理
	for (int nCnt = 0; nCnt < MAX_LIFE; nCnt++)
	{
		m_apPolygon[nCnt] = CPolygon::Create(D3DXVECTOR3(
			(pos.x - (LIFE_WIDTH * LIFE_HALF) + (LIFE_WIDTH * nCnt)) + (LIFE_WIDTH / 2.0f),
			(pos.y), 0.0f), D3DXVECTOR3(LIFE_WIDTH, LIFE_HEIGHT, 0));

		// テクスチャの割り当て
		m_apPolygon[nCnt]->BindTexture(m_pTexture);
	}

	// 初期ライフ
	m_nLife = MAX_LIFE;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CLife::Uninit(void)
{
	for (int nCnt = 0; nCnt < m_nLife; nCnt++)
	{
		if (m_apPolygon[nCnt] != NULL)
		{
			m_apPolygon[nCnt]->Uninit();
			delete m_apPolygon[nCnt];
			m_apPolygon[nCnt] = NULL;
		}
	}

	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CLife::Update(void)
{

}

//=============================================================================
// 描画処理
//=============================================================================
void CLife::Draw(void)
{
	for (int nCnt = 0; nCnt < m_nLife; nCnt++)
	{
		m_apPolygon[nCnt]->Draw();
	}
}

//=============================================================================
// ライフの設定
//=============================================================================
void CLife::SetLife(int nLife)
{
	// 代入
	m_nLife = nLife;
}

//=============================================================================
//	ライフ加算の処理
// nValue : 加算するライフ
//=============================================================================
void CLife::AddLife(int nValue)
{
	// 加算
	m_nLife += nValue;

	// 5つ分並べる処理
	for (int nCnt = 0; nCnt < MAX_LIFE; nCnt++)
	{
		// NULLだったら
		if (m_apPolygon[nCnt] == NULL)
		{
			// 表示させる
			m_apPolygon[nCnt] = CPolygon::Create(D3DXVECTOR3((m_pos.x - (LIFE_WIDTH * LIFE_HALF) + (LIFE_WIDTH * nCnt)) + (LIFE_WIDTH / 2.0f),
				(m_pos.y), 0.0f), D3DXVECTOR3(LIFE_WIDTH, LIFE_HEIGHT, 0));
		}
	}
}

//=============================================================================
//	ライフ減算の処理
// nValue : 減算するライフ
//=============================================================================
void CLife::SubtractLife(int nValue)
{
	if (m_nLife > 0)
	{
		// ライフが0以上の時のみ
		// 5つ分並べる処理
		for (int nCnt = m_nLife - 1; nCnt > m_nLife - nValue - 1; nCnt--)
		{
			// NULLじゃなかったら
			if (m_apPolygon[nCnt] != NULL)
			{
				m_apPolygon[nCnt]->Uninit();
				delete m_apPolygon[nCnt];
				m_apPolygon[nCnt] = NULL;
			}
		}

		// 減算
		m_nLife -= nValue;
	}
}

//=============================================================================
// ライフを取得
//=============================================================================
int CLife::GetLife(void)
{
	// スコアの情報の先頭アドレスを返す
	return m_nLife;
}

//=============================================================================
// ロード処理
//=============================================================================
HRESULT CLife::Load(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/life.png", &m_pTexture);

	return S_OK;
}

//=============================================================================
// アンロード処理
//=============================================================================
void CLife::Unload(void)
{
	// テクスチャの破棄
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}