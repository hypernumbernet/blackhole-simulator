# ブラックホールシミュレーター

重力多体シミュレーター （Qt GUI 版）

**バージョン 2.0 _β_**

Qt Framework によるデスクトップアプリケーションです。OpenGL 4.5 を使用しています。

## ダウンロード
https://github.com/hypernumbernet/blackhole-simulator/blob/master/binary/blackhole-simulator-win-x64.7z

## 特徴

*まだ開発中で不安定です。下記特徴は実装中です。*

このプログラムでは、多数の粒子間に働く力をエンジンとして定義していて、エンジンを切り替えながら様々な初期条件からの時間進展を検証できます。
現実世界の惑星のデータを入力して軌道を予測したり、曲がった時空間をシミュレーションすることもできます。

実世界の惑星や衛星データを準備すれば読み込ませることが出来ます。

## 使い方

* シミュレーションの操作

  - "Settings..." をクリックしてダイアログを開き、シミュレーションの条件を設定します。
  - "Start" をクリックしてシミュレーションをスタートします。
  - "Reset" クリックすると最初に戻ります。
  - コマ送りボタンで少しずつシミュレーションを進めることが出来ます。
  - スケールスライダーで原点を中心とした拡大縮小が出来ます。
  - "Monitoring..." をクリックしてダイアログを開き、現在の粒子の位置を数値で取得出来ます。
  - 設定値は設定ファイルとしてセーブ／ロード出来ます。
  - 設定ファイルをテキストで編集することで任意のデータをロードすることが出来ます。

* カメラ位置の操作
  - マウスホイール : 前進と後退
  - マウス左ドラッグ : 回転
  - マウス右ドラッグ : 平行移動
  - マウス左右同時押しドラッグ : ローリング
  - マウス中クリック : 原点を見てY軸を上に
  - W-S-A-D キー : 平行移動
  - Q-E キー : ローリング
  - 矢印キー : 周囲を見る
  - スペースキー : 上下移動
  - タブキー : 原点を見てY軸を上に
  - シフトキー : XZ平面に立つ
  - エスケープ : 位置をリセット
  - ホーム／エンド : 原点接近／離脱
  - バックスペース : 原点を見る

## 開発環境

### Qt Creator 10, Qt 6.5.1

* コンパイラー : MSVC2019 または MinGW または GCC (すべて64bit)
* OS: Windows11 または Kubuntu 22.04
* ビデオカード : NVIDIA GeForce または AMD Radeon

## 技術要素

* C++17
* GPGPUを使った並列化された高速数値計算。
* CPUのコアの数に最適化された可変マルチスレッドでのCPU計算のためのクラス設計。
* 3DCGのカメラ視点で制限のない動きの計算。軸に固定されたり上下の制限のない回転。

## シミュレーションエンジンの解説

### "Gravity" 古典物理学による単純な重力多体シミュレーション

物質粒子が力によって相互作用するのは、ニュートンの式 F=ma によります。a は加速度で時間あたりの速度の変化量です。言い換えれば力とは、運動量 mv の変化量です。実際に計算するに当たっては運動量 mv の交換の計算を中心に考えると分かりやすいです。同量の運動量の交換を繰り返せば、系としては運動量は保存されます。N個の粒子の相互作用の組み合わせの数はとんでもない量になりますが、それをひたすら単純計算していきます。

### "Gravity and Collision" 古典重力と衝突計算

粒子同士がゴムボールのようにバウンドしたら面白いと思い衝突の計算を付加したものです。結果として星の誕生のようなシミュレーションになりました。条件によって連星になったりします。衝突の力の計算のみでは、塊が圧縮された後に爆発四散してしまうので、近接時には強い斥力も付与しています。

### "Universe 1" 3次元超球面での重力多体シミュレーション No.1

3次元超球面の角度を3次元座標とみなしたシミュレーションです。四元数の自然対数を使って可視化しています。（調整中）

### "Universe 2" 3次元超球面での重力多体シミュレーション No.2

こちらも3次元超球面上の座標になります。重力の計算を3次元平面で行い算出された速度を3次元超球面の回転に変換して計算するものです。四元数のひねりにより銀河のようなシミュレーション結果になり、大変面白く見ていて飽きないものです。条件によってコアが2つに分裂してから再統合する様子は、不規則銀河のような感じにも見えます。"Universe2-003.ini" を読み込むと調整された結果を見ることが出来ます。拡大表示して見てください。

### "Relativity 1" 相対性理論の考え方でのシミュレーション No.1

