// @author Tomohito Inoue <hypernumbernet@users.noreply.github.com>
//VERSION 0.71
//Windows

//重力多体シミュレータ
//データ出力プログラム本体

using namespace hnn;

//出力するフレーム数
#define NUM_FRAME 5000

//粒子数
#define NUM_PARTICLE 8000

//粒子の初期運動量の平均
#define MOMENTUM_AVRG 0.000001

//重力定数
#define GRAVITATIONAL_CONSTANT 0.0000004

//重力計算の切り替え定数
//#define DIFFERENTIAL_SCOPE 0.00001

//表示拡大率
#define ZOOM 100

//初期位置集積度
#define DENSITY 500

//初期位置配置方式
#define INIT_PRESET 5

//データファイル関連
#define VERSION 1
#define HEADER_SIZE 256
#define VECTOR3_BYTE_LEN 24 // 24 = 8 * 3 ： Vector3のdoubleが3つ分の長さ

//出力するファイル名
#define MAX_FILE_NAME 256
#define OUT_DAT L"gravity%d%02d%02d-%02d%02d%02d.3dp"

//スレッド数
#define NUM_THREAD 8

//----------------------------------------------------------------------------
//グローバル変数宣言
//----------------------------------------------------------------------------
//粒子間の距離を格納しておく一時変数
extern double * distance;

//粒子の位置を表す四元数
extern Quaternion<double> * locat;

//四元数の対数関数から三次元に変換した位置
extern Vector3<double> * locat_ln_r;

//各粒子の運動量
extern Quaternion<double> * momentum;

extern int num_particle;
extern int density;
extern double zoom;
extern double momentum_avrg;
extern double g_const;
