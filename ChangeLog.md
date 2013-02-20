<h2>LevAWC version 0.25</h2>
<h4>Changes</h4>
<ul>
  <li>Added new API source: <code>chashtbl.c, chashtbl.h</code> - a chained hash table ADT.</li>
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

<h2>LevAWC version 0.2</h2>
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

<h2>LevAWC version 0.1</h2>
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
