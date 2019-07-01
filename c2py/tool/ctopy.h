#ifndef CTOPY_H
#define CTOPY_H

#include <Python.h>
#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include <numpy/arrayobject.h>
#include <string>
#include <vector>
#include <iostream>

class C_TO_PY {

   public:

      // constructor name: C_TO_PY
      C_TO_PY (const std::string py_path,
               const std::string module_name,
               const std::string function_name,
               const int num_args)
      {

         // set the python path to the requested location
         setenv ("PYTHONPATH", py_path.c_str (), 1);

         // initialize the python interpreter
         Py_Initialize ();

         PyObject *pName = PyString_FromString (module_name.c_str ());
         pyModule         = PyImport_Import (pName);
         Py_DECREF (pName);

         if (pyModule != NULL)
         {
            pyFunc = PyObject_GetAttrString (pyModule, function_name.c_str ());
         }

         pyArgs = PyTuple_New (num_args);
         argument = 0;
      }

      // destructor name: C_TO_PY
      ~C_TO_PY (void)
      {
         // dereference the python type
         Py_XDECREF (pyType);

         // dereference the python type
         Py_XDECREF (pyResult);

         // dereference the python function
         Py_XDECREF (pyFunc);

         // dereference the python module
         Py_XDECREF (pyModule);

         // dereference the python function
         Py_XDECREF (pyFunc);

         // dereference the python arguments
         Py_XDECREF (pyArgs);

         // close the python interpreter
         Py_Finalize ();
      }

      // function name: set_arg from C_TO_PY
      template <typename type>
         int set_arg (type src)
         {
            int stat = 0;

            PyObject *pyValue;
            if (std::is_same <type, int>::value)
            {
               pyValue = PyInt_FromLong (src);
            }

            // set the python value as the argument
            stat |= PyTuple_SetItem (pyArgs, argument++, pyValue);

            // dereference the python value
            if (pyValue != nullptr) Py_DECREF (pyValue);

            return stat;
         }

      // function name: set_arg from C_TO_PY
      template <typename type>
         int set_arg (type *src, int num_el)
         {
            int stat = 0;

            // instantiate a new python list
            PyObject *pyList = PyList_New (num_el);

            for (int ind = 0; ind < num_el; ind++)
            {
               if (std::is_same <type, int>::value)
                  pyType = PyInt_FromLong ((long)src[ind]);
               else if (std::is_same <type, float>::value)
                  pyType = PyFloat_FromDouble ((double)src[ind]);
               else
                  pyType = nullptr;

               // set each integer value to the python list
               PyList_SetItem (pyList, ind, pyType);
            }

            PyObject *list_as_tuple = PyTuple_New (num_el);

            list_as_tuple = PyList_AsTuple (pyList);

            // set the argument value
            stat |= PyTuple_SetItem (pyArgs, argument++, list_as_tuple);

            // dereference the python list
            if (pyList != nullptr) Py_DECREF (pyList);

            return stat;
         }

      // function name: load from C_TO_PY
      template <typename type>
         int set_numpy_arg (type *src, int num_el)
         {
            int stat = 0;

            // instantiate a new numpy array
            PyObject *pyArray;

            npy_intp dims = num_el;

            if (std::is_same <type, int>::value)
            {
               pyArray = PyArray_SimpleNewFromData (
                  num_el,
                  dims,
                  NPY_FLOAT,
                  reinterpret_cast<void*>(src));
            }

            return stat;
         }

      // function name: execute from C_TO_PY
      void execute (void)
      {
         if (pyFunc && PyCallable_Check (pyFunc))
         {
            // call the python function
            pyResult = PyObject_CallObject (pyFunc, pyArgs);
         }
      }

   private:

      PyObject *pyModule;
      PyObject *pyFunc;
      PyObject *pyArgs;
      PyObject *pyResult;
      PyObject *pyType;

      int argument;
};

#endif
