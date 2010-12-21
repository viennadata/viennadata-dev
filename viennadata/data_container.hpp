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
#include "viennadata/key_value_registration.hpp"

namespace viennadata
{

  /** @brief The central container class holding the data
   * 
   * @tparam key_type      The type of the key used for access
   * @tparam value_type    Type of the data that is stored for the element
   * @tparam element_type  The type of the object the data is associated with
   * 
   */
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

      //////////////////  copy data: ////////////////////
      
      //multiple copy - stage 1: this is the source, forward the request to the destination:
      template <typename element_dest_type>
      void copy(element_type const & el_src, element_dest_type const & el_dest)
      {
        data_container<key_type, value_type, element_dest_type>::instance().copy(container, el_src, el_dest);
      }
      
      
      //multiple copy - stage 2: this is the destination: Perform the copy
      template <typename container_src_type, typename element_src_type>
      void copy(container_src_type & cont_src, element_src_type const & el_src,
                element_type const & el_dest)
      {
        container_traits<key_type, value_type, element_type>::copy(cont_src, el_src, container, el_dest);
      }

      //////////////////  move data: ////////////////////
      //multiple move:
      template <typename element_dest_type>
      void move(element_type const & el_src, element_dest_type const & el_dest)
      {
        copy(el_src, el_dest);
        erase(el_src);
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
        key_value_registration<element_type>::instance().template add<key_type, value_type>();
        
        //std::cout << "Reserving..." << std::endl;
        container_traits<key_type, value_type, element_type>::reserve(container, num);
      }
      
      value_type * find(element_type const & el, key_type const & key)
      {
        return container_traits<key_type, value_type, element_type>::find(container, el, key);
      }

  };



} // namespace viennadata

#endif
