# 项目介绍

这是一个简单的记账项目，名为cashnote


## 依赖

Qt版本为`6.6.2`，依赖于 MinGW


## 如何运行和开发

在终端适当的目录下运行
```
git clone https://github.com/sillyDaibo/cashnote.git
```
即可得到cashnote文件夹，内含`CMakeList.txt`文件，使用QtCreator点击该项目的`CMakeList.txt`即可识别该项目，
尊重默认设置**无脑点下一步**，最后点击构建并运行即可。

后续推荐直接在QtCreator开发。也有替代方案是修改`run.ps1`和`build.ps1`的qt动态库文件路径，
以及将build文件夹中`compile_command.json`复制到`main.cpp`所在目录，即可使用VSCode等开发，
在powershell中`main.cpp`所在目录下输入`run`来运行，`build`来构建。但是**初次构建项目必须用QtCreator完成**。
