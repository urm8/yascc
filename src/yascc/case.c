#define PY_SSIZE_T_CLEAN
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stack.h>
#include <Python.h>

typedef enum
{
    false,
    true
} bool;

static const char del = '_';

static inline void to_snake_case(const char *in, char *target)
{
    char ch;
    do
    {
        ch = *(in++);
        if (65 <= ch && ch <= 90)
        {
            *(target++) = '_';
            // to lower case
            ch |= 0b1100000;
        }
        *(target++) = ch;
    } while (ch);
}

static inline void to_camel_case(const char *in, char *target)
{
    char ch;
    do
    {
        ch = *(in++);
        if (ch == del)
            ch = *(in++) & 0b1011111;

        *(target++) = ch;
    } while (ch);
}

static PyObject *camelcase_to_snake_case(PyObject *self, PyObject *args)
{
    const char *camel_case_str;
    Py_ssize_t len;
    if (!PyArg_ParseTuple(args, "s#", &camel_case_str, &len))
        return NULL;
    char *buf = malloc(sizeof(char) * ((int)(len * 2)));
    to_snake_case(camel_case_str, buf);
    PyObject *obj = PyUnicode_FromString(buf);
    free(buf);
    return obj;
}

static inline PyObject *camelcase_string_to_snake_case(PyObject *string)
{
    /* Accepts python str object, outputs same object, but snakecased. */
    Py_ssize_t len;
    const char *in_str = PyUnicode_AsUTF8AndSize(string, &len);
    char *out_str = malloc(sizeof(char) * ((int)(len * 2)));
    to_snake_case(in_str, out_str);
    PyObject *obj = PyUnicode_FromString(out_str);
    free(out_str);
    return obj;
}

static PyObject *snakecase_to_camel_case(PyObject *self, PyObject *args)
{
    const char *snake_case_str;
    Py_ssize_t len;
    if (!PyArg_ParseTuple(args, "s#", &snake_case_str, &len))
        return NULL;
    char *buf = malloc(sizeof(char) * ((int)(len * 2)));
    to_camel_case(snake_case_str, buf);
    PyObject *obj = PyUnicode_FromString(buf);
    free(buf);
    return obj;
}

static inline PyObject *snakecase_string_to_camel_case(PyObject *string)
{
    /* Accepts python str object, outputs same object, but camelcased. */
    Py_ssize_t len;
    const char *in_str = PyUnicode_AsUTF8AndSize(string, &len);
    char *out_str = malloc(sizeof(char) * ((int)(len * 2)));
    to_camel_case(in_str, out_str);
    PyObject *obj = PyUnicode_FromString(out_str);
    free(out_str);
    return obj;
}

static PyObject *camelize(PyObject *self, PyObject *args)
{
    PyObject *obj;
    if (!PyArg_ParseTuple(args, "O", &obj))
        return NULL;
    Py_XINCREF(obj);
    Stack_T stack = Stack_init();
    Stack_push(stack, obj);
    while (!Stack_empty(stack))
    {
        PyObject *candidate = (PyObject *)Stack_pop(stack);
        if (PyDict_Check(candidate))
        {
            PyObject *keys = PyDict_Keys(candidate);
            int keys_len = PyList_GET_SIZE(keys);
            for (int i = 0; i < keys_len; i++)
            {
                PyObject *key = PyList_GET_ITEM(keys, i);
                PyObject *value = PyDict_GetItem(candidate, key);

                if (PyUnicode_Check(key))
                {
                    PyObject *new_key = snakecase_string_to_camel_case(key);
                    Py_XINCREF(value);
                    PyDict_DelItem(candidate, key);
                    PyDict_SetItem(candidate, new_key, value);
                    Py_DECREF(new_key);
                    Py_XDECREF(value);
                }
                if (PyDict_Check(value) || PyList_Check(value))
                {
                    Stack_push(stack, value);
                }
            }
            Py_DECREF(keys);
        }
        else if (PyList_Check(candidate))
        {
            Py_ssize_t size = PyList_GET_SIZE(candidate);
            for (int i = 0; i < size; i++)
            {
                Stack_push(stack, PyList_GET_ITEM(candidate, i));
            }
        }
    }
    free(stack);
    return obj;
}
static PyObject *decamelize(PyObject *self, PyObject *args)
{
    PyObject *obj;
    if (!PyArg_ParseTuple(args, "O", &obj))
        return NULL;
    Py_XINCREF(obj);
    Stack_T stack = Stack_init();
    Stack_push(stack, obj);
    while (!Stack_empty(stack))
    {
        PyObject *candidate = (PyObject *)Stack_pop(stack);
        if (PyDict_Check(candidate))
        {
            PyObject *keys = PyDict_Keys(candidate);
            int keys_len = PyList_GET_SIZE(keys);
            for (int i = 0; i < keys_len; i++)
            {
                PyObject *key = PyList_GET_ITEM(keys, i);
                PyObject *value = PyDict_GetItem(candidate, key);

                if (PyUnicode_Check(key))
                {
                    PyObject *new_key = camelcase_string_to_snake_case(key);
                    Py_XINCREF(value);
                    PyDict_DelItem(candidate, key);
                    PyDict_SetItem(candidate, new_key, value);
                    Py_DECREF(new_key);
                    Py_XDECREF(value);
                }
                if (PyDict_Check(value) || PyList_Check(value))
                {
                    Stack_push(stack, value);
                }
            }
            Py_XDECREF(keys);
        }
        else if (PyList_Check(candidate))
        {
            Py_ssize_t size = PyList_GET_SIZE(candidate);
            for (int i = 0; i < size; i++)
            {
                Stack_push(stack, PyList_GET_ITEM(candidate, i));
            }
        }
    }
    free(stack);
    return obj;
}

static PyMethodDef case_methods[] = {
    {"_to_snake_case", camelcase_to_snake_case, METH_VARARGS, PyDoc_STR("convert camel case to snake case string")},
    {"_to_camel_case", snakecase_to_camel_case, METH_VARARGS, PyDoc_STR("convert snake case to camel case string")},
    {"_camelize", camelize, METH_VARARGS, PyDoc_STR("convert snake case dict/list to camel case dict/list")},
    {"_decamelize", decamelize, METH_VARARGS, PyDoc_STR("convert snake case dict/list to camel case dict/list")},
    {NULL, NULL, 0, NULL} /* Sentinel */
};

static struct PyModuleDef _case_module = {
    PyModuleDef_HEAD_INIT,
    "_case",
    NULL,
    -1,
    case_methods};

PyMODINIT_FUNC PyInit__case(void)
{
    return PyModule_Create(&_case_module);
}
