---
title: SVN 常用命令小记
date: 2016-07-15
tags: devops
path: /svn-usage/
---

最近经常需要在 Linux 下用 SVN 维护代码, 简单记录几个常用的命令:

(commit, update, add 之类, 就不列出了)

## 增加忽略文件

编辑一个 txt 作为忽略文件的记录, 然后执行 `propset` 对当前目录进行设置:

```sh
svn propset svn:ignore ignore.txt .
```

千万注意最后一个点.

还有一种更直接的方式编辑当前目录下的 ignore file:

```sh
svn propedit svn:ignore .
```

> 这里值得注意的是, 可能会出一个错误: `svn: E205007`. 需要在环境变量里设置一个 `SVN_EDITOR` , 譬如在 Linux 下我们常使用 vim 的话:
>
> ```sh
> vim ~/.bashrc
> export SVN_EDITOR=vim
> :wq
> source ~/.bashrc
> ```
>
> 在执行上述命令, 就会进入 vim 编辑 ignore 列表了.

## 解决冲突

如果以你本地修改为准:

```sh
svn resolve --accept=woking filepath
```

如果以服务器上的版本为准:

```sh
svn resolve --accept=theirs-full filepath
```

实践中, 貌似只有上面那个比较管用.

## 撤销添加状态

当用 `svn add` 命令增加了文件之后, 后悔想 `undo` 该如何做:

```sh
svn revert thefile
```

## 删除服务器上的文件

当有些 ignore file 被误上传上服务器, 该如何将其删除呢?

```sh
svn delete del_file --force
```