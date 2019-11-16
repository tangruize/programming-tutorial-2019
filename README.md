# programming-tutorial-2019

本项目是一个简易的C/C++编程指南，为南京大学计算机系问题求解课程2019级学生准备。往届的编程指南见[problem-solving-class-programming-tutorial](https://github.com/hengxin/problem-solving-class-programming-tutorial)。

项目分为3个子项目：[OJ](OJ)、[Slides](Slides) 和 [Tutorial](Tutorial)，使用cmake管理，可以用CLion打开查看。

## [OJ](OJ)

提供了一个管理 OJ 的 [CMakeLists.txt](OJ/CMakeLists.txt) 模板 和 方便调试的[输入输出重定向](OJ/lib)函数。也可以下载这个[压缩包](http://problemoverflow.top/download/OJ.zip)。

## [Slides](Slides)

线下讲解程序设计的PPT源代码，使用了 beamer 的 [mtheme](https://github.com/matze/mtheme) 主题。目录内只有源代码没有pdf，pdf下载见下面的表格：

| 日期 | 主题 | 文件 |
|------------|---------------------|-----------------------------------------------------------------------------------------------------------------------|
| 2019-10-25 | 环境搭建和C语言入门 | [introduction.pdf](https://github.com/tangruize/programming-tutorial-2019/releases/download/slide_1/introduction.pdf) |
| 2019-11-01 | 调试技巧, 函数和数组 | [function_array.pdf](https://github.com/tangruize/programming-tutorial-2019/releases/download/slide_2/function_array.pdf) |
| 2019-11-08 | 指针和动态内存 | [pointer_memory.pdf](https://github.com/tangruize/programming-tutorial-2019/releases/download/slide_3/pointer_memory.pdf) |
| 2019-11-16 | 结构体和malloc实现 | [struct_malloc.pdf](https://github.com/tangruize/programming-tutorial-2019/releases/download/slide_4/struct_malloc.pdf) |

## [Tutorial](Tutorial)

使用markdown编写，可以用 [gitbook-cli](https://github.com/GitbookIO/gitbook) 生成 html。目录内只有 markdown 没有 html，可以直接在Github上浏览（[目录](Tutorial/SUMMARY.md)），也可以打开 [problemoverflow.top/c](http://problemoverflow.top/c/)。

## 编译

本工程使用了cmake，很方便编译 exe, pdf 和 html。
```bash
git clone https://github.com/tangruize/programming-tutorial-2019  # 需安装git
cmake -S . -B build  # 初始化cmake，需安装cmake和make
cd build
make  # 编译OJ的例子，需安装cc和c++编译器，比如gcc和g++
make -j4  # 同上，但同时使用4个线程编译 
make -j4 slides  # 编译latex，需安装texlive、latexmk 和 xelatex
make gitbook  # 生成gitbook的html并在本地打开4000端口服务器，需安装gitbook-cli
```

## 其他
线下程设辅导斗鱼房间号: [7725816](https://www.douyu.com/7725816), 开播时间: 每周五 20:30-22:00