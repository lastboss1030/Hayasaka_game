//=============================================================================
//
// ランキングの処理 [ranking.cpp]
// Author:Taiki Hayasaka
//
//=============================================================================
#include "ranking.h"
#include "score.h"
#include "input.h"
#include "fade.h"
#include "Sound.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MAX_PATTERN				(12)				//背景の種類
#define MAX_TEXTURE				(47)				//テクスチャの最大数
#define TEXTURE_X				(10)				//アニメーションパターンNo.の横の最大数
#define TEXTURE_Y				(1)					//アニメーションパターンNo.の縦の最大数
#define MAX_RANKING_RANK		(5)					//順位の数

#define RANKING_LOGO_X			(SCREEN_WIDTH / 2)	//ランキングロゴの頂点座標(X)
#define RANKING_LOGO_Y			(120.0f)				//ランキングロゴの頂点座標(Y)
#define RANKING_LOGO_WIDTH		(600.0f)			//ランキングロゴの幅
#define RANKING_LOGO_HEIGHT		(90.0f)				//ランキングロゴの高さ

#define RANKING_RANK_X			(500.0f)			//ランキングランクの頂点座標(X)
#define RANKING_RANK_Y			(400.0f)			//ランキングランクの頂点座標(Y)
#define RANKING_RANK_WIDTH		(140.0f)			//ランキングランクの幅
#define RANKING_RANK_HEIGHT		(80.0f)				//ランキングランクの高さ
#define RANKING_RANK_SPACE		(100.0f)			//ランキングランクを配置する間隔

#define RANKING_SCORE_X			(750.0f)			//ランキングスコアの頂点座標(X)
#define RANKING_SCORE_Y			(400.0f)			//ランキングスコアの頂点座標(Y)
#define RANKING_SCORE_WIDTH		(60.0f)				//ランキングスコアの幅
#define RANKING_SCORE_HEIGHT	(100.0f)			//ランキングスコアの高さ
#define RANKING_SCORE_SPACE		(70.0f)				//ランキングスコアを配置する間隔

#define FLASH_SCORE_INTERVAL	(20)				//点滅のインターバル

#define MAX_VERTEX				(4)					//頂点数

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRanking = NULL;					//ランキングの頂点バッファのポインタ
LPDIRECT3DTEXTURE9 g_pTextureRanking[MAX_PATTERN] = {};				//ランキングのテクスチャのポインタ
D3DXVECTOR3 g_posRankingLogo;										//ランキングロゴの頂点座標
D3DXVECTOR3 g_posRankingRank;										//ランキングランクの頂点座標
D3DXVECTOR3 g_posRankingScore;										//ランキングスコアの頂点座標
D3DXCOLOR g_ScoreCol[MAX_RANKING_RANK];								//スコアの色
int g_aScore[MAX_RANKING_RANK] = { 4000,3000,2000,1000,500 };		//初期スコア
int g_nCntScore;													//表示するスコアの数
int g_nSubScore;													//スコアの保存先
bool g_bFlash;														//点滅
int g_nFlashScore;													//点滅させるスコア
int g_nFlashScoreTimer;												//点滅タイマー

//=============================================================================
// ランキングの初期化処理
//=============================================================================
HRESULT InitRanking(void)
{
	//変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	//変数の受け渡し
	pDevice = GetDevice();

	//変数の初期化
	g_posRankingLogo = D3DXVECTOR3(RANKING_LOGO_X, RANKING_LOGO_Y, 0.0f);
	g_posRankingRank = D3DXVECTOR3(RANKING_RANK_X, RANKING_RANK_Y, 0.0f);
	g_posRankingScore = D3DXVECTOR3(RANKING_SCORE_X, RANKING_SCORE_Y, 0.0f);
	g_nCntScore = 0;
	g_nSubScore = 0;
	g_bFlash = false;
	g_nFlashScore = 0;
	g_nFlashScoreTimer = MAX_RANKING_RANK;

	//スコアの色の初期化
	for (int nCnt = 0; nCnt < MAX_RANKING_RANK; nCnt++)
	{
		g_ScoreCol[nCnt] = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
	}

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ranking_bg.png", &g_pTextureRanking[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ranking_logo.png", &g_pTextureRanking[1]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ranking_1st.png", &g_pTextureRanking[2]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ranking_2nd.png", &g_pTextureRanking[3]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ranking_3rd.png", &g_pTextureRanking[4]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ranking_4th.png", &g_pTextureRanking[5]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ranking_5th.png", &g_pTextureRanking[6]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/number001.png", &g_pTextureRanking[7]);

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX * MAX_TEXTURE, // 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,					// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffRanking,
		NULL)))
	{
		return E_FAIL;
	}

	//変数宣言
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffRanking->Lock(0, 0, (void**)&pVtx, 0);

	//初期化
	for (int nCntRanking = 0; nCntRanking < MAX_PATTERN; nCntRanking++)
	{
		//頂点座標の設定
		SetVertexRanking(nCntRanking);

		//4つ進める
		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffRanking->Unlock();
	
	//ランキングの入れ替え
	Ranking();

	//BGM
	PlaySound(SOUND_LABEL_BGM_RANKING);

	return S_OK;
}

