//=============================================================================
//
// プレイヤーの処理 [model.cpp]
// Author:Taiki Hayasaka
//
//=============================================================================
#include "player.h"
#include "input.h"
#include "camera.h"
#include "bullet.h"
#include "shadow.h"
#include "explosion.h"
#include "effect.h"
#include "select.h"
#include "enemy.h"
#include "fade.h"
#include "Sound.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MOVE_MODEL		(1.0f)			//モデル移動量
#define MOVE_BULLET		(5.0f)			//弾移動量
#define HIT_WALL		(750.0f)		//壁
#define MAX_TEX			(10)			//テクスチャ
#define MAX_BOOST		(200)			//ブースト
#define PLAYER_LIFE		(200)			//ライフ基準

#define VTX_MINP	(D3DXVECTOR3(10000.0f, 10000.0f, 10000.0f))		// 仮頂点最小値
#define	VTX_MAXP	(D3DXVECTOR3(-10000.0f, -10000.0f, -10000.0f))	// 仮頂点最大値

#define GRAVITY	(0.5f)				//重力
#define HEIGHT_LIMIT	(150.0f)	//高さ上限	

//=============================================================================
// グローバル変数
//=============================================================================
LPD3DXMESH g_pMeshModel = NULL;						//メッシュ(頂点情報)へのポインタ
LPD3DXBUFFER g_pBuffMatModel = NULL;				//マテリアル(材質情報)へのポインタ
DWORD g_nNumMatModel = 0;							//マテリアルの数
D3DXVECTOR3 g_vtxMinPlayer, g_vtxMaxPlayer;			//モデルの最小、最大
LPDIRECT3DTEXTURE9 g_apTextureModel[MAX_TEX] = {};	//テクスチャへのポインタ
Player g_player;									//モデル情報

D3DXVECTOR3 aCollisionPos[2];						//当たり判定ライン

int g_nStateP;										//ステート
int g_nShootCount = 0;								//発射カウント
int g_nEffect = 0;									//エフェクト
int g_life = 0;										//ライフ
float g_move;										//移動速度

int g_nCntDeath;									//死亡時の待機
int g_nCntEffectDeath = 0;							//死亡エフェクト

