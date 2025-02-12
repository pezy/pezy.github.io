---
layout: post
title: "Do-nothing 脚本"
tags: [do-nothing, script, automation]
---

> <https://blog.danslimmon.com/2019/07/15/do-nothing-scripting-the-key-to-gradual-automation/>

这篇 2019 年的文章在 Hacker News 上被讨论过三次，可见这种向流程自动化卖出的一小步，是值得去实践的。

<!--more-->

三次讨论分别是:

- <https://news.ycombinator.com/item?id=42976698> - today
- <https://news.ycombinator.com/item?id=29083367> - 3 years ago
- <https://news.ycombinator.com/item?id=20495739> - 6 years ago

今年加入了一个初创团队，体会尤其明显。很多流程，暂时还无法完全自动化，或者也暂时没有自动化的必要（一次性行为）。但未来很难预测，已经遇到很多次，本以为的一次性行为，后面却出现了反复。

所以，看到这篇旧文，我觉得这种脚本化的习惯，值得养成。

不过这篇文章里的代码，已经遭到多次吐槽，作者感觉是从 OOP 语言过来的人，竟然隐隐还看到了某种设计模式的影子 —— [模版方法](https://segmentfault.com/a/1190000010706695#item-4-25)？

和大家一样，我还是忍不住改为了函数式的例子（并增加一些必要的美化）：

```python
from rich.console import Console
from rich.prompt import Prompt
import sys

console = Console()

def wait_for_enter() -> None:
    try:
        Prompt.ask("[cyan]完成后请按 Enter 继续 (Ctrl+C 退出)[/cyan]")
    except KeyboardInterrupt:
        console.print("\n[yellow]用户中断进程[/yellow]")
        sys.exit(0)

def create_ssh_keypair(context: dict) -> None:
    console.print("[green]运行:[/green]")
    console.print(f"   ssh-keygen -t rsa -f ~/{context['username']}")
    wait_for_enter()

def commit_to_git(context: dict) -> None:
    console.print("[green]将 ~/new_key.pub 复制到 `user_keys` 仓库，然后运行:[/green]")
    console.print(f"   git commit {context['username']}")
    console.print("   git push")
    wait_for_enter()

def wait_for_build(context: dict) -> None:
    build_url = "http://example.com/builds/user_keys"
    console.print(f"[green]等待构建任务完成: {build_url}[/green]")
    wait_for_enter()

def get_user_email(context: dict) -> None:
    dir_url = "http://example.com/directory"
    console.print(f"[green]访问 {dir_url}[/green]")
    console.print(f"查找用户 `{context['username']}` 的邮箱地址")
    context["email"] = Prompt.ask("请粘贴邮箱地址")

def send_private_key(context: dict) -> None:
    console.print("[green]前往 1Password[/green]")
    console.print("将 ~/new_key 的内容粘贴到新文档中")
    console.print(f"将文档分享给 {context['email']}")
    wait_for_enter()

def main() -> None:
    if len(sys.argv) != 2:
        console.print("[red]错误：请提供用户名作为参数[/red]")
        sys.exit(1)

    context = {"username": sys.argv[1]}
    steps = [
        (create_ssh_keypair, "创建 SSH 密钥对"),
        (commit_to_git, "提交 SSH 密钥到仓库"),
        (wait_for_build, "等待构建任务"),
        (get_user_email, "获取用户邮箱"),
        (send_private_key, "发送私钥"),
    ]

    console.print("[bold blue]本工具将引导您完成所有步骤。[/bold blue]")
    console.print("[blue]随时可以按 Ctrl+C 退出。[/blue]\n")

    try:
        for i, (step_func, description) in enumerate(steps, 1):
            console.print(f"\n[bold blue]步骤 {i}/{len(steps)}: {description}[/bold blue]")
            step_func(context)

        console.print("[bold green]所有步骤已完成！✨[/bold green]")
    except KeyboardInterrupt:
        console.print("\n[yellow]用户中断进程[/yellow]")
        sys.exit(0)

if __name__ == "__main__":
    main()
```

效果如下:

![image](https://github.com/user-attachments/assets/00dd6ded-2651-48b6-bd3b-cf2cd4367baa)

当然，如何实现并不重要，关键是这些步骤不仅仅只是文档记录（甚至我认为可以没有文档），更应该直接写成脚本。

# Invoke 库可能更适合做这个任务

有老哥分享了 [Invoke 库](https://www.pyinvoke.org/index.html)，我尝试了下，的确会更适合明确流程的依赖顺序：

```python
from invoke import task, Config

config = Config()
config.run = {'username': None, 'email': None}


@task
def init(ctx):
    username = input("请输入用户名: ")
    ctx.config.run.username = username
    print(f"[init] 用户名设置为: {username}", flush=True)


@task(pre=[init])
def create_ssh_keypair(ctx):
    username = ctx.config.run.username
    print(f"运行: ssh-keygen -t rsa -f ~/{username}")
    input("按回车键继续: ")


@task(pre=[create_ssh_keypair])
def git_commit(ctx):
    username = ctx.config.run.username
    print(f"将 ~/new_key.pub 复制到 `user_keys` Git 仓库，然后运行:")
    print(f"    git commit {username}")
    print("    git push")
    input("按回车键继续: ")


@task(pre=[git_commit])
def wait_for_build(ctx):
    build_url = "http://example.com/builds/user_keys"
    print(f"等待构建任务完成，构建地址: {build_url}")
    input("按回车键继续: ")


@task(pre=[wait_for_build])
def retrieve_user_email(ctx):
    username = ctx.config.run.username
    dir_url = "http://example.com/directory"
    print(f"访问 {dir_url}")
    print(f"查找用户 `{username}` 的邮箱地址")
    email = input("请粘贴邮箱地址: ")
    ctx.config.run.email = email


@task(pre=[retrieve_user_email])
def send_private_key(ctx):
    print("打开 1Password")
    print("将 ~/new_key 的内容粘贴到新文档中")
    print(f"与 {ctx.config.run.email} 共享该文档")
    input("按回车键继续: ")


@task(pre=[send_private_key])
def provision_user(ctx):
    print("所有步骤已完成。")
```

这段代码非常符合我的审美需求，首先明确流程依赖的配置输入，然后以函数的方式依此往下写伪逻辑。后续有时间，直接在函数内部改为自动化执行的脚本，会非常容易。

# 延伸: Just

上面的脚本化（关键是定义了顺序）如果算是一步，那么可能写一个 makefile 记录常用的一些指令，算是半步？

我倾向于反复执行的命令行，统一收归到 makefile 里去，这样每次只需要 `make xxx` 即可。—— 这就好比在 `~/.ssh/config` 里给每个远程机器起一个好记的别名。下次可以直接 `ssh xxx` 一样。

而讨论里也有人提到一个更现代的 makefile 工具: [just](https://just.systems/man/zh/安装包.html)，我觉得一定值得安装尝试一下。

当然，makefile 永远比它强的地方，可能就是不用安装就可以用。

同样的流程，我们如果对 shell 足够熟悉（AI 加持下完全不是问题），可以写一个 `justfile` 来做这件事:

```makefile
default: provision-user

init:
    #!/usr/bin/env sh
    printf "请输入用户名: "
    read username
    echo "[init] 用户名设置为: $username"
    echo $username > .username

create-ssh-keypair: init
    #!/usr/bin/env sh
    username=$(cat .username)
    echo "运行: ssh-keygen -t rsa -f ~/$username"
    read -p "按回车键继续: "

git-commit: create-ssh-keypair
    #!/usr/bin/env sh
    username=$(cat .username)
    echo "将 ~/new_key.pub 复制到 'user_keys' Git 仓库，然后运行:"
    echo "    git commit $username"
    echo "    git push"
    read -p "按回车键继续: "

wait-for-build: git-commit
    #!/usr/bin/env sh
    echo "等待构建任务完成，构建地址: http://example.com/builds/user_keys"
    read -p "按回车键继续: "

retrieve-user-email: wait-for-build
    #!/usr/bin/env sh
    username=$(cat .username)
    echo "访问 http://example.com/directory"
    echo "查找用户 '$username' 的邮箱地址"
    printf "请粘贴邮箱地址: "
    read email
    echo $email > .email

send-private-key: retrieve-user-email
    #!/usr/bin/env sh
    email=$(cat .email)
    echo "打开 1Password"
    echo "将 ~/new_key 的内容粘贴到新文档中"
    echo "与 $email 共享该文档"
    read -p "按回车键继续: "

provision-user: send-private-key
    echo "所有步骤已完成。"
    rm -f .username .email
```

这样你直接输入 `just` 就可以跑完整个流程。