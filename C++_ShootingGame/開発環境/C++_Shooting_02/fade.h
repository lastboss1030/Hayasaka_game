//=============================================================================
//
//	フェード処理 [fade.h]
//	Author: Taiki Hayasaka
//
//=============================================================================
#ifndef _FADE_H_
#define	_FADE_H_

#include "main.h"
#include "scene2D.h"
#include "manager.h"

//=============================================================================
// 敵のクラス(派生クラス)
//=============================================================================
class CFade : public CScene2D
{
public:

	typedef enum
	{
		FADE_NONE = 0,		//何もしていない
		FADE_IN,			//フェードイン
		FADE_OUT,			//フェードアウト
		FADE_MAX,
	}FADE;

	CFade(PRIORITY nPriority = PRIORITY_FADE);
	~CFade();

	// 生成
	static CFade *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, CManager::MODE modeNext);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetFade(FADE fade, CManager::MODE modeNext);
	FADE GetFade(void);

private:
	FADE m_fade;					//フェードの状態
	CManager::MODE m_modeNext;		//次のモード(画面)
	D3DXCOLOR m_colorFade;			//フェード色
};

#endif
