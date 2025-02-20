// SPDX-License-Identifier: Apache-2.0

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Helper: Trim whitespace
char *trim_whitespace(char *str) {
    while (*str == ' ' || *str == '\t' || *str == '\n' || *str == '\r') str++;
    if (*str == '\0') return str;
    char *end = str + strlen(str) - 1;
    while (end > str && (*end == ' ' || *end == '\t' || *end == '\n' || *end == '\r')) end--;
    end[1] = '\0';
    return str;
}

// Helper: Check if string is an integer
int is_integer(const char *str) {
    if (*str == '-' || *str == '+') str++;
    while (*str) {
        if (*str < '0' || *str > '9') return 0;
        str++;
    }
    return 1;
}

// Helper: Check if string is boolean ("true" or "false")
int is_boolean(const char *str) {
    return (strcasecmp(str, "true") == 0 || strcasecmp(str, "false") == 0);
}

// Function: Load environment variables (from .env or system)
static PyObject *validate_env(PyObject *self, PyObject *args) {
    const char *file_path;
    PyObject *schema; // Dictionary of expected types

    // Parse arguments
    if (!PyArg_ParseTuple(args, "sO!", &file_path, &PyDict_Type, &schema)) {
        return NULL;
    }

    FILE *file = fopen(file_path, "r");
    PyObject *result_dict = PyDict_New();

    if (file) {
        char line[512];
        while (fgets(line, sizeof(line), file)) {
            if (line[0] == '#' || line[0] == '\n') continue; // Ignore comments/empty lines

            char *key = strtok(line, "=");
            char *value = strtok(NULL, "\n");
            if (!key || !value) continue;

            key = trim_whitespace(key);
            value = trim_whitespace(value);
            PyDict_SetItemString(result_dict, key, PyUnicode_FromString(value));
        }
        fclose(file);
    }

    // Check environment variables if key is missing
    PyObject *keys = PyDict_Keys(schema);
    Py_ssize_t size = PyList_Size(keys);

    for (Py_ssize_t i = 0; i < size; i++) {
        PyObject *key_obj = PyList_GetItem(keys, i);
        const char *key = PyUnicode_AsUTF8(key_obj);
        PyObject *expected_type = PyDict_GetItem(schema, key_obj);

        if (!PyDict_Contains(result_dict, key_obj)) {
            // Try to get from system environment
            const char *env_value = getenv(key);
            if (env_value) {
                PyDict_SetItemString(result_dict, key, PyUnicode_FromString(env_value));
            } else {
                PyErr_Format(PyExc_ValueError, "Missing required environment variable: %s", key);
                return NULL;
            }
        }

        // Validate types
        PyObject *value_obj = PyDict_GetItemString(result_dict, key);
        const char *value = PyUnicode_AsUTF8(value_obj);

        if (PyUnicode_CompareWithASCIIString(expected_type, "int") == 0 && !is_integer(value)) {
            PyErr_Format(PyExc_ValueError, "Invalid integer for key '%s'", key);
            return NULL;
        } else if (PyUnicode_CompareWithASCIIString(expected_type, "bool") == 0 && !is_boolean(value)) {
            PyErr_Format(PyExc_ValueError, "Invalid boolean for key '%s'", key);
            return NULL;
        }
    }

    return result_dict;
}

// Define module methods
static PyMethodDef EnvCheckMethods[] = {
    {"validate_env", validate_env, METH_VARARGS, "Validate a .env file against a schema, with system variable fallback."},
    {NULL, NULL, 0, NULL}
};

// Define module
static struct PyModuleDef envcheckmodule = {
    PyModuleDef_HEAD_INIT,
    "envcheck",
    NULL,
    -1,
    EnvCheckMethods
};

// Initialize module
PyMODINIT_FUNC PyInit_envcheck(void) {
    return PyModule_Create(&envcheckmodule);
}
