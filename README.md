# PjdController
Seeed XIAO RP2040 で 初音ミク Project DIVA MEGA39’s＋ 用アーケードコントローラー を作るためのコード。
RP2040 を搭載したボードで、Arduino-Picoでサポートされているものは、ちょっとコードを変更すれば動くはず。

# 環境構築
Visual Studio Code(VS Code) のArduino 拡張機能を使用しています。ボードの設定が保存されているので、VS Codeで開くと、ボード設定が復元されます。Arduino IDEも使えます。Arduino IDEは、ボード設定が保存されない(前回の設定を覚えているだけ)ので、ビルドするときに「ツール」→「ボード」の設定を確認してください。
* Arduino IDE をインストールする。
* [Arduino-Pico](https://github.com/earlephilhower/arduino-pico) ボードパッケージを導入する。詳細は[こちら](https://github.com/earlephilhower/arduino-pico#installing-via-arduino-boards-manager)を参照。
* Visual Studio Code をインストールする。(オプション)
* Visual Studio Code の Arduino 拡張機能をインストールする。(オプション)
* 「pjdController.ino」ダブルクリックでArduino IDEを開く。(Arduino IDEを使う場合)  
pjdController.ino の有るフォルダで右クリック→「Codeで開く」を押してVS Codeで開く。(VS Code を使う場合)
* Arduino IDEを使う場合、ボードの設定をする。  
「ツール」→「ボード」→「Raspberry Pi RP2040 Boards」→「Seeed XAIO RP2040」を選択する。  
もう一度ツールメニューを開くとメニューが増えている。  
「USB Stack」→「Adafruit TinyUSB」を選択する。
* Seeed XIAO RP2040 を Boot ボタンを押しながらPCに接続する。
* IDEの書き込みボタンを押す。シリアルポートの設定は何でも良いです。

