---
title: OSG(OpenSceneGraph) 的场景截图功能
date: 2016-05-13
tags: osg
path: /screenshot-in-osg/
---

三维场景, 往往有截图保存的需求. 而 OSG 中也提供了 `ScreenCaptureHandler` 类来干这件事. 但美中不足的是, 该类构造函数虽然提供了 `CaptureOperation` 与 `numFrames` 两个参数, 但却没有接口来设置截图的方式. 只能接受预定的按键(`c`/`C`)截图.

我想实现的, 是通过界面 tool bar button 来激发截图过程. 那么只好自己来实现一个源码以及 Example 中类似的类: `WindowCaptureCallback` 了.

实际上, 直接把 `WindowCaptureCallback` 直接拿过来即可. 该类代码足够简单.

但如果按照 Example 中的示例代码调用:

```c++
// Add the WindowCaptureCallback now that we have full resolution
GLenum buffer = viewer.getCamera()->getGraphicsContext()->getTraits()->doubleBuffer ? GL_BACK : GL_FRONT;
viewer.getCamera()->setFinalDrawCallback(new WindowCaptureCallback(buffer, fileName));
```

会默认**让保存的截图随着场景的变化实时改变**.

可这样显然不符合常规的静态截图条件, 我们只需要截取当前 frame 的图片.

所以我们在上述调用的基础上, 稍微增加两行代码, 来满足需求:

```cpp
// Do rendering with capture callback
m_pOSGWidget->renderingTraversals();
// Restore to a null callback.
m_pOSGWidget->getCamera()->setFinalDrawCallback(0);
```

Done.