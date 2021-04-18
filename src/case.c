#define PY_SSIZE_T_CLEAN
#include <Python.h>

typedef enum
{
    false,
    true
} bool;

static inline void to_snake_case(char *src, char *dest)
{
    size_t dest_n = 0;
    for (size_t ch_n = 0; ch_n < strlen(src); ch_n++)
    {
        char ch = *(src + ch_n);
        if (isupper(ch))
        {
            if (ch_n)
            {
                *(dest + dest_n) = '_';
                dest_n++;
            }
            ch = tolower(ch);
        }
        *(dest + dest_n) = ch;
        dest_n++;
    }
    dest[dest_n] = 0;
}

static inline void to_camel_case(char *src, char *dest)
{
    size_t dest_n = 0;
    bool prev_u = false;
    for (size_t ch_n = 0; ch_n < strlen(src); ch_n++)
    {
        char ch = *(src + ch_n);
        if (ch != '_')
        {
            *(dest + dest_n) = prev_u ? toupper(ch) : ch;
            prev_u = false;
            dest_n++;
        }
        else
        {
            prev_u = true;
        }
    }
    dest[dest_n] = 0;
}

static PyObject *camelcase_to_snake_case(PyObject *self, PyObject *args)
{
    char *camel_case_str;
    if (!PyArg_ParseTuple(args, "s", &camel_case_str))
        return NULL;
    char *snake_str = calloc(strlen(camel_case_str) * 2, sizeof(char));
    to_snake_case(camel_case_str, snake_str);
    PyObject *obj = PyUnicode_FromString(snake_str);
    free(snake_str);
    return obj;
}

static PyObject *snakecase_to_camel_case(PyObject *self, PyObject *args)
{
    char *snake_case_str;
    if (!PyArg_ParseTuple(args, "s", &snake_case_str))
        return NULL;
    char *camel_case_str = calloc(strlen(snake_case_str) * 2, sizeof(char));
    to_camel_case(snake_case_str, camel_case_str);
    PyObject *obj = PyUnicode_FromString(camel_case_str);
    free(camel_case_str);
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