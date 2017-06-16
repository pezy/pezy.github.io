---
title: 比例尺的绘制思路
date: 2016-05-10
tags: dev
path: /scale-on-map/
---

一般来说, 地图上常见的比例尺有三种表现形式, 如下图所示:

 ![map_scale](map_scale.png)其中[^1]:

1. a) 是文字表示法, 即**直接用文字表述**. 如这里是地图(底图)上的 1 cm 代表实际距离的 250 m.
1. b) 是数值表示法, 即用**比值**来表示. 需要注意的是, 这种方式要确保`:`两侧的单位统一.
1. c) 是图示表示法, 即绘制一把示意性的尺子直观的表达地图(底图)上的长度, 并标注上实际距离.(一般会取整数刻度)

[^1]: https://docs.qgis.org/2.6/en/docs/gentle_gis_introduction/map_production.html