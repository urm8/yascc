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
        if (ch == del)
        {
            ch = toupper(*(in++));
        }
        *(target++) = ch;
    } while (ch);
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

static PyObject *camelize(PyObject *self, PyObject *args)
{
    PyObject *obj;
    if (!PyArg_ParseTuple(args, "O", &obj))
        return NULL;
    Py_INCREF(obj);
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
                    const char *key_str = PyUnicode_AsUTF8(key);
                    to_camel_case((char *)key_str);
                    PyObject *new_key = PyUnicode_FromString(buf);
                    Py_INCREF(value);
                    PyDict_DelItem(candidate, key);
                    PyDict_SetItem(candidate, new_key, value);
                    Py_DECREF(new_key);
                    Py_DECREF(value);
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
    Py_INCREF(obj);
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
                    const char *key_str = PyUnicode_AsUTF8(key);
                    to_snake_case((char *)key_str);
                    PyObject *new_key = PyUnicode_FromString(buf);
                    Py_INCREF(value);
                    PyDict_DelItem(candidate, key);
                    PyDict_SetItem(candidate, new_key, value);
                    Py_DECREF(new_key);
                    Py_DECREF(value);
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
