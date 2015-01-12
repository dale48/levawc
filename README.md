d<h2>Project "LevAWC"</h2>
<h4>Purpose:</h4> Some fundamental, pure, generic container ADT:s - written in ANSI C. Primarily for educational purposes. 

Original source code was written by <a href="http://www.kyleloudon.com/" target="_blank">Kyle Loudon</a> - in his book <a href="http://shop.oreilly.com/product/9781565924536.do" target="_blank">Mastering Algorithms with C</a> - published at <a href="http://www.oreilly.com" target="_blank">O'Reilly Company</a>. I have rewritten and extended some minor parts of the original source code. Finally, I want to thank Kyle and O'Reilly for giving me permisson to publish this code here at GitHub.

<h4>Authors:</h4><a href="http://www.kyleloudon.com/" target="_blank">Kyle Loudon</a> - and me.
<h4>Changelog</h4>see file <a href="ChangeLog.md">ChangeLog.md</a>
<h4>License/Disclaimer:</h4> see file "UNLICENSE" above.

<h4>Documentation:</h4> 
<p>The <a href="http://dale48.github.com/levawc/documentation/index.html" target="_blank"> project documentation</a> can be found <a href="http://dale48.github.com/levawc/documentation/index.html" target="_blank">here</a>.</p>
<p>Project webpage: <a href="http://dale48.github.com/levawc/" target="_blank">http://dale48.github.com/levawc/</a></p>
<!--
-->
<p>In case you want to <i>regenerate the documentation locally</i> - on your box:</p>
<i>Clone</i> (or <i>download zipfile</i> of) this project, <i>install</i> <a href="http://www.stack.nl/~dimitri/doxygen/" target="_blank">Doxygen</a> - and finally <i>run command:</i> <code>doxygen</code> - from the command prompt - when standing in the top-most directory of your clone. Then point your browser to the file <code>index.html</code> in the the new subdirectory  <code>html</code> - recently created by this command.

<h4>Demos:</h4> In the <code>demos</code> subdirectory you will find some simple demos (<code>demo1.c, demo2.c, demo3.c, ...</code>) - using the ADT containers. You can build these yourself - together with a complete, single library (<code>liblevawc.a/levawc.lib</code>) of all the ADT:s - by simply using the enclosed makefiles in this directory, like this:
<ul>
  <li><code>linux.mak</code> - uses the native gcc compiler in Linux. <i>Command</i>: <code>make -f linux.mak</code></li>
  <li><code>mingw32.mak</code> - uses mingw32 gcc compiler in Win32. <i>Command</i>: <code>mingw32-make -f mingw32.mak</code></li>
  <li><code>vcwin32.mak</code> - uses Visual C compiler in Win32. <i>Command</i>: <code>nmake -f vcwin32.mak</code></li>
</ul>
<p>Another way, to build the library and the demos, is to download a <a href="http://dale48.github.com/levawc/gnu_tarballs/levawc-0.40.tar.gz">GNU Tarball</a> - if you are sitting on a UNIX/Linux box or if you have Cygwin or MSYS/MinGW installed - on Windows. Decompress the tarball - enter the root directory of the unpacked tarball - and then execute the usual commands for GNU Tarballs: <code>./configure [--prefix=...]</code>, <code>make</code> and finally - <code>[sudo] make install</code> - if you want to install. The demos are created just where you are - ready to run - and the library and header-files are installed on your system.</p>
<p>The demos are trying to test and show most of the public interface of the ADT:s - accordingly:</p>
<ul>
 <li><code>demo1.c</code> - testing/showing Singly-linked List ADT..</li>
  <li><code>demo2.c</code> - testing/showing Doubly-linked List ADT..</li>
  <li><code>demo3.c</code> - testing/showing Stack and Queue ADT..</li>
  <li><code>demo4.c</code> - testing/showing Chained Hash Table ADT..</li>
  <li><code>demo5.c</code> - testing/showing Heap and Priority Queue ADT..</li>
  <li><code>demo6.c</code> - testing/showing Binary Search Tree ADT..</li>
  <li><code>demo7.c</code> - testing/showing AVL Tree ADT..</li>
  <li><code>demo8.c</code> - testing/showing Circular, Singly-linked List ADT..</li>
  <li><code>demo9.c</code> - testing/showing Set ADT..</li>
  <li><code>demo10.c</code> - testing/showing Open-addressed Hash Table ADT..</li>
<!--   <li> <code>demo11.c</code> - testing/showing Graph ADT..</li> -->
</ul>
<p>All demos are tested against memory leaks (Valgrind, Linux)</p>
<!--
<ul>
  <li></li>
</ul>
-->
