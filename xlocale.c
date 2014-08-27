#include <assert.h>
#ifdef PLATFORM_BSD
#include <xlocale.h>
#else
#include <locale.h>
#endif
#include <Python.h>
#include <structseq.h>
#include "localeconv.h"

static PyObject *LanguageError;
static PyTypeObject LocaleType;

static PyMethodDef LanguageMethods[] = {
	{NULL, NULL, 0, NULL},
};


typedef struct {
	PyObject_HEAD
	locale_t locale;
} Locale;


static Locale* create_locale(PyTypeObject* type, locale_t loc) {
	Locale *self;
	if ((self=(Locale*)type->tp_alloc(type, 0))!=NULL)
		self->locale=loc;
	return self;
}


static PyObject* Locale_new(PyTypeObject* type, PyObject* args, PyObject* kwargs) {
	return (PyObject*)create_locale(type, LC_GLOBAL_LOCALE);
}


static void Locale_dealloc(Locale* self) {
	if (self->locale!=LC_GLOBAL_LOCALE)
		freelocale(self->locale);
	self->ob_type->tp_free((PyObject*)self);
}


static int Locale_init(Locale* self, PyObject* args, PyObject* kwargs) {
	static char *keywords[] = {"mask", "locale", "base", NULL}; 
	int mask = LC_ALL_MASK;
	const char* locale = NULL;
	PyObject* base = NULL;

	if (!PyArg_ParseTupleAndKeywords(args, kwargs, "|izO!", keywords,
				&mask, &locale, &LocaleType, &base))
		return 0;

	locale_t loc = newlocale(mask, locale,
			base!=NULL ? ((Locale*)base)->locale : NULL);
	if (loc==NULL)
		PyErr_SetString(PyExc_RuntimeError, "Error creating new Locale.");
	else {
		if (self->locale!=LC_GLOBAL_LOCALE)
			freelocale(self->locale);
		self->locale=loc;
	}
	return 0;
}


static PyObject* Locale_current_locale(PyObject* cls) {
	locale_t loc = uselocale(NULL);
	if (loc==NULL) {
		PyErr_SetString(PyExc_RuntimeError, "Error getting current locale.");
		return NULL;
	}
	PyObject* result = (PyObject*)create_locale(&LocaleType, loc);
	if (result==NULL)
		freelocale(loc);
	return result;
}


static PyObject* Locale_use(Locale* self) {
	locale_t previous = uselocale(self->locale);
	PyObject* result = (PyObject*)create_locale(&LocaleType, previous);
	if (result==NULL)
		freelocale(previous);
	return result;
}

static PyObject* Locale_name(Locale* self, PyObject* mask) {
#ifdef PLATFORM_BSD
	if (!PyInt_Check(mask)) {
		PyErr_SetString(PyExc_ValueError, "Mask must be an integer.");
		return NULL;
	}

	int m = (int)PyInt_AsSsize_t(mask);
	if (!m) {
		PyErr_SetString(PyExc_ValueError, "Mask may not be zero.");
		return NULL;
	}
	const char* name = querylocale(m, self->locale);
	if (name==NULL) {
		Py_RETURN_NONE;
	} else 
		return PyString_FromString(name);
#else
	PyErr_SetString(PyExc_RuntimeError, "This OS does not support querylocalae()");
	return NULL;
#endif
}

#include "localeconv.c"


static PyMethodDef Locale_methods[] = {
	{"current_locale", (PyCFunction)Locale_current_locale,
		METH_NOARGS|METH_CLASS,
		"Get the currently active locale."},
	{"name", (PyCFunction)Locale_name, METH_O,
		"Get locale name for a specified category."},
	{"use", (PyCFunction)Locale_use, METH_NOARGS,
	 	"Switch the current thread to this locale."},
	{"lconv", (PyCFunction)Locale_localeconv, METH_NOARGS,
		"Return locale conventions"},
	{NULL},
};

static PyTypeObject LocaleType = {
	PyObject_HEAD_INIT(NULL)
	0,				/* ob_size */ 
	"xlocale.Locale",		/* tp_name */ 
	sizeof(Locale),			/* tp_basicsize */ 
	0,				/* tp_itemsize */ 
	(destructor)Locale_dealloc,	/* tp_dealloc */ 
	0,				/* tp_print */ 
	0,				/* tp_getattr */ 
	0,				/* tp_setattr */ 
	0,				/* tp_compare */ 
	0,				/* tp_repr */ 
	0,				/* tp_as_number */ 
	0,				/* tp_as_sequence */ 
	0,				/* tp_as_mapping */ 
	0,				/* tp_hash  */ 
	0,				/* tp_call */ 
	0,				/* tp_str */ 
	0,				/* tp_getattro */ 
	0,				/* tp_setattro */ 
	0,				/* tp_as_buffer */ 
	Py_TPFLAGS_DEFAULT,		/* tp_flags */ 
	"Locale objects",		/* tp_doc */
	0,				/* tp_traverse */
	0,				/* tp_clear */
	0,				/* tp_richcompare */
	0,				/* tp_weaklistoffset */
	0,				/* tp_iter */
	0,				/* tp_iternext */
	Locale_methods,			/* tp_methods */
	0,				/* tp_members */
	0,				/* tp_getset */
	0,				/* tp_base */
	0,				/* tp_dict */
	0,				/* tp_descr_get */
	0,				/* tp_descr_set */
	0,				/* tp_dictoffset */
	(initproc)Locale_init,		/* tp_init */
	0,				/* tp_alloc */
	Locale_new,			/* tp_new */
};


void init_xlocale(PyObject* module) {
	PyModule_AddIntConstant(module, "LC_COLLATE_MASK", LC_COLLATE_MASK);
	PyModule_AddIntConstant(module, "LC_CTYPE_MASK", LC_CTYPE_MASK);
	PyModule_AddIntConstant(module, "LC_MESSAGES_MASK", LC_MESSAGES_MASK);
	PyModule_AddIntConstant(module, "LC_MONETARY_MASK", LC_MONETARY_MASK);
	PyModule_AddIntConstant(module, "LC_NUMERIC_MASK", LC_NUMERIC_MASK);
	PyModule_AddIntConstant(module, "LC_TIME_MASK", LC_TIME_MASK);
	PyModule_AddIntConstant(module, "LC_ALL_MASK", LC_ALL_MASK);

	LanguageError=PyErr_NewException("language.LanguageError", NULL, NULL);
	Py_INCREF(LanguageError);
	PyModule_AddObject(module, "LanguageError", LanguageError);

	if (PyType_Ready(&LocaleType)<0)
		return;
	Py_INCREF(&LocaleType);
	PyModule_AddObject(module, "Locale", (PyObject*)&LocaleType);
}


PyMODINIT_FUNC
initxlocale(void) {
	PyObject *module;

	if ((module=Py_InitModule("xlocale", LanguageMethods))==NULL)
		return;

	init_xlocale(module);
	init_localconv(module);
}

