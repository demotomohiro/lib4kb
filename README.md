lib4kb
===============
by Tomohiro

概要
---------------
4k intro作成時に便利なライブラリをまとめたものです。
header onlyなライブラリとツールが含まれています。

ファイル構成
---------------
* bin
    ツールの実行ファイルを置く場所

* src
    ツールのソースコード

* tofu
    ヘッダーファイル

インストール
---------------
1. どこかにこのディレクトリをコピーする.
2. プロジェクトのインクルードディレクトリにlib4kbへのパスを追加する.
3. shader minifier.exeをbinディレクトリへコピーする.

Shader Minifier
https://github.com/laurentlb/Shader_Minifier/
http://www.pouet.net/prod.php?which=55176

glslpp
---------------
GLSL言語用のプリプロセッサです。

 #version
 #extension
といったGLSL言語固有のものはそのまま残ります。

これでGLSL内で#includeが使えるようになります。
きっと#defineも展開したほうがより圧縮されやすくなるでしょう。

Boost waveのサンプルを少し改造して作りました。

ライセンス
---------------
This software is released under the MIT License, see LICENSE.txt.
