Changelog
=========

1.3.4 - February 25, 2018
-------------------------

- More Python 3 compilation fixes.


1.3.3 - February 25, 2018
-------------------------

- Python 3 has no PyInt.


1.3.2 - February 25, 2018
-------------------------

- Fix handling of exceptions raised when creating a Locale: they are now
  correctly seen as being raised in the constructor.


1.3.1 - February 23, 2018
-------------------------

- Add Python 3 support.
- Add manylinux support.


1.2 - November 15, 2014
-----------------------

- No longer return the current locale from Locale.use(). This created
  unsolvable double free problems.

1.1 - October 10, 2014
----------------------

- Fix a memory handling error in Locale.use() which could result in a segfault
  when using a locale instance multiple times.


1.0 - August 27, 2014
---------------------

- First release

