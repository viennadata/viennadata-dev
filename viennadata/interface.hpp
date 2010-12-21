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

#include "forwards.h"
#include "viennadata/data_container.hpp"
#include "viennadata/key_value_registration.hpp"

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
  
  //prevent messing around with all-type and supplied key:
  template <typename key_type>
  class data_erasor_with_key<key_type, all>
  {
    typedef typename error_indicator<key_type>::ERROR_NO_KEY_ARGUMENT_ALLOWED_WHEN_USING_ERASE_FUNCTION_WITH_ALL   error_type;
  };

  template <typename value_type>
  class data_erasor_with_key<all, value_type>
  {
    typedef typename error_indicator<value_type>::ERROR_NO_KEY_ARGUMENT_ALLOWED_WHEN_USING_ERASE_FUNCTION_WITH_ALL   error_type;
  };

  template <>
  class data_erasor_with_key<all, all>
  {
    //typedef typename key_type::ERROR_NO_KEY_ARGUMENT_ALLOWED_WHEN_USING_ERASE_FUNCTION_WITH_ALL   error_type;
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

  template <typename key_type>
  class data_erasor_no_key <key_type, all>
  {
    public:
      data_erasor_no_key() {}
      
      template <typename element_type>
      void operator()(element_type const & el)
      {
        key_value_registration<element_type>::instance().template erase_key_all<key_type>(el);
      }
  };

  template <typename value_type>
  class data_erasor_no_key <all, value_type>
  {
    public:
      data_erasor_no_key() {}
      
      template <typename element_type>
      void operator()(element_type const & el)
      {
        key_value_registration<element_type>::instance().template erase_all_value<value_type>(el);
      }
  };
  
  template <>
  class data_erasor_no_key <all, all>
  {
    public:
      data_erasor_no_key() {}
      
      template <typename element_type>
      void operator()(element_type const & el)
      {
        key_value_registration<element_type>::instance().erase_all_all(el);
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


  
  //////////////////////// Move data //////////////////////////////////
  template <typename key_type, typename value_type>
  class data_mover_with_key
  {
    public:
      data_mover_with_key(key_type const & key) : key_(key) {}
      
      template <typename element_src_type, typename element_dest_type>
      void operator()(element_src_type const & el_src, element_dest_type const & el_dest)
      {
        //std::cout << "Get data from element" << std::endl;
        access<key_type, value_type>(key_)(el_dest) = access<key_type, value_type>(key_)(el_src);
        erase<key_type, value_type>(key_)(el_src);
      }
    private:
      key_type const & key_;
  };
  
  
  template <typename key_type, typename value_type>
  data_mover_with_key<key_type, value_type> move(key_type const & key)
  {
    return data_mover_with_key<key_type, value_type>(key);
  }
  
  
  template <typename key_type, typename value_type>
  class data_mover_no_key
  {
    public:
      data_mover_no_key() {}
      
      template <typename element_src_type, typename element_dest_type>
      void operator()(element_src_type const & el_src, element_dest_type const & el_dest)
      {
        data_container<key_type, value_type, element_src_type>::instance().move(el_src, el_dest);
      }
  };
  
  template <typename key_type, typename value_type>
  data_mover_no_key<key_type, value_type> move()
  {
    return data_mover_no_key<key_type, value_type>();
  }
  
  
  //////////////////////// Copy data //////////////////////////////////
  template <typename key_type, typename value_type>
  class data_copy_with_key
  {
    public:
      data_copy_with_key(key_type const & key) : key_(key) {}
      
      template <typename element_src_type, typename element_dest_type>
      void operator()(element_src_type const & el_src, element_dest_type const & el_dest)
      {
        access<key_type, value_type>(key_)(el_dest) = access<key_type, value_type>(key_)(el_src);
      }
    private:
      key_type const & key_;
  };
  
  //prevent messing around with all-type and supplied key:
  template <typename key_type>
  class data_copy_with_key<key_type, all>
  {
    typedef typename error_indicator<key_type>::ERROR_NO_KEY_ARGUMENT_ALLOWED_WHEN_USING_COPY_FUNCTION_WITH_ALL   error_type;
  };

  template <typename value_type>
  class data_copy_with_key<all, value_type>
  {
    typedef typename error_indicator<value_type>::ERROR_NO_KEY_ARGUMENT_ALLOWED_WHEN_USING_COPY_FUNCTION_WITH_ALL   error_type;
  };

  template <>
  class data_copy_with_key<all, all>
  {
    //typedef typename key_type::ERROR_NO_KEY_ARGUMENT_ALLOWED_WHEN_USING_COPY_FUNCTION_WITH_ALL   error_type;
  };
  
  template <typename key_type, typename value_type>
  data_copy_with_key<key_type, value_type> copy(key_type const & key)
  {
    return data_copy_with_key<key_type, value_type>(key);
  }
  
  
  template <typename key_type, typename value_type>
  class data_copy_no_key
  {
    public:
      data_copy_no_key() {}
      
      template <typename element_src_type, typename element_dest_type>
      void operator()(element_src_type const & el_src, element_dest_type const & el_dest)
      {
        data_container<key_type, value_type, element_src_type>::instance().copy(el_src, el_dest);
      }
  };
  
  template <typename key_type>
  class data_copy_no_key <key_type, all>
  {
    public:
      data_copy_no_key() {}
      
      template <typename element_type>
      void operator()(element_type const & el_src, element_type const & el_dest)
      {
        key_value_registration<element_type>::instance().template copy_key_all<key_type>(el_src, el_dest);
      }
      
      template <typename element_src_type, typename element_dest_type>
      void operator()(element_src_type const & el_src, element_dest_type const & el_dest)
      {
        typedef typename error_indicator<element_src_type>::ERROR_SOURCE_AND_DESTINATION_MUST_BE_OF_SAME_TYPE_WHEN_USING_COPY_WITH_ALL  error_type;
      }
  };

  template <typename value_type>
  class data_copy_no_key <all, value_type>
  {
    public:
      data_copy_no_key() {}
      
      template <typename element_type>
      void operator()(element_type const & el_src, element_type const & el_dest)
      {
        key_value_registration<element_type>::instance().template copy_all_value<value_type>(el_src, el_dest);
      }
      
      template <typename element_src_type, typename element_dest_type>
      void operator()(element_src_type const & el_src, element_dest_type const & el_dest)
      {
        typedef typename error_indicator<element_src_type>::ERROR_SOURCE_AND_DESTINATION_MUST_BE_OF_SAME_TYPE_WHEN_USING_COPY_WITH_ALL  error_type;
      }
  };
  
  template <>
  class data_copy_no_key <all, all>
  {
    public:
      data_copy_no_key() {}
      
      template <typename element_type>
      void operator()(element_type const & el_src, element_type const & el_dest)
      {
        key_value_registration<element_type>::instance().copy_all_all(el_src, el_dest);
      }
      
      template <typename element_src_type, typename element_dest_type>
      void operator()(element_src_type const & el_src, element_dest_type const & el_dest)
      {
        typedef typename error_indicator<element_src_type>::ERROR_SOURCE_AND_DESTINATION_MUST_BE_OF_SAME_TYPE_WHEN_USING_COPY_WITH_ALL  error_type;
      }
  };
  
  
  
  template <typename key_type, typename value_type>
  data_copy_no_key<key_type, value_type> copy()
  {
    return data_copy_no_key<key_type, value_type>();
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
  
  
  /////////// find sparse data /////////////
  template <typename key_type, typename value_type>
  class find_proxy
  {
    public:
      find_proxy(key_type const & key) : key_(key) {}
      
      template <typename element_type>
      value_type * operator()(element_type const & el)
      {
        return data_container<key_type, value_type, element_type>::instance().find(el, key_);
      }
    
    private:
      key_type const & key_;
  };

  template <typename key_type, typename value_type>
  find_proxy<key_type, value_type> find(key_type const & key)
  {
    return find_proxy<key_type, value_type>(key);
  }
  
} //namespace viennadata

#endif 

