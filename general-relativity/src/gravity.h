// @author Tomohito Inoue <hypernumbernet@users.noreply.github.com>

//重力多体シミュレータ
//データ出力プログラム本体

using namespace hnn;

//出力するフレーム(時間進捗)数
#define NUM_FRAME 10000

//粒子数
#define NUM_PARTICLE 2000

//初期位置配置方式
#define INIT_PRESET 3

//1フレームの時間(s)
#define TIME_OF_ONE_FRAME 3600.0 * 24.0
//#define TIME_OF_ONE_FRAME 3600.0 * 24.0 * 365.0 * 1.0e+3

//初期位置集積度
#define DENSITY 0.3
//#define DENSITY 0.9

//粒子の初期運動量の平均
#define MOMENTUM_AVRG 0

//重力定数(m3 s−2 kg−1)
#define GRAVITATIONAL_CONSTANT 6.67259e-11

//光速度(m/s)
#define SPEED_OF_LIGHT 2.99792458e+8

//データファイル関連
#define VERSION 1
#define HEADER_SIZE 256
#define VECTOR3_BYTE_LEN 24 // 24 = 8 * 3 ： Vector3のdoubleが3つ分の長さ

//出力するファイル名
#define MAX_FILE_NAME 256
#define OUT_DAT L"gravity%d%02d%02d-%02d%02d%02d.3dp"

//スレッド数
#define NUM_THREAD 8

extern Vector3<double> * location;
extern Vector3<double> * velocity;
extern Vector3<double> * skewness;
extern double * mass;
extern int num_particle;
extern double density;
extern double zoom;
extern double momentum_avrg;
extern double g_const;
extern double speed_of_light;
