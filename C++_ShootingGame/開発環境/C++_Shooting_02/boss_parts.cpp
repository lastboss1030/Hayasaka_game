//=============================================================================
//
// ボスの処理 [boss_parts.cpp]
// Author : Taiki hayasaka
//
//=============================================================================
#include "boss_parts.h"
#include "renderer.h"
#include "bullet.h"
#include "scene2D.h"
#include "player.h"	
#include "game.h"	
#include "enemy.h"
#include "explosion.h"
#include "fade.h"
#include "sound.h"
#include "score.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CParts::m_apTexture[] = {};
CEnemy *CParts::m_pEnemy = NULL;

//=============================================================================
// コンストラクタ
//=============================================================================
CParts::CParts(PRIORITY nPriority) :CScene2D(nPriority)
{
	// 初期化
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Color = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

	m_Partstype = PARTSTYPE_NONE;
	m_nHitCnt = 0;
	m_bShoot = false;
	m_nCntShoot = 0;
	m_nBreakCnt = 0;
	m_nExplosionCnt = 0;
	m_nSwingWidth = 40.0f;

	// 移動カウント
	m_nMoveCnt = 0;
	m_nMoveCnt2 = MOVECNT2 / 2;
	m_nMoveCnt3 = 0;

	// レーザーの初期化
	m_nLaserCnt = 0;
	m_nLaserCnt2 = 0;
	m_nLaserCnt3 = 0;

	// ホーミング弾の初期化
	m_nCntShotHoming = 0;
	m_nCntShotHoming2 = 0;

	// 羽が壊れたとき用カウント
	m_nWingBreakCnt = 2;

	// サウンドカウントの初期化
	m_nSoundCnt = 0;
	m_nSoundCnt2 = 0;
	m_nSoundCnt3 = 0;
}

