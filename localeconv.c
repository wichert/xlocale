#ifndef PLATFORM_BSD
struct lconv* localeconv_l(locale_t loc) {
    locale_t old = uselocale(loc);
    struct lconv* result = localeconv();
    uselocale(old);
    return result;
}

size_t mbstowcs_l(wchar_t *wcstr, const char *mbstr, size_t count, locale_t loc) {
    locale_t old = uselocale(loc);
    size_t result = mbstowcs(wcstr, mbstr, count);
    uselocale(old);
    return result;
}
#endif


PyDoc_STRVAR(lconv__doc__,
		"lconv: Result from localconv");

static PyStructSequence_Field lconv_fields[] = {
/* 0 */	{"decimal_point",
		"The decimal point character, except for currency values, cannot be an empty string."},
/* 1 */	{"thousands_sep",
		"The separator between groups of digits before the decimal point, except\n"
		"for currency values."},
/* 2 */	{"grouping",
		"The sizes of the groups of digits, except for currency values.\n\n"
		"This is a list of integers representing group size from low order\n"
		"digit groups to high order (right to left). The list may be\n"
		"terminated with 0 or CHAR_MAX. If the list is terminated with 0,\n"
		"the last group size before the 0 is repeated to account for all\n"
		"the digits. If the list is terminated with CHAR_MAX,\n"
		"no more grouping is performed."},
/* 3 */	{"int_curr_symbol", "The standardized international currency symbol."},
/* 4 */	{"currency_symbol", "The local currency symbol."},
/* 5 */	{"mon_decimal_point", "The decimal point character for currency values."},
/* 6 */	{"mon_thousands_sep", "The separator for digit groups in currency values."},
/* 7 */	{"mon_grouping", "Like grouping but for currency values."},
/* 8 */	{"positive_sign",
		"The character used to denote nonnegative currency values, usually the\n"
		"empty string."},
/* 9 */	{"negative_sign",
		"The character used to denote negative currency values, usually a\n"
		"minus sign."},
/*10 */	{"int_frac_digits",
		"The number of digits after the decimal point in an international-\n"
		"style currency value."},
/*11 */	{"frac_digits",
		"The number of digits after the decimal point in the local style\n"
		"for currency values."},
/*12 */	{"p_cs_precedes",
		"True if the currency symbol precedes the currency value for nonnegative\n"
		"values, False if it follows."},
/*13 */	{"p_sep_by_space",
		"True if a space is inserted between the currency symbol and the currency\n"
		"value for nonnegative values, False otherwise."},
/*14 */	{"n_cs_precedes", "Like p_cs_precedes but for negative values."},
/*15 */	{"n_sep_by_space", "Like p_sep_by_space but for negative values."},
/*16 */	{"p_sign_posn",
		"The location of the positive_sign with respect to a nonnegative quantity\n"
		"and the currency_symbol, coded as follows:\n"
		"\n"
		"    0    Parentheses around the entire string.\n"
		"    1    Before the string.\n"
		"    2    After the string.\n"
		"    3    Just before currency_symbol.\n"
		"    4    Just after currency_symbol.\n"},
/*17 */	{"n_sign_posn", "Like p_sign_posn but for negative currency values."},
/*18 */	{"int_p_cs_precedes",
		"Same as p_cs_precedes, but for internationally formatted monetary\n"
		"quantities."},
/*19 */	{"int_n_cs_precedes",
		"Same as n_cs_precedes, but for internationally formatted monetary\n"
		"quantities."},
/*20 */	{"int_p_sep_by_space",
		"Same as p_sep_by_space, but for internationally formatted monetary\n"
		"quantities."},
/*21 */	{"int_n_sep_by_space",
		"Same as n_sep_by_space, but for internationally formatted monetary\n"
		"quantities."},
/*22 */	{"int_p_sign_posn",
		"Same as p_sign_posn, but for internationally formatted monetary\n"
		"quantities."},
/*23 */	{"int_n_sign_posn",
		"Same as n_sign_posn, but for internationally formatted monetary\n"
		"quantities."},
	{0},
};

static PyStructSequence_Desc lconv_desc = {
	"lconv",
	lconv__doc__,
	lconv_fields,
	24,
};

static PyTypeObject LconvType;


