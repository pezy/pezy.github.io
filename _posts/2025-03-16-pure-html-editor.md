---
layout: post
title: "一个最简化的 HTML 编辑器"
tags: [note, tools]
---

> <https://keepworking.github.io/nash/>

关注过我早年兴趣的人知道，我太喜欢研究各种形式的笔记软件了，没想到 2025 年还能看到令人耳目一新的形式，阐释着何谓大道至简。

<!--more-->

我觉得这个工具的形态曾经一度是我苦苦追寻的，但随着 Markdown 逐步成为主流，我似乎也落入了 Markdown 形式主义的藩篱中。

即使是现在，我们在自己信息茧房里坚持认为谁都会 Markdown 格式，这是不成文的规定。但其实，大部分应该还是未能掌握或真正深入接触过 Markdown。

它远远不如 Word 文档对大家的影响之深。而 Web 时代，HTML 是背后的唯一载体，是稍微深入接触一下 web 的人就能捅到的窗户纸。—— 毕竟谁不会 Ctrl + S 一下当前浏览的网页呢？

但你要真的去研究 HTML 里的内容（各种标签），以及无孔不入的 CSS + JS，就开始存在门槛了。

所以我们才会乐于接受 Markdown 这样一种中间格式，它完美的承接了技术者对于格式化文本的习惯，同时又有效避免了过度复杂。从而让我们的问题点，马上转移到 Markdown <-> HTML 的互转上。大概在 10 年前，我在 Github 上最为关注的就是这类工具或库。

似乎忘记了最初，我们的目标仅仅是“封装 HTML 的复杂性”，而 Markdown 只是其中公认主流的一种方式。而更为主流的一种方式：GUI，似乎象征着过时与保守。

当时很接近的一个笔记工具叫做：writings.io，它的目标就是通过 GUI 的方式，提供一个简化版的 Word 界面，让你直接在 Web 上编辑，Web 上预览与发布。

但它的技术实现很传统，还是存在后端的，没有脱离 PHP + 数据库那一套。

我当时的技术栈，对前端有一些了解，但对于后端，除了 Java + Ruby 之外，是基本没有概念的。然而 Ruby 也没学到家，Java 又始终觉得很繁复。最终就对后端是相当排斥的。总觉得从架构上，为什么一定要存在后端？后端可以不可以无限做薄呢？最理想状态下，就是借助浏览器，把这件事完全在前端的范畴里做了。

很开心，终于看到了这样形态的工具，由衷的高兴。我会介绍给身边不那么技术的朋友，让他们可以自由发布一个 web page，且完全本地化。—— 这个年代，本地化是一种潮流。

不客气的 fork 了它，也可以在自己的域名下访问了:

> <https://dtun.org/writings/>

----

如果对比其 readonly 版本与 editor 版本，会发现，其实它利用了 HTML 的 contenteditable 属性，来实现了一个非常简单的编辑器。

```html
<div contenteditable="false">
    <p>Hello, world!</p>
</div>

<div contenteditable="true">
    <p>Hello, world!</p>
</div>

<div contenteditable="plaintext-only">
    <p>Hello, world!</p>
</div>
```

工具栏的隐藏也非常简洁:

```html
<div style="display: none;">
    <button>Bold</button>
    <button>Italic</button>
    <button>Underline</button>
</div>
```

----

还看到很多网友有一些尝试，比如:

- <https://github.com/rcarmo/onepage-by-spec>
- <https://github.com/Webstrates/MyWebstrates>

值得一提的，还有一些前辈工具，比如 TiddlyWiki，但我始终接受不了它的 UI Style（原谅我，已经被极简影响的病入膏肓了），以及受其影响的一派工具:

- <https://feather.wiki>
