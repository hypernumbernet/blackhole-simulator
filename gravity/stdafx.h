// @author Tomohito Inoue <hypernumbernet@users.noreply.github.com>
// stdafx.h : 標準のシステム インクルード ファイルのインクルード ファイル、または
// 参照回数が多く、かつあまり変更されない、プロジェクト専用のインクルード ファイル
// を記述します。
//

#pragma once

#ifndef _WIN32_WINNT		// Windows XP 以降のバージョンに固有の機能の使用を許可します。                   
#define _WIN32_WINNT 0x0501	// これを Windows の他のバージョン向けに適切な値に変更してください。
#endif						

// TODO: プログラムに必要な追加ヘッダーをここで参照してください。

#include <stdio.h>
#include <windows.h>
//#include <tchar.h>
//#include <mmsystem.h>

#include <math.h>
//#include <stdlib.h>
#include <time.h>
#include <process.h>

#include "Quaternion.h"
#include "Vector3.h"

//#define DegreeToRadian(degree) ((degree) * 0.0174532925199433)
#define PI 3.1415926535897
#define HALF_PI 1.5707963267949

//#include <gl\glut.h>
//
//extern int num_particle;
//
//extern hnn::Vector3 <double> *locat_ln_r;
