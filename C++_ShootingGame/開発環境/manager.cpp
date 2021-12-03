//=============================================================================
//
// マネージャー処理 [manager.cpp]
// Author : Taiki Hayasaka
//
//=============================================================================
#include "manager.h"
#include "scene2D.h"
#include "player.h"
#include "bullet.h"
#include "explosion.h"
#include "bg.h"
#include "enemy.h"
#include "score.h"
#include "life.h"
#include "title.h"
#include "game.h"
#include "result.h"
#include "renderer.h"		
#include "scene.h"			
#include "Input_Keyboard.h"	
#include "sound.h"			
#include "fade.h"			
#include "number.h"			
#include "polygon.h"	
#include "fade.h"
#include "tutorial.h"
#include "logo.h"
#include "animation.h"
#include "pause.h"

//=============================================================================
// 静的メンバ変数
//=============================================================================
CRenderer *CManager::m_pRenderer = NULL;
CInputKeyboard *CManager::m_pInputKeyboard = NULL;
CSound *CManager::m_pSound = NULL;
CFade *CManager::m_pFade = NULL;
CPause *CManager::m_pPause = NULL;

CTitle *CManager::m_pTitle = NULL;
CTutorial *CManager::m_pTutorial = NULL;
CGame *CManager::m_pGame = NULL;
CResult *CManager::m_pResult = NULL;

CManager::MODE CManager::m_mode = MODE_TITLE;		// 現在のモード
bool CManager::m_bPause = false;					// ポーズ使用変数

//=============================================================================
// コンストラクタ
//=============================================================================
CManager::CManager()
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CManager::~CManager()
{
}

//=============================================================================
// 初期化
//=============================================================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, bool bWindow)
{
	// レンダリングクラス
	if (m_pRenderer == NULL)
	{
		// 動的確保
		m_pRenderer = new CRenderer;

		if (m_pRenderer != NULL)
		{
			// 初期化処理
			m_pRenderer->Init(hWnd, true);
		}
	}

	// キーボード
	if (m_pInputKeyboard == NULL)
	{
		// 動的確保
		m_pInputKeyboard = new CInputKeyboard;
		if (m_pInputKeyboard != NULL)
		{
			// 初期化処理
			m_pInputKeyboard->Init(hInstance, hWnd);
		}
	}

	// サウンド
	if (m_pSound == NULL)
	{
		// 動的確保
		m_pSound = new CSound;
		if (m_pSound != NULL)
		{
			// 初期化処理
			m_pSound->Init(hWnd);
		}
	}
	//ロゴのテクスチャの読み込み
	CLogo::Load();

	// アニメーション画像ロード
	CAnimation::Load();

	// フェードの生成
	m_pFade = CFade::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f),
							D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f), m_mode);

	// サウンド関係
	CSound *pSound;
	pSound = CManager::GetSound();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CManager::Uninit(void)
{
	// ナンバーの破棄
	CNumber::Unload();

	// 敵の破棄
	CEnemy::Unload();

	// 背景の破棄
	CBg::Unload();

	// プレイヤーの破棄
	CPlayer::Unload();

	// 爆発の破棄
	CExplosion::Unload();

	// 弾の破棄
	CBullet::Unload();

	// アニメーションの破棄
	CAnimation::Unload();

	//レンダラーの破棄
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Uninit();
		delete m_pRenderer;
		m_pRenderer = NULL;
	}

	// キーボードの破棄
	if (m_pInputKeyboard != NULL)
	{
		m_pInputKeyboard->Uninit();
		delete m_pInputKeyboard;
		m_pInputKeyboard = NULL;
	}

	// サウンドの破棄
	if (m_pSound != NULL)
	{
		m_pSound->Uninit();
		delete m_pSound;
		m_pSound = NULL;
	}

	// 全破棄
	CScene::ReleaseAll();
}

//=============================================================================
// 更新処理
//=============================================================================
void CManager::Update(void)
{
	// キーボードの更新処理
	if (m_pInputKeyboard != NULL)
	{
		m_pInputKeyboard->Update();
	}

	// レンダラーの更新処理
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Update();
	}

	//ゲーム画面時のみにポーズを描画＆更新できる
	if (CManager::m_mode == MODE_GAME)
	{
		//ポーズの切り替えオン
		if (m_bPause == false)
		{
			if (m_pInputKeyboard->GetTrigger(DIK_P) == true)
			{
				m_bPause = true;
			}
		}

		//ポーズの切り替えオフ
		else if (m_bPause == true)
		{
			if (m_pInputKeyboard->GetTrigger(DIK_P) == true)
			{
				m_bPause = false;
			}
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CManager::Draw(void)
{
	if (m_pRenderer != NULL)
	{
		// レンダラの描画処理
		m_pRenderer->Draw();
	}
}

//=============================================================================
// モードの設定
//=============================================================================
void CManager::SetMode(MODE mode)
{
	m_bPause = false;

	// 現在のモードの破棄
	switch (m_mode)
	{
	case MODE_TITLE:	// タイトル画面
		if (m_pTitle != NULL)
		{
			m_pTitle->Uninit();
			m_pTitle = NULL;
		}
		break;

	case MODE_TUTORIAL:	// チュートリアル画面
		if (m_pTutorial != NULL)
		{
			m_pTutorial->Uninit();
			m_pTutorial = NULL;
		}
		break;

	case MODE_GAME:		// ゲーム画面
		if (m_pGame != NULL)
		{
			m_pGame->Uninit();
			m_pGame = NULL;
		}
		break;
			
	case MODE_RESULT:	// リザルト画面
		if (m_pResult != NULL)
		{
			m_pResult->Uninit();
			m_pResult = NULL;
		}
		break;
	}

	// シーン全破棄
	CScene::ReleaseAll();

	// モードの設定
	m_mode = mode;

	switch (m_mode)
	{
	case MODE_TITLE:
		if (m_pTitle == NULL)
		{
			// 動的確保
			m_pTitle = new CTitle;
			if (m_pTitle != NULL)
			{
				// 初期化処理
				m_pTitle->Init({ SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2,0.0f }, { SCREEN_WIDTH,SCREEN_HEIGHT,0.0f });
			}
		}
		break;

	case MODE_TUTORIAL:
		if (m_pTutorial == NULL)
		{
			// 動的確保
			m_pTutorial = new CTutorial;
			if (m_pTutorial != NULL)
			{
				// 初期化処理
				m_pTutorial->Init({ SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2,0.0f }, { SCREEN_WIDTH,SCREEN_HEIGHT,0.0f });
			}
		}
		break;

	case MODE_GAME:
		if (m_pGame == NULL)
		{
			// 動的確保
			m_pGame = new CGame;

			if (m_pGame != NULL)
			{
				// 初期化処理
				m_pGame->Init({ 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f });
			}
		}
		break;

	case MODE_RESULT:
		if (m_pResult == NULL)
		{
			// 動的確保
			m_pResult = new CResult;

			if (m_pResult != NULL)
			{
				// 初期化処理
				m_pResult->Init({ SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2,0.0f }, { SCREEN_WIDTH,SCREEN_HEIGHT,0.0f });
			}
		}
		break;
	}
}

//=============================================================================
// ポーズの処理
//=============================================================================
void CManager::SetPause(void)
{
	if (m_pFade->GetFade() == CFade::FADE_NONE)
	{
		m_bPause = m_bPause ? false : true;
	}
}