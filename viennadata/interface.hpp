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

  //////////// data access ////////////////////
  template <typename key_type, typename value_type>
  class data_accessor_with_key
  {
    public:
      data_accessor_with_key(key_type const & key) : key_(key) {}
      
      template <typename element_type>
      value_type & operator()(element_type const & el)
      {
        //std::cout << "Get data from element" << std::endl;
        return data_container<key_type, value_type, element_type>::instance().access(el, key_);
      }
      
    private:
      key_type const & key_;    
  };

  template <typename key_type, typename value_type>
  class data_accessor_no_key
  {
    public:
      data_accessor_no_key() {}
      
      template <typename element_type>
      value_type & operator()(element_type const & el)
      {
        //std::cout << "Get data from element" << std::endl;
        return data_container<key_type, value_type, element_type>::instance().access(el);
      }
  };
  
  
  // data getter and setter:
  template <typename key_type, typename value_type>
  data_accessor_with_key<key_type, value_type> access(key_type const & key)
  {
    return data_accessor_with_key<key_type, value_type>(key);
  }

  template <typename key_type, typename value_type>
  data_accessor_no_key<key_type, value_type> access()
  {
    return data_accessor_no_key<key_type, value_type>();
  }
  

  //////////// erase data ////////////////////
  template <typename key_type, typename value_type>
  class data_erasor_with_key
  {
    public:
      data_erasor_with_key(key_type const & k) : key(k) {}
      
      template <typename element_type>
      void operator()(element_type const & el)
      {
        data_container<key_type, value_type, element_type>::instance().erase(el, key);
      }
    private:
      key_type const & key;
  };

  template <typename key_type, typename value_type>
  class data_erasor_no_key
  {
    public:
      data_erasor_no_key() {}
      
      template <typename element_type>
      void operator()(element_type const & el)
      {
        data_container<key_type, value_type, element_type>::instance().erase(el);
      }
  };

  template <typename key_type, typename value_type>
  data_erasor_with_key<key_type, value_type> erase(key_type const & key)
  {
    return data_erasor_with_key<key_type, value_type>(key);
  }

  template <typename key_type, typename value_type>
  data_erasor_no_key<key_type, value_type> erase()
  {
    return data_erasor_no_key<key_type, value_type>();
  }


  //////////// memory allocation for data ////////////////////
  
  template <typename key_type, typename value_type>
  class reservation_proxy
  {
    public:
      reservation_proxy(long num) : num_(num) {}
      
      template <typename element_type>
      void operator()(element_type const & e)
      {
        data_container<key_type, value_type, element_type>::instance().reserve(num_);
      }
    
    private:
      long num_;
  };
  

  template <typename key_type, typename value_type>
  reservation_proxy<key_type, value_type> reserve(long num)
  {
    return reservation_proxy<key_type, value_type>(num);
  }
  
  
} //namespace viennadata

#endif 

