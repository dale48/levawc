<h2>Project "LevAWC"</h2>
<h4>Purpose:</h4> Some fundamental, pure, generic container ADT:s - written in ANSI C. Primarily for educational purposes. 

Original source code was written by <a href="http://www.kyleloudon.com/" target="_blank">Kyle Loudon</a> - in his book <a href="http://shop.oreilly.com/product/9781565924536.do" target="_blank">Mastering Algorithms with C</a> - published at <a href="http://www.oreilly.com" target="_blank">O'Reilly Company</a>. I have rewritten and extended some minor parts of the original source code. Finally, I want to thank Kyle and O'Reilly for giving me permisson to publish this code here at GitHub.

<h4>Authors:</h4><a href="http://www.kyleloudon.com/" target="_blank">Kyle Loudon</a> - and me.
<h4>Changelog</h4>see file <a href="ChangeLog.md">ChangeLog.md</a>
<h4>License/Disclaimer:</h4> see file "UNLICENSE" above.

<h4>Documentation:</h4> <b>Clone</b> this project, <b>install</b> <a href="http://www.stack.nl/~dimitri/doxygen/" target="_blank">Doxygen</a> on your box - and finally <b>run command</b> <i>doxygen</i> - from the command prompt - when standing in the top-most directory of your clone. Then point your browser to the file <code>index.html</code> in the the new subdirectory  <code>html</code> - recently created by this command.

<h4>Demos:</h4> In the <code>demos</code> subdirectory you will find some simple demos (<code>demo1.c, demo2.c, demo3.c</code>) - using the ADT containers. You can build these yourself - by simply using the enclosed makefiles in this directory, accordingly:
<ul>
  <li><code>linux.mak</code> - uses the native gcc compiler in Linux. <b>Command</b>: <code>make -f linux.mak</code></li>
  <li><code>mingw32.mak</code> - uses mingw32 gcc compiler in Win32. <b>Command</b>: <code>mingw32-make -f mingw32.mak</code></li>
  <li><code>vcwin32.mak</code> - uses Visual C compiler in Win32. <b>Command</b>: <code>nmake -f vcwin32.mak</code></li>
</ul>

<!--
<ul>
  <li></li>
</ul>
-->
