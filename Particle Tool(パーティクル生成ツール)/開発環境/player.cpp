//=============================================================================
//
// プレイヤー処理[player.cpp]
// Author : Taiki Hayasaka
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "player.h"
#include "input.h"
#include "manager.h"
#include "renderer.h"
#include "scene2D.h"		
#include "Input_Keyboard.h"	
#include "game.h"			
#include "fade.h"			
#include "particle.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//=============================================================================
// 静的メンバ変数
//=============================================================================
LPDIRECT3DTEXTURE9 CPlayer::m_pTexture = NULL;

// パーティクル
CPlayer::PARTICLE CPlayer::m_aParticle[MAX_PARTICLE] = 
{
	20,										// 生成数
	8,										// 速度
	20.0f,									// 半径
	70,										// 寿命
	1.00f,									// 慣性
	628,									// 範囲
	1.0f,									// 角度
	D3DXCOLOR(1.0f,1.0f,1.0f,1.0f),			// 色(R,G,B,A)
};

int CPlayer::m_nIndexParticle = 0;			// 保存番号
D3DXVECTOR3 g_pos;	// 取得用
#define PARTICLE_FILE_NAME "particle.txt"	// テキスト

//=============================================================================
// マクロ定義
//=============================================================================
#define PLAYER_RIGHT	(5.0f)			// プレイヤーの移動量(右)
#define PLAYER_LEFT		(-5.0f)			// プレイヤーの移動量(左)