//=============================================================================
// 初期化処理
//=============================================================================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;	//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//プレイヤーの構造体の初期化
	g_player.pos = D3DXVECTOR3(0.0f, 10.0f, 0.0f);
	g_player.posOld = D3DXVECTOR3(0.0f, 10.0f, 0.0f);
	g_player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_player.rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_player.Life = PLAYER_LIFE;
	g_player.boost = MAX_BOOST;
	g_player.nNumModel = MAX_USE_MODEL;
	g_player.bUse = true;

	//変数初期化
	g_vtxMinPlayer = VTX_MINP;
	g_vtxMaxPlayer = VTX_MAXP;

	aCollisionPos[0] = D3DXVECTOR3(-200, 0, 200);
	aCollisionPos[1] = D3DXVECTOR3(200, 0, 200);

	g_nCntDeath = 0;
	
	//モーションの初期化
	InitMotion();

	if (g_player.playertype == PLAYERTYPE_GOLEM)
	{
		//移動速度
		g_move = 1.4f;

		//Xファイルの読み込み
		D3DXLoadMeshFromX("data/MODEL/golem.x",	//ゴーレム
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_player.aModel[0].pBuffMat,
			NULL,
			&g_player.aModel[0].nNumMat,
			&g_player.aModel[0].pMesh);

		D3DXLoadMeshFromX("data/MODEL/kang_dae.x",	//キャノン
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_player.aModel[1].pBuffMat,
			NULL,
			&g_player.aModel[1].nNumMat,
			&g_player.aModel[1].pMesh);

		//各パーツの階層構造の設定
		g_player.aModel[0].nIdxModelParent = -1;	//親のインデックスを設定
		g_player.aModel[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_player.aModel[0].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		g_player.aModel[1].nIdxModelParent = 0;		//親のインデックスを設定
		g_player.aModel[1].pos = D3DXVECTOR3(0.0f, 75.0f, 5.0f);
		g_player.aModel[1].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		g_player.pos = D3DXVECTOR3(0.0f, 10000.0f, 0.0f);	//位置
		g_player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//向き
	}

	if (g_player.playertype == PLAYERTYPE_LEO)
	{
		//移動速度
		g_move = 1.1f;

		//Xファイルの読み込み
		D3DXLoadMeshFromX("data/MODEL/leo.x",	//レオ
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_player.aModel[0].pBuffMat,
			NULL,
			&g_player.aModel[0].nNumMat,
			&g_player.aModel[0].pMesh);

		D3DXLoadMeshFromX("data/MODEL/gekko.x",	//月光
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_player.aModel[1].pBuffMat,
			NULL,
			&g_player.aModel[1].nNumMat,
			&g_player.aModel[1].pMesh);

		//各パーツの階層構造の設定
		g_player.aModel[0].nIdxModelParent = -1;	//親のインデックスを設定
		g_player.aModel[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_player.aModel[0].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		g_player.aModel[1].nIdxModelParent = 0;		//親のインデックスを設定
		g_player.aModel[1].pos = D3DXVECTOR3(0.0f, 80.0f, 10.0f);
		g_player.aModel[1].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		g_player.pos = D3DXVECTOR3(0.0f, 10000.0f, 0.0f);	//位置
		g_player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//向き
	}	

	if (g_player.playertype == PLAYERTYPE_STALKER)
	{
		//移動速度
		g_move = 1.8f;

		//Xファイルの読み込み
		D3DXLoadMeshFromX("data/MODEL/stalker.x",	//ストーカー
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_player.aModel[0].pBuffMat,
			NULL,
			&g_player.aModel[0].nNumMat,
			&g_player.aModel[0].pMesh);

		D3DXLoadMeshFromX("data/MODEL/punishert000.x",	//パニッシャー
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_player.aModel[1].pBuffMat,
			NULL,
			&g_player.aModel[1].nNumMat,
			&g_player.aModel[1].pMesh);

		//各パーツの階層構造の設定
		g_player.aModel[0].nIdxModelParent = -1;	//親のインデックスを設定
		g_player.aModel[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_player.aModel[0].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		g_player.aModel[1].nIdxModelParent = 0;		//親のインデックスを設定
		g_player.aModel[1].pos = D3DXVECTOR3(0.0f, 68.0f, 10.0f);
		g_player.aModel[1].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		g_player.pos = D3DXVECTOR3(0.0f, 10000.0f, 0.0f);	//位置
		g_player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//向き
	}
	
	//変数宣言
	int nNumVtx;		//頂点数
	DWORD sizeFVF;		//頂点フォーマットのサイズ
	BYTE *pVtxBuff;		//頂点バッファへのポインタ
	D3DXMATERIAL *pMat;	//マテリアルへのポインタ

	for (int nCntModel = 0; nCntModel < g_player.nNumModel; nCntModel++)
	{
		//頂点数を取得
		nNumVtx = g_player.aModel[nCntModel].pMesh->GetNumVertices();

		//頂点フォーマットのサイズを取得
		sizeFVF = D3DXGetFVFVertexSize(g_player.aModel[nCntModel].pMesh->GetFVF());

		//頂点バッファをロック
		g_player.aModel[nCntModel].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		//頂点情報の抜き出し
		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			//頂点座標の代入
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

			//全ての頂点を比較しモデルの最小値、最大値を抜き出す
			// 各座標の最大値の比較
			if (g_player.minVecPlayer.x > vtx.x)
			{
				// X座標の最小値
				g_player.minVecPlayer.x = vtx.x;
			}
			else if (g_player.maxVecPlayer.x < vtx.x)
			{
				// X座標の最大値
				g_player.maxVecPlayer.x = vtx.x;
			}

			if (g_player.minVecPlayer.y > vtx.y)
			{
				// Y座標の最小値
				g_player.minVecPlayer.y = vtx.y;
			}
			else if (g_player.maxVecPlayer.y < vtx.y)
			{
				// Y座標の最大値
				g_player.maxVecPlayer.y = vtx.y;
			}

			if (g_player.minVecPlayer.z > vtx.z)
			{
				// Z座標の最小値
				g_player.minVecPlayer.z = vtx.z;
			}
			else if (g_player.maxVecPlayer.z < vtx.z)
			{
				// Z座標の最大値
				g_player.maxVecPlayer.z = vtx.z;
			}

			//頂点フォーマットのサイズ分ポインタを進める
			pVtxBuff += sizeFVF;
		}

		// 0.0fより小さいとき-1.0倍する
		if (g_player.minVecPlayer.x < 0.0f)
		{
			g_player.minVecPlayer.x *= -1.0f;
		}
		if (g_player.minVecPlayer.y < 0.0f)
		{
			g_player.minVecPlayer.y *= -1.0f;
		}
		if (g_player.minVecPlayer.z < 0.0f)
		{
			g_player.minVecPlayer.z *= -1.0f;
		}

		//マテリアル情報に対するポインタを取得
		pMat = (D3DXMATERIAL*)g_player.aModel[nCntModel].pBuffMat->GetBufferPointer();

		//テクスチャ
		for (int nCntMat = 0; nCntMat < (int)g_player.aModel[nCntModel].nNumMat; nCntMat++)
		{
			if (pMat[nCntMat].pTextureFilename != NULL)
			{
				//テクスチャの読み込み
				D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &g_apTextureModel[nCntMat]);
			}
		}

		//頂点バッファをアンロック
		g_player.aModel[nCntModel].pMesh->UnlockVertexBuffer();
	}
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPlayer(void)
{
	for (int nCntModel = 0; nCntModel < MAX_USE_MODEL; nCntModel++)
	{
		//メッシュの破棄
		if (g_player.aModel[nCntModel].pMesh != NULL)
		{
			g_player.aModel[nCntModel].pMesh->Release();
			g_player.aModel[nCntModel].pMesh = NULL;
		}

		//マテリアルの破棄
		if (g_player.aModel[nCntModel].pBuffMat != NULL)
		{
			g_player.aModel[nCntModel].pBuffMat->Release();
			g_player.aModel[nCntModel].pBuffMat = NULL;
		}
	}

	//テクスチャの開放
	for (int nCntMat = 0; nCntMat < (int)g_nNumMatModel; nCntMat++)
	{
		if (g_apTextureModel[nCntMat] != NULL)
		{
			g_apTextureModel[nCntMat]->Release();
			g_apTextureModel[nCntMat] = NULL;
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePlayer(void)
{
	Camera *pCamera;
	pCamera = GetCamera();

	//発射カウントを進める
	g_nShootCount++;

	g_player.posOld = g_player.pos;

	//位置の更新
	g_player.pos.x += g_player.move.x;
	g_player.pos.y += g_player.move.y;
	g_player.pos.z += g_player.move.z;

	//移動量の減衰
	g_player.move.x += (0.0f - g_player.move.x) * 0.2f;
	g_player.move.y += (0.0f - g_player.move.y) * 0.2f;
	g_player.move.z += (0.0f - g_player.move.z) * 0.2f;

	//重力
	if (g_player.pos.y > 0.0f)
	{
		g_player.move.y -= GRAVITY;
	}

	//地面めり込み戻す
	if (g_player.pos.y < 0.0f)
	{
		g_player.pos.y = 0.0f;
	}

	//ブースト残量の追加
	if (g_player.pos.y == 0.0f)
	{
		if (g_player.boost < MAX_BOOST)
		{
			if ((g_nShootCount % 3) == 0)
			{
				g_player.boost++;
			}
		}
	}


	//高さ上限
	if (g_player.pos.y > HEIGHT_LIMIT)
	{
		g_player.pos.y = HEIGHT_LIMIT;
	}


	// モデルの移動
	if (GetKeyboardPress(DIK_W) == true)
	{
		if (GetKeyboardPress(DIK_A) == true)
		{
			// 左上方向
			g_player.move.x -= cosf(pCamera->rot.y + D3DX_PI / 4) * g_move;
			g_player.move.z += sinf(pCamera->rot.y + D3DX_PI / 4) * g_move;
			g_player.rotDest.y = pCamera->rot.y + (D3DX_PI * 3 / 4);
		}
		else if (GetKeyboardPress(DIK_D) == true)
		{
			// 右上方向
			g_player.move.x += cosf(pCamera->rot.y - D3DX_PI / 4) * g_move;
			g_player.move.z -= sinf(pCamera->rot.y - D3DX_PI / 4) * g_move;
			g_player.rotDest.y = pCamera->rot.y - (D3DX_PI * 3 / 4);
		}
		else
		{
			// 上方向
			g_player.move.x += sinf(pCamera->rot.y) * g_move;
			g_player.move.z += cosf(pCamera->rot.y) * g_move;
			g_player.rotDest.y = pCamera->rot.y + D3DX_PI;
		}
	}
	else if (GetKeyboardPress(DIK_S) == true)
	{
		if (GetKeyboardPress(DIK_A) == true)
		{
			// 左下方向
			g_player.move.x += cosf(pCamera->rot.y + D3DX_PI * 3 / 4) * g_move;
			g_player.move.z -= sinf(pCamera->rot.y + D3DX_PI * 3 / 4) * g_move;
			g_player.rotDest.y = pCamera->rot.y + D3DX_PI / 4;
		}
		else if (GetKeyboardPress(DIK_D) == true)
		{
			// 右下方向
			g_player.move.x -= cosf(pCamera->rot.y - D3DX_PI * 3 / 4) * g_move;
			g_player.move.z += sinf(pCamera->rot.y - D3DX_PI * 3 / 4) * g_move;
			g_player.rotDest.y = pCamera->rot.y + D3DX_PI / -4;
		}
		else
		{
			// 下方向
			g_player.move.x += sinf(pCamera->rot.y - D3DX_PI) * g_move;
			g_player.move.z += cosf(pCamera->rot.y - D3DX_PI) * g_move;
			g_player.rotDest.y = pCamera->rot.y;
		}
	}
	else if (GetKeyboardPress(DIK_A) == true)
	{
		// 左方向
		g_player.move.x -= cosf(pCamera->rot.y) * g_move;
		g_player.move.z += sinf(pCamera->rot.y) * g_move;
		g_player.rotDest.y = pCamera->rot.y + (D3DX_PI / 2);
	}
	else if (GetKeyboardPress(DIK_D) == true)
	{
		// 右方向
		g_player.move.x += cosf(pCamera->rot.y) * g_move;
		g_player.move.z -= sinf(pCamera->rot.y) * g_move;
		g_player.rotDest.y = pCamera->rot.y + (D3DX_PI / -2);
	}

	// 目的の回転角の上限
	if (g_player.rotDest.y - g_player.rot.y < -D3DX_PI)
	{
		g_player.rotDest.y += D3DX_PI * 2.0f;
	}
	else if (g_player.rotDest.y - g_player.rot.y > D3DX_PI)
	{
		g_player.rotDest.y -= D3DX_PI * 2.0f;
	}

	// 向きの更新
	g_player.rot.y += (g_player.rotDest.y - g_player.rot.y) * 0.1f;

	// 現在の回転角の上限
	if (g_player.rot.y < -D3DX_PI)
	{
		g_player.rot.y += D3DX_PI * 2.0f;
	}
	else if (g_player.rot.y > D3DX_PI)
	{
		g_player.rot.y -= D3DX_PI * 2.0f;
	}

	//弾の発射
	if (GetKeyboardPress(DIK_SPACE) == true)
	{
		//ゴーレム選択時
		if (g_player.playertype == PLAYERTYPE_GOLEM)
		{
			if ((g_nShootCount % 30) == 0)
			{
				if (g_player.boost > 40)
				{
					SetBullet(D3DXVECTOR3(g_player.pos.x, g_player.pos.y + 85.0f, g_player.pos.z),
						D3DXVECTOR3(sinf(g_player.rot.y) * -15.0f, 0.0f, cosf(g_player.rot.y) * -15.0f),
						30.0f, 30.0f,
						BULLETTYPE_PLAYER);

					//効果音
					PlaySound(SOUND_LABEL_SE_SHOOT_GOLEM);

					g_player.boost -= 25;
				}
			}
		}

		//レオ選択時
		if (g_player.playertype == PLAYERTYPE_LEO)
		{
			if ((g_nShootCount % 60) == 0)
			{
				if (g_player.boost > 20)
				{
					SetBullet(D3DXVECTOR3(g_player.pos.x, g_player.pos.y + 90.0f, g_player.pos.z),
						D3DXVECTOR3(sinf(g_player.rot.y) * -12.0f, 0.0f, cosf(g_player.rot.y) * -12.0f),
						30.0f, 30.0f,
						BULLETTYPE_PLAYER);

					//効果音
					PlaySound(SOUND_LABEL_SE_SHOOT_LEO);

					g_player.boost -= 40;
				}
			}
		}

		//ストーカー選択時
		if (g_player.playertype == PLAYERTYPE_STALKER)
		{
			if ((g_nShootCount % 10) == 0)
			{
				if (g_player.boost > 20)
				{
					SetBullet(D3DXVECTOR3(g_player.pos.x, g_player.pos.y + 85.0f, g_player.pos.z),
						D3DXVECTOR3(sinf(g_player.rot.y) * -20.0f, 0.0f, cosf(g_player.rot.y) * -20.0f),
						30.0f, 30.0f,
						BULLETTYPE_PLAYER);

					//効果音
					PlaySound(SOUND_LABEL_SE_SHOOT_STALKER);

					g_player.boost -= 10;
				}
			}
		}

	}

	//影の追従
	SetPositionShadow(g_player.nShadow, D3DXVECTOR3(g_player.pos.x, 0.0f, g_player.pos.z));

	//壁の設定
	if (g_player.pos.x < -HIT_WALL)
	{
		//左端
		g_player.pos.x = -HIT_WALL;
	}
	if (g_player.pos.x > HIT_WALL)
	{
		//右端
		g_player.pos.x = HIT_WALL;
	}
	if (g_player.pos.z > HIT_WALL)
	{
		//奥
		g_player.pos.z = HIT_WALL;
	}
	if (g_player.pos.z < -HIT_WALL)
	{
		//手前
		g_player.pos.z = -HIT_WALL;
	}

	//モーション
	MotionPlayer();

	//エフェクト
	EffectPlayer();

	if (g_player.Life <= 0)
	{
		//使った判定
		g_player.bUse = false;

		//影を消す
		DeleteShadow(g_player.nShadow);

		g_nCntEffectDeath++;

		if (g_nCntEffectDeath % 1 == 0)
		{
			for (int g_nCntEffect = 0; g_nCntEffect < 1; g_nCntEffect++)
			{
				//角度の設定
				float fAngle = ((float)(rand() % 800)) / 100.0f;
				float fmove = (float)(rand() % 1 + 1);

				//エフェクトの設定
				SetEffect(g_player.pos,
					D3DXVECTOR3(sinf(fAngle) * fmove, 5, cosf(fAngle) * fmove),
					D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),
					1.0f,
					5.0f,
					0.01f,
					0.1f);
			}
		}

		g_nCntDeath++;
		if (g_nCntDeath == 20)
		{
			SetFade(FADE_OUT, MODE_RESULT);	//リザルト画面に切り替え
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;	//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス
	D3DMATERIAL9 matDef;			//現在のマテリアル保存用
	D3DXMATERIAL *pMat;				//マテリアルデータへのポインタ

	if (g_player.bUse == true)
	{
		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_player.mtxWorld);

		//向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_player.rot.y, g_player.rot.x, g_player.rot.z);
		D3DXMatrixMultiply(&g_player.mtxWorld, &g_player.mtxWorld, &mtxRot);

		//位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_player.pos.x, g_player.pos.y, g_player.pos.z);
		D3DXMatrixMultiply(&g_player.mtxWorld, &g_player.mtxWorld, &mtxTrans);

		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_player.mtxWorld);

		//現在のマテリアルを取得
		pDevice->GetMaterial(&matDef);

		for (int nCntModel = 0; nCntModel < g_player.nNumModel; nCntModel++)
		{
			D3DXMATRIX mtxRotModel, mtxTransModel;	//計算用マトリックス
			D3DXMATRIX mtxParent;					//親のマトリックス

			//マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_player.aModel[nCntModel].pBuffMat->GetBufferPointer();

			//各パーツのワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_player.aModel[nCntModel].mtxWorld);

			//各パーツの向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRotModel, g_player.aModel[nCntModel].rot.y, g_player.aModel[nCntModel].rot.x, g_player.aModel[nCntModel].rot.z);
			D3DXMatrixMultiply(&g_player.aModel[nCntModel].mtxWorld, &g_player.aModel[nCntModel].mtxWorld, &mtxRotModel);

			//各パーツの位置を反映
			D3DXMatrixTranslation(&mtxTransModel, g_player.aModel[nCntModel].pos.x, g_player.aModel[nCntModel].pos.y, g_player.aModel[nCntModel].pos.z);
			D3DXMatrixMultiply(&g_player.aModel[nCntModel].mtxWorld, &g_player.aModel[nCntModel].mtxWorld, &mtxTransModel);

			//各パーツの親のマトリックスを設定
			if (g_player.aModel[nCntModel].nIdxModelParent != -1)
			{
				mtxParent = g_player.aModel[g_player.aModel[nCntModel].nIdxModelParent].mtxWorld;
			}
			else
			{
				mtxParent = g_player.mtxWorld;	//プレイヤーのマトリックスを設定
			}

			//算出した各パーツのワールドマトリックスと親のマトリックスを掛け合わせる
			D3DXMatrixMultiply(&g_player.aModel[nCntModel].mtxWorld,
				&g_player.aModel[nCntModel].mtxWorld,
				&mtxParent);

			//各パーツのワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_player.aModel[nCntModel].mtxWorld);

			for (int nCntMat = 0; nCntMat < (int)g_player.aModel[nCntModel].nNumMat; nCntMat++)
			{
				//マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				//テクスチャの設定
				pDevice->SetTexture(0, g_apTextureModel[nCntMat]);

				//モデルの描画
				g_player.aModel[nCntModel].pMesh->DrawSubset(nCntMat);
			}
		}

		//保存していたマテリアルを戻す
		pDevice->SetMaterial(&matDef);
	}
}

//=============================================================================
// モーションの初期化
//=============================================================================
void InitMotion(void)
{
	//g_player.aMotionInfo[0].bLoop = false;
	//g_player.aMotionInfo[0].nNumKey = 6;
	//g_player.aMotionInfo[0].aKeyInfo[0].nFrame = 8;			
	//														//pos↓			//rot↓
	//g_player.aMotionInfo[0].aKeyInfo[0].aKey[0] = KeyPosRot(0.0, 0.0, 0.0,   0.0, 0.0, 0.0);
	//g_player.aMotionInfo[0].aKeyInfo[0].aKey[1] = KeyPosRot(0.0, 0.0, 0.0,   0.0, 0.0, 0.0);
	//g_player.aMotionInfo[0].aKeyInfo[0].aKey[2] = KeyPosRot(0.0, 0.0, 0.0,   0.0, -0.25f, 0.79f);
	//g_player.aMotionInfo[0].aKeyInfo[0].aKey[3] = KeyPosRot(0.0, 0.0, 0.0,   0.0, -0.53f, 0.0);
	//g_player.aMotionInfo[0].aKeyInfo[0].aKey[4] = KeyPosRot(0.0, 0.0, 0.0,   0.0, 0.25f, -0.79f);
	//g_player.aMotionInfo[0].aKeyInfo[0].aKey[5] = KeyPosRot(0.0, 0.0, 0.0,   0.0, 0.53f, 0.0);
	//g_player.aMotionInfo[0].aKeyInfo[0].aKey[6] = KeyPosRot(0.0, 0.0, 0.0,   0.06f, 0.0, 0.0);
	//g_player.aMotionInfo[0].aKeyInfo[0].aKey[7] = KeyPosRot(0.0, 0.0, 0.0,   -0.18f, 0.0, 0.0);
	//g_player.aMotionInfo[0].aKeyInfo[0].aKey[8] = KeyPosRot(0.0, 0.0, 0.0,   0.06f, 0.0, 0.0);
	//g_player.aMotionInfo[0].aKeyInfo[0].aKey[9] = KeyPosRot(0.0, 0.0, 0.0,   -0.18f, 0.0, 0.0);
}

//=============================================================================
// モーション
//=============================================================================
void MotionPlayer(void)
{
	if (GetKeyboardPress(DIK_RETURN) == true)
	{
		for (int nCntMotion = 0; nCntMotion < MAX_PARTS; nCntMotion++)
		{
		
		}
	}
}

//=============================================================================
// キー情報
//=============================================================================
KEY KeyPosRot(float posX, float posY, float posZ, float rotX, float rotY, float rotZ)
{
	KEY key;

	key.fPosX = posX;
	key.fPosY = posY;
	key.fPosZ = posZ;

	key.fRotX = rotX;
	key.fRotY = rotY;
	key.fRotZ = rotZ;

	return key;
}

//=============================================================================
// エフェクト
//=============================================================================
void EffectPlayer(void)
{
	g_nEffect++;

	if (g_nEffect % 1 == 0)
	{
		for (int g_nEffect = 0; g_nEffect < 1; g_nEffect++)
		{
			// 角度の設定
			float fAngle = ((float)(rand() % 800)) / 100.0f;
			float fmove = (float)(rand() % 1 + 1);

			SetEffect(D3DXVECTOR3(g_player.pos.x, g_player.pos.y + 40.0f, g_player.pos.z),
				D3DXVECTOR3(sinf(fAngle) * fmove, 0, cosf(fAngle) * fmove / 2),
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
				1.0f,
				10.0f,
				0.2f,
				0.25f);
		}
	}
}

//=============================================================================
// プレイヤーの当たり判定
//=============================================================================
bool HitPlayer(int nDamage)
{
	if (g_player.Life <= PLAYER_LIFE)
	{
		//プレイヤーダメージのマイナス
		g_player.Life -= nDamage;
		return true;
	}
	return false;
}

//=============================================================================
// モデルの位置情報の取得
//=============================================================================
Player *GetPlayer(void)
{
	return &g_player;
}