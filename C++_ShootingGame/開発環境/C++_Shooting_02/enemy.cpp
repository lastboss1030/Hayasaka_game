//=============================================================================
//
// 敵処理 [enemy.cpp]
// Author : Taiki hayasaka
//
//=============================================================================
#include "enemy.h"
#include "manager.h"
#include "renderer.h"
#include "bullet.h"
#include "scene2D.h"
#include "player.h"	
#include "game.h"	
#include "sound.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CEnemy::m_apTexture = {};
int CEnemy::m_enmeyCnt = 0;

//=============================================================================
// CEnemyのコンストラクタ
//=============================================================================
CEnemy::CEnemy(PRIORITY nPriority) :CScene2D(nPriority)
{
	// 初期化
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Color = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_nCounter = 0;
	m_nCntColor = 0;
	m_nLife = 0;

	m_Enemytype = ENEMYTYPE_NONE;
	m_enmeyCnt++;
}

//=============================================================================
// CEnemyのデストラクタ
//=============================================================================
CEnemy::~CEnemy()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CEnemy::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 move, D3DXVECTOR3 speed, ENEMYTYPE type, int nLife)
{
	//初期化
	m_size = size;
	m_move = move;
	m_Enemytype = type;
	m_nLife = nLife;

	// CScene2Dの初期化処理
	CScene2D::Init(pos, size);

	// オブジェクトの種類設定
	SetObjType(CScene::OBJTYPE_ENEMY);

	// 敵の状態を通常にする
	SetState(ENEMYSTATE::ENEMY_NORMAL);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CEnemy::Uninit(void)
{
	// CScene2Dの終了処理
	CScene2D::Uninit();
	m_enmeyCnt--;
}

//=============================================================================
// 更新処理
//=============================================================================
void CEnemy::Update(void)
{
	// 位置の取得
	D3DXVECTOR3 pos;
	pos = GetPosition();

	// サウンド関係
	CSound *pSound;
	pSound = CManager::GetSound();

	// プレイヤーの移動量取得
	D3DXVECTOR3 PlayerMove;
	CPlayer *pPlayer;
	pPlayer = CGame::GetPlayer();
	PlayerMove = pPlayer->GetMove();

	// 移動量加算
	pos += m_move;

	// 一定の位置まで来たら止める
	if (pos.y >= 250 && m_Enemytype == ENEMYTYPE_STOP)
	{
		pos.y -= 2;
	}

	// 敵の移動
	MoveEnemy();

	// 敵の位置情報を2Dポリゴンに渡す
	CScene2D::SetPosition(pos, m_size);

	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// 敵の状態
	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	switch (m_State)
	{
	// ノーマル状態
	case ENEMY_NORMAL:
		// 通常色
		m_Color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		break;

	// ダメージ状態
	case ENEMY_DAMAGE:
		// 赤くする
		m_Color = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

		// 当たってからのカウント増やす
		m_nHitCnt++;
		if (m_nHitCnt >= 10)
		{
			// ノーマルに戻す
			m_nHitCnt = 0;
			m_State = ENEMY_NORMAL;
		}

		break;
	}

	// 色チェンジ
	CScene2D::SetCol(m_Color);

	// 画面外に行ったら
	if (pos.x - m_size.x / 2.0f < 0.0f || pos.x + m_size.x / 2.0f > SCREEN_WIDTH ||  pos.y - m_size.y / 2.0f > SCREEN_HEIGHT)
	{
		// ライフを減らす
		m_nLife -= ENEMY_LIFE;
	}

	// 敵の弾を出す
	m_nShotCnt++;

	if (m_nShotCnt == 70)
	{
		// カウント戻す
		m_nShotCnt = 0;

		// 発射音
		pSound->Play(CSound::SOUND_LABEL_SE_SHOTENEMY);

		// 弾の生成
		CBullet::Create(D3DXVECTOR3(pos.x + 15.0f, pos.y + 10.0f, 1.0f), D3DXVECTOR3(0.0f, 8.0f, 0.0f), D3DXVECTOR3(15.0f, 15.0f, 0.0f), CBullet::BULLETTYPE_ENEMY, CBullet::ATTACKTYPE_NORMAL, 1);
		CBullet::Create(D3DXVECTOR3(pos.x - 15.0f, pos.y + 10.0f, 1.0f), D3DXVECTOR3(0.0f, 8.0f, 0.0f), D3DXVECTOR3(15.0f, 15.0f, 0.0f), CBullet::BULLETTYPE_ENEMY, CBullet::ATTACKTYPE_NORMAL, 1);
	}

	// 敵のライフが0になったら消す
	if (m_nLife <= 0)
	{
		Uninit();
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CEnemy::Draw(void)
{
	// CScene2Dの描画処理
	CScene2D::Draw();
}

//=============================================================================
// テクスチャのロード処理
//=============================================================================
HRESULT CEnemy::Load(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/enemy100.png", &m_apTexture);

	return S_OK;
}

//=============================================================================
// テクスチャの破棄
//=============================================================================
void CEnemy::Unload(void)
{
	// テクスチャの開放
	if (m_apTexture != NULL)
	{
		m_apTexture->Release();
		m_apTexture = NULL;
	}
}

//=============================================================================
// 生成処理
//=============================================================================
CEnemy *CEnemy::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 move, D3DXVECTOR3 speed, ENEMYTYPE type, int nLife)
{
	// 変数宣言
	CEnemy *pEnemy = NULL;

	if (pEnemy == NULL)
	{
		// 動的確保
		pEnemy = new CEnemy;

		if (pEnemy != NULL)
		{
			// 初期化処理を呼び出す
			pEnemy->Init(pos, size, move, speed, type, nLife);

			// 敵のテクスチャを割り当てる
			pEnemy->BindTexture(m_apTexture);
		}
	}
	// 値を返す
	return pEnemy;
}

//=============================================================================
// 敵の移動管理
//=============================================================================
void CEnemy::MoveEnemy(void)
{
	// 位置の取得
	D3DXVECTOR3 pos;
	pos = GetPosition();


	// 敵の位置情報を2Dポリゴンに渡す
	CScene2D::SetPosition(pos, m_size);
}

//=============================================================================
// 当たり判定
//=============================================================================
bool CEnemy::HitEnemy(int nDamage)
{
	// サウンド関係
	CSound *pSound;
	pSound = CManager::GetSound();

	// ライフ減少
	m_nLife -= nDamage;

	// ダメージ判定
	m_State = ENEMY_DAMAGE;

	// ヒット音
	pSound->Play(CSound::SOUND_LABEL_SE_EXPLOSION);

	return false;
}