//=============================================================================
//
// チュートリアル処理 [tutorial.h]
// Author : Taiki Hayasaka
//
//=============================================================================
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

#include "main.h"
#include "scene2D.h"


//=============================================================================
// クラス
//=============================================================================
class CTutorial : public CScene
{
public:
	CTutorial();
	~CTutorial();

	// 生成
	static CTutorial *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);

	// テクスチャ関係
	static HRESULT Load(void);
	static void Unload(void);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;
	CScene2D *m_pScene2D;
};

#endif
