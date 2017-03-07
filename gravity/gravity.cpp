// @author Tomohito Inoue <hypernumbernet@users.noreply.github.com>
#include "stdafx.h"
#include "gravity.h"
#include "init_particle.h"

//----------------------------------------------------------------------------
//グローバル変数宣言
//----------------------------------------------------------------------------

//粒子の位置
Vector3<double> * location;

//速度：粒子の1フレーム時間における空間移動距離
Vector3<double> * velocity;

//その粒子の時空の歪み
//rs / r をベクトルにして合算した値
//from Schwarzschild metric
//ds^2 = 0
//dr / dt = 1 - rs / r ; (dr > 0)
Vector3<double> * skewness;

//粒子の質量
double * mass;

//Schwarzschild radius
double * rs;

//1フレームの時間
double d_time;

int num_particle;
int density;
double zoom;
double momentum_avrg;
double g_const;
double speed_of_light;

//ファイル処理関係
HANDLE hFile;
DWORD dwWriteSize;

//マルチスレッド
typedef struct
{
	int start;
	int end;
}
INT_RANGE, *lpINT_RANGE;

//----------------------------------------------------------------------------
//初期計算
unsigned __stdcall prepare(void * pArguments)
{
	lpINT_RANGE para = (lpINT_RANGE)pArguments;
	double constant = 2.0 * g_const * (1.0 * (speed_of_light * speed_of_light));
	int i;
	for (i = para->start; i < para->end; ++i)
	{
		rs[i] = constant * mass[i];
		skewness[i].x = 1.0;
		skewness[i].y = 1.0;
		skewness[i].z = 1.0;
	}
	return 0;
}
//時空の歪みから速度の増加分を取得
double get_velocity(double skewness)
{
	return skewness * (1.0 - fabs(skewness)) * d_time * speed_of_light;
}
//----------------------------------------------------------------------------
//1フレーム時間が進行するごとにする計算
unsigned __stdcall time_progress(void * pArguments)
{
	lpINT_RANGE para = (lpINT_RANGE)pArguments;
	int i;
	for (i = para->start; i < para->end; ++i)
	{
		//skewness = rs / r
		//Speed = dr / dt = c * (1 - rs / r)
		velocity[i].x += get_velocity(skewness[i].x);
		velocity[i].y += get_velocity(skewness[i].y);
		velocity[i].z += get_velocity(skewness[i].z);
		location[i] += velocity[i];
		skewness[i].x = 1.0;
		skewness[i].y = 1.0;
		skewness[i].z = 1.0;
	}
	return 0;
}
//----------------------------------------------------------------------------
//相互作用の計算
unsigned __stdcall relation(void * pArguments)
{
	lpINT_RANGE para = (lpINT_RANGE)pArguments;

	int i, j, k;
	double rinv, ri, rj;
	Vector3<double> direction;

	k = para->start * num_particle - (para->start + 1) * para->start / 2;

	//重力相互作用
	for (i = para->start; i < para->end; ++i)
	{
		for (j = i + 1; j < num_particle; ++j)
		{
			//重力は瞬間的に伝達されるとして近似
			//skewness = rs / r
			direction = location[j] - location[i];
			//rinv = 1 / r^2
			rinv = 1.0 / direction.Norm();
			ri = rs[i] * rinv;
			rj = rs[j] * rinv;
			skewness[i].x += rj * direction.x;
			skewness[i].y += rj * direction.y;
			skewness[i].z += rj * direction.z;
			skewness[j].x -= ri * direction.x;
			skewness[j].y -= ri * direction.y;
			skewness[j].z -= ri * direction.z;
			++k;
		}
	}

	return 0;
}
//----------------------------------------------------------------------------

