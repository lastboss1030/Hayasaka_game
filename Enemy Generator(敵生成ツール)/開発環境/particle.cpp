//=============================================================================
//
// パーティクルの処理 [particle.cpp]
// Author : Taiki Hayasaka
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "particle.h"
#include "manager.h"
#include "renderer.h"
#include "explosion.h"
#include "Input_Keyboard.h"
#include "enemy.h"
#include "scene.h"
#include "scene2D.h"
#include "effect.h"
#include "game.h"
#include "player.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//=============================================================================
// 静的メンバ変数
//=============================================================================
LPDIRECT3DTEXTURE9 CParticle::m_pTexture = NULL;

// パーティクル
CParticle::PARTICLE CParticle::m_aParticle[MAX_PARTICLE] =
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

int CParticle::m_nIndexParticle = 0;			// 保存番号

//=============================================================================
// コンストラクタ
//=============================================================================
CParticle::CParticle(PRIORITY nPriority) :CScene2D(nPriority)
{
	// 初期化
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

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
CParticle::~CParticle()
{

}

//=============================================================================
// 生成処理
//=============================================================================
CParticle *CParticle::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 size, D3DXCOLOR col, int nLife, float fInertia)
{
	// ポインタ
	CParticle *pParticle = NULL;

	if (pParticle == NULL)
	{
		// 動的確保
		pParticle = new CParticle;

		if (pParticle != NULL)
		{
			pParticle->Init(pos, move, size, col, nLife, fInertia);

			pParticle->BindTexture(m_pTexture);
		}
	}

	return pParticle;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CParticle::Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 size, D3DXCOLOR col, int nLife, float fInertia)
{
	// メンバ変数の初期化
	m_move = move;
	m_size = size;
	m_life = m_aParticle[m_nIndexParticle].nLife;
	m_fInertia = fInertia;
	m_col = col;

	// パーティクル用数値初期化
	m_aParticle[m_nIndexParticle].nCreateNum = 20;	// 生成数
	m_aParticle[m_nIndexParticle].nSpeed = 8;		// 速度
	m_aParticle[m_nIndexParticle].fRadius = 20.0f;	// 半径
	m_aParticle[m_nIndexParticle].nLife = 70;		// 寿命
	m_aParticle[m_nIndexParticle].fInertia = 0.95f;	// 慣性
	m_aParticle[m_nIndexParticle].nRange = 628;		// 範囲
	m_aParticle[m_nIndexParticle].fAngle = 1.0;		// 角度
	m_aParticle[m_nIndexParticle].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 色(R,G,B,A)

	// CScene2Dの初期化処理
	CScene2D::Init(pos, size);

	// オブジェクトの種類設定
	SetObjType(CScene::OBJTYPE_PARTICLE);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CParticle::Uninit(void)
{
	// CScene2Dの終了処理
	CScene2D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CParticle::Update(void)
{
	// 変数宣言
	D3DXVECTOR3 Pos;
	Pos = GetPosition();

	// カラー設定
	SetCol(m_aParticle[m_nIndexParticle].col);

	// posにmoveの値を毎秒+する
	Pos += m_move;

	// 慣性
	m_move *= m_fInertia;

	// 座標
	SetPosition(Pos, m_size);

	// エフェクトをだんだん透明にする
	SetEffect(m_col, (float)m_life);

	// ライフ減少
	m_life--;

	// ファイルロード
	LoadData();

	//- - - - - - - - - - - - - - - - - - - - - - - - - - -
	// ライフが0以下になったとき
	//- - - - - - - - - - - - - - - - - - - - - - - - - - -
	if (m_life <= 0)
	{
		Uninit();

		return;
	}

	// パーティクルが画面外に行ったら消す処理
	if (Pos.x - m_size.x / 2.0f < 0.0f || Pos.x + m_size.x / 2.0f > SCREEN_WIDTH ||
		Pos.y - m_size.y / 2.0f < 0.0f || Pos.y + m_size.y / 2.0f > SCREEN_HEIGHT)
	{
		// 弾を消す
		Uninit();

		return;
	}
}

//=============================================================================
// 描画
//=============================================================================
void CParticle::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();

	// 加算合成
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	CScene2D::Draw();

	// 加算合成を外す
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//=============================================================================
// ロード処理
//=============================================================================
HRESULT CParticle::Load(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect000.jpg", &m_pTexture);

	return S_OK;
}

//=============================================================================
// アンロード処理
//=============================================================================
void CParticle::Unload(void)
{
	// テクスチャの破棄
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================================================================
// パーティクル構造体取得
//=============================================================================
CParticle::PARTICLE CParticle::GetParticle(void)
{
	return m_aParticle[m_nIndexParticle];
}

//=============================================================================
// パーティクル用取得
// 色情報取得
//=============================================================================
D3DXCOLOR CParticle::GetCol(void)
{
	return m_aParticle[m_nIndexParticle].col;
}

//=============================================================================
// パーティクル発生数取得
//=============================================================================
int CParticle::GetCreateNum(void)
{
	return m_aParticle[m_nIndexParticle].nCreateNum;
}

//=============================================================================
// パーティクル速度取得
//=============================================================================
int CParticle::GetSpeed(void)
{
	return m_aParticle[m_nIndexParticle].nSpeed;
}

//=============================================================================
// パーティクル半径取得
//=============================================================================
float CParticle::GetRadius(void)
{
	return m_aParticle[m_nIndexParticle].fRadius;
}

//=============================================================================
// パーティクル寿命取得
//=============================================================================
int CParticle::GetLife(void)
{
	return m_aParticle[m_nIndexParticle].nLife;
}

//=============================================================================
// パーティクル慣性取得
//=============================================================================
float CParticle::GetInertia(void)
{
	return m_aParticle[m_nIndexParticle].fInertia;
}

//=============================================================================
// パーティクル範囲取得
//=============================================================================
int CParticle::GetRange(void)
{
	return m_aParticle[m_nIndexParticle].nRange;
}

//=============================================================================
// パーティクル角度取得
//=============================================================================
float CParticle::GetAngle(void)
{
	return m_aParticle[m_nIndexParticle].fAngle;
}

//=============================================================================
// パーティクル保存番号取得
//=============================================================================
int CParticle::GetIndexParticle(void)
{
	return m_nIndexParticle;
}

//=============================================================================
// ファイルロード処理
//=============================================================================
void CParticle::LoadData(void)
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
