/* =======================================================================
   Copyright (c) 2010, Institute for Microelectronics, TU Vienna.
   http://www.iue.tuwien.ac.at
                             -----------------
                     ViennaData - The Vienna Data Storage Library
                             -----------------

   authors:    Karl Rupp                          rupp@iue.tuwien.ac.at

   license:    MIT (X11), see file LICENSE in the ViennaMesh base directory
======================================================================= */


  /******************* ID Handling *******************************/

#ifndef VIENNADATA_KEY_VALUE_ERASE_HPP
#define VIENNADATA_KEY_VALUE_ERASE_HPP

#include <map>
#include <vector>
#include <list>
#include <set>
#include <iostream>
#include <algorithm>
#include <memory>

#include "viennadata/data_container.hpp"

namespace viennadata
{
  template <typename element_type_src>
  class key_value_erase;


  // abstract base class for comparing <key, value> type pairs
  class type_comparison_interface
  {
    public:
      virtual ~type_comparison_interface() {};

      virtual bool operator==(type_comparison_interface const &) const = 0;
  };

  template <typename T>
  class type_comparison_wrapper : public type_comparison_interface
  {
      typedef type_comparison_wrapper<T>    self_type;
    public:
      bool operator==(type_comparison_interface const & rhs) const
      {
        return dynamic_cast< self_type const *>(&rhs) != NULL;
      }
  };

  /////////////////////// erase section //////////////////////////

  template <typename element_type_src>
  class key_value_erase_interface
  {
    public:
      virtual ~key_value_erase_interface() {};

      virtual void erase(element_type_src const & src) const = 0;
      virtual bool operator==(key_value_erase_interface<element_type_src> const &) const = 0;
  };


  template <typename element_type_src,
            typename key_type,
            typename value_type>
  class key_value_erase_wrapper : public key_value_erase_interface<element_type_src>
  {
      typedef key_value_erase_wrapper<element_type_src, key_type, value_type>    self_type;
      typedef key_value_erase_wrapper<element_type_src, key_type, all>           self_type_key_all;
      typedef key_value_erase_wrapper<element_type_src, all, value_type>         self_type_all_value;
      typedef key_value_erase_wrapper<element_type_src, all, all>                self_type_all_all;

    public:
      key_value_erase_wrapper() : wrapped_key(new type_comparison_wrapper<key_type>()),
                                  wrapped_value(new type_comparison_wrapper<value_type>()) {}

      void erase(element_type_src const & src) const
      {
        data_container<key_type, value_type, element_type_src>::instance().erase(src);
      }

      bool operator==(key_value_erase_interface<element_type_src> const & other) const
      {
        return (dynamic_cast< self_type const *>( &other ) != NULL
                || dynamic_cast< self_type_key_all const *>( &other ) != NULL
                || dynamic_cast< self_type_all_value const *>( &other ) != NULL
                || dynamic_cast< self_type_all_all const *>( &other ) != NULL);
      }

    private:
      type_comparison_interface * wrapped_key;
      type_comparison_interface * wrapped_value;
  };

  //wrapper with (source) element type. Used for erase()
  template <typename element_type_src>
  class key_value_erase
  {
    public:
      key_value_erase() : key_value(NULL), owner(false) {};

      key_value_erase(key_value_erase const & other)
      {
        key_value = other.key_value;
        owner = true;
        other.owner = false;
      }

      ~key_value_erase()
      {
        if (owner && key_value != NULL)
          delete key_value;
      }

      bool operator==(key_value_erase<element_type_src> const & other) const
      {
        return *key_value == *(other.key_value);
      }

      template <typename key_type, typename value_type>
      void add()
      {
        assert(key_value == NULL);
        key_value = new key_value_erase_wrapper<element_type_src, key_type, value_type>();
      }

      void erase(element_type_src const & el) const
      {
        //std::cout << "key_value_erase::erase() called!" << std::endl;
        key_value->erase(el);
      }

    private:
      key_value_erase_interface<element_type_src> * key_value;
      mutable bool owner;
  };



}

#endif
