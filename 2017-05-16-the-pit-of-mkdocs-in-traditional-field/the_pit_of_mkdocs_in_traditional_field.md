---
title: Mkdocs 在传统领域遇到的坑
date: 2017-05-16
tags: devops
path: /the-pit-of-mkdocs-in-traditional-field/
---

如果在 Google 中输入 "project documentation markdown" 这几个关键字, 排在第一位的就是 MkDocs.

加之对此早有耳闻, 故产品发布前, 我向团队直接推荐了这个工具.

但在使用过程中, 还是不可避免的遇到了很多坑.

# 离线文档

也许 MkDocs 的初衷, 是部署在 github page 或 Amazon S3 的在线文档. 本质上就是一个托管文档的网站. 这样的好处是明显的, 只要打开浏览器, 连上互联网, 就可以随时看到. 如果是开源软件, 则更有利于开发者贡献或改进(类似 wiki).

然而, 对传统企业的产品手册而言, 要求在不联网的时候, 依然可以用浏览器查看.

对此, 有两种解决方案:

1. 在产品启动时, 同时开启一个小的 http server, 然后打开文档时, 浏览器的地址栏将会是 `localhost:8000` 这样的. 也就相当于在线网站了. 经过一番调研, 比较轻巧的 http server 基本都是 nodejs 或 python 的. 对于一款纯 C++ (Qt) 的产品而言, 深度集成一个小巧强悍的 http server 还是比较麻烦的, 短时间内为此写很多代码, 也很不划算. 故这个方案暂时搁浅.
1. 将生成的 HTML 文件直接打包发布. 用户只不过是用浏览器来查看 HTML 而已. 这样不涉及任何通讯, 节省不少工作量. 但会丧失很多有 server 而具备的功能, 如 search 等.

MkDocs 默认是推荐第一种方案的, 以至于其访问路径, 都是指向目录, 各级目录下都有一个 index.html, 但不会在路径中出现. 如果采用第二种方案的话, 就会出现跳转链接时, 找不到文件的情况. 于是需要在配置文件中增加这么一项:

```yml
use_directory_urls: false
```

# MathJax

有了离线文档这个需求, 就给后续很多事情带来了麻烦. 譬如文档中涉及公示渲染, 会用到大名鼎鼎的 MathJax(Latex 语法). 如果是在线文档, 这很容易:

```yml
markdown_extensions:
    - mdx_math

extra_javascript:
    - //cdn.bootcss.com/mathjax/2.7.1/MathJax.js?config=TeX-AMS-MML_HTMLorMML
```

因为需要离线, 所以利用 cdn 来借用 JS 库的通用做法行不通了. 改为使用本地 JS. 但坑爹的是, 这里有个莫名其妙的 Bug:

如果使用 cdn 形式的字符串, 解析一切正常. 如果使用相对地址, 如:

```yml
extra_javascript:
    - MathJax/MathJax.js?config=TeX-AMS-MML_HTMLorMML
```

就压根找不到 JS 文件. 追查生成的网页以及 JS 代码才发现, `MathJax/MathJax.js?config=TeX-AMS-MML_HTMLorMML` 会被解析成: `MathJax/MathJax.js%3Fconfig%3DTeX-AMS-MML_HTMLorMML`, 也就是 `?` 与 `=` 会被解析为 `%3F` 与 `%3D` 等十六进制 ASCII 码. 很神奇的是, 如果是 cdn 形式, URL 便是正常的.

为了绕过这个 BUG, 只好又去翻 MathJax 的文档, 看能否不通过 URL 式的配置, 同样达到目的. 很快在找到了这么一段代码:

```html
<script type="text/x-mathjax-config">
    MathJax.Hub.Config({
    extensions: ["tex2jax.js"],
    jax: ["input/TeX", "output/HTML-CSS"],
    tex2jax: {
        inlineMath: [ ['$','$'], ["\\(","\\)"] ],
        displayMath: [ ['$$','$$'], ["\\[","\\]"] ],
        processEscapes: true
    },
    "HTML-CSS": { availableFonts: ["TeX"] }
    });
</script>
```

原来 MathJax 提供了两套配置方式, 一种就是通过传入 `config` 参数的方式渲染, 另一种就是在 `<head>` 标签中嵌入上面这段 JS 代码.

