//=============================================================================
//
// 敵の処理 [enemy.cpp]
// Author : Taiki hayasaka
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "enemy.h"
#include "manager.h"
#include "renderer.h"
#include "bullet.h"
#include "scene2D.h"
#include "player.h"	
#include "game.h"	
#include "sound.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CEnemy::m_apTexture[] = {};
int CEnemy::m_enmeyCnt = MAX_ENEMY;

// 敵構造体
CEnemy::ENEMY CEnemy::m_aEnemy[MAX_ENEMY] =
{
	40,
	D3DXVECTOR3(320.0f, 100.0f ,0.0f),	// 座標
	D3DXVECTOR3(60.0f, 60.0f ,0.0f),	// サイズ
	D3DXVECTOR3(0.0f, 2.0f ,0.0f),		// 移動量
	2,									// ライフ
	0,									// タイプ
};

int CEnemy::m_nIndexEnemy = 0;			// 番号
int CEnemy::m_nCounterGame = 0;			// ゲームカウンター

//=============================================================================
// CEnemyのコンストラクタ
//=============================================================================
CEnemy::CEnemy(PRIORITY nPriority) :CScene2D(nPriority)
{
	// 初期化
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Color = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_nType = 0;

	// 敵構造体 数値初期化
	for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++)
	{
		m_aEnemy[nCnt].pos = D3DXVECTOR3(320.0f, 100.0f, 0.0f);		// 座標
		m_aEnemy[nCnt].size = D3DXVECTOR3(60.0f, 60.0f, 0.0f);		// サイズ
		m_aEnemy[nCnt].move = D3DXVECTOR3(0.0f, 2.0f, 0.0f);		// 移動量
		m_aEnemy[nCnt].nLife = 2;									// ライフ
		m_aEnemy[nCnt].nType = 0;									// タイプ
	}

	// タイプ初期化
	m_Enemytype = ENEMYTYPE_NONE;

	// 外部ファイル読み込み
	LoadData();
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
HRESULT CEnemy::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 move, int nLife, int nType)
{
	// 初期化
	m_size = size;
	m_move = move;
	m_nLife = nLife;
	m_nType = nType;

	// 構造体初期化
	m_aEnemy[m_nIndexEnemy].pos = D3DXVECTOR3(320.0f, 100.0f, 0.0f);	// 座標
	m_aEnemy[m_nIndexEnemy].size = D3DXVECTOR3(60.0f, 60.0f, 0.0f);		// サイズ
	m_aEnemy[m_nIndexEnemy].move = D3DXVECTOR3(0.0f, 2.0f, 0.0f);		// 移動量
	m_aEnemy[m_nIndexEnemy].nLife = 2;									// ライフ
	m_aEnemy[m_nIndexEnemy].nType = 0;									// タイプ

	// カウント初期化
	m_nHomingCnt = 0;
	m_nMoveCnt = 0;
	m_nShotCnt = 0;
	m_enmeyCnt = MAX_ENEMY;

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

	// カウント++
	m_nShotCnt++;
	m_nMoveCnt++;

	// 一定の位置まで来たら止める (停止型)
	if (pos.y >= 300 && m_nType == 2 && m_nMoveCnt < 300)
	{
		m_move.y = 0;

		// 弾発射
		if (m_nShotCnt >= 70 && m_nType == 2)
		{
			// カウント戻す
			m_nShotCnt = 0;

			// 弾の生成
			CBullet::Create(D3DXVECTOR3(pos.x, pos.y + 10.0f, 1.0f),
				D3DXVECTOR3(0.0f, 8.0f, 0.0f), D3DXVECTOR3(15.0f, 15.0f, 0.0f),
				CBullet::BULLETTYPE_ENEMY, CBullet::ATTACKTYPE_NORMAL, 1);
		}

	}

	// 一定時間で下がる
	else if (m_nMoveCnt == 900 && m_nType == 2)
	{
		m_move.y += 3;
	}

	// 敵の移動
	MoveEnemy();

	// 追尾型
	if (m_nType == 1)
	{
		m_nHomingCnt++;

		if (m_nHomingCnt <= 300)
		{
			HomigEnemy();
		}
	}

	// 敵の位置情報を2Dポリゴンに渡す
	CScene2D::SetPosition(pos, m_size);

	//- - - - - - - - - - - - - - - - - - - - -
	// 敵の弾を出す
	//- - - - - - - - - - - - - - - - - - - - -
	if (m_nShotCnt >= 70 && m_nType == 2)
	{
		// カウント戻す
		m_nShotCnt = 0;

		// 弾の生成
		CBullet::Create(D3DXVECTOR3(pos.x, pos.y + 10.0f, 1.0f),
			D3DXVECTOR3(0.0f, 8.0f, 0.0f), D3DXVECTOR3(15.0f, 15.0f, 0.0f),
			CBullet::BULLETTYPE_ENEMY, CBullet::ATTACKTYPE_NORMAL, 1);

		// 発射音
		pSound->Play(CSound::SOUND_LABEL_SE_SHOTENEMY);
	}

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

	//- - - - - - - - - - - - - - - - - - - - - -
	// 画面外に行ったら
	//- - - - - - - - - - - - - - - - - - - - - -
	if (pos.x + m_size.x / 2.0f < 0.0f ||
		pos.x - m_size.x / 2.0f > SCREEN_WIDTH ||
		pos.y - m_size.y / 2.0f > SCREEN_HEIGHT)
	{
		Uninit();

		return;
	}

	//- - - - - - - - - - - - - - - - - - - - - -
	// 敵のライフが0になったら消す
	//- - - - - - - - - - - - - - - - - - - - - -
	if (m_nLife <= 0)
	{
		Uninit();

		return;
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
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/enemy100.png", &m_apTexture[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/enemy004.png", &m_apTexture[1]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/enemy003.png", &m_apTexture[2]);

	return S_OK;
}

//=============================================================================
// テクスチャの破棄
//=============================================================================
void CEnemy::Unload(void)
{
	// テクスチャの開放
	for (int nCnt = 0; nCnt < MAX_TEX_ENEMY; nCnt++)
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
CEnemy *CEnemy::Create(int nIndex)
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
			pEnemy->Init(m_aEnemy[nIndex].pos, m_aEnemy[nIndex].size,
							m_aEnemy[nIndex].move,m_aEnemy[nIndex].nLife, m_aEnemy[nIndex].nType);

			// 敵のテクスチャを割り当てる
			pEnemy->BindTexture(m_apTexture[m_aEnemy[nIndex].nType]);
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

//=============================================================================
// 追尾型の敵
//=============================================================================
void CEnemy::HomigEnemy(void)
{
	// 座標
	D3DXVECTOR3 pos;
	pos = GetPosition();

	// 変数宣言
	float fAngle = 0.0f;

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

					fAngle = atan2f(VecX, VecY);

					// 追尾計算
					m_move.x = sinf(fAngle) * 3.5f;
					m_move.y = cosf(fAngle) * 3.5f;
				}
			}
		}
	}
}

