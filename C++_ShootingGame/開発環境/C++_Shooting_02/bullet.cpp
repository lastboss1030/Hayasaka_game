//=============================================================================
//
// 弾の処理 [bullet.cpp]
// Author : Taiki Hayasaka
//
//=============================================================================
#include "bullet.h"
#include "manager.h"
#include "renderer.h"
#include "explosion.h"
#include "sound.h"
#include "score.h"
#include "life.h"
#include "Input_Keyboard.h"
#include "enemy.h"
#include "scene.h"
#include "effect.h"
#include "game.h"
#include "player.h"
#include "boss_parts.h"

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
	m_bulletType = BULLETTYPE_NONE;
	m_life = 0;
	m_nCntHoming = 0;
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
CBullet *CBullet::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 size, BULLETTYPE type, ATTACKTYPE AttackType, int nDamage)
{
	// ポインタ
	CBullet *pBullet = NULL;

	if (pBullet == NULL)
	{
		// 動的確保
		pBullet = new CBullet;

		if (pBullet != NULL)
		{
			pBullet->Init(pos, move, size, type, nDamage);

			pBullet->BindTexture(m_pTexture);
		}
	}

	// 攻撃種類情報の呼び出し
	pBullet->SetAttackType(AttackType);

	return pBullet;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CBullet::Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 size, BULLETTYPE type, int nDamage)
{
	// メンバ変数の初期化
	m_move = move;
	m_size = size;
	m_bulletType = type;
	m_nDamage = nDamage;

	// 弾のライフ設定
	if (m_bulletType != BULLETTYPE_ENEMYHOMING)
	{
		// 追尾以外の弾
		m_life = 40;
	}
	else
	{
		// 追尾の弾
		m_life = 140;
	}


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

	// サウンド関係
	CSound *pSound;
	pSound = CManager::GetSound();


	// 追尾カウント
	m_nCntHoming++;

	// 攻撃タイプが追尾だったら
	if (m_AttackType == ATTACKTYPE_HOMING)
	{
		// 追尾処理
		HomingBullet();
	}

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
		// 爆発の生成
		CExplosion::Create(D3DXVECTOR3(Pos.x, Pos.y, 0.0f), D3DXVECTOR3(50, 50, 0), D3DXVECTOR3(0, 0, 0));

		// 弾を消す
		Uninit();

		return;
	}


	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// エフェクトを生成
	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	if (m_bulletType == BULLETTYPE_PLAYER)
	{
		// プレイヤーの弾
		CEffect::Create(Pos, 50, m_size*1.8f, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f));
		CEffect::Create(Pos, 50, m_size*1.8f, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f));
	}
	else if (m_bulletType == BULLETTYPE_ENEMY)
	{
		// 敵の弾
		CEffect::Create(Pos, 20, m_size * 2, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
		CEffect::Create(Pos, 20, m_size * 2, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
	}
	else if (m_bulletType == BULLETTYPE_BOSSPARTS)
	{
		// ボスの弾
		CEffect::Create(Pos, 20, m_size * 2, D3DXCOLOR(0.5f, 1.0f, 1.0f, 1.0f));
	}
	else if (m_bulletType == BULLETTYPE_ENEMYHOMING)
	{
		// ボスの弾(追尾)
		CEffect::Create(Pos, 40, m_size * 2, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		CEffect::Create(Pos, 40, m_size * 2, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	}


	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// 当たり判定
	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		for (int nCntScene = 0; nCntScene < MAX_POLYGON; nCntScene++)
		{
			// シーン
			CScene *pScene;
			pScene = CScene::GetScene(nCntPriority, nCntScene);

			if (pScene != NULL)
			{
				// オブジェタイプ
				CScene::OBJTYPE objType;
				objType = pScene->GetObjType();

				// 弾種類
				CBullet::BULLETTYPE bulletType = m_bulletType;

				// プレイヤー
				int nPlayerState = CPlayer::GetPlayerState();
				CPlayer *pPlayer;
				pPlayer = CGame::GetPlayer();

				// 敵だったら
				if (objType == CScene::OBJTYPE_ENEMY && m_bulletType == BULLETTYPE_PLAYER)
				{
					// 変数宣言
					D3DXVECTOR3 PosEnemy;
					D3DXVECTOR3 EnemySize;

					// 敵の位置とサイズを取得
					PosEnemy = pScene->GetPos();
					EnemySize = pScene->GetSize();

					// 弾と敵の当たり判定
					if (Pos.x - (m_size.x / 2) < PosEnemy.x + (EnemySize.x / 2) &&
						Pos.x + (m_size.x / 2) > PosEnemy.x - (EnemySize.x / 2) &&
						Pos.y - (m_size.y / 2) < PosEnemy.y + (EnemySize.y / 2) &&
						Pos.y + (m_size.y / 2) > PosEnemy.y - (EnemySize.y / 2))
					{
						// 爆発の生成
						CExplosion::Create(PosEnemy, D3DXVECTOR3(50, 50, 0), D3DXVECTOR3(0, 0, 0));

						// ライフ減少
						CEnemy *pEnemy = (CEnemy*)pScene;
						pEnemy->HitEnemy(1);

						// スコアの加算
						CScore::AddScore(200);

						// 弾の破棄
						Uninit();

						return;
					}
				}

				// ボスだったら
  				if (objType == CScene::OBJTYPE_BOSS && m_bulletType == BULLETTYPE_PLAYER)
				{
					// 変数宣言
					D3DXVECTOR3 PosParts;
					D3DXVECTOR3 PartsSize;

					// 敵の位置とサイズを取得
					PosParts = pScene->GetPos();
					PartsSize = pScene->GetSize();

					// 弾と敵の当たり判定
					if (Pos.x - (m_size.x / 2) < PosParts.x + (PartsSize.x / 2) &&
						Pos.x + (m_size.x / 2) > PosParts.x - (PartsSize.x / 2) &&
						Pos.y - (m_size.y / 2) < PosParts.y + (PartsSize.y / 2) &&
						Pos.y + (m_size.y / 2) > PosParts.y - (PartsSize.y / 2))
					{
						// 爆発の生成
						CExplosion::Create(D3DXVECTOR3(Pos.x, Pos.y, 0.0f), D3DXVECTOR3(50, 50, 0), D3DXVECTOR3(0, 0, 0));

						// ライフ減少
						CParts *pParts = (CParts*)pScene;
						pParts->HitBossParts(1);

						// スコアの加算
						CScore::AddScore(100);

						// 弾の破棄
						Uninit();

						return;
					}
				}

				// プレイヤーだったら
				if (objType == CScene::OBJTYPE_PLAYER && ( m_bulletType == BULLETTYPE_ENEMY || m_bulletType== BULLETTYPE_BOSSPARTS || m_bulletType == BULLETTYPE_ENEMYHOMING) && nPlayerState == CPlayer::PLAYERSTATE_NORMAL)
				{
					// プレイヤーの情報
					D3DXVECTOR3 posPlayer = pScene->GetPos();
					D3DXVECTOR3 sizePlayer = pScene->GetSize();
					CLife *pLife = CGame::GetLife();

					// プレイヤーの状態
					int nPlayerState = CPlayer::GetPlayerState();
					CPlayer *pPlayer;
					pPlayer = CGame::GetPlayer();

					if (Pos.y - m_size.y / 2.0f < posPlayer.y + sizePlayer.y / 2 &&
						Pos.y + m_size.y / 2.0f > posPlayer.y - sizePlayer.y / 2 &&
						Pos.x - m_size.x / 2.0f < posPlayer.x + sizePlayer.x / 2 &&
						Pos.x + m_size.x / 2.0f > posPlayer.x - sizePlayer.x / 2)
					{
						// 爆発の生成
						CExplosion::Create(posPlayer, D3DXVECTOR3(50, 50, 0), D3DXVECTOR3(0, 0, 0));

						// SEの追加
						pSound->Play(CSound::SOUND_LABEL_SE_EXPLOSION);

						// プレイヤーの残機減少
						pPlayer->HitPlayer(1);

						// 弾を消す
						Uninit();

						return;
					}

				}
			}
		}
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

//=============================================================================
// 追尾処理
//=============================================================================
void CBullet::HomingBullet(void)
{
	// 座標
	D3DXVECTOR3 pos;
	pos = GetPosition();

	// 変数宣言
	float fAngle = 0.0f;
	float fAngleDest;

	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		for (int nCntScene = 0; nCntScene < MAX_POLYGON; nCntScene++)
		{
			// シーン
			CScene *pScene;
			pScene = CScene::GetScene(nCntPriority, nCntScene);

			if (pScene != NULL)
			{
				// オブジェタイプ取得
				CScene::OBJTYPE objType;
				objType = pScene->GetObjType();

				// プレイヤーの情報取得
				int nPlayerState = CPlayer::GetPlayerState();
				CPlayer *pPlayer;
				pPlayer = CGame::GetPlayer();

				// タイプがプレイヤーでノーマル状態だったら
				if (objType == CScene::OBJTYPE_PLAYER && nPlayerState == CPlayer::PLAYERSTATE_NORMAL)
				{
					// プレイヤーの情報
					D3DXVECTOR3 posPlayer = pScene->GetPos();

					// 角度計算
					float VecX = (posPlayer.x - pos.x);
					float VecY = (posPlayer.y - pos.y);

					//fAngleDest = atan2f(VecX, VecY);
					//fAngle = (fAngleDest - fAngle) * 0.6f;

					fAngle = atan2f(VecX, VecY);

				//  fAngleDest = (目的の位置 - 現在の角度) * 目的地への速度

					// 追尾計算
					m_move.x = sinf(fAngle) * 6.0f;
					m_move.y = cosf(fAngle) * 6.0f;
				}
			}
		}
	}
}