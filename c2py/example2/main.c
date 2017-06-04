// https://docs.python.org/release/2.6.5/extending/embedding.html#pure-embedding

#include <Python.h>
#include <stdio.h>

int
main(int argc, char *argv[])
{
    // Set PYTHONPATH TO working directory
    setenv("PYTHONPATH",".",1);

    PyObject *pName, *pModule, *pDict, *pFunc;
    PyObject *pArgs, *pValue;
    int i;

    Py_Initialize();
//    pName = PyString_FromString(argv[1]);
    pName = PyString_FromString((char*)"multiply");
    /* Error checking of pName left out */

    pModule = PyImport_Import(pName);
    Py_DECREF(pName);

    char input[100] = {"multiply"};
    argc = 3;

    if (pModule != NULL)
    {
        pFunc = PyObject_GetAttrString(pModule, input);

        if (pFunc && PyCallable_Check(pFunc))
        {
            pArgs = PyTuple_New(2);
            for (i = 0; i < argc - 3; ++i) {
                if (!pValue) {
                    Py_DECREF(pArgs);
                    Py_DECREF(pModule);
                    fprintf(stderr, "Cannot convert argument\n");
                    return 1;
                }
            }
            char num[2];
            num[0] = '7';
            pValue = PyInt_FromLong( atoi( num ) );
            PyTuple_SetItem(pArgs, 0, pValue);
            num[0] = '5';
            pValue = PyInt_FromLong( atoi( num ) );
            PyTuple_SetItem(pArgs, 1, pValue);

            pValue = PyObject_CallObject(pFunc, pArgs);
            Py_DECREF(pArgs);

            if (pValue != NULL)
            {
                printf("Result of call: %ld\n", PyInt_AsLong(pValue));
                Py_DECREF(pValue);
            }
            else {
                Py_DECREF(pFunc);
                Py_DECREF(pModule);
                PyErr_Print();
                fprintf(stderr,"Call failed\n");
                return 1;
            }
        }
        else {
            if (PyErr_Occurred())
                PyErr_Print();
            fprintf(stderr, "Cannot find function \"%s\"\n", argv[2]);
        }
        Py_XDECREF(pFunc);
        Py_DECREF(pModule);
    }
    else {
        PyErr_Print();
        fprintf(stderr, "Failed to load \"%s\"\n", argv[1]);
        return 1;
    }
    Py_Finalize();
    return 0;
}
