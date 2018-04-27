# CharacterCount
Version 1

Copyright © Emurasoft Inc. 2018

Licensed under the MIT license. See LICENSE for details.

CharacterCount, a plug-in for EmEditor, counts the sums of various types of characters in your document. This plug-in is inspired by the Text Information plug-in by KKSoftWare. CharacterCount retains the same feature sets from Text Information, but adds a 64-bit compatible version.

# Installation
1. Copy the 32-bit or 64-bit CharacterCount DLL file (CharacterCount-64bit-v1.dll or CharacterCount-32bit-v1.dll), that corresponds to your EmEditor build, to a folder to keep the plug-in in.
2. Run EmEditor and go to Tools > Plug-ins > Customize Plug-ins.
3. On the Customize Plug-ins dialog box, click the "Add" button, and select the copied DLL file.

# How To Use CharacterCount
Open CharacterCount by either clicking on the CharacterCount plug-in on the plug-ins toolbar, or going to Tools > Plug-ins > CharacterCount. CharacterCount will parse through the current document, or the current selection if text is selected, to find the sums of 16 different types of characters. The results will be displayed in a small dialog window.

From the dialog window, you can click on "Copy" or press Ctrl+C to copy a text-based version of the results to your clipboard. The classification of several characters can be configured by clicking "Settings...".

# Character Type Information
The following list describes the Unicode code point(s) that each type of character is defined as, when default settings are used. The classifications of characters under default settings match those in the current Unicode standard.

* Characters - U+0000 to U+10FFFF
* Logical lines - Number of lines in file
* View lines - Number of lines shown in editor
* LF - U+000A
* CR - U+000D
* CR+LF - U+000D and U+000A
* Width - Sum of character widths
* Halfwidth character - Halfwidth character
* Fullwidth character - Fullwidth character
* Space - U+0020
* Ideographic space - U+3000
* Tab character - U+0009
* Control character - U+0000 to U+001F and U+007F to U+009F
* CJK Unified Ideograph - U+4E00 to U+9FFF
* Hiragana - U+3040 to U+309F
* Katakana - U+30A0 to U+30FF
* Halfwidth katakana - U+FF61 to U+FF9F

Notes:
* Control characters, except tab characters, are subtracted from the character sum and halfwidth character sum. Combining characters, variation selectors, and VERTICAL KANA REPEAT MARK LOWER HALF are not counted as characters.
* LF stands for LINE FEED and CR stands for CARRIAGE RETURN. A sequence of CR and LF is counted as CR+LF and not CR and LF.
* The width of a character is determined inside EmEditor, which follows "Unicode Standard Annex #11: East Asian Width". The number displayed next to "Width" is the sum of the widths of all characters. The numbers displayed next to "Halfwidth character" and "Fullwidth character" shows the frequency of each type of width.
* If text is selected in the editor, CharacterCount will count only the characters in the current selection. If this is the case, additional information labeled "Selection start line" and "Selection end line" will appear, indicating the lines where the selection starts and ends.

# Settings Window
The settings window allows you to set the classification of various characters that are often counted differently according to context.

The settings window is opened either by clicking "Settings..." in the main dialog window, or right clicking on the CharacterCount icon in the plug-ins toolbar and clicking on "Properties".

In the settings window, the left column is the various characters that can have their classifications changed. Those characters can be assigned a classification with the corresponding drop-down menu in the right column. The "Reset" button resets all options to default.

Changing the classification of a character will change which type it falls under, but will not change the character sum, except for the end-of-line setting.

The following list describes the Unicode code points that each option adjusts the classifications for.

* Newline (CR,LF,CR+LF) - U+000D, U+000A, U+000D and U+000A
* Comma and full stop (、,。) - U+3001, U+3002
* Halfwidth full stop and comma (｡,､) - U+FF61, U+FF64
* Voiced Sound Mark (゛,゜) - U+309B, U+309C
* Halfwidth Voiced Sound Mark (ﾞ,ﾟ) - U+FF9E, U+FF9F
* Prolonged Sound Mark (ー) - U+30FC
* Halfwidth Prolonged Sound Mark (ｰ) - U+FF70
* Halfwidth Corner Brackets (｢,｣) - U+FF62, U+FF63
* Katakana Middle Dots (・,･) - U+30FB, U+FF65
* Vertical Kana Repeat Marks (〱,〲,〳,〴,〵) - U+3031, U+3032, U+3033, U+3034, U+3035
* Hiragana and Katakana Iteration Marks (ゝ,ゞ,ヽ,ヾ) - U+309D, U+309E, U+30FD, U+30FE
* IDEOGRAPHIC ITERATION MARK (々) - U+3005
* IDEOGRAPHIC CLOSING MARK (〆) - U+3006
* IDEOGRAPHIC NUMBER ZERO (〇) - U+3007

Notes:
* When newline is counted as "1 character", CR+LF is counted as 1 character together.
* If Katakana Middle Dots is treated as "Katakana or Half Kana", KATAKANA MIDDLE DOT is counted as katakana and HALFWIDTH KATAKANA MIDDLE DOT is counted as halfwidth katakana.
* If Hiragana and Katakana Iteration Marks is treated as "Hiragana or Katakana", HIRAGANA ITERATION MARK and HIRAGANA VOICED ITERATION MARK are counted as hiragana, and KATAKANA ITERATION MARK and KATAKANA VOICED ITERATION MARK are counted as katakana.

# Uninstall
1. Open EmEditor and go to Tools > Plug-ins > Customize Plug-ins...
2. On the Customize Plug-ins window, click on the CharacterCount plug-in on the left, and click the "Uninstall" button.

# Feedback and Questions
There are several ways to ask a question about CharacterCount or to submit feedback:

* Post an issue in the GitHub repository: https://github.com/Emurasoft/CharacterCount
* Contact the main developer, Makoto Emura, by email: makoto@emurasoft.com
* Make a post on the plug-ins forum: https://www.emeditor.com/forums/forum/plugins/
* Use the contact form on emeditor.com: https://www.emeditor.com/support/#contact

# Acknowledgement
We would like to thank KKSoftWare, who developed the Text Information plug-in, which our product is inspired by.

# Source Code
The source code for CharacterCount is available on [Github](https://github.com/Emurasoft/CharacterCount).
