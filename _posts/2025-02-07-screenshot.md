---
layout: post
title: "浏览器自动截图工具"
tags: [automation, chrome, screenshot]
---

> <https://news.ycombinator.com/item?id=42965267>

这个帖子本来只是分享一个返回浏览器页面截图的服务 API，但下面的讨论很有意思

<!--more-->

# Chrome 自带命令实现网页截图

> <https://developer.chrome.com/blog/headless-chrome?hl=zh-cn#taking_screenshots>

Chrome 提供了无头浏览器的一些附加参数，可实现网页截图，譬如:

```sh
chrome --headless --disable-gpu --screenshot --window-size=1280,5000 "https://news.ycombinator.com/item?id=42965267"
```

不过对于 macos 用户，`chrome` 命令可能不是现成的，需要设置一下 alias:

```sh
alias chrome="/Applications/Google\ Chrome.app/Contents/MacOS/Google\ Chrome"
```

为了使用方便，我直接讲这个写入到 `~/.zprofile` 文件中。

这个方法简单有效，但有个问题，就是截图的尺寸是固定的，如果想要灵活控制截图的尺寸，则需要使用 `puppeteer` 来实现。

# 使用 puppeteer 实现灵活截图

比较坑的是，`npm install puppeteer` 会慢到让你怀疑人生。但其实我们所需的功能，只需要 `puppeteer-core` 即可。

```sh
mkdir screenshot-foo && cd screenshot-foo
npm init -y
npm install puppeteer-core
```

然后就可以愉快地写代码了：

```js
const puppeteer = require("puppeteer-core");
const url = process.argv[2] || "https://dtun.org";

(async () => {
  const browser = await puppeteer.launch({
    executablePath:
      "/Applications/Google Chrome.app/Contents/MacOS/Google Chrome",
  });
  const page = await browser.newPage();
  await page.goto(url);
  await page.screenshot({ path: "screenshot.png", fullPage: true });
  await browser.close();
})();
```

注意：

- 其中 `executablePath` 需要根据实际情况填写，macos 基本就是我写的这个。
- `path` 也可以根据自己的情况设置。
- 默认值我暂时写为本 blog 了。

然后就可以愉快地使用啦：

```sh
node screenshot.js https://news.ycombinator.com/item?id=42965267
```

它会完整的截取这篇帖子的全部内容。

# 帖子里提到的其他成熟工具

- <https://browserless.io> - low level browser control
- <https://scrapingbee.com> - scraping specialists
- <https://urlbox.com> - screenshot specialists
- <https://github.com/sensepost/gowitness> 一个 golang 实现的强大工具，除了截图还可以将一些元信息直接写入数据库
- <https://shot-scraper.datasette.io/en/stable/> 一个 python 实现的工具，一行命令整页截图

另外，还延伸到一些可视化的回归测试工具上：

- <https://github.com/topics/visual-regression> Github 的话题
- <https://testplane.io> 其中一个很直观易用的回归测试工具

以及，还有一些有趣的 app 也需要依赖上述讨论的网页截图：

- <https://www.getchangelog.com> 看起来很棒的 changelog 监测订阅工具
