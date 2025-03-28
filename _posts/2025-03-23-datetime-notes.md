---
layout: post
title: "每条笔记都有日期时间"
tags: [note, tools]
---

> <https://www.notetimeapp.com>

书接上回，最近 notes 类软件又有一款引起我注意的，它极简，但特点鲜明：每件事都有日期时间。

<!--more-->

我乍一看这个笔记，真的非常喜欢，无论是设计还是背后的理念。其实我刚工作不久，受到[《程序员的思维修炼》](https://book.douban.com/subject/5372651/)一书的影响，一直记录 dailynotes，最早我就是用 Windows 的记事本，F5 就会带上当前时间戳，我觉得非常神奇，也的确很方便。

后来当然又切换了很多笔记软件，但第一时间我总是想去研究如何能更快的插入时间，记录当下。

没想到真的有人就这一点做了这么一款应用，感觉超级幸运。

不过我也不再青葱，本地化是我对笔记应用的底线要求。这款 app 虽然喜欢，但我也只是把玩一二，应该不会实际使用了。

但我在想，其实这个理念如此简单，甚至可以简化成一个命令。

```sh
echo "$(date) xxxx" | tee -a dailynotes.md
```

那么我们可以在 `~/.bashrc` 或者 `~/.zshrc` 中添加这么一个函数：

```sh
note() {
    echo "$(date "+%Y-%m-%d %H:%M:%S") $*" | tee -a notes
}
```

这样你在终端里，敲入 `note 这是一条笔记`，就会自动记录成为一条 datetime 笔记了。

简洁，但有力。我会在 Obsidian 里专门建一个 DailyNotes.md，然后利用上述命令记录日常。

坚持记录，后面的事情，交给 AI 吧。

----

更新：

上述命令虽然简洁，但始终都是往后追加内容，但根据最近 karpathy 大神的分享:

> <https://karpathy.bearblog.dev/the-append-and-review-note/>

追加笔记最好是插入在头部，所以我们借助 `sed` 改进一版：

```sh
note() {
    sed -i '' '1i\
'"$(date "+%Y-%m-%d %H:%M:%S") $*"'
' notes
}
```

这样就更加完美结合了日期时间笔记与追加回顾笔记的双重理念了。
