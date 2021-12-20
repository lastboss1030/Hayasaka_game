//=============================================================================
//
// プレイヤー処理[player.cpp]
// Author : Taiki Hayasaka
//
//=============================================================================
#include "player.h"
#include "input.h"
#include "manager.h"
#include "renderer.h"
#include "bullet.h"
#include "explosion.h"
#include "scene2D.h"		
#include "Input_Keyboard.h"	
#include "game.h"			
#include "fade.h"			
#include "effect.h"
#include "enemy.h"

//=============================================================================
// 静的メンバ変数
//=============================================================================
LPDIRECT3DTEXTURE9 CPlayer::m_pTexture = NULL;
CPlayer::PLAYERSTATE CPlayer::m_state = CPlayer::PLAYERSTATE_NORMAL;

D3DXVECTOR3 g_pos;	// 取得用

//=============================================================================
// マクロ定義
//=============================================================================
#define PLAYER_RIGHT	(5.0f)			// プレイヤーの移動量(右)
#define PLAYER_LEFT		(-5.0f)			// プレイヤーの移動量(左)

//=============================================================================
// グローバル変数
//=============================================================================
bool g_bBumpedPlayer = false;		//無敵状態

//=============================================================================
// コンストラクタ
//=============================================================================
CPlayer::CPlayer(PRIORITY nPriority) :CScene2D(nPriority)
{
	// 初期化
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fPosTexU = 0;
	m_fPosTexV = 0;

	m_shootspeed = 0;
	g_bBumpedPlayer = false;
	m_state = PLAYERSTATE_NORMAL;
}

//=============================================================================
// デストラクタ
//=============================================================================
CPlayer::~CPlayer()
{

}

