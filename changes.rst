Changelog
=========

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

