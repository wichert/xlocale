This package exposes part of the xlocale-family of C functions to Python. This
is particularly useful to set the locale per-thread.


Normally you use the `locale.setlocale
<https://docs.python.org/2/library/locale.html#locale.setlocale>`_
function to change the locale for the entire process::

    import locale

    locale.setlocale(locale.LC_ALL, 'nl_NL')

This may not be desirable for webservers or other applications that use
multiple threads. To change the locale for just the current thread you
do this::

    import xlocale

    loc = xlocale.Locale(xlocale.LC_ALL_MASK, 'nl_NL')
    loc.use()


Creating a locale object
========================

`xlocale.Locale([mask[, locale[, base]])`
   Create a new locale object. The new locale is created by taken
   a base locale and changing one or more locale categories. If
   no base is given the C locale will be used.

   ``mask`` is a bitmask build from the ``LC_*_MASK`` constants. Normally you
   will want to use ``LC_ALL_MASK`` to change the locale completely.

   ``locale`` is a string with the locale name. If not provided the C locale
   will be used.

`xlocale.Locale.current_locale()`
   Return a Locale instance for the currently locale.


Locale instances
================

`Locale.use()`
  Switch the locale for the current thread. This will also return a Locale
  instance for the previously active locale.

`Locale.name(mask)`
  Return the name for a locale category. ``mask`` is a bitmask of locale
  categories, and the name for the first configured category will be returned.

  **This method is not available on all platforms, and will raise an exception
  on unsupported platforms.**

`Locale.lconv()`
  Return information describing how numbers and currency values must be
  displayed in the current locale. This is an object with the following
  attributes:

+--------------------+--------------------------------------------------------+
| decimal_point      | The decimal point character, except for currency       |
|                    | values, cannot be an empty string.                     |
+--------------------+--------------------------------------------------------+
| thousands_sep      | The separator between groups of digits before the      |
|                    | decimal point, except for currency values.             |
+--------------------+--------------------------------------------------------+
| grouping           | The sizes of the groups of digits, except for currency |
|                    | This is a list of integers representing group size     |
|                    | from low order digit groups to high order (right to    |
|                    | left). The list may be terminated with 0 or CHAR_MAX.  |
|                    | If the list is terminated with 0 the last group size   |
|                    | before the 0 is repeated to account for all the        |
|                    | digits. If the list isterminated with CHAR_MAX, no     |
|                    | more grouping  is performed.                           |
+--------------------+--------------------------------------------------------+
| int_curr_symbol    | The standardized international currency symbol.        |
+--------------------+--------------------------------------------------------+
| currency_symbol    | The local currency symbol.                             |
+--------------------+--------------------------------------------------------+
| mon_decimal_point  | The decimal point character for currency values.       |
+--------------------+--------------------------------------------------------+
| mon_thousands_sep  | The separator for digit groups in currency values.     |
+--------------------+--------------------------------------------------------+
| mon_grouping       | Like grouping but for currency values.                 |
+--------------------+--------------------------------------------------------+
| positive_sign      | The character used to denote nonnegative currency      |
|                    | values, usually the empty string.                      |
+--------------------+--------------------------------------------------------+
| negative_sign      | The character used to denote negative currency values, |
|                    | usually a minus sign.                                  |
+--------------------+--------------------------------------------------------+
| int_frac_digits    | The number of digits after the decimal point in an     |
|                    | international-style currency value.                    |
+--------------------+--------------------------------------------------------+
| frac_digits        | The number of digits after the decimal point in the    |
|                    | locale style for currency values.                      |
+--------------------+--------------------------------------------------------+
| p_cs_precedes      | True if the currency symbol precedes the currency      |
|                    | value for nonnegative values, False if it follows.     |
+--------------------+--------------------------------------------------------+
| p_sep_by_space     | True if a space is inserted between the currency       |
|                    | symbol and the currency value for nonnegative values,  |
|                    | False otherwise.                                       |
+--------------------+--------------------------------------------------------+
| n_cs_precedes      | Like p_cs_precedes but for negative values.            |
+--------------------+--------------------------------------------------------+
| n_sep_by_space     | Like p_sep_by_space but for negative values.           |
+--------------------+--------------------------------------------------------+
| p_sign_posn        | The location of the positive_sign with respect to a    |
|                    | nonnegative quantity and the currency_symbol, coded as |
|                    | follows:                                               |
|                    |                                                        |
|                    | * 0 - Parentheses around the entire string.            |
|                    | * 1 - Before the string.                               |
|                    | * 2 - After the string.                                |
|                    | * 3 - Just before currency_symbol.                     |
|                    | * 4 - Just after currency_symbol.                      |
+--------------------+--------------------------------------------------------+
| n_sign_posn        | Like p_sign_posn but for negative currency values.     |
+--------------------+--------------------------------------------------------+
| int_p_cs_precedes  | Same as p_cs_precedes, but for internationally         |
|                    | monetary quantities.                                   |
|                    | formatted monetary quantities.                         |
+--------------------+--------------------------------------------------------+
| int_n_cs_precedes  | Same as n_cs_precedes, but for internationally         |
|                    | monetary quantities.                                   |
|                    | formatted monetary quantities.                         |
+--------------------+--------------------------------------------------------+
| int_p_sep_by_space | Same as p_sep_by_space, but for internationally        |
|                    | formatted monetary quantities.                         |
+--------------------+--------------------------------------------------------+
| int_n_sep_by_space | Same as n_sep_by_space, but for internationally        |
|                    | formatted monetary quantities.                         |
+--------------------+--------------------------------------------------------+
| int_p_sign_posn    | Same as p_sign_posn, but for internationally formatted |
|                    | monetary quantities.                                   |
+--------------------+--------------------------------------------------------+
| int_n_sign_posn    | Same as n_sign_posn, but for internationally formatted |
|                    | monetary quantities.                                   |
+--------------------+--------------------------------------------------------+


Constants
=========

+----------------------+----------------------------------+
| Constant             | Description                      |
+======================+==================================+
| ``LC_COLLATE_MASK``  | Collation                        |
+----------------------+----------------------------------+
| ``LC_CTYPE_MASK``    | Character type                   |
+----------------------+----------------------------------+
| ``LC_MESSAGES_MASK`` | Messages                         |
+----------------------+----------------------------------+
| ``LC_MONETARY_MASK`` | Monetary                         |
+----------------------+----------------------------------+
| ``LC_NUMERIC_MASK``  | Numeric                          |
+----------------------+----------------------------------+
| ``LC_TIME_MASK``     | Time                             |
+----------------------+----------------------------------+
| ``LC_ALL_MASK``      | Combination of all of the above. |
+----------------------+----------------------------------+

