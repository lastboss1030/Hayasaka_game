//=============================================================================
//
// オブジェクト処理 [scene.cpp]
// Author : Taiki Hayasaka
//
//=============================================================================
#include "scene.h"
#include "main.h"
#include "renderer.h"
#include "scene2D.h"

//=============================================================================
// 静的メンバ変数
//=============================================================================
CScene *CScene::m_apScene[PRIORITY_MAX][MAX_POLYGON] = {};		// 画面に表示するオブジェクト数
int CScene::m_nNumAll = 0;										// 最大数

//=============================================================================
// コンストラクタ
//=============================================================================
CScene::CScene(CScene::PRIORITY Priority)
{
	for (int nCntScene = 0; nCntScene < MAX_POLYGON; nCntScene++)
	{
		if (m_apScene[Priority][nCntScene] == NULL)
		{
			m_apScene[Priority][nCntScene] = this;
			m_nID = nCntScene;
			m_nNumAll++;
			m_nPriority = Priority;
			break;
		}
	}

	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nLife = 100;
}

//=============================================================================
// デストラクタ
//=============================================================================
CScene::~CScene()
{

}

//=============================================================================
// 終了処理
//=============================================================================
void CScene::ReleaseAll()
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		for (int nCntScene = 0; nCntScene < MAX_POLYGON; nCntScene++)
		{
			if (m_apScene[nCntPriority][nCntScene] != NULL)
			{
				 // 終了処理
				if (nCntPriority != PRIORITY_FADE)
				{
					// フェード以外破棄
					m_apScene[nCntPriority][nCntScene]->Uninit();
					delete m_apScene[nCntPriority][nCntScene];
					m_apScene[nCntPriority][nCntScene] = NULL;
				}
			}
		}
	}
}

//=============================================================================
// 全更新処理
//=============================================================================
void CScene::UpdateAll()
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		for (int nCntScene = 0; nCntScene < MAX_POLYGON; nCntScene++)
		{
			if (m_apScene[nCntPriority][nCntScene] != NULL)
			{
				 // 更新処理
				m_apScene[nCntPriority][nCntScene]->Update();
			}
		}
	}
}

//=============================================================================
// 全描画
//=============================================================================
void CScene::DrawAll()
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		for (int nCntScene = 0; nCntScene < MAX_POLYGON; nCntScene++)
		{
			if (m_apScene[nCntPriority][nCntScene] != NULL)
			{
				 // 描画処理
				m_apScene[nCntPriority][nCntScene]->Draw();
			}
		}
	}
}

//=============================================================================
// 破棄
//=============================================================================
void CScene::Release(void)
{
	if (m_apScene[m_nPriority][m_nID] != NULL)
	{
		// 格納
		int nID = m_nID;
		int nPriority = m_nPriority;

		//インスタンスの破棄
		delete m_apScene[nPriority][nID];
		m_apScene[nPriority][nID] = NULL;
		m_nNumAll--;
	}
}

//=============================================================================
// オブジェクトの種類の設定
//=============================================================================
void CScene::SetObjType(OBJTYPE objType)
{
	m_objType = objType;
}

//=============================================================================
// 描画順の設定
//=============================================================================
void CScene::SetPriority(CScene::PRIORITY priority)
{
	m_nPriority = priority;
}

//=============================================================================
// オブジェクトの種類の取得
//=============================================================================
CScene::OBJTYPE CScene::GetObjType(void)
{
	return m_objType;
}

//=============================================================================
// オブジェクトの取得
//=============================================================================
CScene *CScene::GetScene(int nPriority, int nCntScene)
{
	return m_apScene[nPriority][nCntScene];
}

//=============================================================================
// 位置を設定
//=============================================================================
void CScene::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//=============================================================================
// サイズを設定
//=============================================================================
void CScene::SetSize(D3DXVECTOR3 size)
{
	m_size = size;
}

//=============================================================================
// 位置を取得
//=============================================================================
D3DXVECTOR3 CScene::GetPos(void)
{
	return m_pos;
}

//=============================================================================
// サイズを取得
//=============================================================================
D3DXVECTOR3 CScene::GetSize(void)
{
	return m_size;
}

//=============================================================================
//　位置管理
//=============================================================================
D3DXVECTOR3 CScene::GetPosition(void)
{
	return D3DXVECTOR3();
}

//=============================================================================
//　ライフ管理
//=============================================================================
void CScene::SetLife(int nLife)
{
	m_nLife = nLife;
}

//=============================================================================
//　Get関数Life
//=============================================================================
int CScene::GetLife(void)
{
	return m_nLife;
}

//=============================================================================
//　ライフを引く関数
//=============================================================================
void CScene::Damage(int nDamage)
{
	m_nLife -= nDamage;
}