/* =======================================================================
   Copyright (c) 2010, Institute for Microelectronics, TU Vienna.
   http://www.iue.tuwien.ac.at
                             -----------------
                     ViennaData - The Vienna Data Storage Library
                             -----------------

   authors:    Karl Rupp                          rupp@iue.tuwien.ac.at

   license:    MIT (X11), see file LICENSE in the ViennaData base directory
======================================================================= */

#ifndef VIENNADATA_CONTAINER_TRAITS_HPP
#define VIENNADATA_CONTAINER_TRAITS_HPP

#include <map>

#include "viennadata/forwards.h"
#include "viennadata/element_identifier.hpp"
#include "viennadata/dispatch_traits.hpp"
#include "viennadata/storage_traits.hpp"

namespace viennadata
{

  // Case: 
  // This is the default case:
  // - full key dispatch
  // - sparse storage
  template <typename key_type,
            typename value_type,
            typename element_type,
            typename element_identification_tag = typename element_identifier<element_type>::tag,
            typename key_dispatch_tag = typename dispatch_traits<key_type>::tag,
            typename storage_tag = typename storage_traits<key_type, value_type, element_type>::tag>
  struct container_traits
  {
    // the datatype:
    typedef typename element_identifier<element_type>::id_type     id_type;
    typedef std::map< id_type, std::map< key_type, value_type > >  container_type;

    // the accessors:
    static value_type & access(container_type & cont, element_type const & element, key_type const & key)
    {
       //std::cout << "Accessing sparse data by pointer" << std::endl;
       return cont[element_identifier<element_type>::id(element)][key];
       //return cont[&element][key];
    }

    static value_type & access(container_type & cont, element_type const & element)
    {
      //not allowed in this case!
      typedef typename container_type::ERROR_ACCESS_WITHOUT_KEY_ARGUMENT_SUPPLIED   error_type;
    }

    // resizing:
    static void resize(container_type & cont, long num) {}

    // erase data for a particular key:
    static void erase(container_type & cont, element_type const & element, key_type const & key)
    {
      cont[element_identifier<element_type>::id(element)].erase(key);
    }

    // erase data for all keys of that type
    static void erase(container_type & cont, element_type const & element)
    {
      cont[element_identifier<element_type>::id(element)].clear();
    }

    //find
    static value_type * find(container_type & cont, element_type const & element, key_type const & key)
    {
      typename std::map< key_type, value_type >::iterator it = cont[element_identifier<element_type>::id(element)].find(key);
      if (it == cont[element_identifier<element_type>::id(element)].end())
        return NULL;
      
      return &(it->second);
    }

  };



  // Case: 
  // - no key dispatch
  // - sparse storage
  template <typename key_type,
            typename value_type,
            typename element_type>
  struct container_traits <key_type, value_type, element_type,
                           pointer_based_id, type_key_dispatch_tag, sparse_data_tag>
  {
    // the datatype:
    typedef std::map< const element_type *, value_type >  container_type;

    // the accessors:
    static value_type & access(container_type & cont, element_type const & element, key_type const & key)
    {
       //std::cout << "Accessing sparse data by pointer, key per type" << std::endl;
       return cont[element_identifier<element_type>::id(element)];
    }

    static value_type & access(container_type & cont, element_type const & element)
    {
       return cont[element_identifier<element_type>::id(element)];
    }

    static void resize(container_type & cont, long num) {}

    // erase data for a particular key:
    static void erase(container_type & cont, element_type const & element, key_type const & key)
    {
      cont.erase(element_identifier<element_type>::id(element));
    }

    // erase data for all keys of that type
    static void erase(container_type & cont, element_type const & element)
    {
      cont.erase(element_identifier<element_type>::id(element));
    }

    //find
    //find
    static value_type * find(container_type & cont, element_type const & element, key_type const & key)
    {
      typedef typename container_type::ERROR_FIND_IS_NOT_AVAILABLE_WHEN_USING_TYPE_BASED_KEY_DISPATCH   ErrorType;
    }

  };


  // Case: 
  // - full key dispatch
  // - dense storage
  template <typename key_type,
            typename value_type,
            typename element_type>
  struct container_traits <key_type, value_type, element_type,
                           element_provided_id, full_key_dispatch_tag, dense_data_tag>
  {
    // the datatype:
    typedef std::vector< std::map< key_type, value_type > >  container_type;

    // the accessors:
    static value_type & access(container_type & cont, element_type const & element, key_type const & key)
    {
       //std::cout << "Accessing index " << element_identifier<element_type>::id(element) << "/" << cont.size() << std::endl;
       //std::cout << "Accessing dense data by ID" << std::endl;
       return cont[element_identifier<element_type>::id(element)][key];
    }

    static value_type & access(container_type & cont, element_type const & element)
    {
      //not allowed in this case!
      typedef typename container_type::ERROR_ACCESS_WITHOUT_KEY_ARGUMENT_SUPPLIED   error_type;
    }

    static void resize(container_type & cont, long num) { cont.resize(num); }

    // erase data for a particular key:
    static void erase(container_type & cont, element_type const & element, key_type const & key)
    {
      cont[element_identifier<element_type>::id(element)].erase(key);
    }

    // erase data for all keys of that type
    static void erase(container_type & cont, element_type const & element)
    {
      cont[element_identifier<element_type>::id(element)].clear();
    }

    //find
    static value_type * find(container_type & cont, element_type const & element, key_type const & key)
    {
      typename std::map< key_type, value_type >::iterator it = cont[element_identifier<element_type>::id(element)].find(key);
      if (it == cont[element_identifier<element_type>::id(element)].end())
        return NULL;
      
      return &(it->second);
    }

  };

  // Case: 
  // - no key dispatch
  // - dense storage
  template <typename key_type,
            typename value_type,
            typename element_type>
  struct container_traits <key_type, value_type, element_type,
                           element_provided_id, type_key_dispatch_tag, dense_data_tag>
  {
    // the datatype:
    typedef std::vector< value_type >  container_type;

    // the accessors:
    static value_type & access(container_type & cont, element_type const & element, key_type const & key)
    {
       //std::cout << "Accessing dense data by ID, key per type" << std::endl;
       return cont[element_identifier<element_type>::id(element)];
    }

    static value_type & access(container_type & cont, element_type const & element)
    {
       return cont[element_identifier<element_type>::id(element)];
    }

    static void resize(container_type & cont, long num) { cont.resize(num); }

    // erase data for a particular key:
    static void erase(container_type & cont, element_type const & element, key_type const & key)
    {
      cont[element_identifier<element_type>::id(element)] = value_type();
    }

    // erase data for all keys of that type
    static void erase(container_type & cont, element_type const & element)
    {
      cont[element_identifier<element_type>::id(element)] = value_type();
    }

    //find
    static value_type * find(container_type & cont, element_type const & element, key_type const & key)
    {
      typedef typename container_type::ERROR_FIND_IS_NOT_AVAILABLE_WHEN_USING_TYPE_BASED_KEY_DISPATCH   ErrorType;
    }

  };


} // namespace viennadata

#endif
