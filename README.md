# C语言五子棋

## 编译运行方法
先新建一个名叫Bulid的文件夹，然后运行build.sh即可编译，编译出的文件为main。

## 模式选择
- AI.h中可以选择训练模式和比赛模式
- game.c开头有控制功能的选项

## 功能模块
- AI:用于计算自动落子
- game:用于控制比赛，提供比赛接口,图形化界面,裁判等功能
- main:用于整合所有功能,提供全局功能函数

##  棋形判断
把一个方向上的所有子存到一个uint32_t变量中，用两位表示棋盘的一种状态,00为不可落子，11为空，01为和stdchess相同。

### 活四检测
只有一种情况：11 01 01 01 01 11

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
假活三看似没有意义,既不会形成禁手,把假活三补为冲四也会被马上拦下，但补为冲四后会逼对方防守一步。

### 间隔三检测
有两个空的无需管两端
- 01 11 11 01 01
- 01 11 01 11 01
- 01 01 11 11 01

只有一个空的需要管两端,区别在于补上后是活四还是冲四
- 11 01 11 01 01 00
- 11 01 01 11 01 00
- 00 01 11 01 01 11
- 00 01 01 11 01 11

### 单子检测
单子有3种情况:两边空,一边空,两边堵。
两边空:11 01 11

一边空:
- 11 01 00
- 00 01 11

两边堵:00 01 00

### 双子检测
双子有6种情况:两边空,一边空,两边堵,间隔一,间隔二,间隔三(带间隔是为成5准备)

两边空: 11 01 01 11 

一边空,两边堵: 略

间隔一:11 01 11 01 11

间隔二:
- 11 01 11 11 01
- 01 11 11 01 11

间隔三:01 11 11 11 01

## 禁手判断
为应对假活三的情况,在判断活三时对空位进行递归判断。

递归清除黑子:清除这一轮刚放的。

### 同一条线上的三三禁手
不可能下一个子,在同一条线上形成2个活三。

证明如下:

采用反证法,下一个子形成了两个活三,则一定有
- 上述活三的中间11重合的情况,这样会导致两端的11和01相对应。
- 中间的11和两头的11重合,会产生无重合的三三(1+4)或假三三(2+3)

### 同一条线上的四四禁手
因为四可以是活四也可以是冲四,所以可能在同一条线上产生四四禁手。

判断时只需要在检测窗口不同时都检测到冲四或者活四,就是四四禁手。

只需要不可能出现两个四的后5位和前5位相同即可。

事实上有两个反例:
- 01 11 01 01 01 xx and 11 01 01 01 01 00
- 01 11 01 01 01 xx and 11 01 01 01 01 11

把xx改为00/11就不会出此问题

## AI
### alpha-beta剪枝
图片来自Wikipedia

![alpha-beta](https://cdn.jsdelivr.net/gh/RedrockerLi/RedrockerLi.github.io@main/Pics/alpha-beta.69mwimtqw1og.png)
我们这样解释这一颗树:
- MAX行代表AI得到的分数,MIN行代表对手的分数
- MAX到MIN的棋子是AI下的。
- 所有分数都来源于最后行的叶子节点,所以要按深度搜索。
- 事实上搜索偶数层,因为要考虑对手的防守。
- 剪枝参考:当前层已经计算出来的节点和下一层已经被计算出来的节点。
- 重点看最右边的一支,第二层前面已经计算出有一个值为6,且下一层算出了5,那么不管下一层计算出何种值,第二层最后一个数不可能大于5,根节点肯定会取到大于等于6的数,故把最右边的分支全部减掉。

#### 数学语言
对于MAX层(Y在MAX):
$Y=\max\{a,b,c,\min\{d,e,f\}\}$,
这些值依次被计算出来,如果
$d\le \max\{a,b,c\}$
则后面的$ef$不用计算。

对于MIN层:
$Y=\min\{a,b,c,\max\{d,e,f\}\}$,
这些值依次被计算出来,如果
$d\ge \min\{a,b,c\}$
则后面的$ef$不用计算。

#### 具体实现
从上面的信息可以马上想到,每一层都需要一个参数来记录对应的max/min。采用递归,把他们放在栈上,这里只做到了对于父节点相同的一系列子节点下分支的剪枝。

下面需要看的是,对于父节点不同的在同一级上的子节点,剪枝的比较有没有意义。其实是有意义的,比如上图的第二MIN层,3节点的下面如果还有6,8等若干情况,都没办法使第二MIN层3节点变化,更别说影响更高层的节点。也就是说,上述的剪枝效率是较低的。

$\alpha$: 在MAX轮次会被更新，用来记录当前节点的各个子节点中的最大值，如果子节点被剪枝了，那就是抛去被裁剪部分之后的最大值。

$\beta$: 在MIN轮次会被更新，用来记录当前节点的各个子节点中的最小值，如果子节点被剪枝了，那就是抛去被裁剪部分之后的最小值。

每一个节点都会有独立的$\alpha,\beta$,这两个参数会从叶子节点向根节点传递,进入其他分支。

```
function alphabeta(node, depth, α, β, maximizingPlayer) // node = 节点，depth = 深度，maximizingPlayer = 大分玩家
     if depth = 0 or node是终端节点
         return 打分值
     if maximizingPlayer
         v := -∞
         for 每个子节点
             v := max(v, alphabeta(child, depth - 1, α, β, FALSE)) // child = 子节点
             α := max(α, v)
             if β ≤ α
                 break // β裁剪
         return v
     else
         v := ∞
         for 每个子节点
             v := min(v, alphabeta(child, depth - 1, α, β, TRUE))
             β := min(β, v)
             if β ≤ α
                 break // α裁剪
         return v
```

### 打分
当前玩家的棋子分数为正,对手为负。

打分时不用理会一个棋形中的棋子被算多次。如活四为4000分,每一个棋子+1000分就正好4000分。

## 多线程
对于指数级复杂度的搜索而言，多线程并不能有效地帮助多搜一层或每层多几个点，但对提升使用者的体验有帮助.

采用[线程池](https://github.com/Pithikos/C-Thread-Pool)进行多线程管理.

因为计算时会改变棋盘等信息,所以每一条线程的数据应该是独立的.

没有调试成功，请勿使用.



## 训练
在使用位运算时增加棋形不会增加很大的开销,所以可以分出很多种情况.但随之而来的问题是调整各棋形的分数较困难.在这个部分,采用[粒子群算法](https://zh.wikipedia.org/zh-cn/%E7%B2%92%E5%AD%90%E7%BE%A4%E4%BC%98%E5%8C%96)进行调参。

如果初始条件不够好很容易陷入局部最优,可以同步使用多个随机初始条件训练。使用train.sh即可,注意里面分为开始和终止两部分。

实际用下来，跑完程序需要两天，且效果很差，最终采用手动打分。