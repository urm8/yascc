#define PY_SSIZE_T_CLEAN
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
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
    char *buf = malloc(sizeof(char) * (len * 2));
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
    char *out_str = malloc(sizeof(char) * (len * 2));
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
    char *buf = malloc(sizeof(char) * (len * 2));
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
    char *out_str = malloc(sizeof(char) * (len * 2));
    to_camel_case(in_str, out_str);
    PyObject *obj = PyUnicode_FromString(out_str);
    free(out_str);
    return obj;
}

struct StringBuf
{
    char *string;
    size_t size;
};

typedef struct StringBuf *StringBuf_T;

StringBuf_T StringBuf_init(int size)
{
    StringBuf_T buf;
    buf = (StringBuf_T)malloc(sizeof(struct StringBuf));
    buf->string = malloc(sizeof(char *) * size);
    buf->size = size;
    return buf;
}

void StringBuf_update_bounds(StringBuf_T buf, size_t size)
{
    if (buf->size < size)
    {
        buf->size *= 2;
        buf->string = realloc(buf->string, buf->size * sizeof(char *));
    }
}

void StringBuf_free(StringBuf_T buf)
{
    free(buf->string);
    buf->string = NULL;
    buf->size = 0;
    free(buf);
}

/* stupid dynamic stack

push -> size = 0 > 1;
push -> size = 1 > 2;
pop  -> size = 2 > 1;
pop  -> size = 1 > 0;
 */
struct Stack
{
    PyObject **array;
    size_t size;
    size_t capacity;
};

typedef struct Stack *Stack_T;

Stack_T Stack_init(int size)
{
    Stack_T stack;
    stack = (Stack_T)malloc(sizeof(struct Stack));
    stack->array = malloc(sizeof(PyObject *) * size);
    stack->size = 0;
    stack->capacity = size;
    return stack;
}

int Stack_empty(Stack_T stack)
{
    return stack->size == 0;
}

void Stack_push(Stack_T stack, PyObject *elem)
{
    if (stack->size == stack->capacity)
    {
        stack->capacity *= 2;
        stack->array = realloc(stack->array, stack->capacity * sizeof(PyObject *));
    }
    stack->array[stack->size++] = elem;
}

PyObject *Stack_pop(Stack_T stack)
{
    PyObject *obj = stack->array[--stack->size];
    stack->array[stack->size] = NULL;
    return obj;
}

void Stack_free(Stack_T stack)
{
    free(stack->array);
    stack->array = NULL;
    stack->size = stack->capacity = 0;
    free(stack);
}
static PyObject *mutate(PyObject *args, void mutator(const char *, char *))
{
    Py_ssize_t n = 0, len = 0;
    PyObject *obj, *ret;
    if (!PyArg_ParseTuple(args, "O", &obj))
        return NULL;
    if (PyDict_Check(obj))
    {
        ret = PyDict_New();
        Py_XINCREF(ret);
    }
    else if (PyList_Check(obj))
    {
        ret = PyList_New(PyList_GET_SIZE(obj));
        Py_XINCREF(ret);
    }
    else
    {
        return obj;
    }
    PyObject *source, *target, *key, *value, *new_key;
    StringBuf_T buf = StringBuf_init(50);
    Stack_T stack = Stack_init(100);
    Stack_push(stack, obj);
    Stack_push(stack, ret);
    while (!Stack_empty(stack))
    {
        target = (PyObject *)Stack_pop(stack);
        source = (PyObject *)Stack_pop(stack);

        if (PyDict_Check(source))
        {
            n = 0;
            while (PyDict_Next(source, &n, &key, &value))
            {
                value = PyObject_GetItem(source, key);
                Py_XINCREF(value);
                if (PyUnicode_Check(key))
                {
                    const char *temp = PyUnicode_AsUTF8AndSize(key, &len);
                    StringBuf_update_bounds(buf, len);
                    mutator(temp, buf->string);
                    new_key = PyUnicode_FromString(buf->string);
                    Py_XINCREF(new_key);
                    if (PyDict_Check(value))
                    {
                        Stack_push(stack, value);
                        obj = PyDict_New();
                        Py_XINCREF(obj);
                        Stack_push(stack, obj);
                        PyDict_SetItem(target, new_key, obj);
                    }
                    else if (PyList_Check(value))
                    {
                        Stack_push(stack, value);
                        obj = PyList_New(PyList_GET_SIZE(value));
                        Py_XINCREF(obj);
                        Stack_push(stack, obj);
                        PyDict_SetItem(target, new_key, obj);
                    }
                    else
                    {
                        PyDict_SetItem(target, new_key, value);
                    }
                }
                else if (PyDict_Check(value))
                {
                    Stack_push(stack, value);
                    obj = PyDict_New();
                    Py_XINCREF(obj);
                    Stack_push(stack, obj);
                    PyDict_SetItem(target, key, obj);
                }
                else if (PyList_Check(value))
                {
                    Stack_push(stack, value);
                    obj = PyList_New(PyList_GET_SIZE(value));
                    Py_XINCREF(obj);
                    Stack_push(stack, obj);
                    PyDict_SetItem(target, key, obj);
                }
                else
                {
                    PyDict_SetItem(target, key, value);
                }
            }
        }
        else if (PyList_Check(source))
        {
            n = PyList_GET_SIZE(source);
            for (Py_ssize_t i = 0; i < n; i++)
            {
                value = PyList_GET_ITEM(source, i);
                if (PyDict_Check(value))
                {
                    Stack_push(stack, value);
                    obj = PyDict_New();
                    Py_XINCREF(obj);
                    Stack_push(stack, obj);
                    PyList_SET_ITEM(target, i, obj);
                }
                else if (PyList_Check(value))
                {
                    Stack_push(stack, value);
                    obj = PyList_New(PyList_GET_SIZE(value));
                    Py_XINCREF(obj);
                    Stack_push(stack, obj);
                    PyList_SET_ITEM(target, i, obj);
                }
                else
                {
                    PyList_SET_ITEM(target, i, value);
                }
            }
        }
    }
    Stack_free(stack);
    StringBuf_free(buf);
    return ret;
};

static PyObject *camelize(PyObject *self, PyObject *args)
{
    return mutate(args, *to_camel_case);
}

static PyObject *decamelize(PyObject *self, PyObject *args)
{
    return mutate(args, *to_snake_case);
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
