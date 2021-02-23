//=============================================================================
//
// 敵の処理 [enemy.cpp]
// Author:Taiki Hayasaka
//
//=============================================================================
#include "enemy.h"
#include "camera.h"
#include "shadow.h"
#include "input.h"
#include "effect.h"
#include "player.h"
#include "bullet.h"
#include "fade.h"
#include "Sound.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MAX_ENEMY	(8)
#define MOVE_ENEMY (1.5f)
#define ENEMY_LIFE (100)

//=============================================================================
// グローバル変数
//=============================================================================
Enemy g_aEnemy[MAX_ENEMY];							// モデル情報
D3DXVECTOR3 g_vtxMinEnemy, g_vtxMaxEnemy;				// 各頂点座標の最小値、最大値
LPDIRECT3DTEXTURE9 g_apTextureEnemy[MAX_ENEMY] = {};	// テクスチャへのポインタ

int g_nCntEffectGetE = 0;
int g_nCntEffectDeathE = 0;							//死亡エフェクト
int nCntShootEnemy = 0;								//発射カウント
int	g_nEnemyCnt = 0;								//敵数
int nCntWait = 0;									//待機時間

//=============================================================================
// 敵の初期化処理
//=============================================================================
HRESULT InitEnemy(void)
{
	//変数宣言
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		//初期化
		g_aEnemy[nCntEnemy].pMeshEnemy = NULL;
		g_aEnemy[nCntEnemy].pBuffMatEnemy = NULL;
		g_aEnemy[nCntEnemy].nNumMatEnemy = NULL;
		g_aEnemy[nCntEnemy].pos = D3DXVECTOR3(50.0f, 10.0f, 0.0f);
		g_aEnemy[nCntEnemy].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].rotDest = g_aEnemy[nCntEnemy].rot;
		g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].moverot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].nLife = ENEMY_LIFE;
		g_aEnemy[nCntEnemy].nIdx = -1;
		g_aEnemy[nCntEnemy].bUse = false;

		//Xファイルの読み込み
		if (FAILED(D3DXLoadMeshFromX("data/MODEL/enemy.x",
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_aEnemy[nCntEnemy].pBuffMatEnemy,
			NULL,
			&g_aEnemy[nCntEnemy].nNumMatEnemy,
			&g_aEnemy[nCntEnemy].pMeshEnemy)))
		{
			return E_FAIL;
		}
	}
	//敵の設置
	SetEnemy(D3DXVECTOR3(0.0f, 0.0f, 600.0f));

	return S_OK;
}

//=============================================================================
// 敵の終了処理
//=============================================================================
void UninitEnemy(void)
{
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		//メッシュの破棄
		if (g_aEnemy[nCntEnemy].pBuffMatEnemy != NULL)
		{
			g_aEnemy[nCntEnemy].pBuffMatEnemy->Release();
			g_aEnemy[nCntEnemy].pBuffMatEnemy = NULL;
		}

		//マテリアルの破棄
		if (g_aEnemy[nCntEnemy].pMeshEnemy != NULL)
		{
			g_aEnemy[nCntEnemy].pMeshEnemy->Release();
			g_aEnemy[nCntEnemy].pMeshEnemy = NULL;
		}
	}
}

