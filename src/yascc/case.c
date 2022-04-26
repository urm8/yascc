#define PY_SSIZE_T_CLEAN
#include <Python.h>

typedef enum
{
    false,
    true
} bool;

char buf[4096];
const char del = '_';

static inline void to_snake_case(char *in)
{
    char *target = buf;
    char *source = in;
    char l;
    while (*source)
    {
        l = tolower(*source);
        if (l != *source)
            *(target++) = '_';
        *(target++) = l;
        source++;
    }
    *target = '\0';
}

static inline void to_camel_case(char *in)
{
    char *target = buf;
    char ch;
    do
    {
        ch = *(in++);
        if (ch == del) {
            ch = toupper(*(in++));
        }
        *(target++) = ch;
    } while(ch);
    *target = '\0';
}

static PyObject *camelcase_to_snake_case(PyObject *self, PyObject *args)
{
    char *camel_case_str;
    if (!PyArg_ParseTuple(args, "s", &camel_case_str))
        return NULL;
    to_snake_case(camel_case_str);
    PyObject *obj = PyUnicode_FromString(buf);
    return obj;
}

static PyObject *snakecase_to_camel_case(PyObject *self, PyObject *args)
{
    char *snake_case_str;
    if (!PyArg_ParseTuple(args, "s", &snake_case_str))
        return NULL;
    to_camel_case(snake_case_str);
    PyObject *obj = PyUnicode_FromString(buf);
    return obj;
}

static PyMethodDef case_methods[] = {
    {"_to_snake_case", camelcase_to_snake_case, METH_VARARGS, "convert camel case to snake case string"},
    {"_to_camel_case", snakecase_to_camel_case, METH_VARARGS, "convert snake case to camel case string"},
    {NULL, NULL, 0, NULL} /* Sentinel */
};

static struct PyModuleDef _case_module = {
    PyModuleDef_HEAD_INIT,
    "_camelcase",
    NULL,
    -1,
    case_methods};

PyMODINIT_FUNC PyInit__case(void)
{
    return PyModule_Create(&_case_module);
}