//=============================================================================
// カウンター取得
//=============================================================================
int CEnemy::GetCounterGame(void)
{
	return m_nCounterGame;
}

//=============================================================================
// 外部ファイル読み込み
//=============================================================================
void CEnemy::LoadData(void)
{
	FILE *pFile;

	// ファイル開く
	pFile = fopen(ENEMY_TEXT_NAME, "r");

	if (pFile != NULL)
	{
		// ファイルが開けたら
		for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++)
		{
			fscanf(pFile, "%d\n", &m_aEnemy[nCnt].nTime);
			fscanf(pFile, "%f %f %f\n", &m_aEnemy[nCnt].pos.x,  &m_aEnemy[nCnt].pos.y,  &m_aEnemy[nCnt].pos.z);		// 生成数
			fscanf(pFile, "%f %f %f\n", &m_aEnemy[nCnt].size.x, &m_aEnemy[nCnt].size.y, &m_aEnemy[nCnt].size.z);	// 速度
			fscanf(pFile, "%f %f %f\n", &m_aEnemy[nCnt].move.x, &m_aEnemy[nCnt].move.y, &m_aEnemy[nCnt].move.z);	// 半径
			fscanf(pFile, "%d\n", &m_aEnemy[nCnt].nLife);															// ライフ
			fscanf(pFile, "%d\n", &m_aEnemy[nCnt].nType);															// タイプ
		}
	}

	// ファイルを閉じる
	fclose(pFile);
}

//=============================================================================
// 敵生成
//=============================================================================
void CEnemy::EnemyGenerator(void)
{
	// ゲームカウント
	m_nCounterGame++;

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		// ゲームカウントが.nTimeと同じになったら
		if (m_nCounterGame == m_aEnemy[nCntEnemy].nTime)
		{
			// 生成
			Create(nCntEnemy);
		}
	}
}

//=============================================================================
// 敵生成リセット用 (game.cppで呼ぶだけ)
//=============================================================================
void CEnemy::EnemyGeneratorReset(void)
{
	m_nCounterGame = 0;
}