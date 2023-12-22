# C语言五子棋大作业

## 编译运行方法
先新建一个名叫Bulid的文件夹，然后运行build.sh即可编译，编译出的文件为main。

## 模式选择
- AI.h中可以选择训练模式和比赛模式
- game.c开头有控制功能的选项

## 功能模块
- AI:用于计算自动落子
- game:用于控制比赛，提供比赛接口,图形化界面,裁判等功能
- main:用于整合所有功能

##  棋形判断
把一个方向上的所有子存到一个uint16_t变量中，用两位表示棋盘的一种状态,00为不可落子，11为空，01为和stdchess相同.

### 活四检测
两个点成五
活4有只有一种情况:11 01 01 01 01 11

### 冲四检测
一个点成五
冲4有以下几种情况:
- 00 01 01 01 01 11
- 11 01 01 01 01 00
- 01 01 01 11 01 xx
- 01 01 11 01 01 xx
- 01 11 01 01 01 xx

### 活三检测
一步活四
活三有以下几种情况:
- 11 01 01 01 11 11
- 11 01 01 11 01 11
- 11 01 11 01 01 11
- 11 11 01 01 01 11

### 假活三检测
假活三没有意义,既不会形成禁手,把假活三补为冲四也会被马上拦下,因此不做检测.

## 禁手判断
采取递归判断的方式，判断过程中遇到空格就判断该空格是否为禁手，如此不断递归。

### 同一条线上的三三禁手
不可能下一个子,在同一条线上形成2个活三.

证明如下:

采用反证法,下一个子形成了两个活三,则一定有
- 上述活三的中间11重合的情况,这样会导致两端的11和01相对应.
- 中间的11和两头的11重合,会产生无重合的三三(1+4)或假三三(2+3)

### 同一条线上的四四禁手
因为四可以是活四也可以是冲四,所以可能在同一条线上产生四四禁手.

判断时只需要在检测窗口不同时都检测到冲四或者活四,就是四四禁手.

只需要不可能出现两个四的后5位和前5位相同即可.

事实上有两个反例:
- 01 11 01 01 01 xx and 11 01 01 01 01 00
- 01 11 01 01 01 xx and 11 01 01 01 01 11

把xx改为00/11就不会出此问题