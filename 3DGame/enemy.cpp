//=============================================================================
//
// 敵の処理 [enemy.h]
// Author:Taiki Hayasaka
//
//=============================================================================
#include "enemy.h"
#include "player.h"
#include "shadow.h"
#include "bullet.h"
#include "player.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MAX_ENEMY	(8)			//敵最大数
#define MOVE_ENEMY	(2.0f)		//敵の移動量
#define HIT_WALL	(760.0f)	//壁の設定

//=============================================================================
// グローバル変数
//=============================================================================
Enemy g_aEnemy[MAX_ENEMY];								//モデル情報
LPDIRECT3DTEXTURE9 g_apTextureEnemy[MAX_ENEMY] = {};	//テクスチャへのポインタ
int g_nShootEnemy = 0;									//発射カウント

//=============================================================================
// 敵の初期化処理
//=============================================================================
HRESULT InitEnemy(void)
{
	//変数宣言
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();

	for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++)
	{
		//情報の初期化
		g_aEnemy[nCnt].pMeshModel = NULL;
		g_aEnemy[nCnt].pBuffMatModel = NULL;
		g_aEnemy[nCnt].nNumMatModel = NULL;
		g_aEnemy[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCnt].rotDest = g_aEnemy[nCnt].rot;
		g_aEnemy[nCnt].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCnt].moverot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCnt].nIdx = -1;
		g_aEnemy[nCnt].bUse = false;

		//Xファイルの読み込み
		if (FAILED(D3DXLoadMeshFromX("data/MODEL/enemy.x",
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_aEnemy[nCnt].pBuffMatModel,
			NULL,
			&g_aEnemy[nCnt].nNumMatModel,
			&g_aEnemy[nCnt].pMeshModel)))
		{
			return E_FAIL;
		}

		//敵の頂点座標
		VecEnemy(nCnt);
	}

	//敵の配置
	SetEnemy(D3DXVECTOR3(0.0f, 0.0f, 500.0f));

	return S_OK;
}

//=============================================================================
// 敵の終了処理
//=============================================================================
void UninitEnemy(void)
{
	for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++)
	{
		//メッシュの破棄
		if (g_aEnemy[nCnt].pBuffMatModel != NULL)
		{
			g_aEnemy[nCnt].pBuffMatModel->Release();
			g_aEnemy[nCnt].pBuffMatModel = NULL;
		}

		//マテリアルの破棄
		if (g_aEnemy[nCnt].pMeshModel != NULL)
		{
			g_aEnemy[nCnt].pMeshModel->Release();
			g_aEnemy[nCnt].pMeshModel = NULL;
		}
	}
}

//=============================================================================
// 敵の更新処理
//=============================================================================
void UpdateEnemy(void)
{
	Player *pPlayer = GetPlayer();

	//変数宣言
	int nCntTrue = 0;

	//発射カウントを進める
	g_nShootEnemy++;

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true)
		{
			//敵の移動処理
			MoveEnemy(nCntEnemy);

			if ((g_nShootEnemy % 50) == 0)
			{
				SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y + 75, g_aEnemy[nCntEnemy].pos.z),
					D3DXVECTOR3(sinf(g_aEnemy[nCntEnemy].rot.y) * +8.0f, 0.0f, cosf(g_aEnemy[nCntEnemy].rot.y) * + 8.0f),
					30.0f, 30.0,
					BULLETTYPE_ENEMY);
			}
		}

		//壁の設定
		if (g_aEnemy[nCntEnemy].pos.x < -HIT_WALL)
		{
			//左端
			g_aEnemy[nCntEnemy].pos.x = -HIT_WALL;
		}
		if (g_aEnemy[nCntEnemy].pos.x > HIT_WALL)
		{
			//右端
			g_aEnemy[nCntEnemy].pos.x = HIT_WALL;
		}
		if (g_aEnemy[nCntEnemy].pos.z > HIT_WALL)
		{
			//奥
			g_aEnemy[nCntEnemy].pos.z = HIT_WALL;
		}
		if (g_aEnemy[nCntEnemy].pos.z < -HIT_WALL)
		{
			//手前
			g_aEnemy[nCntEnemy].pos.z = -HIT_WALL;
		}
	}


}

