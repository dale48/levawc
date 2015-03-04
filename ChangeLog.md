<h2>LevAWC version 0.50 (Tue Feb 24 09:16:13 2015)</h2>
<h4>Changes</h4>
<p>Since this is a <i>major revision</i>, the changes are numerous. Now all the data structures in Kyle's book are included in this version, AFAIK. Some graph algorithms are present, too.</p>
<ul>
  <li>Added <i>new container source</i>: <code>cslist.c, cslist.h, set.c, set.h, ohashtbl.c, ohashtbl.h, graph.c, graph.h, algo.c, algo.h</code></li>
  <li>Added <i>new demo</i> source: <code>demo8.c, demo9.c, demo10.c, demo11.c</code> - showing usage of Circular List, Set, Open-Addressed Hashtable and Graph, respectivly.</li>
  <li>Demo program <code>demo12.c</code> is a more extensive graph demo - implementing the "Dijkstra's Shortest Path" algorithm.</li>
  <li>Made all demos <i>menu-driven</i> - with <i>rudimentary error handling</i></li>
  <li><i>Moved</i> some <i>utility functions</i> from the demo source files - to file <code>utils.c</code> and <code>utils.h</code>.</li>
</ul>
<h4>New features</h4>
<ul>
  <li>Four new container ADT:s - Circular List, Set, Open-Addressed Hashtable and Graph - accompanied by demos (<code>demo8.c, demo9.c, demo10.c, demo11.c</code>)</li>
  <li>More extensiv demo: <code>demo12.c</code></li>
  <li>Introducing Graph Algorithms (in files <code>algo.c algo.h)</code></li>
  <li>Extended and fine-tuned the documentation accordingly...</li>
</ul>
<h4>Bugfixes</h4>
<ul>
  <li>A lot...</li>
</ul>

<h2>LevAWC version 0.40</h2>
<h4>Changes</h4>
<ul>
  <li>Added new source: <code>bitree.cp, bitree.h, avltree.c, avltree.h, utils.c, utils.h</code></li>
  <li>Added new demo source: <code>demo6.c, demo7.c</code> - showing usage of binary search and AVL trees.</li>
  <li>Some changes in demo4.c so the user can interactively decide nr of insertions/removals of nodes. </li>
  <li>Improved the documentation somewhat.</li>
</ul>
<h4>New features</h4>
<ul>
  <li>Two new container ADT:s - simple binary search tree - and the more advanced AVL tree - accompanied by demos (<code>demo6.c, demo7.c</code>).</li>
</ul>
<h4>Bugfixes</h4>
<ul>
  <li>Minor ones...</li>
</ul>

<h2>LevAWC version 0.30</h2>
<h4>Changes</h4>
<ul>
  <li>Added new source: <code>heap.c, heap.h, pqueue.c, pqueue.h</code></li>
  <li>Added new demo source: <code>demo5.c</code> - showing both the array and tree structure of a priority queue.</li>
  <li>Improved the documentation - when it comes to describing "in/out" parameters - implemented as double-pointers in C.</li>
  <li>Improved "defensive programming" in all the demo programs (using assertions and checking return values by if-blocks)
</ul>
<h4>New features</h4>
<ul>
  <li>Two new container ADT:s - heap and prioity queue - accompanied by a demo (<code>demo5.c</code>).</li>
</ul>
<h4>Bugfixes</h4>
<ul>
  <li>Minor ones...</li>
</ul>

<h2>LevAWC version 0.25</h2>
<h4>Changes</h4>
<ul>
  <li>Added new source: <code>chashtbl.c, chashtbl.h</code> - a chained hash table ADT.</li>
  <li>Added new demo source for the hashtable ADT: <code>demo4.c</code></li>
  <li>Made following changes to function <code>int SLISTremnode(Slist list, void **data)</code>:<br />
  Changed function name to <code>int SLISTfind_remove(Slist list, void **data)</code><br />
  Changed return value of this function - for missing "match-callback"(=not set) - from -1 to -2<br />
  Changed return value - for node not found - from -1 to 1 (see documentation for further information)</li>
  <li>Added a new function: <code>int DLISTfind_remove(Dlist list, void **data)</code> - to the public interface of doubly-linked lists (see Doxygen documentation and <code>dlist.c/h</code>).
  <li>Made some minor revision to the Doxygen documentation. Enhanced the description of in/out parameters - i.e. double-pointers</li>
</ul>
<h4>New features</h4>
<ul>
  <li>A new container ADT - chained hash table (<code>chashtbl.c/h</code>) - accompanied by a demo (<code>demo4.c</code>).</li>
</ul>
<h4>Bugfixes</h4>
<ul>
  <li>Some minor ones...</li>
</ul>

<h2>LevAWC version 0.20</h2>
<h4>Changes</h4>
<ul>
  <li>Added new API source: <code>stack.c, stack.h, queue.c, queue.h</code> (mainly simple "wrappers" around code in <code>slist.c</code>)</li>
  <li>Added new demo source: <code>demo3.c</code></li>
</ul>
<h4>New features</h4>
<ul>
  <li>Two new container ADT:s - stack and queue - accompanied by a demo.</li>
</ul>
<h4>Bugfixes</h4>
<ul>
  <li>None</li>
</ul>

<h2>LevAWC version 0.10</h2>
<h4>Changes</h4>
<ul>
  <li>Added initial API source: <code>slist.c, slist.h, dlist.c, dlist.h</code></li>
  <li>Added initial demo source: <code>demo1.c, demo2.c</code></li>
</ul>
<h4>New features</h4>
<ul>
  <li>Uploaded 2 container ADT:s - singly- and doubly-linked-list - accompanied by demos.</li>
</ul>
<h4>Bugfixes</h4>
<ul>
  <li>None</li>
</ul>

<!--
<ul>
  <li></li>
</ul>
-->
