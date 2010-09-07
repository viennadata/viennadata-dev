/* =======================================================================
   Copyright (c) 2010, Institute for Microelectronics, TU Vienna.
   http://www.iue.tuwien.ac.at
                             -----------------
                     ViennaData - The Vienna Data Storage Library
                             -----------------

   authors:    Karl Rupp                          rupp@iue.tuwien.ac.at

   license:    MIT (X11), see file LICENSE in the ViennaData base directory
======================================================================= */

#ifndef VIENNADATA_INTERFACE_HPP
#define VIENNADATA_INTERFACE_HPP

#include "viennadata/data_container.hpp"

namespace viennadata
{

  
  template <typename key_type, typename value_type>
  class data_getter
  {
    public:
      data_getter(key_type const & key) : key_(key) {}
      
      template <typename element_type>
      value_type & operator()(element_type const & el)
      {
        //std::cout << "Get data from element" << std::endl;
        return data_container<key_type, value_type, element_type>::instance()(el, key_);
      }
      
    private:
      key_type const & key_;
    
  };
  
  
  // data getter and setter:
  template <typename key_type, typename value_type>
  data_getter<key_type, value_type> access(key_type const & key)
  {
    return data_getter<key_type, value_type>(key);
  }
  
    
  
} //namespace viennadata

#endif 

