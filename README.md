
# 前言
想必大多数同学都有一定的使用计算机的经验，经验告诉我们，`.exe`文件是一个可执行程序，打开QQ、微信，甚至浏览本文时，必然有相关的程序在运行。双击打开程序这个动作非常简单，以至于每天不知道会点击鼠标多少次。

但是你有没有认真想过：**即使是一个最简单的打印 "Hello, world!" 的程序，计算机究竟做了些什么？** 这个问题将伴随你们学习计算机系统的整个过程（不小心泄露了计算机系统基础课程某年的整个期末考试题目）。很遗憾本教程不能将答案也一起泄露了（其实我也很难一口气把这件事讲清楚=_=）。

本教程将逐步让同学们理解程序从何而来，掌握基本的编写C/C++程序的技能，开启计算机系统学习的第一步。

# 如何获得帮助
在学习过程中，一定会遇到各种问题，有效地获得帮助是提高效率的关键。

首先，一定要自己尝试解决，上网搜搜，你的问题一般都有前人遇到过。但是网上的资料水平参差不齐，有时候找到了一个不正确的资料折腾半天仍然解决不了问题。

我们推荐从现在开始锻炼查阅英文资料的能力。尽管现在遇到的很多问题中文搜索引擎还勉强能解决。但越深入地学习，很多问题很难找到相关的中文资料。

我们推荐的网站：

|            	| 搜索引擎                                          	| 百科                                 	| 问答网站                                                                                  	|
|------------	|---------------------------------------------------	|--------------------------------------	|-------------------------------------------------------------------------------------------	|
| 推荐使用   	| Google ([搜索镜像](http://dir.scmor.com/google/)) 	| [维基百科](http://en.wikipedia.org/) 	|  [ProblemOverflow](http://39.100.120.199) <br> [StackOverflow](http://stackoverflow.com/) 	|
| 不推荐使用 	| ~~百度搜索~~                                      	| ~~百度百科~~                         	|  ~~百度知道~~ <br> ~~CSDN~~                                                               	|

如果部分网站不能访问，可以使用一些浏览器插件，比如Firefox的[Hoxx](https://addons.mozilla.org/zh-CN/firefox/addon/hoxx-vpn-proxy/)插件。

你可以尝试在百度和Google分别搜索与C++相关的关键词，对比两者的区别。
StackOverflow 是一个在程序设计领域的问答网站，专业性很强。

# 关于提问
请先阅读[提问的智慧](https://github.com/ryanhanwu/How-To-Ask-Questions-The-Smart-Way/blob/master/README-zh_CN.md) 和 ProblemOverflow的[如何提问](http://39.100.120.199/index.php?qa=1&qa_1=%E5%A6%82%E4%BD%95%E6%8F%90%E9%97%AE%E4%BB%A5%E5%8F%8A%E5%A6%82%E4%BD%95%E5%9B%9E%E7%AD%94%E9%97%AE%E9%A2%98%EF%BC%9F)。

# 相关书籍资料推荐
首先要说明的是，C语言和C++应该当做两门不同的语言。并且尽量不要写出C和C++混合风格的程序（如果没有概念可以先记住）。
我们推荐纯的C语言或C++学习的书籍或资料。下面的资料可以结合使用，内容上可能有覆盖。

## C++
- [C++ Primer 第五版](https://www.amazon.com/Primer-5th-Stanley-B-Lippman/dp/0321714113/)（不是C++ Primer Plus，有[中文版](https://www.phei.com.cn/module/goods/wssd_content.jsp?bookid=37655)可以购买）
- [C++ Tutorials](http://www.cplusplus.com/doc/tutorial/) （非常棒的入门教程）
- [C++ reference](https://en.cppreference.com/w/cpp) （函数的用法有详细示例代码）

## Ｃ
- Ｃ语言程序设计（群文件有）
- [Linux C编程](http://akaedu.github.io/book/)（前面有Ｃ语言基础，后面是[Linux](https://en.wikipedia.org/wiki/Linux)相关内容）
- [C Tutorial](https://www.tutorialspoint.com/cprogramming/index.htm)（在线编译运行）
- [C reference](https://en.cppreference.com/w/c)（函数的用法有详细示例代码）
- [The Linux Programming Interface](http://man7.org/tlpi/index.html) （个人认为学习Linux编程最好的书籍，需要一定C语言基础。[英文版PDF](https://moodle2.units.it/pluginfile.php/115306/mod_resource/content/1/The%20Linux%20Programming%20Interface-Michael%20Kerrisk.pdf)）
- 强烈不推荐：~~[谭浩强](https://www.zhihu.com/question/22793863)~~

# 其他资源
- C/C++在线编译调试：[onlinegdb.com](https://www.onlinegdb.com/)
- C++在线编译运行（可调部分GCC参数）：[cpp.sh](http://cpp.sh/)