//=============================================================================
// 敵の更新処理
//=============================================================================
void UpdateEnemy(void)
{
	Player *pPlayer = GetPlayer();
	Enemy *pEnemy = &g_aEnemy[0];
	BULLET *pBullet = GetBullet();

	// 変数宣言
	int nCntTrue = 0;
	int nFade = GetFade();

	nCntShootEnemy++;
	nCntWait++;

	if (nFade == FADE_NONE && pPlayer->bUse == true)
	{
		for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, nCntEnemy++)
		{
			if (pEnemy->bUse == true)
			{
				MoveEnemy(nCntEnemy);

				//プレイヤーが当たったら消す
				if ((pPlayer->pos.x - pPlayer->minVecPlayer.x) < (pEnemy->pos.x + pEnemy->vtxMaxEnemy.x) &&
					(pPlayer->pos.x + pPlayer->maxVecPlayer.x) > (pEnemy->pos.x - pEnemy->vtxMaxEnemy.x) &&
					(pPlayer->pos.z - pPlayer->minVecPlayer.z) < (pEnemy->pos.z + pEnemy->vtxMaxEnemy.z) &&
					(pPlayer->pos.z + pPlayer->maxVecPlayer.z) > (pEnemy->pos.z - pEnemy->vtxMaxEnemy.z))
				{
					if (pPlayer->Life <= 500)
					{
						//プレイヤー即死
						HitPlayer(100);
					}
				}

				if (pEnemy->nLife <= 0)
				{
					//使った判定
					pEnemy->bUse = false;

					//影を消す
					DeleteShadow(pEnemy->nIdx);

					//効果音
					PlaySound(SOUND_LABEL_SE_DEATH);

					g_nCntEffectDeathE++;

					if (g_nCntEffectDeathE % 1 == 0)
					{
						for (int g_nCntEffect = 0; g_nCntEffect < 100; g_nCntEffect++)
						{
							//角度の設定
							float fAngle = ((float)(rand() % 800)) / 100.0f;
							float fmove = (float)(rand() % 1 + 1);

							//エフェクトの設定
							SetEffect(pEnemy->pos,
								D3DXVECTOR3(sinf(fAngle) * fmove, 5, cosf(fAngle) * fmove),
								D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),
								1.0f,
								5.0f,
								0.01f,
								0.2f);
						}
					}
				}

				//弾発射
				if ((nCntShootEnemy % 50) == 0)
				{
					SetBullet(D3DXVECTOR3(pEnemy->pos.x, pEnemy->pos.y + 75.0f, pEnemy->pos.z),
						D3DXVECTOR3(sinf(pEnemy->rot.y) * 10.0f, 0.0f, cosf(pEnemy->rot.y) * 10.0f),
						30.0f, 30.0f,
						BULLETTYPE_ENEMY);
				}

				//使われているカウント
				nCntTrue++;
			}
		}
	}

	if (pEnemy->nLife <= 0)
	{
		//敵数カウント-
		g_nEnemyCnt--;
	}

	//敵を全部倒したら
	if (g_nEnemyCnt <= 0)
	{
		g_nCntEffectDeathE++;
		if (g_nCntEffectDeathE == 40)
		{
			SetFade(FADE_OUT, MODE_RESULT);	//リザルト画面に切り替え
			g_nEnemyCnt++;
		}
	}
}

//=============================================================================
//敵の描画処理
//=============================================================================
void DrawEnemy(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;		//デバイスへのポインタ
	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス
	D3DMATERIAL9 matDef;			//現在のマテリアル保存用
	D3DXMATERIAL *pMat;				//マテリアルデータへのポインタ
	pDevice = GetDevice();

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aEnemy[nCntEnemy].mtxWorld);

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aEnemy[nCntEnemy].rot.y, g_aEnemy[nCntEnemy].rot.x, g_aEnemy[nCntEnemy].rot.z);
			D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].mtxWorld, &g_aEnemy[nCntEnemy].mtxWorld, &mtxRot);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y, g_aEnemy[nCntEnemy].pos.z);
			D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].mtxWorld, &g_aEnemy[nCntEnemy].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aEnemy[nCntEnemy].mtxWorld);

			//現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			//マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_aEnemy[nCntEnemy].pBuffMatEnemy->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_aEnemy[nCntEnemy].nNumMatEnemy; nCntMat++)
			{
				//マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				//テクスチャの設定
				pDevice->SetTexture(0, g_apTextureEnemy[nCntMat]);

				//モデル(パーツ)の描画
				g_aEnemy[nCntEnemy].pMeshEnemy->DrawSubset(nCntMat);
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
	Enemy *pEnemy = &g_aEnemy[0];

	//敵数カウント+
	g_nEnemyCnt++;

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == false)
		{
			//アイテムの頂点
			VecEnemy(nCntEnemy);

			//座標の設定
			g_aEnemy[nCntEnemy].pos = pos;

			//使っているかどうか
			g_aEnemy[nCntEnemy].bUse = true;

			//影の配置
			g_aEnemy[nCntEnemy].nIdx = SetShadow(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, 0.0f, g_aEnemy[nCntEnemy].pos.z), 20.0f, 20.0f);

			break;
		}
	}
}

