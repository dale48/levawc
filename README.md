<h2>Project "LevAWC"</h2>
<h4>Purpose:</h4> Some fundamental, pure, generic container ADT:s - written in ANSI C. Primarily for educational purposes. 

Original source code was written by <a href="http://www.kyleloudon.com/" target="_blank">Kyle Loudon</a> - in his book <a href="http://shop.oreilly.com/product/9781565924536.do" target="_blank">Mastering Algorithms with C</a> - published at <a href="http://www.oreilly.com" target="_blank">O'Reilly Company</a>. I have rewritten and extended some minor parts of the original source code. Finally, I want to thank Kyle and O'Reilly for giving me permisson to publish this code here at GitHub.

<h4>Authors:</h4><a href="http://www.kyleloudon.com/" target="_blank">Kyle Loudon</a> - and me.
<h4>Changelog</h4>see file <a href="ChangeLog.md">ChangeLog.md</a>
<h4>License/Disclaimer:</h4> see file "UNLICENSE" above.

<h4>Documentation:</h4> Clone this project, install <a href="http://www.stack.nl/~dimitri/doxygen/" target="_blank">Doxygen</a> on your box - and finally run command <b>doxygen</b> - from the command prompt - when standing in the top-most directory of your clone. Then point your browser to the file <b>index.html</b> in the the new subdirectory  <b>html</b> - recently created by this command.

<h4>Demos:</h4> In the <b>demos</b> subdirectory you will find some simple demos (<b>demo1.c, demo2.c, demo3.c</b>) - using the ADT containers. You can build these yourself - by simply using the enclosed makefiles in this directory, accordingly:

<b>linux.mak</b> - uses the native gcc compiler in Linux. Command: <b>make -f linux.mak</b><br />
<b>mingw32.mak</b> - uses mingw32 gcc compiler in Win32. Command: <b>mingw32-make -f mingw32.mak</b><br />
<b>vcwin32.mak</b> - uses Visual C compiler in Win32. Command: <b>nmake -f vcwin32.mak</b><br />
