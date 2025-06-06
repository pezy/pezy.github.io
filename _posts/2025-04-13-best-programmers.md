---
layout: post
title: "最好的程序员"
tags: [coding, programmers]
---

> <https://endler.dev/2025/best-programmers/>

你认识最棒的程序员都有哪些品质和共同点？

<!--more-->

我觉得这篇文章本身已经总结的很棒，我想再咀嚼一下，做一个汇总记录。

# 直接阅读**官方文档**

这一点非常重要，我最近实际遇到的例子就是 `clangd` 的用法，网上充斥着写 `.clangd` 配置文件的文章，但却从不解释跨平台编译时如何处理。而读了[官方文档](https://clangd.llvm.org/config#files)之后，非常明确，根本无需在 repo 里维护 `.clangd` 文件，而是各自在各自账户下维护一个 `config.yaml` 即可。

以及现在有 MCP 的加持，例如 cursor 里的 [Docs 功能](https://docs.cursor.com/context/@-symbols/@-docs)，足以让大模型减少很多幻觉，节省时间。

# 必先利其**器**

其实我感觉不同方向的程序员，某种程度上说只是掌握的工具不同。

如果你在 Windows 上写 C/C++，那么你必须得花点时间深入理解 Visual Studio 的各种功能。

如果你是一个前端开发，那么 Chrome DevTools 的犄角旮旯的功能点最好全部摸一遍，理解透彻其用处。

就我的经历而言，这个节省的开发调试时间不是一点半点。

# 仔细阅读**错误信息**

从最早一出报错就挠头尖叫，到后来无脑粘贴搜索，以及现在直接扔给大模型解决了事。（应该每一个程序员都有这一步吧？）

其实最高效的方式，反而是静下心仔细阅读一下报错信息，然后思考与理解问题可能出在的地方。这个有时候真的要比你像无头苍蝇一样到处询问要有效的多。

# **分解**问题

> <https://rkoutnik.com/2016/04/21/implementers-solvers-and-finders.html>

有一个说法是，程序员的发展路径往往是：Implementers -> Solvers -> Finders。

解决问题的前提就是能够分解问题，而极其善于分解问题的人往往也就会成为最早发现问题的人。

[四千周](https://book.douban.com/subject/36093214/)里提出**耐心的三大原则**，其中第一条就是让自己喜欢上问题。而没有任何人会直接喜欢复杂的问题，所以我们才需要分解他们，逐步喜欢上解决一个个简单的问题。

# 绝知此事要**躬行**

程序员本质上是个手艺活，不动手基本等于没有。不害怕问题的人一定是已经碰过这个问题的人。多尝试，多开 repo，多构建。逐步就会有迎刃而解的快感。

# **助人**为乐

程序员的快乐源泉往往就是来源于帮助他人解决问题。那么多开源项目的初心可能就是如此。

若你练就一身屠龙技艺，却拒绝与龙相遇，岂不是竹篮打水，抑或是叶公好龙？别人的麻烦与需求，往往就是你磨练技艺的机会。

# 不动**笔墨**不编程

写代码与写文章本质上是一回事（代码本就是给人看的，机器并不 care）。自然语言的风格往往也与代码风格紧密相关。尤其是大模型时代，写出来代码的质量，可能就是你给大模型的 prompt 决定的。

说好人话，可能是最被低估的能力之一。

# **学**不可以已

当一个人认为自己掌握了全部真理，或者丢掉了好奇心。那么我觉得那将是一个"静态"的人，精神将会在那一刻停止。

如果你依然保持谦逊，并每天充满问题，那么**学**将是非常自然的事情。

但正如原文里关联的那样，盲目跟随潮流，可能也是某种形式上的自满与懒惰。自满在于认定自己已经把握住了“潮流”，永远站在时尚最前沿。懒惰在于潮流来时放弃了质疑的机会，好奇不仅仅是想知其然，更是想知其所以然。

# 一视**同仁**

程序员在精神上永远年轻，那么心里就不该存在阶级与地位的偏见。首席工程师与初级开发人员，都有值得学习的地方 —— 可以是多年的经验之谈，也可以是初生牛犊的尖锐质疑。而成见，是借鉴与反思的最大障碍。

# 得**道**者多助

儒家的"得道"某种程度上就是一种影响力的获得。不仅程序员，我觉得每个人终其一生，构建影响力都是一件非常有意义的事情。《人类简史》里提到，智人之所以能够成为地球的霸主，就是因为能够构建出虚构的故事，并让虚构的故事成为集体的信仰。故事、信仰、到文化、文明。本质上都是影响力的构建级别。

我们总能听说一些伟大程序员的传奇故事，我还依稀记得阅读[《Doom 启示录》](https://book.douban.com/subject/1152971/)的震撼。声誉在历史中沉淀成文化，依然会启发后人。而具体的代码，可能早已湮没，无人问津。

# 别无他长，但能**耐烦**

成熟的政治家，顶级的管理者，往往都是最有耐心的人。程序员也不例外，除了对人，对计算机也应该报以足够的耐心。

真正有价值的事情，很少会一蹴而就，或者一帆风顺。

你手头的工具可能会陪伴你相当长的时间，沉心专注，耐心打磨，方成利器。

# **不怨**天**不尤**人

不稳定、随机出现、无法复现，应该是程序员最头疼的词汇。

但无论多么诡异，你得相信它绝非偶然，且必定存在一个合乎逻辑的解释。如果有机会，深挖下去，可能收获满满。

如果你开始相信魔法，那么魔法将无处不在。

# **不知**为不知

不知，是对自己的坦诚，也是**学**的开始。

# **慎**思**明**辨

不要靠猜，猜是轻易的事情。也是迷雾的开始。别放弃问题的追寻，操起工具，彻底深挖，方为正道。

# 大道至**简**

KISS 是程序员的明灯，也是一种智慧。

> Brevity Can Be a Virtue

这一点我觉得不用多说。
