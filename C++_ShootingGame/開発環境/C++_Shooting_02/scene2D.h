//=============================================================================
//
// オブジェクトクラス[scene2D.h]
// Author : Taiki hayasaka
//
//=============================================================================
#ifndef _SCENE2D_H_
#define _SCENE2D_H_

#include "scene.h"
#include "main.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define SCREEN_CENTER_X	(640.0f)	// 幅
#define SCREEN_CENTER_Y	(360.0f)	// 高さ

//=============================================================================
// 構造体定義
//=============================================================================
typedef struct
{
	D3DXVECTOR3 pos;	//頂点座標
	float rhw;			//1.0で固定
	D3DCOLOR col;		//頂点カラー
	D3DXVECTOR2 tex;	//テクスチャ座標
}VERTEX_2D;

//=============================================================================
// クラス
//=============================================================================
class CScene2D : public CScene
{
public:
	CScene2D(PRIORITY nPriority = PRIORITY_DEFAULT);
	~CScene2D();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CScene2D *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);		// オブジェクトの生成

	void SetPosition(D3DXVECTOR3 pos, D3DXVECTOR3 size);
	D3DXVECTOR3 GetPosition(void);									// プレイヤーの位置を取得

	void SetTex(int m_nPatternAnim, int PatterMax);					// テクスチャ座標更新(アニメーション)
	void SetTexBg(float TexU, float TexV);							// テクスチャ座標更新(スクロール)
	void SetCol(D3DXCOLOR col);										// カラーの更新
	void CScene2D::SetEffect(D3DXCOLOR col, float nData);

	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);					// テクスチャ割り当て
	bool Collision(D3DXVECTOR3 pos, D3DXVECTOR3 fsize, CScene::OBJTYPE Type, CScene **pScene);
	D3DXVECTOR3 GetMove(void) { return m_move; }					//移動の取得

private:
	LPDIRECT3DTEXTURE9		m_pTexture = NULL;						// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff = NULL;						// 頂点バッファへのポインタ
	D3DXVECTOR3				m_posPolygon;							// ポリゴンの位置(中心座標)
	D3DXVECTOR3				m_size;									// 大きさ
	D3DXVECTOR3				m_move;									// 移動

	int m_nPatternAnim;												// アニメーションのパターンNo
	float m_fsizeX, m_fsizeY;										// サイズ

};

#endif