//=============================================================================
// コンストラクタ
//=============================================================================
CPlayer::CPlayer(PRIORITY nPriority) :CScene2D(nPriority)
{
	// 初期化
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fPosTexU = 0;
	m_fPosTexV = 0;

	// パーティクル用数値初期化
	for (int nCnt = 0; nCnt < MAX_PARTICLE; nCnt++)
	{
		m_aParticle[nCnt].nCreateNum = 20;							// 生成数
		m_aParticle[nCnt].nSpeed = 8;								// 速度
		m_aParticle[nCnt].fRadius = 20.0f;							// 半径
		m_aParticle[nCnt].nLife = 70;								// 寿命
		m_aParticle[nCnt].fInertia = 1.00f;							// 慣性
		m_aParticle[nCnt].nRange = 628;								// 範囲
		m_aParticle[nCnt].fAngle = 1.0f;							// 角度
		m_aParticle[nCnt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 色(R,G,B,A)
	}
}

//=============================================================================
// デストラクタ
//=============================================================================
CPlayer::~CPlayer()
{

}

//=============================================================================
// 生成
//=============================================================================
CPlayer *CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	CPlayer *pPlayer = NULL;

	if (pPlayer == NULL)
	{
		 // 動的確保
		pPlayer = new CPlayer;

		if (pPlayer != NULL)
		{
			 // 初期化処理
			pPlayer->Init(pos, size);

			// テクスチャの割り当て
			pPlayer->BindTexture(m_pTexture);
		}
	}
	return pPlayer;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CPlayer::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// サイズ
	m_size = size;

	// パーティクル用数値初期化
	m_aParticle[m_nIndexParticle].nCreateNum = 20;	// 生成数
	m_aParticle[m_nIndexParticle].nSpeed = 8;		// 速度
	m_aParticle[m_nIndexParticle].fRadius = 20.0f;	// 半径
	m_aParticle[m_nIndexParticle].nLife = 70;		// 寿命
	m_aParticle[m_nIndexParticle].fInertia = 1.00f;	// 慣性
	m_aParticle[m_nIndexParticle].nRange = 628;		// 範囲
	m_aParticle[m_nIndexParticle].fAngle = 1.0;		// 角度

	m_aParticle[m_nIndexParticle].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 色(R,G,B,A)

	// CScene2Dの初期化処理
	CScene2D::Init(pos, size);

	// オブジェクトの種類設定
	SetObjType(CScene::OBJTYPE_PLAYER);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CPlayer::Uninit(void)
{
	// Scene2Dの終了処理
	CScene2D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CPlayer::Update(void)
{
	// CInputKeyboardインスタンスの取得
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

	// 現在の位置の取得
	D3DXVECTOR3 pos = GetPosition();

	// カラー設定
	SetCol(m_aParticle[m_nIndexParticle].col);

	// 移動処理
	if (pInputKeyboard->GetPress(DIK_W) == true)
	{
		m_move.y -= 2.0f;
	}
	if (pInputKeyboard->GetPress(DIK_S) == true)
	{
		m_move.y += 2.0f;
	}
	if (pInputKeyboard->GetPress(DIK_A) == true)
	{
		m_move.x -= 2.0f;
	}
	if (pInputKeyboard->GetPress(DIK_D) == true)
	{
		m_move.x += 2.0f;
	}

	// 慣性処理
	m_move.x += (0.0f - m_move.x) * 0.2f;
	m_move.y += (0.0f - m_move.y) * 0.2f;

	// 座標の設定
	pos += m_move;

	// 画面端処理
	if (pos.x - m_size.x / 2 <= 0.0f)
	{
		pos.x = m_size.x / 2;
	}
	if (pos.x + m_size.x / 2 >= SCREEN_WIDTH)
	{
		pos.x = SCREEN_WIDTH - m_size.x / 2;
	}
	if (pos.y - m_size.y / 2 <= 0.0f)
	{
		pos.y = m_size.y / 2;
	}
	if (pos.y + m_size.y / 2 >= SCREEN_HEIGHT)
	{
		pos.y = SCREEN_HEIGHT - m_size.y / 2;
	}

	// 数値変更
	ChangeParticle();

	// 上限下限
	PlayerLimit();

	// 座標代入
	g_pos = pos;

	// パーティクル生成
	if (pInputKeyboard->GetTrigger(DIK_SPACE) == true)
	{
		for (int nCntEffect = 0; nCntEffect < m_aParticle[m_nIndexParticle].nCreateNum; nCntEffect++)	// 個数
		{
			//角度の設定
			float fAngle = ((float)(rand() % m_aParticle[m_nIndexParticle].nRange - (m_aParticle[m_nIndexParticle].nRange / 2))) / 100 + D3DX_PI * m_aParticle[m_nIndexParticle].fAngle;	// 角度
			float fmove = (float)(rand() % m_aParticle[m_nIndexParticle].nSpeed);	// 速度

			// パーティクル生成
			CParticle::Create(pos,																// 座標
				D3DXVECTOR3(sinf(fAngle) * fmove, cosf(fAngle) * fmove, 5),						// 移動量
				D3DXVECTOR3(m_aParticle[m_nIndexParticle].fRadius, m_aParticle[m_nIndexParticle].fRadius, 0),	// サイズ
				m_aParticle[m_nIndexParticle].col,												// カラー
				m_aParticle[m_nIndexParticle].nLife,											// 寿命
				m_aParticle[m_nIndexParticle].fInertia);										// 慣性
		}
	}

	// セーブ
	if (pInputKeyboard->GetPress(DIK_F1) == true)
	{
		SaveData();
	}

	// ロード
	if (pInputKeyboard->GetPress(DIK_F2) == true)
	{
		LoadData();
	}

	// 座標の設定
	CScene2D::SetPosition(pos, m_size);
}

//=============================================================================
// 描画処理
//=============================================================================
void CPlayer::Draw(void)
{
	// Scene2Dの描画処理
	CScene2D::Draw();
}

//=============================================================================
// テクスチャのロード処理
//=============================================================================
HRESULT CPlayer::Load(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ParticlePlayer.png", &m_pTexture);

	return S_OK;
}

//=============================================================================
// テクスチャの破棄
//=============================================================================
void CPlayer::Unload(void)
{
	// テクスチャの破棄
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================================================================
// 下限上限
//=============================================================================
void CPlayer::PlayerLimit(void)
{
	// 色(R)
	if (m_aParticle[m_nIndexParticle].col.r > 1.0f)
	{
		m_aParticle[m_nIndexParticle].col.r = 1.0f;
	}
	else if (m_aParticle[m_nIndexParticle].col.r < 0.0f)
	{
		m_aParticle[m_nIndexParticle].col.r = 0.0f;
	}

	// 色(G)
	if (m_aParticle[m_nIndexParticle].col.g > 1.0f)
	{
		m_aParticle[m_nIndexParticle].col.g = 1.0f;
	}
	else if (m_aParticle[m_nIndexParticle].col.g < 0.0f)
	{
		m_aParticle[m_nIndexParticle].col.g = 0.0f;
	}

	// 色(B)
	if (m_aParticle[m_nIndexParticle].col.b > 1.0f)
	{
		m_aParticle[m_nIndexParticle].col.b = 1.0f;
	}
	else if (m_aParticle[m_nIndexParticle].col.b < 0.0f)
	{
		m_aParticle[m_nIndexParticle].col.b = 0.0f;
	}

	// 生成数
	if (m_aParticle[m_nIndexParticle].nCreateNum > 500)
	{
		m_aParticle[m_nIndexParticle].nCreateNum = 500;
	}
	if (m_aParticle[m_nIndexParticle].nCreateNum < 10)
	{
		m_aParticle[m_nIndexParticle].nCreateNum = 10;
	}

	// 速度
	if (m_aParticle[m_nIndexParticle].nSpeed > 30)
	{
		m_aParticle[m_nIndexParticle].nSpeed = 30;
	}
	if (m_aParticle[m_nIndexParticle].nSpeed < 2)
	{
		m_aParticle[m_nIndexParticle].nSpeed = 2;
	}

	// 半径
	if (m_aParticle[m_nIndexParticle].fRadius > 70)
	{
		m_aParticle[m_nIndexParticle].fRadius = 70;
	}
	if (m_aParticle[m_nIndexParticle].fRadius < 10)
	{
		m_aParticle[m_nIndexParticle].fRadius = 10;
	}

	// 寿命
	if (m_aParticle[m_nIndexParticle].nLife > 150)
	{
		m_aParticle[m_nIndexParticle].nLife = 150;
	}
	if (m_aParticle[m_nIndexParticle].nLife < 20)
	{
		m_aParticle[m_nIndexParticle].nLife = 20;
	}

	// 慣性
	if (m_aParticle[m_nIndexParticle].fInertia > 1.0f)
	{
		m_aParticle[m_nIndexParticle].fInertia = 1.0f;
	}
	if (m_aParticle[m_nIndexParticle].fInertia < 0.8f)
	{
		m_aParticle[m_nIndexParticle].fInertia = 0.8f;
	}

	// 範囲
	if (m_aParticle[m_nIndexParticle].nRange > 628)
	{
		m_aParticle[m_nIndexParticle].nRange = 628;
	}

	if (m_aParticle[m_nIndexParticle].nRange < 40)
	{
		m_aParticle[m_nIndexParticle].nRange = 40;
	}

	// 角度
	if (m_aParticle[m_nIndexParticle].fAngle > 1.00f)
	{
		m_aParticle[m_nIndexParticle].fAngle = 1.00f;
	}
	if (m_aParticle[m_nIndexParticle].fAngle < -1.00)
	{
		m_aParticle[m_nIndexParticle].fAngle = 1.00f;
	}

}

//=============================================================================
// パーティクル数値変更
//=============================================================================
void CPlayer::ChangeParticle(void)
{
	// CInputKeyboardインスタンスの取得
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

	// リセット
	if (pInputKeyboard->GetTrigger(DIK_1) == true)
	{
		// パーティクル用数値初期化
		m_aParticle[m_nIndexParticle].nCreateNum = 20;
		m_aParticle[m_nIndexParticle].nSpeed = 8;
		m_aParticle[m_nIndexParticle].fRadius = 20.0f;
		m_aParticle[m_nIndexParticle].nLife = 70;
		m_aParticle[m_nIndexParticle].fInertia = 1.00f;
		m_aParticle[m_nIndexParticle].nRange = 628;
		m_aParticle[m_nIndexParticle].fAngle = 1.0;

		m_aParticle[m_nIndexParticle].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}

	// 生成数
	if (pInputKeyboard->GetTrigger(DIK_UP) == true)
	{
		m_aParticle[m_nIndexParticle].nCreateNum += 10;
	}
	else if (pInputKeyboard->GetTrigger(DIK_DOWN) == true)
	{
		m_aParticle[m_nIndexParticle].nCreateNum -= 10;
	}

	// 速度
	if (pInputKeyboard->GetTrigger(DIK_T) == true)
	{
		m_aParticle[m_nIndexParticle].nSpeed += 1;
	}
	if (pInputKeyboard->GetTrigger(DIK_G) == true)
	{
		m_aParticle[m_nIndexParticle].nSpeed -= 1;
	}

	// 半径
	if (pInputKeyboard->GetTrigger(DIK_Y) == true)
	{
		m_aParticle[m_nIndexParticle].fRadius += 2;
	}
	if (pInputKeyboard->GetTrigger(DIK_H) == true)
	{
		m_aParticle[m_nIndexParticle].fRadius -= 2;
	}

	// 寿命
	if (pInputKeyboard->GetTrigger(DIK_U) == true)
	{
		m_aParticle[m_nIndexParticle].nLife += 10;
	}
	if (pInputKeyboard->GetTrigger(DIK_J) == true)
	{
		m_aParticle[m_nIndexParticle].nLife -= 10;
	}

	// 慣性
	if (pInputKeyboard->GetTrigger(DIK_I) == true)
	{
		m_aParticle[m_nIndexParticle].fInertia += 0.01f;
	}
	if (pInputKeyboard->GetTrigger(DIK_K) == true)
	{
		m_aParticle[m_nIndexParticle].fInertia -= 0.01f;
	}

	// 範囲
	if (pInputKeyboard->GetTrigger(DIK_Z) == true)
	{
		m_aParticle[m_nIndexParticle].nRange -= m_aParticle[m_nIndexParticle].nRange / 4;
	}

	if (pInputKeyboard->GetTrigger(DIK_C) == true)
	{
		m_aParticle[m_nIndexParticle].nRange = m_aParticle[m_nIndexParticle].nRange * 2;
	}

	// 角度
	if (pInputKeyboard->GetTrigger(DIK_R) == true)
	{
		m_aParticle[m_nIndexParticle].fAngle += 0.1f;
	}

	if (pInputKeyboard->GetTrigger(DIK_F) == true)
	{
		m_aParticle[m_nIndexParticle].fAngle -= 0.1f;
	}

	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// カラー
	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	// カラーR
	if (pInputKeyboard->GetTrigger(DIK_NUMPAD7) == true)
	{
		m_aParticle[m_nIndexParticle].col.r -= 0.1f;
	}
	if (pInputKeyboard->GetTrigger(DIK_NUMPAD9) == true)
	{
		m_aParticle[m_nIndexParticle].col.r += 0.1f;
	}

	// カラーG
	if (pInputKeyboard->GetTrigger(DIK_NUMPAD4) == true)
	{
		m_aParticle[m_nIndexParticle].col.g -= 0.1f;
	}
	if (pInputKeyboard->GetTrigger(DIK_NUMPAD6) == true)
	{
		m_aParticle[m_nIndexParticle].col.g += 0.1f;
	}

	// カラーB
	if (pInputKeyboard->GetTrigger(DIK_NUMPAD1) == true)
	{
		m_aParticle[m_nIndexParticle].col.b -= 0.1f;
	}
	if (pInputKeyboard->GetTrigger(DIK_NUMPAD3) == true)
	{
		m_aParticle[m_nIndexParticle].col.b += 0.1f;
	}

	// 保存番号
	if (pInputKeyboard->GetTrigger(DIK_RIGHT) == true)
	{
		m_nIndexParticle = (m_nIndexParticle + 1) % MAX_PARTICLE;
	}
	if (pInputKeyboard->GetTrigger(DIK_LEFT) == true)
	{
		m_nIndexParticle = (m_nIndexParticle + MAX_PARTICLE - 1) % MAX_PARTICLE;
	}
}

//=============================================================================
// プレイヤーの移動量取得
//=============================================================================
D3DXVECTOR3 CPlayer::GetMove(void)
{
	return m_move;
}

//=============================================================================
// プレイヤーの位置取得
//=============================================================================
D3DXVECTOR3 CPlayer::GetPlayerPos(void)
{
	return g_pos;
}

//=============================================================================
// 位置を設定
//=============================================================================
void CPlayer::SetPos(D3DXVECTOR3 pos)
{
	// Scene2Dに情報を渡す
	SetPosition(pos, m_size);
}

//=============================================================================
// プレイヤーの移動量を設定
//=============================================================================
void CPlayer::SetMove(D3DXVECTOR3 move)
{
	m_move = move;
}

//=============================================================================
// パーティクル構造体取得
//=============================================================================
CPlayer::PARTICLE CPlayer::GetParticle(void)
{
	return m_aParticle[m_nIndexParticle];
}

//=============================================================================
// パーティクル用取得
// 色情報取得
//=============================================================================
D3DXCOLOR CPlayer::GetCol(void)
{
	return m_aParticle[m_nIndexParticle].col;
}

//=============================================================================
// パーティクル発生数取得
//=============================================================================
int CPlayer::GetCreateNum(void)
{
	return m_aParticle[m_nIndexParticle].nCreateNum;
}

//=============================================================================
// パーティクル速度取得
//=============================================================================
int CPlayer::GetSpeed(void)
{
	return m_aParticle[m_nIndexParticle].nSpeed;
}

//=============================================================================
// パーティクル半径取得
//=============================================================================
float CPlayer::GetRadius(void)
{
	return m_aParticle[m_nIndexParticle].fRadius;
}

//=============================================================================
// パーティクル寿命取得
//=============================================================================
int CPlayer::GetLife(void)
{
	return m_aParticle[m_nIndexParticle].nLife;
}

//=============================================================================
// パーティクル慣性取得
//=============================================================================
float CPlayer::GetInertia(void)
{
	return m_aParticle[m_nIndexParticle].fInertia;
}

//=============================================================================
// パーティクル範囲取得
//=============================================================================
int CPlayer::GetRange(void)
{
	return m_aParticle[m_nIndexParticle].nRange;
}

//=============================================================================
// パーティクル角度取得
//=============================================================================
float CPlayer::GetAngle(void)
{
	return m_aParticle[m_nIndexParticle].fAngle;
}

//=============================================================================
// パーティクル保存番号取得
//=============================================================================
int CPlayer::GetIndexParticle(void)
{
	return m_nIndexParticle;
}


//=============================================================================
// セーブ
//=============================================================================
void CPlayer::SaveData(void)
{
	FILE *pFile;

	// ファイル開く
	pFile = fopen(PARTICLE_FILE_NAME, "w");

	if (pFile != NULL)
	{
		// ファイルが開けたら
		for (int nCnt = 0; nCnt < MAX_PARTICLE; nCnt++)
		{
			fprintf(pFile, "%d\n", m_aParticle[nCnt].nCreateNum);		// 生成数
			fprintf(pFile, "%d\n", m_aParticle[nCnt].nSpeed);			// 速度
			fprintf(pFile, "%.0f\n", m_aParticle[nCnt].fRadius);		// 半径
			fprintf(pFile, "%d\n", m_aParticle[nCnt].nLife);			// 寿命

			fprintf(pFile, "%.2f\n", m_aParticle[nCnt].fInertia);		// 慣性
			fprintf(pFile, "%d\n", m_aParticle[nCnt].nRange);			// 範囲
			fprintf(pFile, "%.1f\n", m_aParticle[nCnt].fAngle);			// 角度

			fprintf(pFile, "%.1f %.1f %.1f\n", m_aParticle[nCnt].col.r, m_aParticle[nCnt].col.g, m_aParticle[nCnt].col.b);
			fprintf(pFile, "\n");
		}
	}

	fclose(pFile);
}

//=============================================================================
// ロード
//=============================================================================
void CPlayer::LoadData(void)
{
	FILE *pFile;

	// ファイル開く
	pFile = fopen(PARTICLE_FILE_NAME, "r");

	if (pFile != NULL)
	{
		// ファイルが開けたら
		for (int nCnt = 0; nCnt < MAX_PARTICLE; nCnt++)
		{
			fscanf(pFile, "%d\n", &m_aParticle[nCnt].nCreateNum);		// 生成数
			fscanf(pFile, "%d\n", &m_aParticle[nCnt].nSpeed);			// 速度
			fscanf(pFile, "%f\n", &m_aParticle[nCnt].fRadius);			// 半径
			fscanf(pFile, "%d\n", &m_aParticle[nCnt].nLife);			// 寿命

			fscanf(pFile, "%f\n", &m_aParticle[nCnt].fInertia);			// 慣性
			fscanf(pFile, "%d\n", &m_aParticle[nCnt].nRange);			// 範囲
			fscanf(pFile, "%f\n", &m_aParticle[nCnt].fAngle);			// 角度

			fscanf(pFile, "%f %f %f\n", &m_aParticle[nCnt].col.r, &m_aParticle[nCnt].col.g, &m_aParticle[nCnt].col.b);	// 色(R,G,B,A)
		}
	}

	fclose(pFile);
}
