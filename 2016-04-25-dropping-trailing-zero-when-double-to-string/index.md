---
title: 浮点数转字符串后, 去掉多余的零
date: 2016-04-25
tags: gist
path: /dropping-trailing-zero-when-double-to-string/
---

当数据用于显示的时候, 经常会遇到这样类似的需求. 今天先用 C++ 给一个简单的实现:

```cpp
std::string trimToString(double dValue)
{
    std::string strRst = std::to_string(dValue);
    while (strRst.back() == '0') strRst.pop_back();
    if (strRst.back() == '.') strRst.pop_back();
    return strRst;
}
```

考虑了三种情况的覆盖, 详细测试代码请见[这里](https://gist.github.com/pezy/33b6fb351cae8b8b820162a4e54ef0bf).

欢迎补充.