//=============================================================================
// 生成
//=============================================================================
CPlayer *CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	CPlayer *pPlayer = NULL;

	if (pPlayer == NULL)
	{
		 // 動的確保
		pPlayer = new CPlayer;

		if (pPlayer != NULL)
		{
			 // 初期化処理
			pPlayer->Init(pos, size);

			// テクスチャの割り当て
			pPlayer->BindTexture(m_pTexture);
		}
	}
	return pPlayer;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CPlayer::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// サイズ
	m_size = size;

	// CScene2Dの初期化処理
	CScene2D::Init(pos, size);

	// オブジェクトの種類設定
	SetObjType(CScene::OBJTYPE_PLAYER);

	// プレイヤーの状態
	m_state = PLAYERSTATE_NORMAL;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CPlayer::Uninit(void)
{
	// Scene2Dの終了処理
	CScene2D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CPlayer::Update(void)
{
	// CInputKeyboardインスタンスの取得
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

	// 現在の位置の取得
	D3DXVECTOR3 pos = GetPosition();


	//- - - - - - - - - - - - - - - - - 
	// プレイヤーの状態
	//- - - - - - - - - - - - - - - - -
	switch (m_state)
	{
	//何もない
	case PLAYERSTATE_NORMAL:

		// 初期色
		m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// 状態カウントを初期化
		m_nStateCnt = 0;

		// カウント初期化
		m_nDispCnt = 0;

		// 表示する
		m_bDisp = true;

		break;

	// 出現
	case PLAYERSTATE_APPEAR:

		// 表示カウント加算
		m_nDispCnt++;

		// 状態カウント加算
		m_nStateCnt++;

		// 10の倍数になったら
		if ((m_nDispCnt % 3) == 0)
		{
			// 点滅 true false切り替え
			m_bDisp = m_bDisp ? false : true;
		}

		// 60の倍数になったら
		if (m_nStateCnt >= 120)
		{
			// 普通状態に
			m_state = PLAYERSTATE_NORMAL;
		}

		break;

	// 待機
	case PLAYERSTATE_WAIT:

		// 出現位置
		pos = D3DXVECTOR3(D3DXVECTOR3(SCREEN_CENTER_X, 700, 0.0f));

		// 非表示にする
		m_bDisp = false;

		// 状態カウント加算
		m_nStateCnt++;

		// 出現までの時間
		if (m_nStateCnt >= 30)
		{
			// 出現にする
			m_state = PLAYERSTATE_APPEAR;

			// 状態カウントを初期化
			m_nStateCnt = 0;
		}

		break;
		
	// 死亡
	case PLAYERSTATE_DEATH:

		// 状態カウント加算
		m_nStateCnt++;

		break;

	}
	

	//- - - - - - - - - - - - - - - - - - - -
	// 移動 (出現 & 通常時)
	//- - - - - - - - - - - - - - - - - - - -
	if (m_state == PLAYERSTATE_NORMAL || m_state == PLAYERSTATE_APPEAR)
	{
		// 移動処理
		if (pInputKeyboard->GetPress(DIK_W) == true)
		{
			m_move.y -= 2.0f;
		}
		if (pInputKeyboard->GetPress(DIK_S) == true)
		{
			m_move.y += 2.0f;
		}
		if (pInputKeyboard->GetPress(DIK_A) == true)
		{
			m_move.x -= 2.0f;
		}
		if (pInputKeyboard->GetPress(DIK_D) == true)
		{
			m_move.x += 2.0f;
		}

		// 慣性処理
		m_move.x += (0.0f - m_move.x) * 0.2f;
		m_move.y += (0.0f - m_move.y) * 0.2f;

		// 座標の設定
		pos += m_move;
	}


	// 画面外処理
	if (pos.x - m_size.x / 2 <= 0.0f)
	{
		pos.x = m_size.x / 2;
	}
	if (pos.x + m_size.x / 2 >= SCREEN_WIDTH)
	{
		pos.x = SCREEN_WIDTH - m_size.x / 2;
	}
	if (pos.y - m_size.y / 2 <= 0.0f)
	{
		pos.y = m_size.y / 2;
	}
	if (pos.y + m_size.y / 2 >= SCREEN_HEIGHT)
	{
		pos.y = SCREEN_HEIGHT - m_size.y / 2;
	}


	// 画面左右反対から出てくる
	if (pos.x + m_size.x / 2 > SCREEN_WIDTH)
	{
		pos.x = m_size.x / 2;
	}
	else if (pos.x - m_size.x / 2 < 0)
	{
		pos.x = SCREEN_WIDTH - m_size.x / 2;
	}


	//- - - - - - - - - - - - - - - - - - - -
	// プレイヤーと敵の当たり判定
	//- - - - - - - - - - - - - - - - - - - -
	//for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	//{
	//	for (int nCntScene = 0; nCntScene < MAX_POLYGON; nCntScene++)
	//	{
	//		// シーン関係
	//		CScene *pScene;
	//		pScene = CScene::GetScene(nCntPriority, nCntScene);

	//		if (pScene != NULL)
	//		{
	//			CScene::OBJTYPE objType;
	//			objType = pScene->GetObjType();

	//			// 敵だったら
	//			if (objType == CScene::OBJTYPE_ENEMY && m_state == PLAYERSTATE_NORMAL)
	//			{
	//				// 変数宣言
	//				D3DXVECTOR3 PosEnemy;
	//				D3DXVECTOR3 EnemySize;

	//				// 敵の位置とサイズを取得
	//				PosEnemy = pScene->GetPos();
	//				EnemySize = pScene->GetSize();

	//				// 敵の情報
	//				CEnemy *pEnemy;
	//				pEnemy = (CEnemy*)pScene;

	//				// プレイヤーと敵の当たり判定
	//				if (pos.x - (m_size.x / 2) < PosEnemy.x + (EnemySize.x / 2) &&
	//					pos.x + (m_size.x / 2) > PosEnemy.x - (EnemySize.x / 2) &&
	//					pos.y - (m_size.y / 2) < PosEnemy.y + (EnemySize.y / 2) &&
	//					pos.y + (m_size.y / 2) > PosEnemy.y - (EnemySize.y / 2))
	//				{
	//					// 敵爆発の生成
	//					CExplosion::Create(PosEnemy, D3DXVECTOR3(50, 50, 0), D3DXVECTOR3(0, 0, 0));

	//					// ライフ減少
	//					CEnemy *pEnemy = (CEnemy*)pScene;
	//					pEnemy->HitEnemy(1);

	//					// ダメージ
	//					HitPlayer(1);

	//					return;
	//				}
	//			}
	//		}
	//	}
	//}


	//- - - - - - - - - - - - - - - - - - - - - - -
	// プレイヤーとボスパーツとの当たり判定
	//- - - - - - - - - - - - - - - - - - - - - - -
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		for (int nCntScene = 0; nCntScene < MAX_POLYGON; nCntScene++)
		{
			// シーン関係
			CScene *pScene;
			pScene = CScene::GetScene(nCntPriority, nCntScene);

			if (pScene != NULL)
			{
				// オブジェタイプ
				CScene::OBJTYPE objType;
				objType = pScene->GetObjType();

				// ボスだったら
				if (objType == CScene::OBJTYPE_BOSS && m_state == PLAYERSTATE_NORMAL)
				{
					// 変数宣言
					D3DXVECTOR3 PosParts;
					D3DXVECTOR3 PartsSize;

					// 敵の位置とサイズを取得
					PosParts = pScene->GetPos();
					PartsSize = pScene->GetSize();

					// 敵の情報
					CParts *pParts;
					pParts = (CParts*)pScene;

					// プレイヤーと敵の当たり判定
					if (pos.x - (m_size.x / 4) < PosParts.x + (PartsSize.x / 4) &&
						pos.x + (m_size.x / 4) > PosParts.x - (PartsSize.x / 4) &&
						pos.y - (m_size.y / 4) < PosParts.y + (PartsSize.y / 4) &&
						pos.y + (m_size.y / 4) > PosParts.y - (PartsSize.y / 4))
					{
						// ダメージ
						HitPlayer(1);

						return;
					}
				}
			}
		}
	}


	//- - - - - - - - - - - - - - - - -
	// 普通状態 & 出現状態
	//- - - - - - - - - - - - - - - - -
	if (m_state == PLAYERSTATE_NORMAL || m_state == PLAYERSTATE_APPEAR)
	{
		// 弾発射
		m_shootspeed++;

		// 弾発射間隔
		if (m_shootspeed >= 15)
		{
			m_shootspeed = 0;
		}

		// 押した瞬間撃てるやつ
		if (pInputKeyboard->GetTrigger(DIK_SPACE) == true)
		{
			// カウントリセット
			m_shootspeed = 0;
		}

		// 弾の発射
		if (pInputKeyboard->GetPress(DIK_SPACE) == true)
		{
			if (m_shootspeed == 0)
			{
				CBullet::Create(D3DXVECTOR3(pos.x + 0.0f, pos.y - 10.0f, 1.0f), D3DXVECTOR3(0.0f, -8.0f, 0.0f), D3DXVECTOR3(15.0f, 15.0f, 0.0f), CBullet::BULLETTYPE_PLAYER, CBullet::ATTACKTYPE_NORMAL, 1);
			}
		}
	}


	//- - - - - - - - - - - - -
	// 死亡状態
	//- - - - - - - - - - - - -
	else if (m_state == PLAYERSTATE_DEATH)
	{
		if (m_nPlayerCnt >= 0)
		{
			m_state = PLAYERSTATE_APPEAR;

			m_nPlayerInvincibleCnt = 120;

			pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT * 3 / 4, 0.0f);
		}
	}

	// 代入
	g_pos = pos;

	// 座標の設定
	CScene2D::SetPosition(pos,m_size);
}

