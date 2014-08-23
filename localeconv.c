
PyDoc_STRVAR(lconv__doc__,
		"lconv: Result from localconv");

static PyStructSequence_Field lconv_fields[] = {
	{"decimal_point",
		"The decimal point character, except for currency values, cannot be an empty string."},
	{"thousands_sep",
		"The separator between groups of digits before the decimal point, except\n"
		"for currency values."},
	{"grouping",
		"The sizes of the groups of digits, except for currency values.\n\n"
		"This is a pointer to a vector of integers, each of size char,\n"
		"representing group size from low order digit groups to high order (right\n"
		"to left). The list may be terminated with 0 or CHAR_MAX. If the list is\n"
		"terminated with 0, the last group size before the 0 is repeated to\n"
		"account for all the digits. If the list is terminated with CHAR_MAX,\n"
		"no more grouping is performed."},
	{"int_curr_symbol", "The standardized international currency symbol."},
	{"currency_symbol", "The local currency symbol."},
	{"mon_decimal_point", "The decimal point character for currency values."},
	{"mon_thousands_sep", "The separator for digit groups in currency values."},
	{"mon_grouping", "Like grouping but for currency values."},
	{"positive_sign",
		"The character used to denote nonnegative currency values, usually the\n"
		"empty string."},
	{"negative_sign",
		"The character used to denote negative currency values, usually a\n"
		"minus sign."},
	{"int_frac_digits",
		"The number of digits after the decimal point in an international-\n"
		"style currency value."},
	{"frac_digits",
		"The number of digits after the decimal point in the local style\n"
		"for currency values."},
	{"p_cs_precedes",
		"1 if the currency symbol precedes the currency value for nonnegative\n"
		"values, 0 if it follows."},
	{"p_sep_by_space",
		"1 if a space is inserted between the currency symbol and the currency\n"
		"value for nonnegative values, 0 otherwise."},
	{"n_cs_precedes", "Like p_cs_precedes but for negative values."},
	{"n_sep_by_space", "Like p_sep_by_space but for negative values."},
	{"p_sign_posn",
		"The location of the positive_sign with respect to a nonnegative quantity\n"
		"and the currency_symbol, coded as follows:\n"
		"\n"
		"    0    Parentheses around the entire string.\n"
		"    1    Before the string.\n"
		"    2    After the string.\n"
		"    3    Just before currency_symbol.\n"
		"    4    Just after currency_symbol.\n"},
	{"n_sign_posn", "Like p_sign_posn but for negative currency values."},
	{"int_p_cs_precedes",
		"Same as p_cs_precedes, but for internationally formatted monetary\n"
		"quantities."},
	{"int_n_cs_precedes",
		"Same as n_cs_precedes, but for internationally formatted monetary\n"
		"quantities."},
	{"int_p_sep_by_space",
		"Same as p_sep_by_space, but for internationally formatted monetary\n"
		"quantities."},
	{"int_n_sep_by_space",
		"Same as n_sep_by_space, but for internationally formatted monetary\n"
		"quantities."},
	{"int_p_sign_posn",
		"Same as p_sign_posn, but for internationally formatted monetary\n"
		"quantities."},
	{"int_n_sign_posn",
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


static void _seq_set_int(PyObject* seq, locale_t loc, ssize_t index, char data) {
	PyStructSequence_SET_ITEM(seq, index, PyInt_From_Size_t(data));
}



static PyObject* Locale_localeconv(Locale* self) {
	struct lconv* lc = localeconv_l(self->locale);
	PyObject* result = PyStructSequence_New(&LconvType);

	_seq_set_string(result, self->locale, 0, lc->decimal_point);
	_seq_set_string(result, self->locale, 1, lc->thousands_sep);
	// grouping
	_seq_set_string(result, self->locale, 3, lc->int_curr_symbol);
	_seq_set_string(result, self->locale, 4, lc->currency_symbol);
	_seq_set_string(result, self->locale, 5, lc->mon_thousands_sep);
	// mon_grouping
	_seq_set_string(result, self->locale, 7, lc->positive_sign);
	_seq_set_string(result, self->locale, 8, lc->negative_sign);
	_seq_set_int(result, self->locale, 9, lc->int_frac_digits);
	_seq_set_int(result, self->locale, 10, lc->frac_digits);
	_seq_set_int(result, self->locale, 11, lc->p_cs_precedes);
	_seq_set_int(result, self->locale, 12, lc->p_sep_by_space);
	_seq_set_int(result, self->locale, 13, lc->n_cs_precedes);
	_seq_set_int(result, self->locale, 14, lc->n_sep_by_space);
	_seq_set_int(result, self->locale, 15, lc->p_sign_posn);
	_seq_set_int(result, self->locale, 16, lc->n_sign_posn);
	_seq_set_int(result, self->locale, 17, lc->int_p_cs_precedes);
	_seq_set_int(result, self->locale, 18, lc->int_n_cs_precedes);
	_seq_set_int(result, self->locale, 19, lc->int_p_sep_by_space);
	_seq_set_int(result, self->locale, 20, lc->int_n_sep_by_space);
	_seq_set_int(result, self->locale, 21, lc->int_p_sign_posn);
	_seq_set_int(result, self->locale, 22, lc->int_n_sign_posn);

	return result;
}


void init_localconv(PyObject* module) {
	PyStructSequence_InitType(&LconvType, &lconv_desc);
	Py_INCREF((PyObject*)&LconvType);
	PyModule_AddObject(module, "lconv", (PyObject*)&LconvType);

}

