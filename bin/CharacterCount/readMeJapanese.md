# 文字数カウント (CharacterCount)
Copyright © Emurasoft Inc. 2020
Licensed under the MIT license. See LICENSE for details.

文字数カウント プラグインは、文書内に様々な種類の文字が何個含まれているかを調べることができます。KKSoftWare 様によって作成され多くの方に利用されている Text Information プラグインを参考にしました。文字数カウント プラグインは、Text Information プラグインと同機能を含みながら、64-bit 版を追加しました。

# インストール
1. 32-bit プラグインは EmEditor の 32-bit 版が、64-bit プラグインは EmEditor の 64-bit 版が必要です。プラグインの DLL ファイル (CharacterCount-64bit-v1.dll または CharacterCount-32bit-v1.dll) を 好きなフォルダにコピーまたは移動します。
2. EmEditor を起動して、ツール メニューの プラグイン - プラグインの設定 を選択します。
3. プラグインの設定 ダイアログ ボックスで、追加 ボタンをクリックして、プラグインの DLL ファイル (CharacterCount-64bit-v1.dll または CharacterCount-32bit-v1.dll) を選択します。

# 文字数カウントの使い方
プラグイン ツール バーで 文字数カウント ボタンをクリックするか、または ツール メニュー - プラグイン - 文字数カウント を選択します。文字数カウント プラグインは現在の文書、またはテキストが選択されている場合は現在の選択範囲を分析し、16種類の文字の合計を調べて、ダイアログ ボックスに表示します。

ダイアログ ボックスで、コピー ボタン、または Ctrl+C を押すと結果がクリップボードにコピーされます。また、設定... リンクをクリックすると、特殊な文字の分類方法を設定することができます。

# 文字タイプの情報
次の一覧では、既定の設定が使用された場合、文字の各タイプが定義されている Unicode コード値を説明します。既定の設定の文字の分類は、現在の Unicode 標準の文字の分類に一致しています。

* 文字 - U+0000 ～ U+10FFFF
* 論理行 - ファイルの行数
* 表示行 - エディタで表示される行数
* LF - U+000A
* CR - U+000D
* CR+LF - U+000D and U+000A
* 幅 (旧バイト数) - 文字幅の合計
* 半角文字 - 半角文字
* 全角文字 - 全角文字
* 半角空白 - U+0020
* 全角空白 - U+3000
* タブ文字 - U+0009
* 制御文字 - U+0000 to U+001F and U+007F to U+009F
* 漢字 - U+4E00 to U+9FFF
* ひらがな - U+3040 to U+309F
* カタカナ - U+30A0 to U+30FF
* 半角カナ - U+FF61 to U+FF9F

注意:
* タブ文字を除く制御文字は文字の合計と半角文字の合計から除かれます。結合文字、異体字セレクタ、くの字点は、文字として数えられません。
* LF は、LINE FEED を CR は CARRIAGE RETURN の略語です。CR と LF のシーケンスは、CR+LF として数えられ、CR と LF の別々に数えられません。
* 文字の幅は EmEditor の内部で決定され、「Unicode Standard Annex #11: East Asian Width」に従います。「幅 (旧バイト数)」の横には全文字の幅の合計が表示され、半角文字と全角文字の横には、各タイプの幅の頻度を示します。
* エディタでテキストが選択されると、プラグインは、現在の選択範囲の文字のみ数えます。この場合、選択開始行と選択終了行の情報が表示されます。

# 設定ウィンドウ
設定ウィンドウにより、文書の内容により異なる数え方が必要なことの多い様々な文字の分類方法を設定することができます。

文字の分類方法を変更すると、文字の種類が変わりますが、改行コードを除き、文字数の合計に変更はありません。

メイン ダイアログ ウィンドウの 設定... リンクをクリックするか、プラグイン ツール バーの 文字数カウント ボタンを右クリックして プロパティ を選択すると、設定ウィンドウが表示します。設定ウィンドウで、左側の列は文字の分類方法を変更できる様々な文字が表示されます。それらの文字は、右側の列の対応するドロップ ダウン リストで分類方法を割り当てられます。

リセット ボタンを押すと、すべてのオプションを既定にリセットします。

次の一覧は、各オプションが調節する分類方法の Unicode コード値について説明します。

* 改行 (CR,LF,CR+LF) - U+000D, U+000A, U+000D and U+000A
* 全角句読点 (、,。) - U+3001, U+3002
* 半角句読点 (｡,､) - U+FF61, U+FF64
* 全角濁点 (゛,゜) - U+309B, U+309C
* 半角濁点Mark (ﾞ,ﾟ) - U+FF9E, U+FF9F
* 全角長音 (ー) - U+30FC
* 半角長音 (ｰ) - U+FF70
* 半角鍵括弧 (｢,｣) - U+FF62, U+FF63
* カタカナ中点 (・,･) - U+30FB, U+FF65
* くの字点 (〱,〲,〳,〴,〵) - U+3031, U+3032, U+3033, U+3034, U+3035
* 一つ点 (ゝ,ゞ,ヽ,ヾ) - U+309D, U+309E, U+30FD, U+30FE
* 同の字点 (々) - U+3005
* 締め(〆) - U+3006
* 漢数字の0 (〇) - U+3007

注意:
* 「改行」が「1文字として含める」の場合、CR+LF は 1文字と数えられます。
* 「カタカナ中点」が「カタカナまたは半角カナ」の場合、全角カタカナ中点はカタカナとして数えられ、半角カタカナ中点は半角カタカナとして数えられます。
* 「一つ点」が「ひらがなまたはカタカナ」の場合、ひらがな一つ点はひらがなとして数えられ、カタカナ一つ点はカタカナとして数えられます。

# アンインストール
1. EmEditor を起動して、ツール メニューの プラグイン - プラグインの設定 を選択します。
2. プラグインの設定 ダイアログ ボックスで、一覧から CharacterCount-64bit-v1 または CharacterCount-32bit-v1 を選択し、アンインストール ボタンをクリックします。

# フィードバックと質問

文字数カウント プラグインについての質問やフィードバックを送信するには、次の方法があります。

* GitHub レポジトリに問題を提起する: https://github.com/Emurasoft/CharacterCount
* Web サイトから送信: https://jp.emeditor.com/support/#contact 

# 謝辞
文字数カウント プラグインの作成にあたって、参考とさせていただいた Text Information プラグインの作者様である KKSoftWare 様に感謝いたします。

# ソース コード
文字数カウント プラグインのソース コードは [Github](https://github.com/Emurasoft/CharacterCount) で利用可能です。
