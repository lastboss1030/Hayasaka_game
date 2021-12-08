//=============================================================================
//
// チュートリアル処理 [tutorial.cpp]
// Author : taiki hayasaka
//
//=============================================================================
#include "tutorial.h"
#include "manager.h"
#include "renderer.h"
#include "Input_Keyboard.h"
#include "fade.h"
#include "sound.h"
#include "animation.h"

//=============================================================================
// 静的メンバ変数
//=============================================================================
LPDIRECT3DTEXTURE9 CTutorial::m_pTexture = NULL;

//=============================================================================
// コンストラクタ
//=============================================================================
CTutorial::CTutorial()
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CTutorial::~CTutorial()
{

}

//=============================================================================
// ロード
//=============================================================================
HRESULT CTutorial::Load(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/tutorial.png", &m_pTexture);

	return S_OK;
}

//=============================================================================
// アンロード
//=============================================================================
void CTutorial::Unload(void)
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
CTutorial *CTutorial::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	CTutorial *pTutorial = NULL;

	if (pTutorial == NULL)
	{
		// 動的確保
		pTutorial = new CTutorial;

		if (pTutorial != NULL)
		{
			// 初期化処理
			pTutorial->Init(pos, size);
		}
	}
	return pTutorial;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CTutorial::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// サウンドを取得
	CSound *pSound;
	pSound = CManager::GetSound();

	// タイトルBGM
	pSound->Play(CSound::SOUND_LABEL_BGM_TUTORIAL);

	// 動的確保(メモリの確保)
	m_pScene2D = new CScene2D;

	// 画像を読み込む
	Load();

	// 初期化処理
	m_pScene2D->Init(pos, size);

	// テクスチャの割り当て
	m_pScene2D->BindTexture(m_pTexture);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CTutorial::Uninit(void)
{
	// 画像の破棄
	Unload();

	// 破棄する
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CTutorial::Update(void)
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

	// タイトルBGMオフ
	pSound->Stop(CSound::SOUND_LABEL_BGM_TITLE);

	// エンターを押したとき
	if (pFade->GetFade() == CFade::FADE_NONE && pInputKeyboard->GetTrigger(DIK_RETURN) == true)
	{
		// タイトルBGM
		pSound->Play(CSound::SOUND_LABEL_SE_ENTER);

		// モードの設定
		pFade->SetFade(CFade::FADE_OUT, CManager::MODE_GAME);
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CTutorial::Draw(void)
{

}