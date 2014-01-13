tree-db
==================
グラフの中心と深さから、対応する部分グラフを得るツール。

`HTTPでhttp://127.0.0.1:12355/?center=foo&depth=2`
のように指定すると、
```
foo,bar
foo,baz
bar,abc
```
のようなテキストを返す。

元になるグラフはテキストファイルとして起動時に読み込んだもののみ有効。

Code Status
------------------
![build status](https://travis-ci.org/mathfur/tree_db.png)

Install
-------
 1. make
 2. sudo make install

Usage
-----
 1. tree_db --port 12355 --interface http --source edges.txt

License
-------
Copyright &copy; 2013 mathfur
Distributed under the [MIT License][mit].
[MIT]: http://www.opensource.org/licenses/mit-license.php
