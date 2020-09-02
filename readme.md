# CharacterCount
Copyright © Emurasoft Inc. 2020<br>
Licensed under the MIT license. See LICENSE for details.

This is the source code for CharacterCount, a plug-in for [EmEditor](https://www.emeditor.com/). Release binaries are found in [bin/CharacterCount](bin/CharacterCount). More information about this program are written in [bin/CharacterCount/readMeEnglish.md](bin/CharacterCount1/readMeEnglish.md).

# Build instructions
1. Download the submodules: `git submodule update --init --recursive`
2. Open CharacterCount.sln in Microsoft Visual Studio and build.
3. In EmEditor, go to Tools > Plug-ins > Customize Plug-ins... and import the compiled DLL.

# Test
```
cd googletest/googletest
mkdir build
cd build
cmake ..
# Build gtest.sln
# Build and run test.vcxproj
```