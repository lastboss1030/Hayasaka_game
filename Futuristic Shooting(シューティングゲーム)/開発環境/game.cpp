//=============================================================================
//
// ゲーム画面処理 [game.cpp]
// Author : taiki hayasaka 
//
//=============================================================================
#include "renderer.h"		
#include "manager.h"		
#include "scene2D.h"		
#include "scene.h"			
#include "input.h"			
#include "player.h"			
#include "bullet.h"			
#include "explosion.h"		
#include "sound.h"			
#include "bg.h"				
#include "enemy.h"			
#include "number.h"			
#include "score.h"			
#include "life.h"			
#include "game.h"			
#include "Input_Keyboard.h"	
#include "fade.h"
#include "effect.h"
#include "boss_parts.h"
#include "pause.h"
#include "particle.h"

//=============================================================================
// 静的メンバ変数
//=============================================================================
CBg *CGame::m_pBg = NULL;
CEnemy *CGame::m_pEnemy = NULL;
CNumber *CGame::m_pNumber = NULL;
CScore *CGame::m_pScore = NULL;
CPolygon *CGame::m_pPolygon = NULL;
CLife *CGame::m_pLife = NULL;
CPlayer *CGame::m_pPlayer = NULL;
CParts *CGame::m_pParts = NULL;
CPause *CGame::m_pPause = NULL;

CGame::RESULTMODE CGame::m_resultmode = RESULTMODE_NONE;

//=============================================================================
// コンストラクタ
//=============================================================================
CGame::CGame()
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CGame::~CGame()
{

}

