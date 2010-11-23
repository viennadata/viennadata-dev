/* =======================================================================
   Copyright (c) 2010, Institute for Microelectronics, TU Vienna.
   http://www.iue.tuwien.ac.at
                             -----------------
                     ViennaData - The Vienna Data Storage Library
                             -----------------

   authors:    Karl Rupp                          rupp@iue.tuwien.ac.at

   license:    MIT (X11), see file LICENSE in the ViennaData base directory
======================================================================= */



#ifndef VIENNADATA_KEY_VALUE_MANAGER_HPP
#define VIENNADATA_KEY_VALUE_MANAGER_HPP

#include <map>
#include <vector>
#include <list>
#include <set>
#include <iostream>
#include <algorithm>
#include <memory>

#include "viennadata/data_container.hpp"
#include "viennadata/forwards.h"

namespace viennadata
{
  template <typename element_type>
  class key_value_manager;

  /////////////////////// erase section //////////////////////////
  
  template <typename element_type>
  class key_value_manager_interface
  {
    public:
      virtual ~key_value_manager_interface() {};
      
      virtual void copy(element_type const & src, element_type const & dest) const = 0;
      virtual void erase(element_type const & src) const = 0;
      virtual bool operator==(key_value_manager_interface<element_type> const &) const = 0;
  };
  
  
  template <typename element_type,
            typename key_type,
            typename value_type>
  class key_value_manager_wrapper : public key_value_manager_interface<element_type>
  {
      typedef key_value_manager_wrapper<element_type, key_type, value_type>    self_type;
      typedef key_value_manager_wrapper<element_type, key_type, all>           self_type_key_all;
      typedef key_value_manager_wrapper<element_type, all, value_type>         self_type_all_value;
      typedef key_value_manager_wrapper<element_type, all, all>                self_type_all_all;
    
    public:
      void copy(element_type const & src,
                element_type const & dest) const
      {
        data_container<key_type, value_type, element_type>::instance().copy(src, dest);
      }
      
      void erase(element_type const & src) const
      {
        data_container<key_type, value_type, element_type>::instance().erase(src);
      }
      
      bool operator==(key_value_manager_interface<element_type> const & other) const
      {
        return (dynamic_cast< self_type const *>( &other ) != NULL
                || dynamic_cast< self_type_key_all const *>( &other ) != NULL
                || dynamic_cast< self_type_all_value const *>( &other ) != NULL
                || dynamic_cast< self_type_all_all const *>( &other ) != NULL);
      } 
      
  };
  
  //wrapper with (source) element type. Used for erase()
  template <typename element_type>
  class key_value_manager
  {
    public:
      key_value_manager() : key_value(NULL), owner(false) {};
      
      key_value_manager(key_value_manager const & other)
      {
        key_value = other.key_value;
        owner = true;
        other.owner = false;
      }
      
      ~key_value_manager()
      {
        if (owner && key_value != NULL)
          delete key_value;
      }

      bool operator==(key_value_manager<element_type> const & other) const
      {
        return *key_value == *(other.key_value);
      }
      
      template <typename key_type, typename value_type>
      void add()
      { 
        assert(key_value == NULL);
        key_value = new key_value_manager_wrapper<element_type, key_type, value_type>();
      }
      
      void copy(element_type const & src,
                element_type const & dest) const
      {
        std::cout << "key_value_manager::copy() called!" << std::endl;
        key_value->copy(src, dest);
      }
      
      void erase(element_type const & el) const
      {
        std::cout << "key_value_manager::erase() called!" << std::endl;
        key_value->erase(el);        
      }
                                                                 
    private:
      key_value_manager_interface<element_type> * key_value;
      mutable bool owner;
  };
  
  
  
}

#endif