//=============================================================================
//
// パーティクルの処理 [particle.cpp]
// Author : Taiki Hayasaka
//
//=============================================================================
#include "particle.h"
#include "manager.h"
#include "renderer.h"
#include "Input_Keyboard.h"
#include "scene.h"
#include "game.h"
#include "player.h"

//=============================================================================
// 静的メンバ変数
//=============================================================================
LPDIRECT3DTEXTURE9 CParticle::m_pTexture = NULL;
D3DXCOLOR CParticle::m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f,1.0f);

//=============================================================================
// コンストラクタ
//=============================================================================
CParticle::CParticle(PRIORITY nPriority) :CScene2D(nPriority)
{
	// 初期化
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	m_life = 0;				// 寿命
	m_fRadius = 0;			// 半径
	m_fInertia = 1.00f;		// 慣性
}

//=============================================================================
// デストラクタ
//=============================================================================
CParticle::~CParticle()
{

}

//=============================================================================
// 生成処理
//=============================================================================
CParticle *CParticle::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 size, D3DXCOLOR col, int nLife, float fInertia)
{
	// ポインタ
	CParticle *pParticle = NULL;

	if (pParticle == NULL)
	{
		// 動的確保
		pParticle = new CParticle;

		if (pParticle != NULL)
		{
			pParticle->Init(pos, move, size, col, nLife, fInertia);

			pParticle->BindTexture(m_pTexture);
		}
	}

	return pParticle;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CParticle::Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 size, D3DXCOLOR col, int nLife, float fInertia)
{
	// メンバ変数の初期化
	m_move = move;
	m_size = size;
	m_life = nLife;
	m_fInertia = fInertia; 
	m_col = col;

	// CScene2Dの初期化処理
	CScene2D::Init(pos, size);

	// オブジェクトの種類設定
	SetObjType(CScene::OBJTYPE_PARTICLE);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CParticle::Uninit(void)
{
	// CScene2Dの終了処理
	CScene2D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CParticle::Update(void)
{
	// 変数宣言
	D3DXVECTOR3 Pos;
	Pos = GetPosition();

	// CInputKeyboardインスタンスの取得
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

	// posにmoveの値を毎秒+する
	Pos += m_move;

	// 慣性
	m_move *= m_fInertia;

	// 座標
	SetPosition(Pos, m_size);

	// エフェクトをだんだん透明にする
	SetEffect(m_col, (float)m_life);

	// ライフ減少
	m_life--;


	//- - - - - - - - - - - - - - - - - - - - - - - - - - -
	// ライフが0以下になったとき
	//- - - - - - - - - - - - - - - - - - - - - - - - - - -
	if (m_life <= 0)
	{
		Uninit();

		return;
	}

	//- - - - - - - - - - - - - - - - - - - - - - - - - -
	// パーティクルが画面外に行ったら消す処理
	//- - - - - - - - - - - - - - - - - - - - - - - - - -
	if (Pos.x - m_size.x / 2.0f < 0.0f || Pos.x + m_size.x / 2.0f > SCREEN_WIDTH ||
		Pos.y - m_size.y / 2.0f < 0.0f || Pos.y + m_size.y / 2.0f > SCREEN_HEIGHT)
	{
		Uninit();

		return;
	}
}

//=============================================================================
// 描画
//=============================================================================
void CParticle::Draw(void)
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

//=============================================================================
// ロード処理
//=============================================================================
HRESULT CParticle::Load(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect000.jpg", &m_pTexture);

	return S_OK;
}

//=============================================================================
// アンロード処理
//=============================================================================
void CParticle::Unload(void)
{
	// テクスチャの破棄
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}