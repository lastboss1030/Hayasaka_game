//=============================================================================
//
// ポーズ処理 [pause.cpp]
// Author : taiki hayasaka
//
//=============================================================================
#include "pause.h"
#include "manager.h"
#include "scene2D.h"
#include "renderer.h"
#include "Input_Keyboard.h"
#include "fade.h"
#include "animation.h"
#include "logo.h"

//=============================================================================
// 静的メンバ変数
//=============================================================================
LPDIRECT3DTEXTURE9 CPause::m_apTexture[PAUSE_MAX] = {};

//=============================================================================
// コンストラクタ
//=============================================================================
CPause::CPause(PRIORITY nPriority) :CScene(nPriority)
{
	float m_fMove = 0;
	float m_fAnim = 0;
	int	  m_ColorCnt = 0;
	int   m_nSelectCnt = 0;

	int nTimeGamePause = 0;
	int nTimeCounterPause = 0;
	int g_nPointerPauseX = 0;
	bool g_bButtonDownPause = false;
}

//=============================================================================
// デストラクタ
//=============================================================================
CPause::~CPause()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CPause::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// アニメーション画像ロード
	CAnimation::Load();

	// 背景
	CLogoPause::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), PAUSE01);

	// ロゴ
	CLogoPause::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 350.0f, 0.0f), D3DXVECTOR3(190, 40, 0.0f), PAUSE_CONTINUE);

	CLogoPause::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 600.0f, 0.0f), D3DXVECTOR3(190, 40, 0.0f), PAUSE_RESTART);

	CLogoPause::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 850.0f, 0.0f), D3DXVECTOR3(120, 40, 0.0f), PAUSE_QUIT);

	// パトランプ
	//	CAnimasion::Create(D3DXVECTOR3(550.0f, 125.0f, 0.0f), 100, 80, 5, 18);

	// 値を返す
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CPause::Uninit(void)
{
	for (int nCnt = 0; nCnt < MAX_PAUSE; nCnt++)
	{
		if (m_apScene2D[nCnt] == NULL)
		{
			m_apScene2D[nCnt] = new CScene2D;
			m_apScene2D[nCnt]->Uninit();
			m_apScene2D[nCnt]->BindTexture(m_apTexture[nCnt]);
		}
	}

	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CPause::Update(void)
{
	// キーボード情報の取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	// フェード情報の取得
	CFade *pFade = CManager::GetFade();

	// 位置の取得
	D3DXVECTOR3 pos;
	pos = m_apScene2D[m_nSelectCnt]->GetPosition();

	// 移動の設定
	D3DXVECTOR3 move = m_apScene2D[m_nSelectCnt]->GetMove();

	// 選択処理
	if (pInputKeyboard->GetTrigger(DIK_UP) == true)
	{
		// 上に行くとき番号を減らす
		m_nSelectCnt--;

		if (m_nSelectCnt < PAUSE_CONTINUE)
		{
			// コンティニューより小さくなった時
			m_nSelectCnt = PAUSE_TITLE;
		}
	}
	else if (pInputKeyboard->GetTrigger(DIK_DOWN) == true)
	{
		// 下に行くとき番号を増やす
		m_nSelectCnt++;

		if (m_nSelectCnt > PAUSE_TITLE)
		{
			// タイトルより大きくなった時
			m_nSelectCnt = PAUSE_CONTINUE;
		}
	}

	// 選択時の処理
	for (int nCntPause = PAUSE_CONTINUE; nCntPause < MAX_PAUSE; nCntPause++)
	{
		if (m_nSelectCnt == nCntPause)
		{
			move.x++;

			// 現在選択されているものの色を変える
			m_apScene2D[nCntPause]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
		else
		{
			// それ以外はα値を変える
			m_apScene2D[nCntPause]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f));
		}
	}

	if (pInputKeyboard->GetTrigger(DIK_RETURN) == true)
	{
		switch (m_nSelectCnt)
		{
		case PAUSE_CONTINUE:
			CManager::SetPause(false);

			break;

		case PAUSE_RETRY:
			CManager::SetPause(false);
			pFade->SetFade(CFade::FADE_OUT, CManager::MODE_GAME);

			break;

		case PAUSE_TITLE:
			CManager::SetPause(false);
			pFade->SetFade(CFade::FADE_OUT, CManager::MODE_TITLE);
			break;
		}
	}

	pos += move;
}

//=============================================================================
// 描画処理
//=============================================================================
void CPause::Draw(void)
{
	m_apScene2D[0]->Draw();
}

//=============================================================================
// 生成処理
//=============================================================================
CPause *CPause::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// 動的確保
	CPause* pPause;
	pPause = new CPause;

	// 初期の呼び出し
	pPause->Init(pos, size);

	// 値を返す
	return pPause;
}