//=============================================================================
// 描画処理
//=============================================================================
void CPlayer::Draw(void)
{
	if (m_bDisp == true)
	{
		// Scene2Dの描画処理
		CScene2D::Draw();
	}
}

//=============================================================================
// テクスチャのロード処理
//=============================================================================
HRESULT CPlayer::Load(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/player.png", &m_pTexture);

	return S_OK;
}

//=============================================================================
// テクスチャの破棄
//=============================================================================
void CPlayer::Unload(void)
{
	// テクスチャの破棄
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================================================================
// プレイヤーの移動量取得
//=============================================================================
D3DXVECTOR3 CPlayer::GetMove(void)
{
	return m_move;
}

//=============================================================================
// プレイヤーの位置取得
//=============================================================================
D3DXVECTOR3 CPlayer::GetPlayerPos(void)
{
	return g_pos;
}

//=============================================================================
// 位置を設定
//=============================================================================
void CPlayer::SetPos(D3DXVECTOR3 pos)
{
	// Scene2Dに情報を渡す
	SetPosition(pos, m_size);
}

//=============================================================================
// プレイヤーの移動量を設定
//=============================================================================
void CPlayer::SetMove(D3DXVECTOR3 move)
{
	m_move = move;
}

//=============================================================================
// プレイヤーの当たり判定
//=============================================================================
bool CPlayer::HitPlayer(int nDamage)
{
	// 現在の位置の取得
	D3DXVECTOR3 pos = GetPosition();

	// 爆発生成
	CExplosion::Create(pos, D3DXVECTOR3(50, 50, 0), D3DXVECTOR3(0, 0, 0));

	// 状態を待機状態に
	m_state = PLAYERSTATE_WAIT;

	return false;
}

//=============================================================================
// プレイヤーの状態
//=============================================================================
CPlayer::PLAYERSTATE CPlayer::GetPlayerState(void)
{
	return m_state;
}