int main(int argc, char * argv[])
{
	//処理時間を計測スタート
	clock_t start;
	start = clock();

	//基本変数の宣言
	int i, j, k;
	int count;

	//マルチスレッド
	HANDLE thHA[NUM_THREAD];
	unsigned thID[NUM_THREAD];
	INT_RANGE arg_relation[NUM_THREAD];
	INT_RANGE arg_particle[NUM_THREAD];

	//相互関係の数
	int num_collision;

	//初期値の代入
	num_particle = NUM_PARTICLE;
	density = DENSITY;
	zoom = ZOOM;
	momentum_avrg = MOMENTUM_AVRG;
	g_const = GRAVITATIONAL_CONSTANT;
	speed_of_light = SPEED_OF_LIGHT;
	int num_frame = NUM_FRAME;
	int init_preset = INIT_PRESET;

	//現在の日時から出力ファイル名決定
	struct tm date;
	time_t now;
	int year, month, day;
	int hour, minute, second;

	time(&now);
	localtime_s(&date, &now);

	year = date.tm_year + 1900;
	month = date.tm_mon + 1;
	day = date.tm_mday;
	hour = date.tm_hour;
	minute = date.tm_min;
	second = date.tm_sec;

	wchar_t out_dat[MAX_FILE_NAME];
	swprintf_s(out_dat, MAX_FILE_NAME, OUT_DAT, year, month, day, hour, minute, second);

	//--------------------------------------------------
	//コマンドライン引数の解釈
	//n:粒子数
	//g:重力定数
	//c:光速度の二乗
	//s:初期運動量平均
	//d:初期位置集積度
	//r:ズーム倍率
	//f:出力フレーム数
	//i:初期条件プリセットナンバー
	for (i = 1; i < argc; ++i)
	{
		switch (argv[i][0])
		{
		case 'n':
			num_particle = atoi(argv[i] + 1);
			break;
		case 'g':
			g_const = atof(argv[i] + 1);
			break;
		case 'c':
			speed_of_light = atof(argv[i] + 1);
			break;
		case 's':
			momentum_avrg = atof(argv[i] + 1);
			break;
		case 'd':
			density = atoi(argv[i] + 1);
			break;
		case 'r':
			zoom = atof(argv[i] + 1);
			break;
		case 'f':
			num_frame = atoi(argv[i] + 1);
			break;
		case 'i':
			init_preset = atoi(argv[i] + 1);
			break;
		}
	}
	printf("N-body Gravity Simulation by General Relativity\nVersion 0.1\n");
	printf("n:%u particles\n", num_particle);
	printf("g:%f - G constant\n", g_const);
	printf("c:%f - Speed of Light (Zero means no limit)\n", speed_of_light);
	printf("i:%u - Initialize Preset\n", init_preset);
	printf("s:%f - Initialize Momentum Average\n", momentum_avrg);
	printf("d:%d - Initialize Density\n", density);
	printf("r:%f - Zoom Ratio\n", zoom);
	wprintf(L"o:%s - output\n", out_dat);
	printf("f:%u frames\n", num_frame);
	printf("\n");
	//--------------------------------------------------
	//重力相互作用数
	num_collision = num_particle * (num_particle - 1) / 2;

	d_time = TIME_OF_ONE_FRAME;

	//乱数準備
	srand((unsigned)time(NULL));

	//--------------------------------------------------
	//メモリ確保
	location = new Vector3<double>[num_particle];
	velocity = new Vector3<double>[num_particle];
	skewness = new Vector3<double>[num_particle];
	mass = new double[num_collision];
	rs = new double[num_collision];
	//--------------------------------------------------
	//マルチスレッド用変数準備
	//直角二等辺三角形の面積を等分する辺の長さを計算
	//x=N-√(N^2-2y)
	k = num_collision / NUM_THREAD;
	j = num_particle - 1;
	for (i = 0; i < NUM_THREAD; ++i)
	{
		arg_relation[i].start = j - (int)floor(sqrt((double)(j * j - 2 * k * i)));
	}
	for (i = 0; i < NUM_THREAD - 1; ++i)
	{
		arg_relation[i].end = arg_relation[i + 1].start;
	}
	arg_relation[NUM_THREAD - 1].end = num_particle;

	k = num_particle / NUM_THREAD;
	for (i = 0; i < NUM_THREAD; ++i)
	{
		arg_particle[i].start = i * k;
	}
	for (i = 0; i < NUM_THREAD - 1; ++i)
	{
		arg_particle[i].end = arg_particle[i + 1].start;
	}
	arg_particle[NUM_THREAD - 1].end = num_particle;
	//--------------------------------------------------
	//書き出しファイルオープン
	hFile = CreateFile(
		out_dat,
		GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,
		FILE_FLAG_SEQUENTIAL_SCAN/*FILE_ATTRIBUTE_NORMAL*/,
		NULL
	);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		printf("ファイルを作成できませんでした");
		return 1;
	}
	//--------------------------------------------------
	//dataファイルヘッダ書き出し

	int header[2];
	header[0] = num_particle;
	header[1] = VERSION;

	WriteFile(hFile, header, 8, &dwWriteSize, NULL);

	j = HEADER_SIZE - 8;
	char buffer[HEADER_SIZE - 8];
	for (i = 0; i < j; ++i)
	{
		buffer[i] = 0;
	}
	i = sprintf_s(buffer, j, "3DP nbodysim-gr ");
	i += sprintf_s(buffer + i, j - i, "n%u ", num_particle);
	i += sprintf_s(buffer + i, j - i, "g%f ", g_const);
	i += sprintf_s(buffer + i, j - i, "c%f ", speed_of_light);
	i += sprintf_s(buffer + i, j - i, "s%f ", momentum_avrg);
	i += sprintf_s(buffer + i, j - i, "d%d ", density);
	i += sprintf_s(buffer + i, j - i, "r%f ", zoom);
	i += sprintf_s(buffer + i, j - i, "i%u ", init_preset);

	WriteFile(hFile, buffer, j, &dwWriteSize, NULL);

	//--------------------------------------------------
	//粒子の初期化
	init_particle(init_preset);

	//最初のフレームの粒子位置情報を出力
	WriteFile(hFile, location, num_particle * VECTOR3_BYTE_LEN, &dwWriteSize, NULL);
	printf("1\r");

	//--------------------------------------------------
	for (i = 0; i < NUM_THREAD; ++i)
	{
		thHA[i] = (HANDLE)_beginthreadex(
			NULL,
			0,
			prepare,
			&arg_particle[i],
			0 /* CREATE_SUSPENDED */,
			&thID[i]
		);
	}
	WaitForMultipleObjects(NUM_THREAD, thHA, TRUE, INFINITE);
	for (i = 0; i < NUM_THREAD; ++i)
	{
		CloseHandle(thHA[i]);
	}

	//--------------------------------------------------
	//メインループ
	for (count = 2; ; )
	{
		for (i = 0; i < NUM_THREAD; ++i)
		{
			thHA[i] = (HANDLE)_beginthreadex(
				NULL,
				0,
				time_progress,
				&arg_particle[i],
				0 /* CREATE_SUSPENDED */,
				&thID[i]
			);
		}
		WaitForMultipleObjects(NUM_THREAD, thHA, TRUE, INFINITE);
		for (i = 0; i < NUM_THREAD; ++i)
		{
			CloseHandle(thHA[i]);
		}
		//--------------------------------------------------
		//粒子位置情報を出力
		WriteFile(hFile, location, num_particle * VECTOR3_BYTE_LEN, &dwWriteSize, NULL);
		printf("%u\r", count);
		//--------------------------------------------------
		++count;
		if (count > num_frame)
		{
			break;
		}
		//--------------------------------------------------
		for (i = 0; i < NUM_THREAD; ++i)
		{
			thHA[i] = (HANDLE)_beginthreadex(
				NULL,
				0,
				relation,
				&arg_relation[i],
				0/*CREATE_SUSPENDED*/,
				&thID[i]
			);
		}
		WaitForMultipleObjects(NUM_THREAD, thHA, TRUE, INFINITE);
		for (i = 0; i < NUM_THREAD; ++i)
		{
			CloseHandle(thHA[i]);
		}
	}

	//メモリ開放
	delete[] location;
	delete[] velocity;
	delete[] skewness;
	delete[] mass;
	delete[] rs;

	CloseHandle(hFile);

	printf("\ncompleted. %d sec.", (clock() - start) / CLOCKS_PER_SEC);
	getchar();

	return 0;
}
