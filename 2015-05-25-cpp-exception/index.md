---
title: 关于 C++ 的异常
date: 2015-05-25
tags: cpp
path: /cpp-exception/
---

大早起来看见知乎上关于 C++ 异常处理的[讨论](http://www.zhihu.com/question/22889420)。立刻让我想起前几天和 [@王月](https://github.com/Mooophy) 关于 `std::vector` 越界访问是否应该判断的讨论。

当时王月认为不需要管它，因为在 C++ 里，使用 `operator[]` 去访问 `std::vector` 中元素时越界，是 UB 行为。同理，在自己封装的类里，这样类似的调用，如果调用者传入越界的 index, 也应该属于 UB。而我认为，UB 是语言层面的事情，如果是自己封装类，则需要考虑做判断，适时的抛出异常。

我们当时并未针对异常展开探讨，而是将话题转移到软件工程上去了（我的思维跳跃了）。然而今天看到知乎上关于异常的讨论，我立刻想到那个场景，又好好想了下应该采取的策略。

其实 STL 是给我们做了示范的：

对于 `std::vector` 而言，[`operator[]`](http://en.cppreference.com/w/cpp/container/vector/operator_at) 不负责检查边界，而 [`at()`](http://en.cppreference.com/w/cpp/container/vector/at) 却负责检查，越界时会抛出 `std::out_of_range` 的标准异常。

这一点细节将 C++ 的灵活体现的淋漓尽致。对于程序员能确保不越界，且需要极致的性能的地方，应该考虑使用 `operator[]` 的方式；不敢确保，属于暴露给上层的接口，且对于性能没有过分的苛求，则应该考虑使用 `at()`。

STL 的 `at()` 是如何实现的？

- Visual Studio 2013:

```cpp
reference at(size_type _Pos)
{ // subscript mutable sequence with checking
    if (size() <= _Pos)
    _Xran(); // _Xout_of_range("invalid vector<T> subscript");
    return (*(this->_Myfirst + _Pos));
}
```

- GCC 4.9.1:

```cpp
const_reference
at(size_type __n) const
{
    _M_range_check(__n);
    return (*this)[__n];
}

void
_M_range_check(size_type __n) const
{
if (__n >= this->size())
    __throw_out_of_range_fmt(__N("vector::_M_range_check: __n "
    "(which is %zu) >= this->size() "
    "(which is %zu)"),
    __n, this->size());
}
```

虽然实现细节略有不同，但这样的处理手法值得借鉴。另外，`size_type` 是 `unsigned int` 类型，从编译期杜绝了传入参数为负数的可能。

所以，我的看法是，不应该畏惧使用异常，着重注意以下几个方面：

- C++11 多考虑可以使用 `noexcept` 关键字，避免歧义。
- 对于无法恢复（严重事故）的现场，果断使用 [dynamic exception specification](http://en.cppreference.com/w/cpp/language/except_spec)
- 区分异常的层次：
  - none: 这个级别应该避免，不处理是不负责，代码可能在任何地方莫名 crash, 可能还会造成内存泄露。
  - basic: 这是级别应该是下限，要保证，如果抛出异常，无内存泄漏，所有对象保持完整。
  - strong: 这个级别的意思是：保证执行要么成功，要么抛出异常，且抛出异常时，能够保证数据状态的一致，并回退到执行前。
  - nothrow/nofail: 执行必须成功。
- 要保证 basic 层次，我觉得应该避免 raw pointer 的出现，坚持使用 smart pointer.
- strong 层次的异常要复杂一些，但如果你的代码一贯坚持 RAII，确保这个层次也并非难事。
- nothrow/nofail 的层次，在于你对代码逻辑的理解。如析构，就要坚决使用 `noexcept`.

最后，提供一些更深入的资料：

- [Exceptions and Error Handling](https://isocpp.org/wiki/faq/exceptions#how-exceptions)
- [C++: do you (really) write exception safe code?](http://stackoverflow.com/questions/1853243/c-do-you-really-write-exception-safe-code)
- [Lessons Learned from Specifying Exception-Safety for the C++ Standard Library](http://www.boost.org/community/exception_safety.html)

> 本文由 [pezy](https://github.com/pezy/) 创作，采用[知识共享署名 3.0 中国大陆许可协议](http://creativecommons.org/licenses/by/3.0/cn/)进行许可。可自由转载、引用，但需署名作者且注明文章出处。