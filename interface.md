# Implement framework and plugin

## Introduction

This chapter describe the concepts of framework, plugin and interface very roughly.
But it is not easy to describe the concepts in a short document.
It would be better to focus on the example sources.

Nevertheless let me tell you the brief concepts.
Interface is a calling convention or a calling policy between the framework and the plugin.
The cstring example is showing a example.

![interface and plug-in of cstring](/interface.png)

Let us skip the framework for a while.
Let us see the interface first.
As you see, the function types in cstring structure are interfaces.
* ``void (*clear)(struct cstring *)``
* ``void (*length)(struct cstring *)``

Interface is a kind of policy.
Plugin implements a real service according to the interface.
So the function implementations in cstring.c file are plugin.
* ``void cstring_clear(struct cstring *)``
* ``void cstring_length(struct cstring *)``

The framework is a final implementation for the user.
User will see the framework and does not care the plugin and the interface.
The user just use cstring object.
The user does not need to care how to implement the clear or length methods.

In the previous example, the main function is the user of cstring class.
Framework, cstring class, provides interfaces for user: cstring.clear, cstring.length
The main function calls cstring.clear and cstring.length without knowing the implementation in cstring.c file.
The main function is able to see only cstring class that is framework.
The main function cannot access the functions in cstring.c because they are plugin.

The plugin, cstring_clear and cstring_length, are called via framework and interface.
If it is necessary, the plugin would be replaced.
Anybody can make a cstring2.c with better performance and replace cstring.c with cstring2.c.
The main function does not know which plugin is the implementation of the methods.
Actually the main function does not need to know it.

Do you notice why I show the implementation of cstring in object-oriented style?
It does not aim to show the mimic of C++ class.
I would like to show you the framework and the plugin, and the interface between the framework and the plugin.
It is not easy to describe the concepts in a short document.
It would be better for you to understand if I show you one more example.
I will show a test framework that includes plugin and interface in next chapters.
