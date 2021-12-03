//=============================================================================
//
// アニメーション処理 [animasion.cpp]
// Author : taiki hayasaka
//
//=============================================================================
#include "animation.h"
#include "manager.h"
#include "renderer.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CAnimation::m_pTexture[MAX_ANIMATION] = {};
int CAnimation::m_nUninitType = NULL;
int g_nUninitType;
CPlayer *CAnimation::m_pPlayer = NULL;

//==============================================================================
// コンストラクタ
//==============================================================================
CAnimation::CAnimation(PRIORITY nPriority) : CScene2D(nPriority)
{
	m_nAnime = 0;
	m_nPatternAnime = 0;
}

//==============================================================================
// デストラクタ
//==============================================================================
CAnimation::~CAnimation()
{

}

//==============================================================================
// 初期化処理
//==============================================================================
HRESULT CAnimation::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, int nAnime, int nType)
{
	CScene2D::Init(pos, size);

	// 代入
	m_pos = pos;
	m_nType = nType;

	// アニメーションの初期化
	CScene2D::SetTex(1, 0);

	m_nAnime = nAnime;

	if (nType == 0)
	{
		m_fAnimeX = 0.2f;
		m_fAnimeY = 0.5f;
		m_nX = 5;
		m_nY = 2;
		m_floop = 1;
	}

	if (nType == 1)
	{
		m_fAnimeX = 1.0f;
		m_fAnimeY = 0.090909f;
		m_nX = 1;
		m_nY = 11;
		m_floop = 1;
	}

	return S_OK;
}

//==============================================================================
// 終了処理
//==============================================================================
void CAnimation::Uninit(void)
{
	CScene2D::Uninit();
}

//==============================================================================
// 更新処理
//==============================================================================
void CAnimation::Update(void)
{
	// アニメーションの更新条件
	if ((m_nCounterAnime % m_nAnime) == 0)
	{
		// アニメーションパターンの更新
		m_nPatternAnime = (m_nPatternAnime + 1);

		// アニメーションの更新
		CScene2D::SetAnimation(m_fAnimeX, m_fAnimeY, m_nX, m_nY, m_nPatternAnime);
	}

	// カウンターを進める
	m_nCounterAnime++;

	// 更新
	CScene2D::Update();

	// アニメーションを終了リセット
	if (m_nPatternAnime == m_nX * m_nY)
	{
		// カウンターのリセット
		m_nPatternAnime = 0;

		// ループさせないとき
		if (m_floop == 0)
		{
			// 終了される
			Uninit();
		}
	}
}

//==============================================================================
// 描画処理
//==============================================================================
void CAnimation::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// アルファテスト
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, 0);

	// 描画
	CScene2D::Draw();

	// アルファテスト
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0x00);
}

//==============================================================================
// 生成処理
//==============================================================================
CAnimation * CAnimation::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, int nAnime, int nType)
{
	// アニメーション
	CAnimation *pAnimation = NULL;
	pAnimation = new CAnimation;

	// NULLチェック
	if (pAnimation != NULL)
	{
		pAnimation->Init(pos, size, nAnime, nType);
	}

	// テクスチャを入れる
	pAnimation->BindTexture(m_pTexture[nType]);

	return pAnimation;
}

//==============================================================================
// テクスチャの読み込み
//==============================================================================
HRESULT CAnimation::Load(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ptranp.png", &m_pTexture[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/magic.png", &m_pTexture[1]);

	return S_OK;
}

//==============================================================================
// テクスチャの破棄
//==============================================================================
void CAnimation::Unload(void)
{
	for (int nCnt = 0; nCnt < MAX_ANIMATION; nCnt++)
	{
		if (m_pTexture[nCnt] != NULL)
		{
			m_pTexture[nCnt]->Release();
			m_pTexture[nCnt] = NULL;
		}
	}
}

//==============================================================================
// 破棄するタイプ
//==============================================================================
void CAnimation::UninitType(int nUninitType)
{
	g_nUninitType = nUninitType;
}

//==============================================================================
// 廃棄したグローバルを初期化
//==============================================================================
void CAnimation::UninitTypeClear(void)
{
	g_nUninitType = 0;
}