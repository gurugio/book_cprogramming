# Professional C programming : Framework and plugin design in C

I first met the C language 20 years ago. I have been working with C language for 12 years.
I have read several C programming books but the purpose of them is teaching how to start C programming.
There is no book about how to do C programming practically for real product.
Fortunately I have learnt good C programming skill when working for companies having good C programmers and good development process.
And also I have seen how great C programmer develops large scale software from Linux kernel sources.

This document is some part of what I have learnt and seen.

For student, this document takes one or two weeks. Please focus on how the C basics, such as function pointer, pointer, macro and etc, are used in the real product.
If you are already professional C programmer, you could compare the source of this document and your product. I would be happy if you find out something to be applied to your product.

Please let me know if you have any tips for practical C programming skill. Please register your tips in the ticket of this repository. I will write a document for it andd add here with your name.

*I am not English native. Please inform me if you find any broken English.*

(1st version in Korean: Feb. 12 2019)
(start translation: Feb. 23 2019)

## INDEX

* [Error handling with goto](error_handle.md)
* [Make if-else short with branch table](long-if.md)
* [Make for-loop general](foreach.md)
* Implement framework and plugin
  * [Object programming in C: make cstring class with C](cstring.md)
  * [Introduction](interface.md)
  * [Implement unittest framework(TBD)_](unittest.md)
  * [Implement unittest for cstring(TBD)](unittest_cstring.md)
  * [Make test case simple(TBD)](unittest_cstring2.md)
* Sample project: serial number generator
  * [Initial version(TBD)](serial_step1.md)
  * [Implementation the framework and plugin for the serial number generator(TBD)](serial_step2.md)

## TODO