static void _seq_set_string(PyObject* seq, locale_t loc, ssize_t index, const char* data) {
	if (!data)
		return;
	size_t needed = mbstowcs_l(NULL, data, 0, loc);
	wchar_t space[30];
	wchar_t *buf;
	if (needed==(size_t)-1)
		// Should we log an error here?
		return;
	if (needed<sizeof(space))
		buf=space;
	else if ((buf=PyMem_Malloc(needed+1))==NULL)
		return;
	size_t length = mbstowcs_l(buf, data, needed+1, loc);
	PyStructSequence_SET_ITEM(seq, index, PyUnicode_FromWideChar(buf, length));
	if (buf!=space)
		PyMem_Free(buf);
}


static void _seq_set_bool(PyObject* seq, locale_t loc, ssize_t index, char data) {
	PyObject* item = PyBool_FromLong(data);

	if (item!=NULL)
		PyStructSequence_SET_ITEM(seq, index, item);
}


static void _seq_set_int(PyObject* seq, locale_t loc, ssize_t index, char data) {
	PyObject* item = PyInt_FromSize_t(data);

	if (item!=NULL)
		PyStructSequence_SET_ITEM(seq, index, item);
}


static void _seq_set_grouping(PyObject* seq, locale_t loc, ssize_t index, const char* data) {
	PyObject* groups = PyList_New(0);;

	if (groups==NULL)
		return;

	if (*data) {
		int i = -1;
		do {
			i++;
			PyObject* item = PyInt_FromLong(data[i]);
			if (item==NULL)
				break;
			if (PyList_Append(groups, item)==-1) {
				Py_DECREF(item);
				Py_DECREF(groups);
				return;
			}
		} while (data[i] && data[i]!=CHAR_MAX);
	}
	PyStructSequence_SET_ITEM(seq, index, groups);
}


static PyObject* Locale_localeconv(Locale* self) {
	struct lconv* lc = localeconv_l(self->locale);
	int i = 0;
	PyObject* result = PyStructSequence_New(&LconvType);

	_seq_set_string(result, self->locale, i++, lc->decimal_point);
	_seq_set_string(result, self->locale, i++, lc->thousands_sep);
	_seq_set_grouping(result, self->locale, i++, lc->grouping);
	_seq_set_string(result, self->locale, i++, lc->int_curr_symbol);
	_seq_set_string(result, self->locale, i++, lc->currency_symbol);
	_seq_set_string(result, self->locale, i++, lc->mon_decimal_point);
	_seq_set_string(result, self->locale, i++, lc->mon_thousands_sep);
	_seq_set_grouping(result, self->locale, i++, lc->mon_grouping);
	_seq_set_string(result, self->locale, i++, lc->positive_sign);
	_seq_set_string(result, self->locale, i++, lc->negative_sign);
	_seq_set_int(result, self->locale, i++, lc->int_frac_digits);
	_seq_set_int(result, self->locale, i++, lc->frac_digits);
	_seq_set_bool(result, self->locale, i++, lc->p_cs_precedes);
	_seq_set_bool(result, self->locale, i++, lc->p_sep_by_space);
	_seq_set_int(result, self->locale, i++, lc->n_cs_precedes);
	_seq_set_int(result, self->locale, i++, lc->n_sep_by_space);
	_seq_set_int(result, self->locale, i++, lc->p_sign_posn);
	_seq_set_int(result, self->locale, i++, lc->n_sign_posn);
	_seq_set_bool(result, self->locale, i++, lc->int_p_cs_precedes);
	_seq_set_bool(result, self->locale, i++, lc->int_n_cs_precedes);
	_seq_set_bool(result, self->locale, i++, lc->int_p_sep_by_space);
	_seq_set_bool(result, self->locale, i++, lc->int_n_sep_by_space);
	_seq_set_int(result, self->locale, i++, lc->int_p_sign_posn);
	_seq_set_int(result, self->locale, i++, lc->int_n_sign_posn);
	assert(i==lconv_desc.n_in_sequence);

	return result;
}


void init_localconv(PyObject* module) {
	PyStructSequence_InitType(&LconvType, &lconv_desc);
	Py_INCREF((PyObject*)&LconvType);
	PyModule_AddObject(module, "lconv", (PyObject*)&LconvType);

}

