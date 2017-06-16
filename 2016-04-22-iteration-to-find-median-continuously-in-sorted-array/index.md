---
title: 有序数组连续计算中位数
date: 2016-04-22
tags: algorithm
path: /iteration-to-find-median-continuously-in-sorted-array/
---

有一个需求, 是绘制坐标轴刻度时遇到的. 因为坐标轴刻度随时需要保证疏密得当, 所以有时会采取以下策略:

1. 获取总长度, 并根据首尾的刻度值, 计算中间可取整的刻度值, 如 1 - 78 中间, 可按 10 取整, 即 10, 20, 30 ... 70 这样.
1. 设一个刻度总和的变量 `sum`, 试探性的绘制位于中间的刻度, 并计算该刻度(字符串)在画布上的长度, 将该长度累加至 `sum`,  并与总长度比较.
   1. 如果小于总长度, 绘制该刻度, 并借助首尾刻度, 以及该刻度, 继续求出两个刻度值. (位于首刻度与该刻度中间, 以及该刻度与尾刻度中间). 递归进入上一步骤.
   1. 如果大于总长度, 则退出.

这个问题可以抽象成, 给定一个有序序列(或直接指定一对 `min` 和 `max`), 要求输出每一次相邻数字的中位数.(如果相邻数字差距仅为 1, 则不必输出). 用一张图来表示即为(以 2-15 为例):

 ![tree](tree.png)

每一次的输出正好构成一颗二叉树, 且是一个二叉堆.

简单说明下: 8 恰好是 2 和 15 的中位数, 然后 5 是 2 和 8 的中位数, 11 是 8 与 15 的中位数. 以此类推.

那么我们的算法应该最终输出: [8 5 11 3 6 9 13 4 7 10 12 14].

下面是我简单考虑后写出的一种实现, 基本满足了要求.

```cpp
#include <vector>
#include <queue>
#include <iostream>

std::vector<int> GetMedianList(int min, int max)
{
    if (min > max)
        return {};

    std::vector<int> vecRst;
    std::queue<std::pair<int, int>> q;
    q.push(std::make_pair(min, max));
    do
    {
        auto range = q.front(); q.pop();
        int mid = (range.first + range.second) / 2;
        if (mid != range.first)
            vecRst.push_back(mid);
        else
            continue;
        q.push(std::make_pair(range.first, mid));
        q.push(std::make_pair(mid, range.second));
    } while (!q.empty());

    return vecRst;
}

int main()
{
    for (int i : GetMedianList(2, 15))
    {
        std::cout << i << " ";
    }
    std::cout << std::endl;
}
```

明眼人一看便知, 这是借用了 BFS 的思路, 借助队列实现的.