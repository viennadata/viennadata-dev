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

#ifndef VIENNADATA_KEY_VALUE_COPY_HPP
#define VIENNADATA_KEY_VALUE_COPY_HPP

#include <map>
#include <vector>
#include <list>
#include <set>
#include <iostream>
#include <algorithm>
#include <memory>

#include "viennadata/data_container.hpp"
#include "viennadata/key_value_erase.hpp"

namespace viennadata
{
  template <typename element_type_src, typename element_type_dest>
  class key_value_copy;

  template <typename element_type_src, typename element_type_dest>
  class key_value_copy_interface
  {
    public:
      virtual ~key_value_copy_interface() {};
      
      virtual void copy(element_type_src const & src, element_type_dest const & dest) const = 0;
      virtual bool operator==(key_value_erase_interface const &) const = 0;
  };
  
  
  template <typename element_type_src,
            typename element_type_dest,
            typename key_type,
            typename value_type>
  class key_value_copy_wrapper : public key_value_copy_interface<element_type_src, element_type_dest>
  {
      typedef key_value_copy_wrapper<element_type_src, element_type_dest, key_type, value_type>    self_type;
    
    public:
      key_value_copy_wrapper() : wrapped_key(new type_comparison_wrapper<key_type>()),
                                 wrapped_value(new type_comparison_wrapper<value_type>()) {}
      
      void copy(element_type_src & src, element_type_dest & dest) const
      {
        data_container<key_type, value_type, element_type_src>::instance().copy(src, dest);
      };
      
      bool operator==(key_value_copy_interface<element_type_src, element_type_dest> const & other) const
      {
        return dynamic_cast< self_type const *>( &other ) != NULL;
      }
      
    private:
      type_comparison_interface * wrapped_key;
      type_comparison_interface * wrapped_value;
  };
  
  
  
  
  
  //wrapper with source and destination element type
  template <typename element_type_src,
            typename element_type_dest>
  class key_value_copy
  {
    public:
      template <typename key_type, typename value_type>
      key_value_pair_double() : key_value(new key_value_copy_wrapper<element_type_src,
                                                                 element_type_dest,
                                                                 key_type,
                                                                 value_type>()) {};
      //rest to come
    
    private:
      key_value_copy_interface<element_type_src, element_type_dest> * key_value;
  };

  
  
}
#endif