特殊相対性理論のローレンツ変換を実際に計算して試してみたかったので作成したものです。単にローレンツ変換を繰り返したらどうなるかのシミュレーションになっています。光速を超える強い重力条件になると、変数が無限大になるので今の計算方法では粒子の動きがストップするようになっており、不完全なシミュレーションになっています。

### "Relativity 2" 相対性理論の考え方でのシミュレーション No.2

先のローレンツ変換の方法は行列を用いたものですが、強い条件下では粒子がフリーズしてしまいました。回避するために行列を使用しない双四元数を利用した計算を試してみています。双四元数によりローレンツ変換を角度の足し算に置き換えることで、計算速度的にも有利になるようです。設定ファイル "Blackhole002.ini" を読み込むと違いが分かります。

## 数学の解説

PDF形式のまとめはこちら

https://github.com/hypernumbernet/blackhole-simulator/blob/master/documents/blackhole-simulator-doc-mathematics.pdf

### 円そして超球

普通4次元を想像してと言われると、まず2次元の平面の上を粒子が相互作用する様子を考え、それを3次元に拡張しながら考えを進めていくと思います。そして4次元の形をイメージすることは出来ないでしょう。出来る人が居たら教えてください。

実はそれとは別の高次元への拡張の仕方があります。円の上しか粒子が存在せず、そこで相互作用する様子を想像してみてください。そして、円を球にしてみてください。地球の表面を思い浮かべると思います。この普通の球をさらに一つ高次元にしてみましょう。ちょっとイメージできない領域になると思います。しかし、数式的には単純に単位四元数で表すことが出来ます。これをS3空間と言うそうです。この空間での相互作用の様子を計算してみています。

### 回転計算

複素数を使うと回転の計算が簡単にできることは習った方も多いと思いますが、複素数で出来るのは2次元での回転です。3次元での回転は四元数を使います。3次元の回転の計算はかなり有用なので計算方法を書いておきます。

3次元空間での方向 a, b がそれぞれ単位ベクトルで表されているとします。a から b の方向へ t の割合だけ任意の3次元ベクトル f (fx, fy, fz) を回転させます。

* a と b の内積から算出した角度を2で割った角度 v と、a と b の外積を正規化した回転軸 e (ex, ey, ez) を算出します。
* 回転の割合 t を v にかけて、回転を表す四元数を計算します。 q = cos(vt) + i ex sin(vt) + j ey sin(vt) + k ez sin(vt)
* q の共役四元数 p = cos(vt) - i ex sin(vt) - j ey sin(vt) - k ez sin(vt) を準備します。
* f を四元数化します。 F = i fx + j fy + k fz
* 四元数の掛け算で p * F * q を計算します。この結果の四元数の虚部3つが回転結果の3次元ベクトルとなります。

面倒な計算ですが、球面線形補間 slerp を行うには必須となります。

### 4次元の回転

3次元のベクトルを回転させようとすると上記の手順で良いですが、4次元の単位四元数の回転はどうなるでしょうか？

回転の様子を見るために、単位四元数を3次元空間に投影して視覚化する手段を考えます。地球の表面を平面の地図に投影する仕方は、色々な図法として存在しますが、必ず歪みが生じます。同じように単位四元数を3次元空間に投影すると歪んで見えてしまうと思われます。ただ、自由度は3なのでやって出来ないことはなさそうです。また、単位四元数の表す超球S3では、普通の球面と違い平行線が引けることが分かっています。本当に平行線として成立しているのか視覚化して確かめてみてやりましょう。

まず、単位四元数に単位四元数を掛け算して視覚化してみます。それが、Grid line の Quaternion S3 Rot. になります。ご覧の通りいびつな形に歪んでいます。また、螺旋を描くような感じになっており左右の対称性も失われています。ただ、お互い交わってはいないようで平行線としては成立しているようです。

### 八元数の導入

四元数での掛け算で平行な回転が出来るのは良いですが、視覚化して分かったこのひねりを何とか解消したいと考えます。四元数をもう一歩深めて、八元数を使って回転を計算してみることを考えましょう。八元数で算出できる7次元の外積を使用すると、4次元の回転軸が3次元で取得できるそうです。しかしながら、4次元を回転させるのに、7次元はオーバースペックでどれを使うか迷います。また、八元数の掛け算が公式として確立しておらず、参照するところによって数式が違っていたりします。

そこで、7次元のうちどの4つを使用するかの組み合わせ、Combination(7, 4) の35パターン全てを計算し表示してみています。それが、Grid line の Octonion S3 Rotation になります。

