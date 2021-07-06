#define PY_SSIZE_T_CLEAN
#include <Python.h>

typedef enum
{
    false,
    true
} bool;

char *buf[4096];
const char *del = "_";

static inline void to_snake_case(char *in)
{
    strcpy(buf, in);
    char *t = buf;
    char *s = in;
    char l;
    while (*s) {
        l = tolower(*s);
        if (l != *s)
          *(t++) = '_';
        *(t++) = l;
        s++;
    }
    *t = NULL;
}

static inline void to_camel_case(char *in)
{
  strcpy (buf, in);
  char *t = buf;
  char *token = strtok (buf, del);
  while (token != NULL)
    {
      while (*token) {
          	*(t++) = *(token++);
      }
      token = strtok (NULL, del);
      if (token != NULL)
        *token = toupper (*token);
    }
    *t = NULL;
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
