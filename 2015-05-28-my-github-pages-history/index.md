---
title: 我的 Github Pages 折腾史
date: 2015-05-28
tags: fontend
path: /my-github-pages-history/
---

[Github Pages](https://pages.github.com/) 绝对是 Github 的一份大礼，我在 2012 年接触到它以后，便一直爱不释手、欲罢不能。最早我用 [Octopress](http://octopress.org/) 来搭建我的博客，当时我也同时在学 Linux，这种基于 Git，类似提交代码的方式写博客，相比传统的博客平台（如新浪博客、CSDN、博客园之类），的确带给了我完全不同的新奇感。直到我因为工作原因，将主战场重新迁移到Windows 上，才发现 Ruby 的环境在 Windows 上搭建起来有多么麻烦，甚至需要写一篇[文章](http://blog.csdn.net/hankai1024/article/details/12787505)来记录这些步骤，然而这还并不包括我遇到并费劲解决的各种报错场景。后来 `node.js` 开始流行，我跟风学了一阵子，于是开始关注基于 `node.js` 的博客框架，诸如 [`hexo`](https://hexo.io/)，[`ghost`](https://ghost.org/)。但很可惜，我并未尝试这俩个比较有名的框架，而是选择了一个比较小众的：[`nico`](http://lab.lepture.com/nico/)，当时选它的原因很简单，它拥有 `node.js` 系的普遍优点——不折腾。`npm` 下载、更新、安装都无比方便。最让我欣慰的是，对于当时还对英语有所恐惧的我来说，它是唯一一个提供了中文文档的框架，真是泪流满面呀。另外，`nico` 的默认主题我超级喜欢，直到现在，都影响着我对于前端页面的审美。

虽然经历了上述这一系列折腾，我本质上还是停留在选择趁手工具的层次。但用了很长一段时间 `nico` 之后，我开始产生自己设计主题的兴趣了，于是开始研究 `nico` 的源码（当然这也得益于它完善的文档）。才逐渐搞清楚这类工具的原理，其实主要就是将 `Markdown` 文法 parse 成为 `HTML`(推荐工具：[marked](https://github.com/chjj/marked))，然后将 HTML 页面组织好通过 Github Pages 发布。

也就是说，如果你对 HTML 足够熟悉，你完全可以直接写 HTML，然后上传到 `gh-pages` branch 里，来发布自己的博客。这样可以完全不依赖工具和框架。但我估计应该没人会这么做，首先遍布标签的 HTML 不是谁都能在里面愉快的输出文字的，其次你可能需要粘贴复制大量重复的东西，如头部内容、logo、css 等。人总是懒的，程序员尤其是，这才有了 Markdown 这样与 HTML 一一映射，却更加人性化的格式。

上述这种种搞法，其实还是受到了 Github Pages 的制约，得是静态页面呀，你想加个评论功能，就得依赖各种插件了（如坑爹的多说，与经常被墙的 Disqus）。更惨的是，你无论写什么，都得基于 Git 这一套，先在本地写好，然后 parse 成 HTML，甚至还要习惯性的先预览一番，最后才心满意足的提交到 repo。

这就会衍生出很多工具链，如 Markdown 编辑器哪家强啊？Git 的命令熟不熟呀？那套静态博客生成工具好呀？等等一系列选择性困难的问题。等到你折腾好了这些东西，写作的热情早已雨打风吹去了。

纯粹的写作应该还是要结合最早那些 blog 平台的做法，提供一个 Web 的后台编辑器，如曾经惊艳一时的 [`writings.io`](http://chloerei.com/writings/)，以及后来收费的 [Logdown](http://logdown.com/) 都是这个思路，操作更傻瓜的简书，已然完全是一个使用 Markdown 语法的传统博客平台了。

这种后台编辑的功能不能少，但也不能限制于此，有时候提交代码，顺便提交下博客也是可以的，另外，有些文章（不是我这种扯淡式的），是需要好几天查资料、编辑、画图、分析而成的，现在离线编辑的需求也是很强的。如何在方便与专业间取舍是个大问题。

但提供后台编辑的，几乎没有是基于 Github Pages 的，一提后台，基本都是 PHP 和 Ruby On Rails 的填写，一个静态页面展示平台咋可能有这样的功能呢？

这也是我一直捉摸的事，能不能利用 Github 的 API 来做一个后台编辑的功能呢？其实已经有很多实现的案例：如 [prose.io](http://prose.io/)，咱们国人弄的 [simple](https://github.com/isnowfy/simple)，都很好的表达了这个理念。这基本就把 Github Pages 动态化了，我可以仅仅在 Web 页面上编辑 Markdown 就能发布或者修改博客。省了不少事情。

顺着这个思路下去，Github Pages 能干的事情就多了，如你可以做一个简单的 Web 应用：To-do List 之类，甚至以前必须要动态网页做的事情你都可以尝试。

我是个前端菜鸟，也试了两个方面的应用：

1. 基于豆瓣读书的[读书笔记](http://pezy.github.io/reading)页面。
1. 一个类似 [豆列](http://www.douban.com/doulist/) 和 [轻单](https://qdan.me/) 的网络资源聚合器：[List](http://pezy.github.io/list)。（类似 Hacker News 和 浏览器书签）
1. 一个基于日历的[每日笔记](http://pezy.github.io/diary/)。（这个是 fork 自 [@joyeecheung](https://github.com/joyeecheung) 的 [diary](https://github.com/joyeecheung/diary)）

我还计划做一个社交网络聚合器，把你感兴趣的人的所有社交动态都集中到一个页面。他可以在知乎，在微博，甚至在人人，无论他在那个平台活跃，只要你关注了他，那么他的所有动态都会集中到这个页面上，与其他你关注的人，按时间顺序混搭。

这个思路也有很多类似的产品，但我还没有发现特别满意的。

我在这方面也就是瞎折腾，如果有专业人士可以指点一二，在下不胜感激。

>本文由 [pezy](https://github.com/pezy/) 创作，采用[知识共享署名 3.0 中国大陆许可协议](http://creativecommons.org/licenses/by/3.0/cn/)进行许可。可自由转载、引用，但需署名作者且注明文章出处。(Writing by [Typora](https://www.typora.io/))