//================================================================================
//
// ポーズ処理 [pause.h]
// Author : taiki hayasaka
//
//================================================================================
#ifndef _PAUSE_H_
#define _PAUSE_H_

#include "scene2D.h"

//================================================================================
//マクロ定義
//================================================================================
#define MAX_PAUSE (4)	// ポーズの最大数

//================================================================================
//オブジェクトクラス(背景)
//================================================================================
class CPause :public CScene
{
public:

	typedef enum
	{
		PAUSE_NONE = 0,
		PAUSE_CONTINUE,		// コンティニュー
		PAUSE_RETRY,		// リトライ
		PAUSE_TITLE,		// タイトル
		PAUSE_MAX
	}PAUSE;

	CPause(PRIORITY nPriority = PRIORITY_PAUSE);						// コンストラクタ
	~CPause();															// デストラクタ

	static CPause *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);			// プレイヤーの生成

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);					// 初期化処理
	void Uninit(void);													// 終了処理
	void Update(void);													// 更新処理
	void Draw(void);													// 描画処理

private:
	float m_fMove;														// 移動
	float m_fAnim;														// アニメーション
	int	  m_ColorCnt;													// 色点滅カウント
	int   m_nSelectCnt;													// 選択カウント

	int nTimeGamePause = 0;												//時間
	int nTimeCounterPause = 0;											//時間カウンター
	int g_nPointerPauseX = 0;											//ポインター位置
	bool g_bButtonDownPause = false;									//連打防止

	static LPDIRECT3DTEXTURE9 m_apTexture[PAUSE_MAX];					// テクスチャ
	CScene2D*m_apScene2D[PAUSE_MAX];									// CScene2Dのポインタ
};
#endif