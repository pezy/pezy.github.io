## SVN 常用命令小记

最近经常需要在 Linux 下用 SVN 维护代码, 简单记录几个常用的命令:

(commit, update, add 之类, 就不列出了)

### 增加忽略文件

编辑一个 txt 作为忽略文件的记录, 然后执行 `propset` 对当前目录进行设置:

```sh
svn propset svn:ignore ignore.txt .
```

千万注意最后一个点.

查看包含 ignore 的状态列表([I]代表 ignore 的文件):

```sh
svn status --no-ignore
```

### 解决冲突

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