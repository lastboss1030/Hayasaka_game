//=============================================================================
//
// オブジェクトクラス[scene.h]
// Author : Taiki hayasaka
//
//=============================================================================
#ifndef _SCENE_H_
#define _SCENE_H_

#include "main.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MAX_POLYGON (2048)	

//=============================================================================
// クラス
//=============================================================================
class CScene
{
public:

	//	描画順の列挙型
	typedef enum
	{
		PRIORITY_DEFAULT = 0,		// デフォルト
		PRIORITY_BGBEHING, 			// 後ろ側の背景
		PRIORITY_BULLET,			// 弾
		PRIORITY_ENEMY,				// 敵
		PRIORITY_BOSS,				// ボス
		PRIORITY_EFFECT,			// エフェクト
		PRIORITY_PLAYER,			// プレイヤー
		PRIORITY_EXPLOSION,			// 爆破
		PRIORITY_BGFRONT,			// 手前側の背景	
		PRIORITY_LIFE,				// ライフ
		PRIORITY_SCORE,				// スコア
		PRIORITY_FADE,				// フェード
		PRIORITY_PAUSE,				// ポーズ
		PRIORITY_MAX,
	} PRIORITY;

	// オブジェクトの種類
	typedef enum
	{
		OBJTYPE_NONE = 0,
		OBJTYPE_PLAYER,				// プレイヤー
		OBJTYPE_ENEMY,				// 敵
		OBJTYPE_BOSS,				// ボス
		OBJTYPE_BOSSBREAK,			// ボス(破壊)
		OBJTYPE_BULET,				// 弾
		OBJTYPE_EFFECT,				// エフェクト
		OBJTYPE_PAUSE,				// ポーズ
		OBJTYPE_MAX,
	}OBJTYPE;

	CScene(PRIORITY nPriority = PRIORITY_DEFAULT);
	virtual ~CScene();

	virtual HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;

	static void ReleaseAll(void);
	static void UpdateAll(void);
	static void DrawAll(void);

	void SetObjType(OBJTYPE objType);								// オブジェクトの設定
	OBJTYPE GetObjType(void);										// オブジェクトの種類
	static CScene *GetScene(int nPriority, int nCntScene);			// オブジェクトの取得

	void SetPos(D3DXVECTOR3 pos);									// 位置を設定
	void SetSize(D3DXVECTOR3 size);									// サイズを設定
	void SetPriority(PRIORITY priority);							// 描画順を設定

	D3DXVECTOR3 GetPos(void);										// posを取得
	D3DXVECTOR3 GetSize(void);										// sizeを取得

	//ライフ管理
	void SetLife(int nLife);
	void Damage(int nDamage);
	int GetLife(void);

	// 位置管理
	virtual D3DXVECTOR3 GetPosition(void);

protected:
	void Release(void);

private:
	static CScene *m_apScene[PRIORITY_MAX][MAX_POLYGON];
	static int m_nNumAll;

	int m_nID;				// 格納先の番号
	int m_nPriority;		// 優先順位の番号
	int	m_nLife;			//ライフ
	D3DXVECTOR3 m_pos;		// 位置
	D3DXVECTOR3 m_size;		// サイズ
	OBJTYPE m_objType;		// オブジェクトの種類
};
#endif