全部で4パターンのS3回転のパターンがあることが見て取れます。中には単位四元数の掛け算とおなじひねりがのものもあり、逆のひねりのものもあるようです。途中で交わってしまい明らかに平行線でなくなっているパターンも多いようです。

### 双四元数によるローレンツ変換

双四元数とは四元数の4つの部位を複素数にしたもので、8つの変数を内包します。四元数部分の虚数 i, j, k と複素数部分の虚数 h は互いに干渉しません。

`q = (q0 + q1 h) + (q2 + q3 h) i + (q4 + q5 h) j + (q6 + q7 h) k`

双四元数は2種類の共役を持ちます。四元数部分を共役したものと複素数部分を共役したものです。

`q* = (q0 + q1 h) - (q2 + q3 h) i - (q4 + q5 h) j - (q6 + q7 h) k`

`q~ = (q0 - q1 h) + (q2 - q3 h) i + (q4 - q5 h) j + (q6 - q7 h) k`

双四元数の実部と hi, hj, hk を時空の4元ベクトルとみなすと、不変量になります。

`s = t + x hi + y hj + z hk`

`ss* = t^2 - x^2 - y^2 - z^2`

`gg* = 1` を満たす g を用いると、ローレンツ変換が定義できます。

`g* s g~ (Lorentz Transformation)`

g は2つの部分を持っています。単位四元数と上記 s の単位時空四元数です。時空四元数はここでの呼称で、ソースコード中に Spacetime class として定義しています。

`g = q0 + q2 i + q4 j + q6 k (単位四元数)`

または

`g = q1 + q3 hi + q5 hj + q7 hk (単位時空四元数)`

ここでは、後者の単位時空四元数の使い方を書きます。指数関数を使って以下のように書けます。

`exp(ahr) = cosh(a) + hr sinh(a)`

`{a: 実数} {r = i x + j y + k z : xx + yy + zz = 1}`

ここで、方向が r で速さが `c tanh(a) {c: 光速}` となります。ローレンツ変換は、

`g* s g~ = exp(-0.5 ahr) s exp(-0.5 ahr)`

として求められます。割り算を使用しないので計算不能に陥りにくいです。また、ローレンツ変換を角度の足し算として計算できることが分かります。

ここで角度 a は、 `a = atanh(|v|/c)` として求められます。ラピディティとして知られているものです。しかし、多粒子シミュレーションとしてはこの式は扱い難いです。強い条件で v を合計すると簡単に c を超え計算不能になります。そこで、運動量 p による算出を行っています。

`|v| / c = |p| / sqrt(m^2 c^2 + |p|^2)`

この関係を利用すると、p がいくら大きくなっても数式は 1 以下になるので都合が良いです。m は質量ですがマイナスにはなりません。m がゼロの時は v=c  なので光子になることが分かります。ただ、atanh は1より小さくないと無限になるので、この方式では光子を扱うことは出来ないことが分かります。

双四元数によるローレンツ変換を成分として書くと、

`g  = ia + jb + kc + p + hiq + hjr + hks`

`g* = - ia - jb - kc + p - hiq - hjr - hks`

`g~ = ia + jb + kc + p - hiq - hjr - hks`

`f = w + hix + hjy + hkz`

と定義して、

`g* f g~ = (aa + bb + cc + pp + qq + rr + ss)w + 2((- bs + cr - pq)x + (as - cq - pr)y + (- ar + bq - ps)z)`

` + hi((aa - bb - cc + pp + qq - rr - ss)x + 2((bs - cr - pq)w + (ab + cp + qr)y + (ac - bp + qs)z))`

` + hj((- aa + bb - cc + pp - qq + rr - ss)y + 2((- as + cq - pr)w + (ab - cp + qr)x + (bc + ap + rs)z))`

` + hk((- aa - bb + cc + pp - qq - rr + ss)z + 2((ar - bq - ps)w + (ac + bp + qs)x + (bc - ap + rs)y))`

となります。つまり、再度時空四元数となります。 `g = p + hiq + hjr + hks` だけを使うとすれば、

`g* f g~ = (pp + qq + rr + ss)w + 2p(- qx - ry - sz)`

` + hi((pp + qq - rr - ss)x + 2q(- pw + ry + sz))`

` + hj((pp - qq + rr - ss)y + 2r(- pw + qx + sz))`

` + hk((pp - qq - rr + ss)z + 2s(- pw + qx + ry))`

となります。
