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
         // dereference the python function
         Py_DECREF (pyFunc);

         // dereference the python module
         Py_DECREF (pyModule);

         // dereference the python function
         Py_DECREF (pyFunc);

         // dereference the python arguments
         Py_DECREF (pyArgs);

         // close the python interpreter
         Py_Finalize ();
      }

      // function name: load from C_TO_PY
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

      // function name: load from C_TO_PY
      template <typename type>
         int set_arg (type *src, int num_el)
         {
            int stat = 0;

            // instantiate a new python list
            PyObject *pyList = PyList_New (num_el);

            if (std::is_same <type, int>::value)
            {
               for (int ind = 0; ind < num_el; ind++)
               {
                  // set each integer value to the python list
                  PyList_SetItem (pyList, ind, PyInt_FromLong ((long)src[ind]));
               }
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
            // instantiate a new numpy array
            PyObject *pyArray = PyArray_SimpleNewFromData (ND, dims, NPY_LONG, reinterpret_cast<void*>(src));
         }

      // function name: execute from C_TO_PY
      void execute (void)
      {
         if (pyFunc && PyCallable_Check (pyFunc))
         {
            // call the python function
            result = PyObject_CallObject (pyFunc, pyArgs);
         }
      }

   private:

      PyObject *pyModule;
      PyObject *pyFunc;
      PyObject *pyArgs;
      PyObject *result;

      int argument;
};

#endif
