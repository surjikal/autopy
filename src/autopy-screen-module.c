#include "autopy-screen-module.h"
#include "screen.h"

/* Syntax: get_size() => tuple (width, height) */
/* Description: Returns a tuple `(width, height)` of the size of the
                main screen. */
static PyObject *screen_get_size(PyObject *self, PyObject *args);

/* Syntax: point_visible(x, y) => Boolean */
/* Arguments: |x| => integer,
              |y| => integer */
/* Description: Returns True if the given point is inside the main
                screen boundaries. */
static PyObject *screen_point_visible(PyObject *self, PyObject *args);

/* Syntax: get_color(x, y) => hexadecimal integer */
/* Arguments: |x| => integer,
              |y| => integer */
/* Description: Returns hexadecimal value describing the RGB color at the
                given point.

                Essentially equivalent to:
                {%
                    rect = ((x, y), (1, 1))
                    bitmap.capture_screen_portion(rect).get_color(0, 0)
                %}
                only more efficient/convenient. */
/* Raises: |ValueError| if the rect is out of bounds,
           |OSError| if the system calls were unsuccessful. */

static PyMethodDef ScreenMethods[] = {
	{"get_size", screen_get_size, METH_NOARGS,
	 "get_size() -> tuple (width, height)\n"
	 "Returns a tuple (width, height) of the size of the main screen."},
	{"point_visible", screen_point_visible, METH_VARARGS,
	 "point_visible(x, y) -> Boolean\n"
	 "Returns whether given (x, y) coordinate is inside the main screen."},
	{NULL, NULL, 0, NULL} /* Sentinel */
};

#ifdef PYTHREE
static struct PyModuleDef screenmodule = {
    PyModuleDef_HEAD_INIT,
    "screen",
    "autopy module for working with the screen",
    -1,
    ScreenMethods
};
#endif

PyMODINIT_FUNC initscreen(void)
{
#ifdef PYTHREE
	return PyModule_Create(&screenmodule);
#else
	Py_InitModule3("screen", ScreenMethods, "autopy module for working with "
	                                        "the screen");
#endif
}

#ifdef PYTHREE
PyMODINIT_FUNC PyInit_screen(void) { return initscreen(); }
#endif

static PyObject *screen_get_size(PyObject *self, PyObject *args)
{
	MMSize screenSize = getMainDisplaySize();
	return Py_BuildValue("(kk)", screenSize.width, screenSize.height);
}

static PyObject *screen_point_visible(PyObject *self, PyObject *args)
{
	MMPoint point;
	if (!PyArg_ParseTuple(args, "kk", &point.x, &point.y)) {
		return NULL;
	}

	if (!pointVisibleOnMainDisplay(point)) {
		Py_RETURN_FALSE;
	}

	Py_RETURN_TRUE;
}