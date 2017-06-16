---
title: Building OSG(OpenSceneGraph) at CentOS7
date: 2016-07-19
tags: osg
path: /building-osg-in-centos/
---

Prepare dependence environment

```sh
yum install libpng-devel
yum install libjpeg-devel
yum install libtiff-dev
yum install libungif-devel
yum install freetype-devel
yum install gdal-devel
yum install libcurl-devel
yum install SDL-devel
yum install libgta-devel
yum install gstreamer-devel
yum install SDL2-devel
yum install jasper-devel
yum install gstreamer1-plugins-base-devel
yum install poppler-glib-devel
yum install librsvg2-devel
yum install gtkglext-devel
```

Download the source and build

```sh
git clone https://github.com/openscenegraph/OpenSceneGraph.git
cd OpenSceneGraph
cmake . -DCMAKE_BUILD_TYPE=Release
make -j 8
make install
```

PS: `DCMAKE_BUILD_TYPE` is optional, and also can be set to `Debug`.

`make -j 8` mean use 8 core CPU to compile and link.