//=============================================================================
//
// 爆発処理 [explosion.cpp]
// Author : Taiki Hayasaka
//
//=============================================================================
#include "explosion.h"
#include "manager.h"
#include "scene2D.h"
#include "renderer.h"
#include "Input_Keyboard.h"

//=============================================================================
// 静的メンバ変数
//=============================================================================
LPDIRECT3DTEXTURE9 CExplosion::m_pTexture = NULL;

//=============================================================================
// コンストラクタ
//=============================================================================
CExplosion::CExplosion(PRIORITY nPriority) :CScene2D(nPriority)
{
	// 初期化
	m_nCountAnim = 0;
	m_nPatternAnim = 0;
	m_colorExplosion = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
}

//=============================================================================
// デストラクタ
//=============================================================================
CExplosion::~CExplosion()
{

}

//=============================================================================
// 初期化
//=============================================================================
HRESULT CExplosion::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 move)
{
	// 初期化
	m_size = size;
	m_move = move;

	// CScene2Dの初期化処理
	CScene2D::Init(pos, size);

	// テクスチャ座標更新
	SetTex(m_nPatternAnim, MAX_ANIMATION_PATTERN);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CExplosion::Uninit(void)
{
	// CScene2Dの終了処理
	CScene2D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CExplosion::Update(void)
{
	// 位置の取得
	D3DXVECTOR3 pos;
	pos = GetPosition();

	// アニメーションカウンター更新
	m_nCountAnim++;

	// 移動量加算
	pos += m_move;

	// 敵の位置情報を2Dポリゴンに渡す
	CScene2D::SetPosition(pos, m_size);

	if (m_nCountAnim % MAX_ANIMATION_COUNTER == 0)
	{
		// パターンNo更新
		m_nPatternAnim += 1;

		// テクスチャ座標更新
		SetTex(m_nPatternAnim, MAX_ANIMATION_PATTERN);

		if (m_nPatternAnim > 7)
		{
			// 終了
			Uninit();
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CExplosion::Draw(void)
{
	// CScene2Dの描画処理
	CScene2D::Draw();
}

//=============================================================================
// 生成処理
//=============================================================================
CExplosion *CExplosion::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 move)
{
	CExplosion *pExplosion = NULL;

	if (pExplosion == NULL)
	{
		// 動的確保
		pExplosion = new CExplosion;

		if (pExplosion != NULL)
		{
			pExplosion->Init(pos, size, move);

			pExplosion->BindTexture(m_pTexture);
		}
	}
	return pExplosion;
}

//=============================================================================
// ロード処理
//=============================================================================
HRESULT CExplosion::Load(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/explosion000.png", &m_pTexture);

	return S_OK;
}

//=============================================================================
// アンロード処理
//=============================================================================
void CExplosion::Unload(void)
{
	// テクスチャの破棄
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}