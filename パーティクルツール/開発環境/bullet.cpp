//=============================================================================
//
// 弾の処理 [bullet.cpp]
// Author : Taiki Hayasaka
//
//=============================================================================
#include "bullet.h"
#include "manager.h"
#include "renderer.h"
#include "Input_Keyboard.h"
#include "scene.h"
#include "effect.h"
#include "game.h"
#include "player.h"
#include "particle.h"

//=============================================================================
// 静的メンバ変数
//=============================================================================
LPDIRECT3DTEXTURE9 CBullet::m_pTexture = NULL;
float g_fSpeed = 0.0f;

//=============================================================================
// コンストラクタ
//=============================================================================
CBullet::CBullet(PRIORITY nPriority) :CScene2D(nPriority)
{
	// 初期化
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_life = 0;
}

//=============================================================================
// デストラクタ
//=============================================================================
CBullet::~CBullet()
{

}

//=============================================================================
// 生成処理
//=============================================================================
CBullet *CBullet::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 size)
{
	// ポインタ
	CBullet *pBullet = NULL;

	if (pBullet == NULL)
	{
		// 動的確保
		pBullet = new CBullet;

		if (pBullet != NULL)
		{
			pBullet->Init(pos, move, size);

			pBullet->BindTexture(m_pTexture);
		}
	}

	return pBullet;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CBullet::Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 size)
{
	// メンバ変数の初期化
	m_move = move;
	m_size = size;

	// 弾のライフ
	m_life = 50;

	// CScene2Dの初期化処理
	CScene2D::Init(pos, size);

	// オブジェクトの種類設定
	SetObjType(CScene::OBJTYPE_BULET);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CBullet::Uninit(void)
{
	// CScene2Dの終了処理
	CScene2D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CBullet::Update(void)
{
	// 変数宣言
	D3DXVECTOR3 Pos;
	Pos = GetPosition();

	// posにmoveの値を毎秒+する
	Pos += m_move;
	SetPosition(Pos, m_size);

	// ライフ減少
	m_life--;

	// 画面と弾の当たり判定
	// 弾が画面外に行ったら消す処理
	if (Pos.x - m_size.x / 2.0f < 0.0f || Pos.x + m_size.x / 2.0f > SCREEN_WIDTH ||
		Pos.y - m_size.y / 2.0f < 0.0f || Pos.y + m_size.y / 2.0f > SCREEN_HEIGHT)
	{
		// 弾を消す
		Uninit();

		return;
	}


	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// 弾のライフが減ったら
	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	if (m_life <= 0)
	{
		// 弾を消す
		Uninit();

		return;
	}
}

//=============================================================================
// 描画
//=============================================================================
void CBullet::Draw(void)
{
	// CScene2Dの描画処理
	CScene2D::Draw();
}

//=============================================================================
// ロード処理
//=============================================================================
HRESULT CBullet::Load(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bullet001.png", &m_pTexture);

	return S_OK;
}

//=============================================================================
// アンロード処理
//=============================================================================
void CBullet::Unload(void)
{
	// テクスチャの破棄
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}