//=============================================================================
//
// マネージャー処理 [manager.cpp]
// Author : Taiki Hayasaka
//
//=============================================================================
#include "manager.h"
#include "scene2D.h"
#include "player.h"
#include "game.h"
#include "renderer.h"		
#include "scene.h"			
#include "Input_Keyboard.h"			
#include "fade.h"					
#include "polygon.h"	
#include "fade.h"

//=============================================================================
// 静的メンバ変数
//=============================================================================
CRenderer *CManager::m_pRenderer = NULL;
CInputKeyboard *CManager::m_pInputKeyboard = NULL;
CFade *CManager::m_pFade = NULL;
CGame *CManager::m_pGame = NULL;
CManager::MODE CManager::m_mode = MODE_GAME;		// 現在のモード

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

	// フェードの生成
	m_pFade = CFade::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f),
							D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f), m_mode);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CManager::Uninit(void)
{
	// プレイヤーの破棄
	CPlayer::Unload();

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
	// 現在のモードの破棄
	switch (m_mode)
	{
	case MODE_GAME:		// ゲーム画面
		if (m_pGame != NULL)
		{
			m_pGame->Uninit();
			m_pGame = NULL;
		}
		break;
	}

	// シーン全破棄
	CScene::ReleaseAll();

	// モードの設定
	m_mode = mode;

	switch (m_mode)
	{
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
	}
}