//=============================================================================
// ランキングの終了処理
//=============================================================================
void UninitRanking(void)
{
	//サウンド停止
	StopSound();

	//頂点バッファの開放
	if (g_pVtxBuffRanking != NULL)
	{
		g_pVtxBuffRanking->Release();
		g_pVtxBuffRanking = NULL;
	}

	//テクスチャの開放
	for (int nCntRanking = 0; nCntRanking < MAX_PATTERN; nCntRanking++)
	{
		if (g_pTextureRanking[nCntRanking] != NULL)
		{
			g_pTextureRanking[nCntRanking]->Release();
			g_pTextureRanking[nCntRanking] = NULL;
		}
	}
}

//=============================================================================
// ランキングの更新処理
//=============================================================================
void UpdateRanking(void)
{
	//変数の初期化
	g_nCntScore = 0;
	int nFade = GetFade();

	//テクスチャ更新
	for (int nCntScore = 7; nCntScore < MAX_PATTERN; nCntScore++)
	{
		//頂点座標の更新
		SetVertexRanking(nCntScore);
	}

	//画面遷移
	if (GetKeyboardTrigger(DIK_RETURN) == true)
	{
		if ((nFade == FADE_NONE))
		{
			//効果音
			PlaySound(SOUND_LABEL_SE_ENTER);

			SetFade(FADE_OUT, MODE_TITLE);
		}
	}
}

//=============================================================================
// ランキングの描画処理
//=============================================================================
void DrawRanking(void)
{
	//変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	//変数の受け渡し
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffRanking, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntRanking = 0; nCntRanking < MAX_TEXTURE; nCntRanking++)
	{
		//ポリゴンの描画
		if (nCntRanking < 7)
		{
			pDevice->SetTexture(0, g_pTextureRanking[nCntRanking]);
		}
		else if (nCntRanking >= 7)
		{
			pDevice->SetTexture(0, g_pTextureRanking[7]);
		}
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntRanking * 4, 2);
	}
}

//=============================================================================
// ランキングの入れ替え
//=============================================================================
void Ranking(void)
{
	//変数宣言
	int nScore = GetScore();
	int nSubScore = 0;

	if (g_aScore[4] <= nScore)
	{
		//5位のスコアより大きいとき
		g_aScore[4] = nScore;
	}

	//ランキングの更新
	for (int nCnt = 0; nCnt < MAX_RANKING_RANK - 1; nCnt++)
	{
		for (int nCnt2 = nCnt + 1; nCnt2 < MAX_RANKING_RANK; nCnt2++)
		{
			if (g_aScore[nCnt] <= g_aScore[nCnt2])
			{
				//ランキングスコア更新したとき
				nSubScore = g_aScore[nCnt];
				g_aScore[nCnt] = g_aScore[nCnt2];
				g_aScore[nCnt2] = nSubScore;
			}
		}
	}
	//どこに入ったか
	for (int nCnt = 0; nCnt < MAX_RANKING_RANK; nCnt++)
	{
		if (g_aScore[nCnt] == nScore)
		{
			g_bFlash = true;
			g_nFlashScore = nCnt;
			break;
		}
	}	
}
//=============================================================================
// ランキングの頂点座標の設定
//=============================================================================
void SetVertexRanking(int nCnt)
{
	//頂点座標の設定
	if (nCnt == 0)
	{
		//背景配置
		RankingBg(nCnt);
	}
	else if (nCnt == 1)
	{
		//ロゴ配置
		RankingLogo(nCnt);
	}
	else if (nCnt >= 2 && nCnt < 7)
	{
		//順位配置
		RankingRank(nCnt);
	}
	else if (nCnt >= 7 && nCnt < MAX_PATTERN)
	{
		//点滅
		if (g_bFlash == true)
		{
			if (g_nFlashScoreTimer == FLASH_SCORE_INTERVAL / 4)
			{
				//色の確定
				g_ScoreCol[g_nFlashScore] = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
			}
			else if (g_nFlashScoreTimer == FLASH_SCORE_INTERVAL)
			{
				//色の確定
				g_ScoreCol[g_nFlashScore] = D3DXCOLOR(1.0f, 0.1f, 0.1f, 1.0f);

				//タイマーの初期化
				g_nFlashScoreTimer = 0;
			}
			g_nFlashScoreTimer++;
		}
		//スコアの更新
		RankingScore(nCnt);
	}
}

//=============================================================================
// ランキングの背景
//=============================================================================
void RankingBg(int nCnt)
{
	//変数宣言
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffRanking->Lock(0, 0, (void**)&pVtx, 0);

	//何番目か
	pVtx += nCnt * 4;

	pVtx[0].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);

	//テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffRanking->Unlock();
}

