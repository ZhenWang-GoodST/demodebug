**DEBUG**

本项目是在vscode里面进行debug的教程，包含普通项目，ros项目，镜像ros项目

https://cloud.tencent.com/developer/article/1759597

**调试配置**

当写好代码和CMakeLists.txt之后，点击左侧Run and Debug按钮（⻓得像个⾍⼦）,点击create a launch.json file，创建⼀个叫launch.json的⽂件。

本⽂的vscode版本较新，旧版本可能界⾯不太⼀样，可直接点击下图中的⻮轮按钮，此时界⾯中⼼会弹出选项，直接点击第⼀个 C++(GDB/LLDB)，然后点最后⼀个默认的即可：

此时，vscode会⾃动创建⼀个launch.json⽂件，这个launch.json⽂件在⾃动创建的隐藏⽂件夹.vscode下（⽂件夹中按 ctrl+H就可以显⽰隐藏⽂件），只需要改2个地⽅：

* “program”：你的程序编译出来的可执⾏⽂件（⽤cmake先编译出⼀个可执⾏⽂件，只需要第⼀次，⼀旦后续配置好了就直接 按F5）
* “preLaunchTask”：新添加⼀个选项为build，这样每次F5调试才会重新编译⽣成新的可执⾏⽂件，否则只是执⾏
* “externalConsole”：如果是true，每次按F5调试会⾃动弹出⼀个额外的终端，程序内容会在这个弹出的终端下运⾏和显⽰， 但是每次调试完了还要⼈⼯关闭。若是false，程序会在vscode下⾯的界⾯运⾏和显⽰，有点像VS

接下去只要再弄2个⽂件，就可以debug你的项⽬了，第⼀个是tasks.json⽂件，可以在⽹上搜索如何⽣成，不过更直接的⽅ 式是直接在.vscode下新建⼀个tasks.json⽂件，然后把以下内容直接复制上去，图中和默认tasks.json⽂件不同的地⽅是”command”选项，这边改成build.sh，这个build.sh是⼀个脚本，可以⾃定义做很多事情：

第⼆个就是刚才提到的build.sh⽂件，这个脚本⽂件随便叫啥名字，⾥⾯随便做啥事情，每当按F5之后，会⾃动运⾏这个脚本 ⾥的事情，⽐如这⾥的build.sh：

**调试代码**

⼀切就绪之后，设好debug断点，直接按F5开始愉快的调试吧！F5直接运⾏到断点或结束，F10单步调试等（与windows下VS 操作类似

**总结**

在vscode下配置C++调试环境主要就是做⼀件事：创(复)建(制)１个⽂件夹和３个⽂件：

* launch.json
* tasks.json
* 编译执⾏的脚本：例⼦中的脚本内容实际就是cmake内容
