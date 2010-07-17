/* =======================================================================
   Copyright (c) 2010, Institute for Microelectronics, TU Vienna.
   http://www.iue.tuwien.ac.at
                             -----------------
                     ViennaData - The Vienna Data Storage Library
                             -----------------

   authors:    Karl Rupp                          rupp@iue.tuwien.ac.at

   license:    MIT (X11), see file LICENSE in the ViennaMesh base directory
======================================================================= */

#ifndef VIENNADATA_QUANTRAITS_GUARD
#define VIENNADATA_QUANTRAITS_GUARD

#include <map>
#include <vector>
#include <list>
#include <set>
#include <iostream>
#include <algorithm>
#include <memory>

#include "forwards.h"

namespace viennadata
{


    template <typename QuanType>
    struct QuanTraits
    {
      typedef QuanType const &    ResultType;
    };

    //do not use references for primitives:
    template <>
    struct QuanTraits<bool>
    {
      typedef bool   ResultType;
    };

    template <>
    struct QuanTraits<char>
    {
      typedef char   ResultType;
    };

    template <>
    struct QuanTraits<long>
    {
      typedef long   ResultType;
    };

    template <>
    struct QuanTraits<int>
    {
      typedef int   ResultType;
    };

    template <>
    struct QuanTraits<float>
    {
      typedef float   ResultType;
    };

    template <>
    struct QuanTraits<double>
    {
      typedef double   ResultType;
    };


    //helper for debugging:
    template <typename T>
    struct TypeNamePrinter
    {
      static void apply() { T::print(); }
    };

    template <>
    struct TypeNamePrinter<double>
    {
      static void apply() { std::cout << "double"; }
    };

    template <>
    struct TypeNamePrinter<long>
    {
      static void apply() { std::cout << "long"; }
    };

    template <>
    struct TypeNamePrinter<char>
    {
      static void apply() { std::cout << "char"; }
    };

    template <>
    struct TypeNamePrinter< std::string >
    {
      static void apply() { std::cout << "std::string"; }
    };
    
} //namespace 

#endif
