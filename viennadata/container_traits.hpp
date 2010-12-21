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
  // 8 possibilities in total:
  // [pointer based|id based] identification
  // [full key|no key] dispatch
  // [sparse|dense] storage
  
  
  // helper: check for valid access without key:
  template <typename key_dispatch_tag>
  struct IS_ACCESS_WITHOUT_KEY_ALLOWED
  {
    typedef typename key_dispatch_tag::ERROR_ACCESS_WITHOUT_KEY_ARGUMENT_SUPPLIED  error_type;
  };
  
  //force error when used without key:
  template <>
  struct IS_ACCESS_WITHOUT_KEY_ALLOWED<type_key_dispatch_tag>
  {
    typedef void value_type; //some dummy type definition
  };
  
  
  
  
  
  //helper: reserve memory based on identification mechanism
  template <typename container_type,
            typename element_identification_tag,
            typename storage_tag>
  struct container_reservation_dispatcher
  {
    //by default, do nothing: (some map-based mechanism)
    static void reserve(container_type & container, long num) {  }
  };
  
  template <typename container_type>
  struct container_reservation_dispatcher<container_type, element_provided_id, dense_data_tag>
  {
    //id based identification:
    static void reserve(container_type & container, long num) { container.resize(num); }
  };
  
  
  
  
  
  ////////////////// container_key_value_pair ////////////////////////////
  template <typename key_type,
            typename value_type,
            typename key_dispatch_tag>
  struct container_key_value_pair
  {
    typedef std::map< key_type, value_type >   key_value_type;  // with key object based dispatch    
    
    template <typename container_type, typename id_type>
    static value_type & access(container_type & cont, id_type const & id, key_type const & key)
    {
      return cont[id][key];
    }
  };
            
  template <typename key_type,
            typename value_type>
  struct container_key_value_pair <key_type, value_type, type_key_dispatch_tag>
  {
    typedef value_type   key_value_type;  // with key object based dispatch    
    
    template <typename container_type, typename id_type>
    static value_type & access(container_type & cont, id_type const & id, key_type const & key)
    {
      return cont[id];
    }

    //also allow access without key here, because uniquely defined:
    template <typename container_type, typename id_type>
    static value_type & access(container_type & cont, id_type const & id)
    {
      return cont[id];
    }
  };
  
  ///////////////////////// erasure dispatch //////////////////////////
  
  //general case: call erase of the underlying map-type
  /** @brief A dispatch facility that unwraps type-erasued key-value type pairs
   * 
   * Refer to Alexandrescu, "Modern C++ Design" for details on type erasure
   * 
   * @tparam key_type      The type of the key used for access
   * @tparam value_type    Type of the data that is stored for the element
   * @tparam element_type  The type of the object the data is associated with
   * @tparam element_identification_tag  Determines the dispatch mechanism for objects the data is associated with. Either 'pointer_based_id' (the default) or 'element_provided_id' (requires suitable overload of 'element_identifier')
   * @tparam key_dispatch_tag   Determines the method for key dispatch (either full dispatch ('full_key_dispatch_tag', default) or dispatch based on key type 'type_key_dispatch_tag')
   * @tparam storage_tag   Specifies dense ('dense_data_tag') or sparse ('storage_data_tag', default) storage of data. The former requires element-identification by id, cf. class 'element_identifier'
   */
  template <typename key_type, typename value_type,
            typename element_identification_tag,
            typename key_dispatch_tag,
            typename storage_tag>
  struct container_erasure_dispatcher
  {
    
    template <typename container_type, typename id_type>
    static void erase(container_type & cont, id_type const & id, key_type const & key)
    {
      cont[id].erase(key);
    }
    
    template <typename container_type, typename id_type>
    static void erase(container_type & cont, id_type const & id)
    {
      cont.erase(id);
    }
    
  };

  //using a type-based key dispatch, erase from id-map:
  template <typename key_type, typename value_type, typename element_identification_tag, typename storage_tag>
  struct container_erasure_dispatcher < key_type, value_type, element_identification_tag, type_key_dispatch_tag, storage_tag >
  {
    template <typename container_type, typename id_type>
    static void erase(container_type & cont, id_type const & id, key_type const & key)
    {
      cont.erase(id);
    }
    
    template <typename container_type, typename id_type>
    static void erase(container_type & cont, id_type const & id)
    {
      cont.erase(id);
    }
  };  

  //for a dense data storage with vectors, reset element:
  template <typename key_type, typename value_type, typename key_dispatch_tag>
  struct container_erasure_dispatcher < key_type, value_type, element_provided_id, key_dispatch_tag, dense_data_tag >
  {
    typedef typename container_key_value_pair< key_type,
                                               value_type,
                                               key_dispatch_tag >::key_value_type   key_value_type;
                                               
    template <typename container_type, typename id_type>
    static void erase(container_type & cont, id_type const & id, key_type const & key)
    {
      cont[id] = key_value_type();
    }
    
    template <typename container_type, typename id_type>
    static void erase(container_type & cont, id_type const & id)
    {
      cont[id] = key_value_type();
    }
  };

  //resolve ambiguity for above specializations
  template <typename key_type, typename value_type>
  struct container_erasure_dispatcher < key_type, value_type, element_provided_id, type_key_dispatch_tag, dense_data_tag >
  {
    typedef typename container_key_value_pair< key_type,
                                               value_type,
                                               type_key_dispatch_tag >::key_value_type   key_value_type;
                                               
    template <typename container_type, typename id_type>
    static void erase(container_type & cont, id_type const & id, key_type const & key)
    {
      cont[id] = key_value_type();
    }
    
    template <typename container_type, typename id_type>
    static void erase(container_type & cont, id_type const & id)
    {
      cont[id] = key_value_type();
    }
  };

  
  
  ///////////////////////// storage traits //////////////////////////
  
  
  //helper: deduce correct storage type:  
  template <typename key_type,
            typename value_type,
            typename element_type,
            typename element_identification_tag,
            typename key_dispatch_tag,
            typename storage_tag>
  struct container_storage_traits
  {
    //default case:
    typedef typename element_identifier<element_type>::id_type     id_type;
    typedef std::map< id_type,
                      typename container_key_value_pair< key_type,
                                                         value_type,
                                                         key_dispatch_tag >::key_value_type
                    >                                              container_type;
                    
     static void reserve(container_type & cont, long num) { }
  };
  
  //dense storage when providing ID
  template <typename key_type,
            typename value_type,
            typename element_type,
            typename key_dispatch_tag>
  struct container_storage_traits <key_type, value_type, element_type,
                                   element_provided_id, key_dispatch_tag, dense_data_tag>
  {
    typedef typename element_identifier<element_type>::id_type     id_type;
    typedef std::vector< typename container_key_value_pair< key_type,
                                                            value_type,
                                                            key_dispatch_tag >::key_value_type
                       >                                           container_type;
                       
     static void reserve(container_type & cont, long num) { cont.resize(num); }                  
  };
  
  
  // Case 1: 
  // This is the default case:
  // - pointer based identification
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
    typedef typename container_storage_traits<key_type, value_type, element_type,
                                              element_identification_tag,
                                              key_dispatch_tag,
                                              storage_tag>::container_type    container_type;

    // the accessors:
    static value_type & access(container_type & cont, element_type const & element, key_type const & key)
    {
       //std::cout << "Accessing sparse data by pointer" << std::endl;
       return container_key_value_pair <key_type,
                                        value_type,
                                        key_dispatch_tag>::access(cont, element_identifier<element_type>::id(element), key);
       //return cont[&element][key];
    }

    static value_type & access(container_type & cont, element_type const & element)
    {
      typedef typename IS_ACCESS_WITHOUT_KEY_ALLOWED<key_dispatch_tag>::value_type   some_type;
       return container_key_value_pair <key_type,
                                        value_type,
                                        key_dispatch_tag>::access(cont, element_identifier<element_type>::id(element));
    }

    //multiple copy (including degenerate case of type based dispatch, where only single data is moved):
    template <typename container_src_type, typename element_src_type>
    static void copy(container_src_type & cont_src, element_src_type const & el_src,
                     container_type & cont_dest, element_type const & el_dest)
    {
      //TODO: can be improved if cont_src[id_src] is actually empty, because there is no move necessary then...
       cont_dest[element_identifier<element_type>::id(el_dest)] = cont_src[element_identifier<element_src_type>::id(el_src)];
    }
    

    // resizing:
    static void reserve(container_type & cont, long num)
    {
       container_storage_traits<key_type, value_type, element_type,
                                element_identification_tag,
                                key_dispatch_tag,
                                storage_tag>::reserve(cont, num);
    }

    // erase data for a particular key:
    static void erase(container_type & cont, element_type const & element, key_type const & key)
    {
      //cont[element_identifier<element_type>::id(element)].erase(key);
      container_erasure_dispatcher<key_type, value_type,
                                       element_identification_tag,
                                       key_dispatch_tag,
                                       storage_tag>::erase(cont, element_identifier<element_type>::id(element), key);
    }

    // erase data for all keys of that type
    static void erase(container_type & cont, element_type const & element)
    {
      container_erasure_dispatcher<key_type, value_type,
                                       element_identification_tag,
                                       key_dispatch_tag,
                                       storage_tag>::erase(cont, element_identifier<element_type>::id(element));
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



} // namespace viennadata

#endif
