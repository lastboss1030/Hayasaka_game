//=============================================================================
//
// ロゴ処理 [logo.cpp]
// Author : taiki hayasaka
//
//=============================================================================
#include "logo.h"
#include "manager.h"
#include "scene2D.h"
#include "scene.h"
#include "manager.h"
#include "renderer.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CLogo::m_apTextureLogo[MAX_LOGO] = {};
CPlayer *CLogo::m_pPlayer = NULL;

//グローバル宣言
int g_nTypeLogo = -1;			// 特殊処理をするタイプを入れる
int g_nTypeMoveLogo = -1;		// 特殊処理をするタイプを入れる
int g_nTypeCollarLogo = -1;		// 特殊処理をするタイプを入れる
D3DXVECTOR3 g_posLogo;			// 位置
D3DXCOLOR g_col;				// 色情報

//=============================================================================
// コンストラクタ
//=============================================================================
CLogo::CLogo(PRIORITY nPriority) : CScene2D(nPriority)
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CLogo::~CLogo()
{
	g_nTypeLogo = -1;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CLogo::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, int nType)
{
	// CScene2Dの初期化処理
	CScene2D::Init(pos, size);

	//タイプを代入
	m_nType = nType;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CLogo::Uninit(void)
{
	CScene2D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CLogo::Update(void)
{
	//特殊更新
	//タイトル移動
	if (m_nType == 4)
	{
		// 現在の位置の取得
		D3DXVECTOR3 pos = GetPosition();
		D3DXVECTOR3 size = GetSize();

		//移動させる
		SetPosition(pos, size);

		//更新
		CScene2D::Update();
	}

	//プレイヤー当たり判定の移動
	if (m_nType == 9)
	{
		//色を変える
		CScene2D::SetCol(D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));

		//プレイヤーの位置を取得
		D3DXVECTOR3 posPlayer = m_pPlayer->GetPos();
		D3DXVECTOR3 sizePlayer = m_pPlayer->GetSize();

		//移動させる
		SetPosition(posPlayer, sizePlayer);

		//更新
		CScene2D::Update();
	}

	//特殊更新
	if (m_nType == 0)
	{
		//色を変える
		CScene2D::SetCol(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
	}

	//途中色変更
	if (m_nType == g_nTypeCollarLogo)
	{
		//色を変える
		CScene2D::SetCol(g_col);
	}

	//途中移動
	if (m_nType == g_nTypeMoveLogo)
	{
		//初期化
		g_nTypeMoveLogo = -1;

		//移動させる
		SetPosition(g_posLogo, D3DXVECTOR3(5.0f,5.0f,0.0f));

		//更新
		CScene2D::Update();
	}

	//途中破棄
	if (g_nTypeLogo == m_nType)
	{
		Uninit();
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CLogo::Draw(void)
{
	CScene2D::Draw();
}

//=============================================================================
// 生成処理
//=============================================================================
CLogo * CLogo::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, int nType)
{
	// ローカル変数宣言
	CLogo *pLogo = NULL;
	pLogo = new CLogo;

	// 初期化
	if (pLogo != NULL)
	{
		pLogo->Init(pos, size, nType);
	}

	// テクスチャ割り当て
	pLogo->BindTexture(m_apTextureLogo[nType]);

	return pLogo;
}

//=============================================================================
// テクスチャの読み込み
//=============================================================================
HRESULT CLogo::Load(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャの読み込み	
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/gameover1.jpg", &m_apTextureLogo[PAUSE_RESTART]);		// ポーズリスタート
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/gameover2.jpg", &m_apTextureLogo[PAUSE_CONTINUE]);		// ポーズコンティニュー
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/gameover3.jpg", &m_apTextureLogo[PAUSE_TITLE]);		// ポーズタイトル

	return S_OK;
}

//=============================================================================
// テクスチャの破棄
//=============================================================================
void CLogo::Unload(void)
{
	for (int nCnt = 0; nCnt < MAX_LOGO; nCnt++)
	{
		if (m_apTextureLogo != NULL)
		{
			m_apTextureLogo[nCnt]->Release();
			m_apTextureLogo[nCnt] = NULL;
		}
	}
}

//=============================================================================
// 破棄したいタイプ
//=============================================================================
void CLogo::TypeUninit(int nType)
{
	g_nTypeLogo = nType;
}

//=============================================================================
// 位置をずらしたいタイプ
//=============================================================================
void CLogo::TypeMove(D3DXVECTOR3 pos, int nTypeMove)
{
	g_posLogo = pos;
	g_nTypeMoveLogo = nTypeMove;
}

//=============================================================================
// 指定したタイプの色を変える
//=============================================================================
void CLogo::TypeChangeCollar(D3DXCOLOR col, int nTypeCollar)
{
	g_nTypeCollarLogo = nTypeCollar;
	g_col = col;
}



//=============================================================================
//
// ポーズロゴ処理 [ logo.cpp (pause) ]
// Author : taiki hayasaka
//
//=============================================================================

// ポーズ画面に表示させたい場合

//=============================================================================
// コンストラクタ
//=============================================================================
CLogoPause::CLogoPause(PRIORITY nPriority) : CLogo(nPriority)
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CLogoPause::~CLogoPause()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CLogoPause::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, int nType)
{
	CScene2D::Init(pos, size);

	CLogoPause::m_nType = nType;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CLogoPause::Uninit(void)
{
	CScene2D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CLogoPause::Update(void)
{
	// 特殊更新
	// タイトル移動
	if (m_nType == 0)
	{
		// 現在の位置の取得
		D3DXVECTOR3 pos = GetPosition();
		D3DXVECTOR3 size = GetSize();

		// 移動させる
		SetPosition(pos, size);

		// 更新
		CScene2D::Update();
	}

	// プレイヤー当たりポインター判定の移動
	if (m_nType == 5)
	{
		// 色を変える
		CScene2D::SetCol(D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));

		// プレイヤーの位置を取得
		D3DXVECTOR3 posPlayer = m_pPlayer->GetPos();
		D3DXVECTOR3 sizePlayer = m_pPlayer->GetSize();

		// 移動させる
		SetPosition(posPlayer, sizePlayer);

		// 更新
		CScene2D::Update();
	}

	if (m_nType == 9)
	{
		// 色を変える
		CScene2D::SetCol(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
	}

	if (m_nType == g_nTypeCollarLogo)
	{
		// 色を変える
		CScene2D::SetCol(g_col);
	}

	// 途中移動
	if (m_nType == g_nTypeMoveLogo)
	{
		// 初期化
		g_nTypeMoveLogo = -1;

		// 移動させる
		SetPosition(g_posLogo, D3DXVECTOR3(5.0f,5.0f,0.0f));

		// 更新
		CScene2D::Update();
	}

	// 途中終了
	if (g_nTypeLogo == m_nType)
	{
		Uninit();
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CLogoPause::Draw(void)
{
	CScene2D::Draw();
}

//=============================================================================
// 生成処理
//=============================================================================
CLogoPause * CLogoPause::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, int nType)
{
	// ローカル変数宣言
	CLogoPause *pLogoPause = NULL;
	pLogoPause = new CLogoPause;

	// 初期化
	if (pLogoPause != NULL)
	{
		pLogoPause->Init(pos, size, nType);
	}

	// テクスチャ割り当て
	pLogoPause->BindTexture(m_apTextureLogo[nType]);

	return pLogoPause;
}

//=============================================================================
// テクスチャの破棄
//=============================================================================
void CLogoPause::Unload(void)
{
	for (int nCnt = 0; nCnt < MAX_LOGO; nCnt++)
	{
		if (m_apTextureLogo != NULL)
		{
			m_apTextureLogo[nCnt]->Release();
			m_apTextureLogo[nCnt] = NULL;
		}
	}
}

//=============================================================================
// 破棄したいタイプ
//=============================================================================
void CLogoPause::TypeUninit(int nType)
{
	g_nTypeLogo = nType;
}

//=============================================================================
// 位置をずらしたいタイプ
//=============================================================================
void CLogoPause::TypeMove(D3DXVECTOR3 pos, int nTypeMove)
{
	g_posLogo = pos;
	g_nTypeMoveLogo = nTypeMove;
}

//=============================================================================
// 色を変えたいタイプ
//=============================================================================
void CLogoPause::TypeChangeCollar(D3DXCOLOR col, int nTypeCollar)
{
	g_nTypeCollarLogo = nTypeCollar;
	g_col = col;
}
