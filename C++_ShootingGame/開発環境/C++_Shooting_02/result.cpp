//=============================================================================
// 
// リザルト処理 [result.cpp]
// Author : taiki hayasaka
//
//=============================================================================
#include "result.h"
#include "bg.h"
#include "input.h"
#include "manager.h"
#include "sound.h"
#include "renderer.h"
#include "Input_Keyboard.h"
#include "fade.h"
#include "game.h"

//=============================================================================
// 静的メンバ変数
//=============================================================================
LPDIRECT3DTEXTURE9 CResult::m_apTexture[] = {};

//=============================================================================
// コンストラクタ
//=============================================================================
CResult::CResult()
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CResult::~CResult()
{


}

//=============================================================================
// ロード
//=============================================================================
HRESULT CResult::Load(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Result.png", &m_apTexture[TEXTURETYPE_GAMECLEAR]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/GameOver.png", &m_apTexture[TEXTURETYPE_GAMEOVER]);

	return S_OK;
}

//=============================================================================
// アンロード
//=============================================================================
void CResult::Unload(void)
{
	for (int nCnt = 0; nCnt < TEXTURETYPE_MAX; nCnt++)
	{
		// テクスチャの開放
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
CResult *CResult::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	CResult *pResult = NULL;

	if (pResult == NULL)
	{
		// 動的確保
		pResult = new CResult;

		if (pResult != NULL)
		{
			// 初期化処理
			pResult->m_pScene2D = NULL;
			pResult->Init(pos, size);
		}
	}
	return pResult;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CResult::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// 初期化
	m_pScene2D = NULL;

	// 画像読み込み
	Load();

	// サウンドを取得
	CSound *pSound;
	pSound = CManager::GetSound();

	// リザルトBGM
	pSound->Play(CSound::SOUND_LABEL_BGM_RESULT);

	if (m_pScene2D == NULL)
	{
		m_pScene2D = CScene2D::Create(CENTER_POS, D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f));
	}

	if (m_pScene2D != NULL)
	{
		// テクスチャの設定
		if (CGame::GetResult() == CGame::RESULTMODE_GAMECLEAR)
		{
			// クリア画面
			m_pScene2D->BindTexture(m_apTexture[TEXTURETYPE_GAMECLEAR]);
		}
		else if (CGame::GetResult() == CGame::RESULTMODE_GAMEOVER)
		{
			// ゲームオーバ画面
			m_pScene2D->BindTexture(m_apTexture[TEXTURETYPE_GAMEOVER]);
		}
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CResult::Uninit(void)
{
	//サウンドを取得
	CSound *pSound;
	pSound = CManager::GetSound();

	// 画像の破棄
	Unload();

	// 破棄
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CResult::Update(void)
{
	// キーボードの取得
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

	// モード関係
	CManager::MODE pManager = CManager::GetMode();

	// フェード関係
	CFade *pFade = CManager::GetFade(); 

	// エンターを押したとき
	if (pFade->GetFade() == CFade::FADE_NONE&& pInputKeyboard->GetTrigger(DIK_RETURN) == true)
	{
		// タイトルに移動
		pFade->SetFade(CFade::FADE_OUT, CManager::MODE_TITLE);
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CResult::Draw(void)
{

}