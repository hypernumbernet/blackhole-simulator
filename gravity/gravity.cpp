// @author Tomohito Inoue <hypernumbernet@users.noreply.github.com>
#include "stdafx.h"
#include "gravity.h"
#include "init_particle.h"

//----------------------------------------------------------------------------
//グローバル変数宣言
//----------------------------------------------------------------------------
//粒子間の距離を格納しておく一時変数
double * distance;

//粒子の位置を表す四元数
Quaternion<double> * locat;

//各粒子の運動量
Quaternion<double> * momentum;

//四元数の対数関数から三次元に変換した位置
Vector3<double> * locat_ln_r;

int num_particle;
int density;
double zoom;
double momentum_avrg;
double g_const;
double speed_of_light;
double speed_of_l_inv;

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
//距離の計算
inline double get_distance(int i, int j)
{
	double dot = locat[i].Dot(locat[j]);
	if (dot >= 1.0) // 距離ゼロを許容しない
	{
		printf("[warning] dot:%f >= 1\r\n", dot);
		return DBL_MIN;
	}
	if (dot < -1.0)
	{
		printf("[warning] dot:%f <= 1\r\n", dot);
		dot = -1.0;
	}
	return acos(dot);
}
//----------------------------------------------------------------------------
//ローレンツ因子
/*
p = mvγ
から、速度イコールに変形して
v = p / sqrt(m^2 + |p|^2 / c^2 )
*/
inline double lorentz_speed(double x)
{
	//return 1.0 / sqrt(1.0 + x * x * (1.0 / speed_of_light));
	return 1.0 / sqrt(1.0 + x * x * speed_of_l_inv);
}
//ローレンツ因子の逆数
inline double lorentz_factor_inv(double x)
{
	return sqrt(1.0 - x * x * (1.0 / (speed_of_light + x * x)));
}
//----------------------------------------------------------------------------
//1フレーム時間が進行するごとにする計算
unsigned __stdcall time_progress(void * pArguments)
{
	lpINT_RANGE para = (lpINT_RANGE)pArguments;

	int i;
	double theta;
	Vector3<double> direction;

	//運動量をかける
	for (i = para->start; i < para->end; ++i)
	{
		if (speed_of_light == 0.0)
		{
			//locat[i] *= momentum[i];
			locat[i] = momentum[i] * locat[i];
		}
		else
		{
			//特殊相対性理論による光速度限定
			theta = acos(momentum[i].i0);
			theta *= lorentz_speed(theta);
			//theta /= 2.0;
			if (theta == theta)
			{
				direction.x = momentum[i].i1;
				direction.y = momentum[i].i2;
				direction.z = momentum[i].i3;
				direction.Normalize();
				locat[i] = locat[i].RotMove(theta, direction);
			}
			else
			{
				momentum[i] = Quaternion<double>::Identity;
			}
		}

		//四元数の自然対数をとって３つの角度を算出。その角度を粒子の位置にする。
		//表示のことを考慮して拡大しておく。
		locat_ln_r[i] = locat[i].LnV3() * zoom;
	}

	return 0;
}
//----------------------------------------------------------------------------
//相互作用の計算
unsigned __stdcall relation(void * pArguments)
{
	lpINT_RANGE para = (lpINT_RANGE)pArguments;

	int i, j, k;
	double theta;
	//double dm;
	Quaternion<double> q;
	Vector3<double> direction;
	//Vector3<double> moment;
	//Vector3<double> v3;

	k = para->start * num_particle - (para->start + 1) * para->start / 2;

	//重力相互作用
	for (i = para->start; i < para->end; ++i)
	{
		for (j = i + 1; j < num_particle; ++j)
		{
			//--------------------------------------------------------------
			//4次元回転計算方式

			//積分計算のための準備
			//以下の計算をしているが、高速性のためにdistanceに次回のための計算結果を残している。
			//theta = 1 / (前フレームの距離) - 1 / (距離);
			theta = distance[k];
			//--------------------------------------------------------------
			//衝突の計算
			//if (theta == theta && theta > 100.0 /*&& theta < 51.0*/)
			//{
				/*momentum[i] = Quaternion<double>::Identity;
				momentum[j] = Quaternion<double>::Identity;*/
				/*q = momentum[i];
				momentum[i] = momentum[j];
				momentum[j] = q;*/
				/*theta = acos(momentum[i].Dot(momentum[j]));
				if (theta == theta)
				{
					theta *= 0.5;
					direction = momentum[j].Cross7V3(momentum[i]);
					direction.Normalize();
					if (direction == direction)
					{
						momentum[i] = momentum[i].RotMove(   theta * 0.4, direction);
						momentum[j] = momentum[j].RotMove( - theta * 0.4, direction);
					}
				}*/
			//	++k;
			//	continue;
			//}
			//--------------------------------------------------------------
			distance[k] = 1.0 / get_distance(i, j);
			theta -= distance[k];

			//	微分計算の場合
			//	theta = get_distance(i, j);
			//	theta = 1.0 / (theta * theta);
			//	theta = fabs(theta);

			if (theta == theta)
			{
				theta = fabs(theta);
				theta *= g_const;

				//位置の差
				direction = locat[i].Cross7V3(locat[j]);
				direction.Normalize();
				//--------------------------------------------------------------
				//ローレンツ短縮(Lorentz contraction)の補正
				//運動量によって重力を受ける方向だけが変わるとしている。
				//実装せず。以下理由
				//・結果がそれほど変わるものではなかった
				//・計算時間がおよそ1.5倍に増える
				//・物理的な意味付けが不完全
				/*if (speed_of_light != 0.0)
				{
					//運動量の差
					dm = acos(momentum[i].Dot(momentum[j]));
					if (dm == dm)
					{
						moment = momentum[i].Cross7V3(momentum[j]);
						moment.Normalize();
						direction =
							direction +
							lorentz_factor_inv(dm) *
							( - moment).Dot(direction) *
							moment;
						direction.Normalize();
					}
				}*/
				//--------------------------------------------------------------
				if (direction == direction)
				{
					momentum[i] = momentum[i].RotMove(-theta, direction);
					momentum[j] = momentum[j].RotMove(theta, direction);
				}
			}
			else
			{
				printf("[warning] invalid number at distance[%d].\r\n", k);
			}
			//--------------------------------------------------------------
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
			g_const = atof(argv[i] + 1) * 0.000001;
			break;
		case 'c':
			speed_of_light = atof(argv[i] + 1) * 0.000001;
			break;
		case 's':
			momentum_avrg = atof(argv[i] + 1) * 0.000001;
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
	printf("N-body Gravity Simulation on S3\nVersion 0.7\n");
	printf("n:%u particles\n", num_particle);
	printf("g:%f - G constant\n", g_const * 1000000);
	printf("c:%f - Speed of Light (Zero means no limit)\n", speed_of_light * 1000000);
	printf("i:%u - Initialize Preset\n", init_preset);
	printf("s:%f - Initialize Momentum Average\n", momentum_avrg * 1000000);
	printf("d:%d - Initialize Density\n", density);
	printf("r:%f - Zoom Ratio\n", zoom);
	wprintf(L"o:%s - output\n", out_dat);
	printf("f:%u frames\n", num_frame);
	printf("\n");
	//--------------------------------------------------
	//重力相互作用数
	num_collision = num_particle * (num_particle - 1) / 2;

	//光速の逆数をあらかじめ計算
	speed_of_l_inv = 1.0 / speed_of_light;

	//乱数準備
	srand((unsigned)time(NULL));

	//--------------------------------------------------
	//メモリ確保
	locat = new Quaternion<double>[num_particle];
	locat_ln_r = new Vector3<double>[num_particle];
	momentum = new Quaternion<double>[num_particle];
	distance = new double[num_collision];
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
	i = sprintf_s(buffer, j, "3DP NBodyGravity-S3.exe ");
	i += sprintf_s(buffer + i, j - i, "n%u ", num_particle);
	i += sprintf_s(buffer + i, j - i, "g%f ", g_const * 1000000);
	i += sprintf_s(buffer + i, j - i, "c%f ", speed_of_light * 1000000);
	i += sprintf_s(buffer + i, j - i, "s%f ", momentum_avrg * 1000000);
	i += sprintf_s(buffer + i, j - i, "d%d ", density);
	i += sprintf_s(buffer + i, j - i, "r%f ", zoom);
	i += sprintf_s(buffer + i, j - i, "i%u ", init_preset);

	WriteFile(hFile, buffer, j, &dwWriteSize, NULL);

	//--------------------------------------------------
	//粒子の初期化
	init_particle(init_preset);

	//最初のフレームの粒子位置情報を出力
	WriteFile(hFile, locat_ln_r, num_particle * VECTOR3_BYTE_LEN, &dwWriteSize, NULL);
	printf("1\r");

	//--------------------------------------------------
	//最初のフレームの粒子間距離の計算
	k = 0;
	for (i = 0; i < num_particle - 1; ++i)
	{
		for (j = i + 1; j < num_particle; ++j)
		{
			distance[k] = 1.0 / get_distance(i, j);
			++k;
		}
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
		WriteFile(hFile, locat_ln_r, num_particle * VECTOR3_BYTE_LEN, &dwWriteSize, NULL);
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
		//--------------------------------------------------
		//念のため正規化
		//実際どのくらい値がずれるのかは要調査
		/*
		if (count % 256 == 255)
		{
			for (i = 0; i < num_particle; ++i)
			{
				momentum[i].Normalize();
				if ( ! (momentum[i] == momentum[i]))
					momentum[i] = Quaternion<double>::Identity;
				locat[i].Normalize();
				if ( ! (locat[i] == locat[i]))
				{
					locat[i] = Quaternion<double>(rand(),rand(),rand(),rand());
					locat[i].Normalize();
				}
			}
		}*/
		//--------------------------------------------------
	}

	//メモリ開放
	delete[] locat;
	delete[] locat_ln_r;
	delete[] momentum;
	delete[] distance;

	CloseHandle(hFile);

	printf("\ncompleted. %d sec.", (clock() - start) / CLOCKS_PER_SEC);
	getchar();

	return 0;
}