//=============================================================================
// 生成処理
//=============================================================================
CGame *CGame::Create()
{
	CGame *pGame = new CGame;

	D3DXVECTOR3 pos;
	D3DXVECTOR3 size;

	// 初期化処理
	pGame->Init(pos, size);
	
	return pGame;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CGame::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// サウンドを取得
	CSound *pSound;
	pSound = CManager::GetSound();

	// BGM
	pSound->Play(CSound::SOUND_LABEL_BGM_NORMAL);

	// 全てのテクスチャ読み込み
	CGame::LoadAll();

	// 背景の生成
	m_pBg = CBg::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0), D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0));

	// プレイヤーの生成
	m_pPlayer = CPlayer::Create(D3DXVECTOR3(SCREEN_CENTER_X, 700, 0.0f), D3DXVECTOR3(MAX_PLAYER_X, MAX_PLAYER_Y, 0));

	// スコアの生成
	m_pScore = CScore::Create(D3DXVECTOR3(1000.0f, 50.0f, 0), D3DXVECTOR3(20.0f, 30.0f, 0.0f));

	// ライフの生成
	m_pLife = CLife::Create(D3DXVECTOR3(130.0f, 40.0f, 0.0f), D3DXVECTOR3(LIFE_WIDTH, LIFE_HEIGHT, 0));

	// ボス生成
	CGame::BossAll();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CGame::Uninit(void)
{
	// 敵生成装置リセット
	CEnemy::EnemyGeneratorReset();

	// 全てのテクスチャを破棄する
	UnloadAll();

	// 全てのオブジェクトを消す
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CGame::Update(void)
{
	// キーボード関係
	CInputKeyboard *plnputKeyboard;
	plnputKeyboard = CManager::GetInputKeyboard();

	// モード関係
	CManager::MODE pManager;
	pManager = CManager::GetMode();

	// フェード関係
	CFade *pFade;
	pFade = CManager::GetFade();

	//サウンドを取得
	CSound *pSound;
	pSound = CManager::GetSound();

	// 敵生成
	CEnemy::EnemyGenerator();

	// ライフが0になったら
	if (m_pLife->GetLife() == 0 && pFade->GetFade() == CFade::FADE_NONE)
	{
		//サウンドストップ
		pSound->Stop(CSound::SOUND_LABEL_SE_LASER);

		// ゲームオーバー
		m_resultmode = RESULTMODE_GAMEOVER;

		// リザルトに移動する
		pFade->SetFade(CFade::FADE_OUT, CManager::MODE_RESULT);
	}

	if (plnputKeyboard->GetPress(DIK_0) == true)
	{
		// ゲームオーバー
		m_resultmode = RESULTMODE_GAMEOVER;

		// リザルトに移動する
		pFade->SetFade(CFade::FADE_OUT, CManager::MODE_RESULT);
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CGame::Draw(void)
{

}

//=============================================================================
// セットリザルト
//=============================================================================
void CGame::SetResult(RESULTMODE resultmode)
{
	m_resultmode = resultmode;
}

//=============================================================================
// テクスチャロード
//=============================================================================
void CGame::LoadAll(void)
{
	// テクスチャの読み込み
	CBullet::Load();
	CExplosion::Load();
	CPlayer::Load();
	CBg::Load();
	CEnemy::Load();
	CNumber::Load();
	CLife::Load();
	CEffect::Load();
	CParts::Load();
	CParticle::Load();
}

//=============================================================================
// テクスチャ破棄
//=============================================================================
void CGame::UnloadAll(void)
{
	// テクスチャを破棄
	CBullet::Unload();
	CExplosion::Unload();
	CPlayer::Unload();
	CBg::Unload();
	CEnemy::Unload();
	CNumber::Unload();
	CLife::Unload();
	CEffect::Unload();
	CParts::Unload();
	CParticle::Unload();
}

//=============================================================================
// ボス管理
//=============================================================================
void CGame::BossAll(void)
{
	// ボスパーツごとに生成
	CParts::Create(D3DXVECTOR3(640.0f, 400.0f - WAIT_Y, 0.0f), D3DXVECTOR3(120.0f, 240.0f, 0.0f), D3DXVECTOR3(1.5f, 1.0f, 0.0f), CParts::PARTSTYPE_FRONT, 0, 50);	// 手前
	CParts::Create(D3DXVECTOR3(425.0f, 295.0f - WAIT_Y, 0.0f), D3DXVECTOR3(60.0f, 60.0f, 0.0f), D3DXVECTOR3(1.5f, 1.0f, 0.0f), CParts::PARTSTYPE_SHOOT1, 1, 20);	// 銃口1
	CParts::Create(D3DXVECTOR3(530.0f, 298.0f - WAIT_Y, 0.0f), D3DXVECTOR3(60.0f, 60.0f, 0.0f), D3DXVECTOR3(1.5f, 1.0f, 0.0f), CParts::PARTSTYPE_SHOOT2, 2, 20);	// 銃口2
	CParts::Create(D3DXVECTOR3(750.0f, 298.0f - WAIT_Y, 0.0f), D3DXVECTOR3(60.0f, 60.0f, 0.0f), D3DXVECTOR3(1.5f, 1.0f, 0.0f), CParts::PARTSTYPE_SHOOT3, 3, 20);	// 銃口3
	CParts::Create(D3DXVECTOR3(845.0f, 295.0f - WAIT_Y, 0.0f), D3DXVECTOR3(60.0f, 60.0f, 0.0f), D3DXVECTOR3(1.5f, 1.0f, 0.0f), CParts::PARTSTYPE_SHOOT4, 4, 20);	// 銃口4
	CParts::Create(D3DXVECTOR3(440.0f, 215.0f - WAIT_Y, 0.0f), D3DXVECTOR3(280.0f, 160.0f, 0.0f), D3DXVECTOR3(1.5f, 1.0f, 0.0f), CParts::PARTSTYPE_LEFT, 5, 30);	// 左翼
	CParts::Create(D3DXVECTOR3(840.0f, 220.0f - WAIT_Y, 0.0f), D3DXVECTOR3(280.0f, 160.0f, 0.0f), D3DXVECTOR3(1.5f, 1.0f, 0.0f), CParts::PARTSTYPE_RIGHT, 6, 30);	// 右翼
	CParts::Create(D3DXVECTOR3(640.0f, 120.0f - WAIT_Y, 0.0f), D3DXVECTOR3(140.0f, 400.0f, 0.0f), D3DXVECTOR3(1.5f, 1.0f, 0.0f), CParts::PARTSTYPE_MIDLE, 7, 80);	// 中央
}