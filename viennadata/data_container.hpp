/* =======================================================================
   Copyright (c) 2010, Institute for Microelectronics, TU Vienna.
   http://www.iue.tuwien.ac.at
                             -----------------
                     ViennaData - The Vienna Data Storage Library
                             -----------------

   authors:    Karl Rupp                          rupp@iue.tuwien.ac.at

   license:    MIT (X11), see file LICENSE in the ViennaData base directory
======================================================================= */

#ifndef VIENNADATA_DATA_CONTAINER_HPP
#define VIENNADATA_DATA_CONTAINER_HPP


#include <map>

#include "viennadata/forwards.h"
#include "viennadata/element_identifier.hpp"
#include "viennadata/container_traits.hpp"

namespace viennadata
{

  
  template <typename key_type,
            typename value_type,
            typename element_type>
  class data_container
  {
    private:
      typedef typename container_traits<key_type, value_type, element_type>::container_type    container_type;

      container_type container; //here is the data stored

      data_container() {};
      data_container(data_container const & other) {}
      ~data_container()
      {
        //if (da != NULL) delete da;
      }

    public:
      static data_container & instance()
      {
        static data_container * da = NULL;
        if (da == NULL)
          da = new data_container();

        return *da;
      }

      value_type & access(element_type const & el, key_type const & key)
      {
        return container_traits<key_type, value_type, element_type>::access(container, el, key);
      }

      // it may be sufficient not to provide a key if type-based dispatching is performed
      value_type & access(element_type const & el)
      {
        return container_traits<key_type, value_type, element_type>::access(container, el);
      }


      // erase data associated with a key
      void erase(element_type const & el, key_type const & key)
      {
        container_traits<key_type, value_type, element_type>::erase(container, el, key);
      }

      // erase data associated with all keys of key_type
      void erase(element_type const & el)
      {
        container_traits<key_type, value_type, element_type>::erase(container, el);
      }

      // reserve memory if a vector type is used
      void reserve(long num)
      {
        //std::cout << "Reserving..." << std::endl;
        container_traits<key_type, value_type, element_type>::resize(container, num);
      }
      
      value_type * find(element_type const & el, key_type const & key)
      {
        return container_traits<key_type, value_type, element_type>::find(container, el, key);
      }

  };



} // namespace viennadata

#endif