//=============================================================================
// 敵の描画処理
//=============================================================================
void DrawEnemy(void)
{
	// 変数宣言
	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス
	D3DMATERIAL9 matDef;			//現在のマテリアル保存用
	D3DXMATERIAL *pMat;				//マテリアルデータへのポインタ

	LPDIRECT3DDEVICE9 pDevice;		//デバイスへのポインタ
	pDevice = GetDevice();

	for (int nCntCoin = 0; nCntCoin < MAX_ENEMY; nCntCoin++)
	{
		if (g_aEnemy[nCntCoin].bUse == true)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aEnemy[nCntCoin].mtxWorld);

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aEnemy[nCntCoin].rot.y, g_aEnemy[nCntCoin].rot.x, g_aEnemy[nCntCoin].rot.z);
			D3DXMatrixMultiply(&g_aEnemy[nCntCoin].mtxWorld, &g_aEnemy[nCntCoin].mtxWorld, &mtxRot);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aEnemy[nCntCoin].pos.x, g_aEnemy[nCntCoin].pos.y, g_aEnemy[nCntCoin].pos.z);
			D3DXMatrixMultiply(&g_aEnemy[nCntCoin].mtxWorld, &g_aEnemy[nCntCoin].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aEnemy[nCntCoin].mtxWorld);

			//現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			//マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_aEnemy[nCntCoin].pBuffMatModel->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_aEnemy[nCntCoin].nNumMatModel; nCntMat++)
			{
				//マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				//テクスチャの設定
				pDevice->SetTexture(0, g_apTextureEnemy[nCntMat]);

				//モデル(パーツ)の描画
				g_aEnemy[nCntCoin].pMeshModel->DrawSubset(nCntMat);
			}
			//保存していたマテリアルを戻す
			pDevice->SetMaterial(&matDef);
		}
	}
}

//=============================================================================
// 敵の移動処理
//=============================================================================
void MoveEnemy(int nCntEnemy)
{
	//変数宣言
	Player *pPlayer = GetPlayer();
	D3DXVECTOR3 EnemyV;		//プレイヤーに向く

	//目的の回転角を制限
	if (g_aEnemy[nCntEnemy].rotDest.y - g_aEnemy[nCntEnemy].rot.y < -D3DX_PI)
	{
		//3.14fより大きくなったら-3.14fにする
		g_aEnemy[nCntEnemy].rotDest.y += D3DX_PI * 2.0f;
	}
	else if (g_aEnemy[nCntEnemy].rotDest.y - g_aEnemy[nCntEnemy].rot.y > D3DX_PI)
	{
		//3.14fより大きくなったら-3.14fにする
		g_aEnemy[nCntEnemy].rotDest.y -= D3DX_PI * 2.0f;
	}

	//向きの更新
	g_aEnemy[nCntEnemy].rot.y += (g_aEnemy[nCntEnemy].rotDest.y - g_aEnemy[nCntEnemy].rot.y) * 0.05f;

	//現在の回転角を制限
	if (g_aEnemy[nCntEnemy].rot.y < -D3DX_PI)
	{
		//3.14fより大きくなったら-3.14fにする
		g_aEnemy[nCntEnemy].rot.y += D3DX_PI * 2.0f;
	}
	else if (g_aEnemy[nCntEnemy].rot.y > D3DX_PI)
	{
		//3.14fより大きくなったら-3.14fにする
		g_aEnemy[nCntEnemy].rot.y -= D3DX_PI * 2.0f;
	}

	//敵がプレイヤーを見るようにする
	EnemyV = pPlayer->pos - g_aEnemy[nCntEnemy].pos;

	//角度を求める
	float fAngle = (float)atan2(EnemyV.x, EnemyV.z);

	//自機と中心点の対角線の長さ
	float fLength = sqrtf((EnemyV.x * EnemyV.x) + (EnemyV.z * EnemyV.z));

	//プレイヤーの追尾
	g_aEnemy[nCntEnemy].move.x = MOVE_ENEMY * sinf(fAngle);
	g_aEnemy[nCntEnemy].move.z = MOVE_ENEMY * cosf(fAngle);

	//向きの更新
	g_aEnemy[nCntEnemy].rotDest.y = fAngle;

	//位置の更新
	g_aEnemy[nCntEnemy].pos.x += g_aEnemy[nCntEnemy].move.x;
	g_aEnemy[nCntEnemy].pos.z += g_aEnemy[nCntEnemy].move.z;

	//影の移動
	SetPositionShadow(g_aEnemy[nCntEnemy].nIdx, g_aEnemy[nCntEnemy].pos);
}

//=============================================================================
// 敵の設置
//=============================================================================
void SetEnemy(D3DXVECTOR3 pos)
{
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == false)
		{
			//座標の設定
			g_aEnemy[nCntEnemy].pos = pos;

			//テクスチャの設定
			TexEnemy(nCntEnemy);

			//使用状態をtrueにする
			g_aEnemy[nCntEnemy].bUse = true;

			//影の配置
			g_aEnemy[nCntEnemy].nIdx = SetShadow(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, 0.0f, g_aEnemy[nCntEnemy].pos.z), SHADOW_SIZE, SHADOW_SIZE);

			break;
		}
	}
}

