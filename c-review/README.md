`c-skeleton`（项目框架目录），使用它来创建一个`liblcthw`项目：

```shell
$ cp -r c-skeleton liblcthw
$ cd liblcthw/
$ ls
LICENSE             Makefile        README.md       bin             build           src             tests
$ vim Makefile
$ ls src/
dbg.h               libex29.c       libex29.o
$ mkdir src/lcthw
$ mv src/dbg.h src/lcthw
$ vim tests/minunit.h
$ rm src/libex29.* tests/libex29*
$ make clean
rm -rf build  tests/libex29_tests
rm -f tests/tests.log
find . -name "*.gc*" -exec rm {} \;
rm -rf `find . -name "*.dSYM" -print`
$ ls tests/
minunit.h  runtests.sh
$
```