//=============================================================================
// ランキングのロゴ
//=============================================================================
void RankingLogo(int nCnt)
{
	//変数宣言
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffRanking->Lock(0, 0, (void**)&pVtx, 0);

	//何番目か
	pVtx += nCnt * 4;


	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_posRankingLogo.x - RANKING_LOGO_WIDTH / 2, g_posRankingLogo.y + RANKING_LOGO_HEIGHT / 2, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_posRankingLogo.x - RANKING_LOGO_WIDTH / 2, g_posRankingLogo.y - RANKING_LOGO_HEIGHT / 2, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_posRankingLogo.x + RANKING_LOGO_WIDTH / 2, g_posRankingLogo.y + RANKING_LOGO_HEIGHT / 2, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_posRankingLogo.x + RANKING_LOGO_WIDTH / 2, g_posRankingLogo.y - RANKING_LOGO_HEIGHT / 2, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);

	//テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffRanking->Unlock();
}

//=============================================================================
// ランキングの順位
//=============================================================================
void RankingRank(int nCnt)
{
	//変数宣言
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffRanking->Lock(0, 0, (void**)&pVtx, 0);

	//何番目か
	pVtx += nCnt * 4;

	//順位を配置する間隔
	int nCntWidth = (nCnt - 2) % MAX_RANKING_RANK;

	pVtx[0].pos = D3DXVECTOR3(g_posRankingRank.x - RANKING_RANK_WIDTH / 2, (g_posRankingRank.y + RANKING_RANK_HEIGHT / 2) + RANKING_RANK_SPACE * nCntWidth, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_posRankingRank.x - RANKING_RANK_WIDTH / 2, (g_posRankingRank.y - RANKING_RANK_HEIGHT / 2) + RANKING_RANK_SPACE * nCntWidth, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_posRankingRank.x + RANKING_RANK_WIDTH / 2, (g_posRankingRank.y + RANKING_RANK_HEIGHT / 2) + RANKING_RANK_SPACE * nCntWidth, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_posRankingRank.x + RANKING_RANK_WIDTH / 2, (g_posRankingRank.y - RANKING_RANK_HEIGHT / 2) + RANKING_RANK_SPACE * nCntWidth, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);

	//テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffRanking->Unlock();
}

//=============================================================================
// ランキングのスコア
//=============================================================================
void RankingScore(int nCnt)
{
	//変数宣言
	VERTEX_2D *pVtx;
	int nCntWidth = 0;
	int nNumber;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffRanking->Lock(0, 0, (void**)&pVtx, 0);

	//何番目か
	pVtx += (nCnt - g_nCntScore + MAX_SCORE * g_nCntScore) * 4;

	for (int nCntScore = 0; nCntScore < MAX_SCORE; nCntScore++)
	{
		//スコアを配置する幅の間隔
		int nWidth = nCntScore % MAX_SCORE;

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_posRankingScore.x - (RANKING_SCORE_WIDTH / 2) + RANKING_SCORE_SPACE * nWidth, g_posRankingScore.y + (RANKING_SCORE_HEIGHT / 2) + RANKING_RANK_SPACE * g_nCntScore, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_posRankingScore.x - (RANKING_SCORE_WIDTH / 2) + RANKING_SCORE_SPACE * nWidth, g_posRankingScore.y - (RANKING_SCORE_HEIGHT / 2) + RANKING_RANK_SPACE * g_nCntScore, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_posRankingScore.x + (RANKING_SCORE_WIDTH / 2) + RANKING_SCORE_SPACE * nWidth, g_posRankingScore.y + (RANKING_SCORE_HEIGHT / 2) + RANKING_RANK_SPACE * g_nCntScore, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_posRankingScore.x + (RANKING_SCORE_WIDTH / 2) + RANKING_SCORE_SPACE * nWidth, g_posRankingScore.y - (RANKING_SCORE_HEIGHT / 2) + RANKING_RANK_SPACE * g_nCntScore, 0.0f);

		// rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// 頂点カラーの設定
		pVtx[0].col = g_ScoreCol[g_nCntScore];
		pVtx[1].col = g_ScoreCol[g_nCntScore];
		pVtx[2].col = g_ScoreCol[g_nCntScore];
		pVtx[3].col = g_ScoreCol[g_nCntScore];

		//スコアを格納
		nNumber = (g_aScore[g_nCntScore] / (int)powf(10.0f, (float)(MAX_SCORE - (nWidth + 1))));

		//テクスチャの更新
		pVtx[0].tex = D3DXVECTOR2(0.0f + nNumber * 0.1f, 1.0f / TEXTURE_Y);
		pVtx[1].tex = D3DXVECTOR2(0.0f + nNumber * 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f / TEXTURE_X + nNumber * 0.1f, 1.0f / TEXTURE_Y);
		pVtx[3].tex = D3DXVECTOR2(1.0f / TEXTURE_X + nNumber * 0.1f, 0.0f);

		//4つ進める
		pVtx += 4;
	}

	//スコア表示の更新
	g_nCntScore++;

	//頂点バッファをアンロックする
	g_pVtxBuffRanking->Unlock();
}