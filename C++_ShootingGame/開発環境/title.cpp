//=============================================================================
//
// タイトル処理 [title.cpp]
// Author : taiki hayasaka
//
//=============================================================================
#include "title.h"
#include "manager.h"
#include "renderer.h"
#include "Input_Keyboard.h"
#include "fade.h"
#include "sound.h"
#include "animation.h"
#include "logo.h"
#include "bg.h"

//=============================================================================
// 静的メンバ変数
//=============================================================================
LPDIRECT3DTEXTURE9 CTitle::m_pTexture = NULL;

// グローバル変数宣言
int nTimeTitle = 0;					// 時間
int nTimeCounterTitle = 0;			// 時間カウンター
int nNexEnterCount = 0;				// ロゴEnterの色変更
int nSaveTime = -1;					// アニメーション用
int nSaveTime2 = -1;				// アニメーション用
int nFlashing = 30;					// PressEnterの点滅
bool g_bPressEnter = false;			// ロゴの表示条件
bool g_bNextEnter = false;			// ロゴEnterの色変化に使用
bool g_bButtonDownTitle = false;	// 連打防止用


//=============================================================================
// コンストラクタ
//=============================================================================
CTitle::CTitle()
{
	//初期化
	nTimeTitle = 0;
	nTimeCounterTitle = 0;
	nNexEnterCount = 0;
	nSaveTime = -1;
	nSaveTime2 = -1;
	nFlashing = 30;
	g_bPressEnter = false;
	g_bNextEnter = false;
	g_bButtonDownTitle = false;
}

//=============================================================================
// デストラクタ
//=============================================================================
CTitle::~CTitle()
{

}

//=============================================================================
// ロード
//=============================================================================
HRESULT CTitle::Load(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/title_002.png", &m_pTexture);

	return S_OK;
}

//=============================================================================
// アンロード
//=============================================================================
void CTitle::Unload(void)
{
	// テクスチャの開放
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================================================================
// 背景の生成
//=============================================================================
CTitle *CTitle::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	CTitle *pTitle = NULL;

	if (pTitle == NULL)
	{
		 // 動的確保
		pTitle = new CTitle;

		if (pTitle != NULL)
		{
			// 初期化処理
			pTitle->Init(pos, size);
		}
	}
	return pTitle;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CTitle::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// サウンドを取得
	CSound *pSound;
	pSound = CManager::GetSound();

	// タイトルBGM
	pSound->Play(CSound::SOUND_LABEL_BGM_TITLE);

	// 動的確保(メモリの確保)
	m_pScene2D = new CScene2D;

	// 画像を読み込む
	Load();

	// 魔法陣テスト
//	CAnimation::Create(D3DXVECTOR3(400.0f, 60.0f, 0.0f), D3DXVECTOR3(100.0f, 80.0f,0.0f), 5, 1);

	// タイトルロゴ生成
	CLogo::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 100.0f, 0.0f), D3DXVECTOR3(700.0f, 300.0f, 0.0f), TITLE_UI_LOGO);

	// 初期化処理
	m_pScene2D->Init(pos, size);

	// テクスチャの割り当て
	m_pScene2D->BindTexture(m_pTexture);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CTitle::Uninit(void)
{
	// 画像の破棄
	Unload();

	// 破棄する
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CTitle::Update(void)
{
	// キーボード関係
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	// モード関係
	CManager::MODE pManager;
	pManager = CManager::GetMode();

	// フェード関係
	CFade *pFade;
	pFade = CManager::GetFade();

	// サウンドを取得
	CSound *pSound;
	pSound = CManager::GetSound();

	// Enterロゴの表示
	if (g_bPressEnter == false)
	{
		CLogo::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 200.0f, 0.0f), D3DXVECTOR3(450.0f, 100.0f, 0.0f), TITLE_UI_PRESS_ENTER);		//Enterロゴ
		g_bPressEnter = true;
	}

	// 点滅処理
	if (g_bPressEnter == true)
	{
		// カウントアップ
		nNexEnterCount++;
		if (nNexEnterCount == nFlashing)
		{
			// 透明にする
			if (g_bNextEnter == true)
			{
				CLogo::TypeChangeCollar(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), TITLE_UI_PRESS_ENTER);		//色変化
				g_bNextEnter = false;
			}
			// 不透明にする
			else if (g_bNextEnter == false)
			{
				CLogo::TypeChangeCollar(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), TITLE_UI_PRESS_ENTER);		//色変化
				g_bNextEnter = true;
			}

			// リセット
			nNexEnterCount = 0;
		}
	}

	if (g_bButtonDownTitle == false)
	{
		// 画面遷移
		if (pInputKeyboard->GetTrigger(DIK_RETURN) == true)
		{
			// EnterSE
			pSound->Play(CSound::SOUND_LABEL_SE_ENTER);

			// 画面遷移に関する変数
			nTimeCounterTitle = 0;

			// 点滅に関する変数
			nFlashing = 5;
			nNexEnterCount = 0;

			// ボタンを押したタイムを保存
			nSaveTime = nTimeTitle;
			nSaveTime2 = 15;

			// 連打防止
			g_bButtonDownTitle = true;
		}
	}

	// 一定カウントで
	if ((nSaveTime + 1) == nTimeTitle && nSaveTime2 == nTimeCounterTitle)
	{
		//モード設定
		pFade->SetFade(CFade::FADE_OUT, CManager::MODE_TUTORIAL);	// チュートリアル画面へ
	}

	// タイムを進める
	nTimeCounterTitle++;

	// カウンターが一定数で
	if (nTimeCounterTitle >= 30)
	{
		// 秒数を進める
		nTimeTitle++;

		//カウンターを戻す
		nTimeCounterTitle = 0;
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CTitle::Draw(void)
{

}