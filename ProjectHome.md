Despite the fact that there are many ways to transform Unicode, there are cases where existing tools can not handle this task properly. Instruments provided by operating systems are not portable and large libraries like libiconv or icu are to big and generic to handle only this task. In my opinion utfcpp project http://utfcpp.sourceforge.net is the best solutions for such problems, but unfortunately it is not possible to use it with C.

The aim of this project is to provide an easy and portable C **source code** for unicode handling, which can be used in any C/C++ project without any restrictions.

Currently utfc implements following functions:
  * utf8 to utf16 little-endian
  * utf8 to utf16 big-endian
  * utf16 little-endian to utf8
  * utf16 big-endian to utf8
  * support for characters outside the BMP ( codepoint > 0xFFFF )

Note, that code is currently valid only for little-endian processor architectures.

You can download current version of utfc from [download page](http://code.google.com/p/utfc/downloads/list)