//=============================================================================
// 敵の頂点座標
//=============================================================================
void VecEnemy(int nCntEnemy)
{
	//変数宣言
	int nNumVtx;			//頂点数
	DWORD sizeFVF;			//頂点フォーマットのサイズ
	BYTE *pVtxBuff;			//頂点バッファへのポインタ

	//頂点数の取得
	nNumVtx = g_aEnemy[nCntEnemy].pMeshEnemy->GetNumVertices();

	//頂点フォーマットのサイズを取得
	sizeFVF = D3DXGetFVFVertexSize(g_aEnemy[nCntEnemy].pMeshEnemy->GetFVF());

	//頂点バッファのロック
	g_aEnemy[nCntEnemy].pMeshEnemy->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		//頂点座標の代入
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

		//全ての頂点を比較しモデルの最小値、最大値を抜き出す
		if (g_aEnemy[nCntEnemy].vtxMinEnemy.x > vtx.x)
		{
			g_aEnemy[nCntEnemy].vtxMinEnemy.x = vtx.x;
		}
		else if (g_aEnemy[nCntEnemy].vtxMinEnemy.x < vtx.x)
		{
			g_aEnemy[nCntEnemy].vtxMinEnemy.x = vtx.x;
		}

		if (g_aEnemy[nCntEnemy].vtxMinEnemy.y > vtx.y)
		{
			g_aEnemy[nCntEnemy].vtxMinEnemy.y = vtx.y;
		}
		else if (g_aEnemy[nCntEnemy].vtxMinEnemy.y < vtx.y)
		{
			g_aEnemy[nCntEnemy].vtxMinEnemy.y = vtx.y;
		}

		if (g_aEnemy[nCntEnemy].vtxMinEnemy.z > vtx.z)
		{
			g_aEnemy[nCntEnemy].vtxMinEnemy.z = vtx.z;
		}
		else if (g_aEnemy[nCntEnemy].vtxMinEnemy.z < vtx.z)
		{
			g_aEnemy[nCntEnemy].vtxMinEnemy.z = vtx.z;
		}

		pVtxBuff += sizeFVF;
	}
	//頂点バッファのアンロック
	g_aEnemy[0].pMeshEnemy->UnlockVertexBuffer();

	//0.0fより小さいとき-1.0倍する
	if (g_aEnemy[nCntEnemy].vtxMinEnemy.x < 0.0f)
	{
		g_aEnemy[nCntEnemy].vtxMinEnemy.x *= -1.0f;
		{
		}
		if (g_aEnemy[nCntEnemy].vtxMinEnemy.y < 0.0f)
			g_aEnemy[nCntEnemy].vtxMinEnemy.y *= -1.0f;
	}
	if (g_aEnemy[nCntEnemy].vtxMinEnemy.z < 0.0f)
	{
		g_aEnemy[nCntEnemy].vtxMinEnemy.z *= -1.0f;
	}
	if (g_aEnemy[nCntEnemy].vtxMinEnemy.x < 0.0f)
	{
		g_aEnemy[nCntEnemy].vtxMinEnemy.x *= -1.0f;
	}
	if (g_aEnemy[nCntEnemy].vtxMinEnemy.y < 0.0f)
	{
		g_aEnemy[nCntEnemy].vtxMinEnemy.y *= -1.0f;
	}
	if (g_aEnemy[nCntEnemy].vtxMinEnemy.z < 0.0f)
	{
		g_aEnemy[nCntEnemy].vtxMinEnemy.z *= -1.0f;
	}
}

//=============================================================================
// 敵の当たり判定
//=============================================================================
bool HitEnemy(int nDamage)
{
	Enemy *pEnemy = &g_aEnemy[0];

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (pEnemy->nLife <= ENEMY_LIFE)
		{
			//プレイヤーダメージのマイナス
			pEnemy->nLife -= nDamage;
			return true;
		}
	}

	return false;
}

//=============================================================================
// モデルの位置情報の取得
//=============================================================================
Enemy *GetEnemy(void)
{
	return &g_aEnemy[0];
}