//=============================================================================
// デストラクタ
//=============================================================================
CParts::~CParts()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CParts::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 move, PARTSTYPE type, int nLife)
{
	// 初期化
	m_size = size;
	m_move = move;
	m_Partstype = type;
	m_nLife = nLife;

	// CScene2Dの初期化処理
	CScene2D::Init(pos, size);

	// オブジェクトの種類設定
	SetObjType(CScene::OBJTYPE_BOSS);

	// ボスの状態を通常にする
	SetState(PARTSSTATE::PARTS_NORMAL);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CParts::Uninit(void)
{
	// CScene2Dの終了処理
	CScene2D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CParts::Update(void)
{
	// 位置の取得
	D3DXVECTOR3 pos;
	pos = GetPosition();

	// フェード関係
	CFade *pFade;
	pFade = CManager::GetFade();

	// ゲーム
	CGame *pGame;
	pGame = CManager::GetGame();

	// サウンド関係
	CSound *pSound;
	pSound = CManager::GetSound();

	// 変数宣言
	int EnemyCnt = m_pEnemy->GetEnemyCnt();		// 敵カウント


	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// ボスパーツの状態
	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	switch (m_state)
	{
	// ノーマル状態
	case PARTS_NORMAL:
		// 通常色
		m_Color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// 撃てる状態
		m_bShoot = true;

		break;

	// ダメージ状態
	case PARTS_DAMAGE:
		// 赤くする
		m_Color = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

		// 当たってからのカウント増やす
		m_nHitCnt++;
		if (m_nHitCnt >= 10)
		{
			// ノーマルに戻す
			m_nHitCnt = 0;
			m_state = PARTS_NORMAL;
		}

		break;
	
	// 破損状態
	case PARTS_BREAK:

		// 爆発カウント
		m_nExplosionCnt++;
		if (m_nExplosionCnt >= 8)
		{
			// 爆発カウント戻す
			m_nExplosionCnt = 0;

			// 爆発生成
			CExplosion::Create(D3DXVECTOR3(pos.x, pos.y - 5.0f, 0.0f), D3DXVECTOR3(50, 50, 0), D3DXVECTOR3(0, 5, 0));
		}
		else if ((rand() % 8) == 0)
		{
			// 爆発生成
			CExplosion::Create(D3DXVECTOR3(pos.x, pos.y, 0.0f), D3DXVECTOR3(30, 30, 0), D3DXVECTOR3(0, 6, 0));
		}

		// オブジェクトの種類設定
		SetObjType(CScene::OBJTYPE_BOSSBREAK);

		// 暗くする
		m_Color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);

		// 撃てない状
		m_bShoot = false;

		break;

	// 壊れて落ちる
	case PARTS_BREAKFALL:

		// 先端部のみ
		if (m_Partstype == PARTSTYPE_FRONT)
		{
			// 爆発カウント
			m_nExplosionCnt++;
			if (m_nExplosionCnt >= 8)
			{
				// 爆発カウント戻す
				m_nExplosionCnt = 0;

				// 爆発生成
				CExplosion::Create(D3DXVECTOR3(pos.x, pos.y - 8.0f, 0.0f), D3DXVECTOR3(50, 50, 0), D3DXVECTOR3(0, 5, 0));
			}
			else if ((rand() % 8) == 0)
			{
				// 爆発生成
				CExplosion::Create(D3DXVECTOR3(pos.x, pos.y, 0.0f), D3DXVECTOR3(30, 30, 0), D3DXVECTOR3(0, 6, 0));
			}

			// 暗くする
			m_Color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f);

			// オブジェクトの種類設定
			SetObjType(CScene::OBJTYPE_BOSSBREAK);

			// 落ちる表現
			pos.y += 4.0f;
			m_move.x *= 0;

			// 撃てない状
			m_bShoot = false;

		}
		break;
	}


	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// 雑魚敵が全部消えたら動き始める
	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	if (EnemyCnt <= 0)
	{
		// 移動
		pos.y += 2.0;
		m_nMoveCnt++;

		// 警告音
		m_nSoundCnt++;
		if (m_nSoundCnt == 1)
		{
			pSound->Play(CSound::SOUND_LABEL_SE_ALERT);
		}

		// 一定時間で停止
		if (m_nMoveCnt >= 250)
		{
			// 移動量減らす
			pos.y -= 2.0;

			// 移動量加算
			pos += m_move;
			m_nMoveCnt2++;
			m_nMoveCnt3++;

			// 発射カウント進める
			m_nCntShoot++;			// 通常弾
			m_nCntShotHoming++;		// ホーミング
			m_nLaserCnt++;			// レーザー

			// 停止したらボスの弾を出す
			if (m_bShoot == true)
			{
				// 4つの銃口の弾
				// 発射カウント
				if (m_nCntShoot >= 60)
				{
					// カウントリセット
					m_nCntShoot = 0;

					// パーツが4つの銃口だったら
					if (m_Partstype == PARTSTYPE_SHOOT1 || m_Partstype == PARTSTYPE_SHOOT2 || m_Partstype == PARTSTYPE_SHOOT3 || m_Partstype == PARTSTYPE_SHOOT4)
					{
						// 発射音
						pSound->Play(CSound::SOUND_LABEL_SE_SHOTBOSS);

						// 弾の生成
						CBullet::Create(D3DXVECTOR3(pos.x, pos.y + 10.0f, 1.0f), D3DXVECTOR3(0.0f, 10.0f, 0.0f), D3DXVECTOR3(15.0f, 15.0f, 0.0f), CBullet::BULLETTYPE_ENEMY, CBullet::ATTACKTYPE_NORMAL, 1);
					}
				}
				else if (m_nCntShotHoming >= 120)
				{
					// プレイヤーの情報取得
					int nPlayerState = CPlayer::GetPlayerState();
					CPlayer *pPlayer;
					pPlayer = CGame::GetPlayer();

					if (nPlayerState == CPlayer::PLAYERSTATE_NORMAL)
					{

						// カウントリセット
						m_nCntShotHoming = 0;

						if (m_Partstype == PARTSTYPE_LEFT)
						{
							// ホーミング弾の生成
							CBullet::Create(D3DXVECTOR3(pos.x, pos.y + 10.0f, 1.0f), D3DXVECTOR3(-2.0f, -5.0f, 0.0f), D3DXVECTOR3(15.0f, 15.0f, 0.0f), CBullet::BULLETTYPE_ENEMYHOMING, CBullet::ATTACKTYPE_HOMING, 1);

							m_nCntShotHoming2++;
						}
						else if (m_Partstype == PARTSTYPE_RIGHT)
						{
							// ホーミング弾の生成
							CBullet::Create(D3DXVECTOR3(pos.x, pos.y + 10.0f, 1.0f), D3DXVECTOR3(2.0f, -5.0f, 0.0f), D3DXVECTOR3(15.0f, 15.0f, 0.0f), CBullet::BULLETTYPE_ENEMYHOMING, CBullet::ATTACKTYPE_HOMING, 1);
						}

						if (m_nCntShotHoming2 >= 20)
						{
							// 追尾開始


							// カウントリセット
							m_nCntShotHoming2 = 0;
						}
					}
				}


				// レーザー処理
				// 発射カウント
				if (m_nLaserCnt >= 400)
				{
					// カウントを増やす
					m_nLaserCnt3++;

					// レーザー音
					m_nSoundCnt3++;
					if (m_nSoundCnt3 == 1 && m_Partstype == PARTSTYPE_FRONT)
					{
						pSound->Play(CSound::SOUND_LABEL_SE_LASER);
					}

					// レーザーの時間
					if (m_nLaserCnt3 >= 400)
					{
						// カウントリセット
						m_nLaserCnt = 0;
						m_nLaserCnt3 = 0;
						m_nSoundCnt3 = 0;
					}

					// パーツが先頭だったら
					if (m_Partstype == PARTSTYPE_FRONT)
					{
						m_nLaserCnt2++;

						// 弾の間隔
						if (m_nLaserCnt2 >= 2)
						{
							// 追従までの時間
							m_nCntShotHoming2++;

							// カウントリセット
							m_nLaserCnt2 = 0;

							// 弾の生成
							CBullet::Create(D3DXVECTOR3(pos.x, pos.y + 10.0f, 1.0f), D3DXVECTOR3(0.0f, 15.0f, 0.0f), D3DXVECTOR3(40.0f, 40.0f, 0.0f), CBullet::BULLETTYPE_BOSSPARTS, CBullet::ATTACKTYPE_NORMAL, 1);
						}
					}
				}
			}

			//- - - - - - - - - - - - - - - - - - - - - - - -
			// 一定時間で移動 (横移動)
			//- - - - - - - - - - - - - - - - - - - - - - - -
			
			// 破損落下物以外
			if (m_state != PARTS_BREAKFALL)
			{
				if (m_nMoveCnt2 >= MOVECNT2)
				{
					m_nMoveCnt2 = 0;
					m_move *= -1.0;
				}

				// 上下移動
				if (m_nMoveCnt3 >= m_nSwingWidth)
				{
					m_nMoveCnt3 = 0;
					m_move.y *= -1.0f;
				}
			}
		}
	}

	// 敵の位置情報を2Dポリゴンに渡す
	CScene2D::SetPosition(pos, m_size);

	// 色チェンジ
	CScene2D::SetCol(m_Color);


	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	//ライフが0の場合破損状態に
	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	if (m_nLife <= 0)
	{
		// パーツ破損状態へ
		m_state = PARTS_BREAK;

		// 破損サウンドカウント
		m_nSoundCnt2++;
		if (m_nSoundCnt2 == 1)
		{
			// 破壊音
			pSound->Play(CSound::SOUND_LABEL_SE_PARTSBREAK);

			// スコアの加算
			CScore::AddScore(500);
		}

		// パーツのタイプが中央だったら
		if (m_Partstype == PARTSTYPE_MIDLE)
		{
			// 破損カウント
			m_nBreakCnt++;

			// リザルトのモードをクリアに
			pGame->SetResult(CGame::RESULTMODE_GAMECLEAR);

			// リザルトに移動する
			pFade->SetFade(CFade::FADE_OUT, CManager::MODE_RESULT);

			// 破損カウントが一定になったら
			if (m_nBreakCnt >= 40)
			{
				// 終了
				Uninit();

				//サウンドストップ
				pSound->Stop(CSound::SOUND_LABEL_SE_LASER);

				// 通常BGMオフ
				pSound->Stop(CSound::SOUND_LABEL_BGM_NORMAL);
			}
		}

		if (m_Partstype == PARTSTYPE_FRONT)
		{
			// パーツ破損状態へ
			m_state = PARTS_BREAKFALL;
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CParts::Draw(void)
{
	// CScene2Dの描画処理
	CScene2D::Draw();
}

//=============================================================================
// テクスチャのロード処理
//=============================================================================
HRESULT CParts::Load(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/boss_parts1.png", &m_apTexture[0]);	// 先端
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/boss_parts2.png", &m_apTexture[1]);	// 銃口１
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/boss_parts3.png", &m_apTexture[2]);	// 銃口２
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/boss_parts4.png", &m_apTexture[3]);	// 銃口３
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/boss_parts5.png", &m_apTexture[4]);	// 銃口４
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/boss_parts6.png", &m_apTexture[5]);	// 左翼
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/boss_parts7.png", &m_apTexture[6]);	// 右翼
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/boss_parts8.png", &m_apTexture[7]);	// 中央

	return S_OK;
}

//=============================================================================
// テクスチャの破棄
//=============================================================================
void CParts::Unload(void)
{
	// テクスチャの開放
	for (int nCnt = 0; nCnt < MAX_PARTS; nCnt++)
	{
		if (m_apTexture[nCnt] != NULL)
		{
			m_apTexture[nCnt]->Release();
			m_apTexture[nCnt] = NULL;
		}
	}
}

//=============================================================================
// 生成処理
//=============================================================================
CParts *CParts::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 move, PARTSTYPE type, int nTexNum, int nLife)
{
	// 変数宣言
	CParts *pParts = NULL;

	if (pParts == NULL)
	{
		// 動的確保
		pParts = new CParts;

		if (pParts != NULL)
		{
			// 初期化処理を呼び出す
			pParts->Init(pos, size, move, type, nLife);

			// 敵のテクスチャを割り当てる
			pParts->BindTexture(m_apTexture[nTexNum]);
		}
	}

	// 値を返す
	return pParts;
}

//=============================================================================
// 衝突判定処理
//=============================================================================
void CParts::Collision(void)
{

}

//=============================================================================
// 当たり判定
//=============================================================================
bool CParts::HitBossParts(int nDamage)
{
	// サウンド関係
	CSound *pSound;
	pSound = CManager::GetSound();

	// ライフ減少
	m_nLife -= nDamage;

	// ダメージ判定
	m_state = PARTS_DAMAGE;

	// ヒット音
	pSound->Play(CSound::SOUND_LABEL_SE_HIT);

	return false;
}

//=============================================================================
// ボスパーツの状態を設定
//=============================================================================
void CParts::SetPartsState(PARTSSTATE PartsState)
{
	// パーツの状態
	m_state = PartsState;
}