但我不能为每一个生成的 HTML 文件, 都手动加这段代码吧. 无奈之下只好通过修改 theme 的方式嵌入了. 下载 [MkDocs theme 的源码](https://github.com/mkdocs/mkdocs/tree/master/mkdocs/themes/mkdocs) 到本地, 可以考虑换个名字. 如 `my_theme`, 然后在配置文件中增加配置:

```yml
theme_dir: my_theme
```

这样一来, 就可以直接对 HTML 模版进行修改了. 在 my_theme 目录中找到 base.html, 然后大概在 `<script src="{{ base_url }}/js/highlight.pack.js"></script>` 下面增加上述 JS 代码即可.

# Markdown 的坑

责任得明确, 接下来遇到的这个问题, 与 Mkdocs 无关, 应该由 Markdown 来负责. 那就是指定图片大小的接口, 似乎在 Markdown 的语法中毫无体现. 王垠也在其[博客](http://www.yinwang.org/blog-cn/2013/04/14/markdown)中讨论过该事.

恰好我们产品文档中有这么一个需求: 在行文过程中, 插入一些 icon 来说明功能. 这个需求其实非常普遍, 在 Word 等工具中也非常好实现. 但 Markdown 却会将一切图片一视同仁, 当你用 `![]()` 的方式插入 icon 时, 行间距将会被拉大, 以至于整段文字非常难看.

还好 MkDocs 提供了 extra.css 的接口, 我可以很方便的自定义一些标签的 CSS. 例如针对上述问题, 我在 extra.css 中加入以下代码:

```css
img.icon {
  padding: 0px;
  border: 0px;
  margin: initial;
}
```

然后在 Markdown 中, 直接用 HTML 来插入 icon, 如 `<img src="../img/4-2.png" class="icon"/>`. 如此一来, 就变相的解决了这个麻烦.

当然, 这样也会给不熟悉前端技术的同事们带来一点麻烦, 因为他们通常会用 Typora 这类所见即所得的工具来撰写 Markdown, 遇到上述 HTML 代码, 编辑器是无法自动转换为图片显示的. 这绝对会逼死处女座的人...

说到这里, 也要提一句, 我以前觉得所见即所得的 Markdown 工具非常人性化, 是真正的 Markdown 编辑器应有的样子. 但在这次编写文档过程中, 我发现此类工具会给使用者造成相当大的误导. Markdown 无非只是对 HTML 语言的一种抽象, 而且它仅仅负责文字内容, 顶多负责一些列表, 表格, 标题, 加粗, 斜体, 链接, 图片等显示上的控制. 它是没办法精确控制外观的. 甚至连简单的列表(1, 2, 3...), 如果在中间不小心插入了空行, 列表就会另起炉灶的又从 1 开始. 如果你用的是所见即所得工具, 很有可能这个空行并非你手动插入的, 而是在你拖拽图片编辑时, 编辑器为你加入的... 于是, 就会出现这样一种景象: 同事说, 我这里显示的明明是这样子, 怎么 Macdocs 生成出来是那样的...

而且由于该类编辑器往往做的精细, 外观选取的 CSS 模版皆是精雕细琢. Mackdocs 相比之下, 就显得过于朴素了. 这样有意无意的对比, 也会让人搞不清重点, 又回到了 Word 的老路上.

毕竟要比所见即所得, 谁也拼不过 Word 不是?

反正我现在写 Markdown 都直接用 VSCode 了, 顺带一个 markdownlint 插件, 用来提示你不规范的写法. 就足够直观可控了.

# 跳转的坑

其实 Markdown 是提供跳转的, 我最早用到这个功能, 是在 Github 上, 写 README 或 wiki 的时候. 当时觉得很方便, 但在这次写文档时, 才觉得功能过于简陋了.

首先, Markdown 只提供了针对标题的跳转, 是在 URL 后面加上 `#HeaderName` 的方式. 但实际书写过程中, 我们经常需要跳转到某图片, 如写到"请见图12", 结果图12和目前正文隔得老远, 跳转一下很有必要. 在 Markdown 里做到此事就比较痛苦. 变通的方式是, 在图12的前面加一个不起眼的五级标题 `#####图12`, 然后在行文超链接里写 `[图12](#图12)`. 才能勉强实现需求.

其次, Markdown 的标题跳转没有特别严格的规定. 如在 Typora 中, 如果你的标题里有空格, 而在超链接的地方, 你把标题内容 copy 过来(含有空格), 也可以跳转. 但在 MkDocs 里, 这样的操作, 就无法跳转. 查看生成的 HTML 才得知, MkDocs 会将你的标题(如果是英文)先全部转小写, 然后将空格转为`-`. 这样你 copy 过来的内容就跟标题不一致了, 当然就无法实现跳转了...

这样的设计很无语, 因为写文档时, 我们无法像程序一样自动转换这个标题, 步骤通常是将标题 copy 到超链接处, 然后修改标题, 看到大写转小写, 看到空格转`-`. 不用几次, 就会觉得超级繁琐. 我当时无奈之下写了一个 python 脚本来自动转换. 但仍然省不了好几次的 copy-paste. 正确的做法, 应该是 MkDocs 扫描 Markdown 的时候, 遇到链接中的文字, 也进行一次自动转换, 让其与最终标题保持一致. 我想有时间, 我应该向 MkDocs 提这么一个 pull request...

---

上述几点, 是记忆深刻的. 也许还有一些细节让人并不满意, 但也无法逐一记录下来了. 开源工具日新月异, 层出不穷, 但能够真正投入日常工作的, 寥寥无几. 在调研的时候, 不能光看其文档之华丽, 还应该躬行一番, 验证一下, 以免后患无穷. 当然, 开源的要义在于 free(自由), 如果你对它不满, 你就应该改进它. 上述这些坑, 有些是因为离线文档这个古老的需求与互联网常见场景不匹配造成的, 但有些的确是 MkDocs 做的不够完美的地方. 上述"坑"只为与我类似的用户提个醒, 接下来还需努力尝试去修复它. 让这类工具可以真正运用在我们日常的开发过程中, 而不至于麻烦不断.