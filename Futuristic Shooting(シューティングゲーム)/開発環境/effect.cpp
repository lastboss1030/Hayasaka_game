//=============================================================================
//
// エフェクトの処理 [effect.cpp]
// Author : Taiki Hayasaka
//
//=============================================================================
#include "effect.h" 
#include "renderer.h"
#include "manager.h"
#include "game.h"
#include "enemy.h"
#include "bullet.h"

//=============================================================================
// 静的メンバ変数
//=============================================================================
LPDIRECT3DTEXTURE9 CEffect::m_pTexture = NULL;			// テクスチャ

//=============================================================================
// テクスチャ読込処理
//=============================================================================
HRESULT CEffect::Load(void)
{
	// デバイスの設定
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect000.jpg", &m_pTexture);

	return S_OK;
}

//=============================================================================
// テクスチャ破棄処理
//=============================================================================
void CEffect::Unload(void)
{
	// テクスチャの開放
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================================================================
// コンストラクタ
//=============================================================================
CEffect::CEffect(PRIORITY nPriority) : CScene2D(nPriority)
{
	// 初期化
	m_nLife = 70;
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//=============================================================================
// デストラクタ
//=============================================================================
CEffect::~CEffect()
{


}

//=============================================================================
// 生成処理
//=============================================================================
CEffect *CEffect::Create(D3DXVECTOR3 pos, int nLife, D3DXVECTOR3 size, D3DXCOLOR col)
{
	CEffect *pEffect = NULL;
	pEffect = new CEffect;

	if (pEffect != NULL)
	{
		pEffect->Init(pos, nLife ,size, col);
	}
	pEffect->BindTexture(m_pTexture);

	return pEffect;
}

//=============================================================================
// ポリゴンの初期化処理
//=============================================================================
HRESULT CEffect::Init(D3DXVECTOR3 pos, int nLife, D3DXVECTOR3 size, D3DXCOLOR col)
{
	// カラーを設定
	m_col = col;

	// 大きさを設定
	m_size = size;

	// ライフの設定
	m_nLife = nLife;

	// CScene2Dを呼び出し
	CScene2D::Init(pos, size);

	// オブジェクトの種類の設定
	SetObjType(OBJTYPE_EFFECT);

	SetEffect(col, 1);

	return S_OK;
}

//=============================================================================
// ポリゴンの終了処理
//=============================================================================
void CEffect::Uninit()
{
	CScene2D::Uninit();
}

//=============================================================================
// ポリゴンの更新処理
//=============================================================================
void CEffect::Update()
{
	// 現在の位置の取得
	D3DXVECTOR3 pos = GetPosition();

	// 更新
	CScene2D::Update();

	// ライフを減らす
	m_nLife--;

	// エフェクトをだんだん透明にする
	SetEffect(m_col, (float)m_nLife);

	// ライフが0以下になったとき
	if (m_nLife <= 0)
	{
		Uninit();
	}

	// 範囲外の場合
	else if (pos.y <= 0)
	{
		Uninit();
	}
	else if (pos.y >= SCREEN_HEIGHT)
	{
		Uninit();
	}
	else if (pos.x <= 0)
	{
		Uninit();
	}
	else if (pos.x >= SCREEN_WIDTH)
	{
		Uninit();
	}
}

//=============================================================================
// ポリゴンの描画処理
//=============================================================================
void CEffect::Draw()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();

	// 加算合成
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	CScene2D::Draw();

	// 加算合成を外す
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

