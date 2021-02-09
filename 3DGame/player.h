//=============================================================================
//
// プレイヤーの処理 [model.h]
// Author:Taiki Hayasaka
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MAX_USE_MODEL (2)	//モデルの最大数
#define MAX_PARTS (2)		//パーツ最大数
#define MAX_KEY (20)		//キーの最大数
#define MAX_MOTION (5)		//モーション最大数

//=============================================================================
// モーションの種類
//=============================================================================
typedef enum
{
	MOTIONTYPE_NEUTRAL = 0,	//ニュートラル
	MOTIONTYPE_ATTACK,		//攻撃
	MOTIONTYPE_MAX
}MOTIONTYPE;

//=============================================================================
// キーの構造体
//=============================================================================
typedef struct
{
	float fPosX;	//位置X
	float fPosY;	//位置Y
	float fPosZ;	//位置Z

	float fRotX;	//向きX
	float fRotY;	//向きY
	float fRotZ;	//向きZ
}KEY;

//=============================================================================
// キー情報の構造体
//=============================================================================
typedef struct
{
	int nFrame;				//再生フレーム
	KEY aKey[MAX_PARTS];	//各モデルのキー要素
}KEY_INFO;

//=============================================================================
// モーション情報の構造体
//=============================================================================
typedef struct
{
	bool bLoop;					//ループするかどうか
	int nNumKey;				//キーの総数
	KEY_INFO aKeyInfo[MAX_KEY];	//キー情報
}MOTION_INFO;

//=============================================================================
// モデルの構造体
//=============================================================================
typedef struct
{
	LPD3DXMESH pMesh;		//メッシュへのポインタ
	LPD3DXBUFFER pBuffMat;	//マテリアルへのポインタ
	DWORD nNumMat;			//マテリアルの数
	D3DXVECTOR3 pos;		//位置(オフセット)
	D3DXVECTOR3 rot;		//向き
	D3DXMATRIX mtxWorld;	//ワールドマトリックス
	int nIdxModelParent;	//親モデルのインデックス
}Model;

//=============================================================================
// プレイヤーの構造体
//=============================================================================
typedef struct
{
	D3DXVECTOR3 pos;		//中心座標
	D3DXVECTOR3 rot;		//回転
	D3DXVECTOR3 rotDest;	//目的の向き
	D3DXVECTOR3 move;		//移動量
	D3DXMATRIX mtxWorld;	//ワールドマトリックス
	float boost;			//ジャンプブースト
	bool bUse;				//使用しているか
	int nShadow;			//影

	Model aModel[MAX_USE_MODEL];	//モデル(パーツ)
	int nNumModel;					//モデル(パーツ)数

	MOTION_INFO aMotionInfo[MAX_MOTION];	//モーション情報
	int nNumMotion;							//モーション数
	MOTIONTYPE motionType;					//モーションタイプ
	bool bLoopMotion;						//ループするかどうか
	int nNumKey;							//キー数
	int nKey;								//キーNo.
	int nCounterMotion;						//モーションのカウンター

	D3DXVECTOR3 minVecPlayer;		// 頂点の最小値
	D3DXVECTOR3 maxVecPlayer;		// 頂点の最大値
}Player;

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void); 

void InitMotion(void);
void MotionPlayer(void);
void EffectPlayer(void);
void CollisionPlayer(void);

KEY KeyPosRot(float posX, float posY, float posZ,
				float rotX, float rotY, float rotZ);
Player *GetPlayer(void);

#endif