//=============================================================================
// 敵のテクスチャ
//=============================================================================
void TexEnemy(int nCntCoin)
{
	//変数宣言
	D3DXMATERIAL *pMat;			//マテリアルデータへのポインタ
	LPDIRECT3DDEVICE9 pDevice;	//デバイスへのポインタ

	//変数の受け渡し
	pDevice = GetDevice();

	//マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)g_aEnemy[nCntCoin].pBuffMatModel->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_aEnemy[nCntCoin].nNumMatModel; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{
			//テクスチャの読み込み
			D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &g_apTextureEnemy[nCntMat]);
		}
	}
}

//=============================================================================
// 敵の頂点座標
//=============================================================================
void VecEnemy(int nCntCoin)
{
	//変数宣言
	int nNumVtx;			//頂点数
	DWORD sizeFVF;			//頂点フォーマットのサイズ
	BYTE *pVtxBuff;			//頂点バッファへのポインタ

	//頂点数の取得
	nNumVtx = g_aEnemy[nCntCoin].pMeshModel->GetNumVertices();

	//頂点フォーマットのサイズを取得
	sizeFVF = D3DXGetFVFVertexSize(g_aEnemy[nCntCoin].pMeshModel->GetFVF());

	//頂点バッファのロック
	g_aEnemy[nCntCoin].pMeshModel->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		//頂点座標の代入
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

		//頂点を比較してモデルの最小値最大値を抜き出す
		if (g_aEnemy[nCntCoin].vtxMinEnemy.x > vtx.x)
		{
			//X座標の最小値
			g_aEnemy[nCntCoin].vtxMinEnemy.x = vtx.x;
		}
		else if (g_aEnemy[nCntCoin].vtxMinEnemy.x < vtx.x)
		{
			//X座標の最大値
			g_aEnemy[nCntCoin].vtxMinEnemy.x = vtx.x;
		}

		if (g_aEnemy[nCntCoin].vtxMinEnemy.y > vtx.y)
		{
			//Y座標の最小値
			g_aEnemy[nCntCoin].vtxMinEnemy.y = vtx.y;
		}
		else if (g_aEnemy[nCntCoin].vtxMinEnemy.y < vtx.y)
		{
			//Y座標の最大値
			g_aEnemy[nCntCoin].vtxMinEnemy.y = vtx.y;
		}

		if (g_aEnemy[nCntCoin].vtxMinEnemy.z > vtx.z)
		{
			//Z座標の最小値
			g_aEnemy[nCntCoin].vtxMinEnemy.z = vtx.z;
		}
		else if (g_aEnemy[nCntCoin].vtxMinEnemy.z < vtx.z)
		{
			//Z座標の最大値
			g_aEnemy[nCntCoin].vtxMinEnemy.z = vtx.z;
		}

		//頂点フォーマットのサイズ文ポインタを進める
		pVtxBuff += sizeFVF;
	}
	//頂点バッファのアンロック
	g_aEnemy[0].pMeshModel->UnlockVertexBuffer();
}

//=============================================================================
// 敵の当たり判定
//=============================================================================
bool CollisionEnemy(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 *pvtxMin, D3DXVECTOR3 *pvtxMax)
{
	//変数宣言
	Enemy *pEnemy = &g_aEnemy[0];
	bool bCollision = false;		//当たったかどうか

	for (int nCntCoin = 0; nCntCoin < MAX_ENEMY; nCntCoin++, pEnemy++)
	{
		if (pEnemy->bUse == true)
		{
			//ベクトル
			pEnemy->aVecB[0] = D3DXVECTOR3(pPos->x + pvtxMax->x - pEnemy->aPos[0].x, 0.0f, pPos->z - pEnemy->aPos[0].z);
			pEnemy->aVecB[1] = D3DXVECTOR3(pPos->x - pEnemy->aPos[1].x, 0.0f, pPos->z + pvtxMin->z - pEnemy->aPos[1].z);
			pEnemy->aVecB[2] = D3DXVECTOR3(pPos->x + pvtxMin->x - pEnemy->aPos[2].x, 0.0f, pPos->z - pEnemy->aPos[2].z);
			pEnemy->aVecB[3] = D3DXVECTOR3(pPos->x - pEnemy->aPos[3].x, 0.0f, pPos->z + pvtxMax->z - pEnemy->aPos[3].z);
		}
	}

	return bCollision;
}

//=============================================================================
// 敵の位置情報の取得
//=============================================================================
Enemy *GetEnemy(void)
{
	return &g_aEnemy[0];
}