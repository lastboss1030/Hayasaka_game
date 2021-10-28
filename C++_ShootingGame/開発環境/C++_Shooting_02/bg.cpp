//=============================================================================
//
// 背景処理 [bg.cpp]
// Author : Taiki hayasaka
//
//=============================================================================
#include "bg.h"
#include "scene2D.h"
#include "manager.h"
#include "renderer.h"
#include "Input_Keyboard.h"
#include "player.h"
#include "game.h"

//=============================================================================
// 静的メンバ変数
//=============================================================================
LPDIRECT3DTEXTURE9 CBg::m_apTexture[] = {};

//=============================================================================
// コンストラクタ
//=============================================================================
CBg::CBg(PRIORITY nPriority) : CScene(nPriority)
{
	// 初期化
	for (int nCnt = 0; nCnt < MAX_BG; nCnt++)
	{
		m_apScene2D[nCnt] = NULL;
		m_fSpeed[nCnt] = 0;
		m_fPosTexU[nCnt] = 0;
		m_fPosTexV[nCnt] = 0;
	}
}

//=============================================================================
// デストラクタ
//=============================================================================
CBg::~CBg()
{

}

//=============================================================================
// ロード処理
//=============================================================================
HRESULT CBg::Load(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャの読み込み	
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bg103.png", &m_apTexture[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bg101.png", &m_apTexture[1]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bg102.png", &m_apTexture[2]);

	return S_OK;
}

//=============================================================================
// アンロード
//=============================================================================
void CBg::Unload(void)
{
	//テクスチャの破棄
	for (int nCntBg = 0; nCntBg < MAX_BG; nCntBg++)
	{
		if (m_apTexture[nCntBg] != NULL)
		{
			m_apTexture[nCntBg]->Release();
			m_apTexture[nCntBg] = NULL;
		}
	}
}

//=============================================================================
// 生成処理
//=============================================================================
CBg *CBg::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	CBg *pBg = NULL;

	if (pBg == NULL)
	{
		pBg = new CBg;

		if (pBg != NULL)
		{
			// 初期化処理
			pBg->Init(pos, size);
		}
	}
	return pBg;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CBg::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// 描画順の設定
	m_apScene2D[0] = new CScene2D(PRIORITY_BGBEHING);
	m_apScene2D[1] = new CScene2D(PRIORITY_BGFRONT);
	m_apScene2D[2] = new CScene2D(PRIORITY_BGBEHING);

	for (int nCnt = 0; nCnt < MAX_BG; nCnt++)
	{
		m_apScene2D[nCnt]->Init(pos, size);
		m_apScene2D[nCnt]->BindTexture(m_apTexture[nCnt]);
		m_fPosTexU[nCnt] = 0;
		m_fPosTexV[nCnt] = 0;
	}

	// 背景スピードを代入
	m_fSpeed[0] = SCROL_SPPED01;
	m_fSpeed[1] = SCROL_SPPED02;
	m_fSpeed[2] = SCROL_SPPED03;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CBg::Uninit(void)
{
	for (int nCntBg = 0; nCntBg < MAX_BG; nCntBg++)
	{
		if (m_apScene2D[nCntBg] != NULL)
		{
			m_apScene2D[nCntBg]->Uninit();
			m_apScene2D[nCntBg] = NULL;
		}
	}

	// オブジェクト破棄
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CBg::Update(void)
{
	// キーボード関係
	CInputKeyboard *plnputKeyboard;
	plnputKeyboard = CManager::GetInputKeyboard();

	for (int nCnt = 0; nCnt < MAX_BG; nCnt++)
	{
		if (plnputKeyboard->GetPress(DIK_D) == true)
		{
			// 右に動かす
			m_fPosTexU[nCnt] += m_fSpeed[nCnt] * 0.5f;
		}
		if (plnputKeyboard->GetPress(DIK_A) == true)
		{
			// 左に動かす
			m_fPosTexU[nCnt] -= m_fSpeed[nCnt] * 0.5f;
		}
		if (plnputKeyboard->GetPress(DIK_W) == true)
		{
			// 下に動かす
			m_fPosTexV[nCnt] -= m_fSpeed[nCnt] * 0.5f;
		}
		if (plnputKeyboard->GetPress(DIK_S) == true)
		{
			// 上に動かす
			m_fPosTexV[nCnt] += m_fSpeed[nCnt] * 0.5f;
		}

		m_fPosTexV[nCnt] -= m_fSpeed[nCnt];

		m_apScene2D[nCnt]->SetTexBg(m_fPosTexU[nCnt], m_fPosTexV[nCnt]);
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CBg::Draw(void)
{
	 
}
