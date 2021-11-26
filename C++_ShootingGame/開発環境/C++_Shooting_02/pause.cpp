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

//=============================================================================
// 静的メンバ変数
//=============================================================================
LPDIRECT3DTEXTURE9 CPause::m_apTexture[PAUSE_MAX] = {};

//=============================================================================
// コンストラクタ
//=============================================================================
CPause::CPause(PRIORITY nPriority) :CScene(nPriority)
{

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
	D3DXVECTOR3 Pos[PAUSE_MAX] =  { D3DXVECTOR3(pos.x,pos.y,pos.z),
								    D3DXVECTOR3(pos.x + 400.0f,pos.y + 140.0f,pos.z) ,
								    D3DXVECTOR3(pos.x + 400.0f,pos.y + 220.0f,pos.z) ,
								    D3DXVECTOR3(pos.x + 400.0f,pos.y + 290.0f,pos.z) };

	D3DXVECTOR3 Size[PAUSE_MAX] = { D3DXVECTOR3(size.x,size.y,size.z),
									D3DXVECTOR3(300.0f,300.0f,0.0f) ,
									D3DXVECTOR3(300.0f,300.0f,0.0f) ,
									D3DXVECTOR3(300.0f,300.0f,0.0f) };


	for (int nCnt = 0; nCnt < PAUSE_MAX; nCnt++)
	{
		if (m_apScene2D[nCnt] == NULL)
		{
			m_apScene2D[nCnt] = new CScene2D();
			m_apScene2D[nCnt]->Init(Pos[nCnt], Size[nCnt]);
			m_apScene2D[nCnt]->BindTexture(m_apTexture[nCnt]);
		}
	}

	// ポーズ画面の背景色を変える
	m_apScene2D[PAUSE_NONE]->SetCol(D3DXCOLOR(0.0f, 0.1f, 0.3f, 0.2f));

	// 選択されているUIの保存
	m_nSelectCnt = (int)PAUSE_CONTINUE;

	// 選択時のUIの色を設定
	for (int nCntPauseUI = PAUSE_CONTINUE; nCntPauseUI < PAUSE_MAX; nCntPauseUI++)
	{
		if (m_nSelectCnt == nCntPauseUI)
		{
			// 選択しているときの色
			m_apScene2D[nCntPauseUI]->SetCol(D3DXCOLOR(1.0f, 0.2f, 0.2f, 1.0f));
		}
		else
		{
			// 選択していないときの色
			m_apScene2D[nCntPauseUI]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f));

		}
	}

	m_fMove = 0.0f;

	// 種類
	SetObjType(OBJTYPE_PAUSE);

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

//=============================================================================
// テクスチャ読み込み
//=============================================================================
HRESULT CPause::Load(void)
{
	// デバイスの設定
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/pause_bg.png", &m_apTexture[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/pause_continue.png", &m_apTexture[1]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/pause_retry.png", &m_apTexture[2]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/pause_title.png", &m_apTexture[3]);

	return S_OK;
}

//=============================================================================
// テクスチャ破棄
//=============================================================================
void CPause::Unload(void)
{
	for (int nCnt = 0; nCnt < MAX_PAUSE; nCnt++)
	{
		// テクスチャの開放
		if (m_apTexture[nCnt] != NULL)
		{
			m_apTexture[nCnt]->Release();
			m_apTexture[nCnt] = NULL;
		}
	}
}