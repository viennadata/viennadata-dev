#ifndef VIENNADATA_CONTAINER_ACCESS_HPP
#define VIENNADATA_CONTAINER_ACCESS_HPP

/* =======================================================================
   Copyright (c) 2011-2013, Institute for Microelectronics,
                            Institute for Analysis and Scientific Computing,
                            TU Wien.

                            -----------------
                     ViennaData - The Vienna Data Library
                            -----------------

   Authors:      Florian Rudolf                    rudolf@iue.tuwien.ac.at
                 Karl Rupp                           rupp@iue.tuwien.ac.at

   (A list of additional contributors can be found in the PDF manual)

   License:      MIT (X11), see file LICENSE in the base directory
======================================================================= */

#include <map>

#include "viennadata/forwards.hpp"
#include "viennadata/meta/result_of.hpp"

namespace viennadata
{
  /** @brief Class that manages container access, depends on container_type, element_type and access_tag */
  template<typename ContainerType, typename ElementType, typename AccessTag>
  struct container_access;

  /** @brief Default implementation: random access container using offset */
  template<typename ContainerType, typename ElementType, typename AccessTag>
  struct container_access
  {
    typedef ContainerType                            container_type;
    typedef ElementType                              element_type;
    typedef typename container_type::value_type      value_type;

    static void erase(container_type & container, element_type const & element)
    {
      typedef typename result_of::offset< typename element_type::id_type >::type offset_type;
      offset_type offset = result_of::offset< typename element_type::id_type >::get(element.id());

      if (offset-1 == container.size()) // container is shrinked only when deleting data for last element
          container.resize(container.size()-1);
    }

    static typename container_type::pointer find(container_type & container, element_type const & element)
    {
      typedef typename result_of::offset< typename element_type::id_type >::type     offset_type;
      offset_type offset = result_of::offset< typename element_type::id_type >::get(element.id());

      return (container.size() > offset) ? (&container[offset]) : NULL; // return NULL if not found
    }

    static typename container_type::const_pointer find(container_type const & container, element_type const & element)
    {
      typedef typename result_of::offset< typename element_type::id_type >::type      offset_type;

      offset_type offset = result_of::offset< typename element_type::id_type >::get(element.id());

      return (container.size() > offset) ? (&container[offset]) : NULL; // return NULL if not found
    }
    
    static typename container_type::reference lookup_unchecked(container_type & container, element_type const & element) // no offset checking
    {
      typedef typename result_of::offset< typename element_type::id_type >::type      offset_type;
      offset_type offset = result_of::offset< typename element_type::id_type >::get(element.id());
      return container[offset];
    }

    static typename container_type::const_reference lookup_unchecked(container_type const & container, element_type const & element)
    {
      typedef typename result_of::offset< typename element_type::id_type >::type      offset_type;
      
      offset_type offset = result_of::offset< typename element_type::id_type >::get(element.id());
      
      assert(container.size() > offset); // no release-runtime check for accessing elements outside container
      return container[offset];
    }

    static typename container_type::reference lookup(container_type & container, element_type const & element)
    {
      typedef typename result_of::offset< typename element_type::id_type >::type      offset_type;
      typedef typename container_type::size_type                                       size_type;

      offset_type offset = result_of::offset< typename element_type::id_type >::get(element.id());

      if (container.size() <= static_cast<size_type>(offset)) container.resize(offset+1); // ensure that container is big enough
      return container[offset];
    }

    static typename container_type::const_reference lookup(container_type const & container, element_type const & element)
    {
      return lookup_unchecked(container, element); // using unchecked lookup
    }

    static void copy(container_type & container, element_type const & src_element, element_type const & dst_element)
    {
      lookup(container, dst_element) = lookup(container, src_element);
    }
    
    static void resize(container_type & container, std::size_t size)
    {
      container.resize( size );
    }
  };


  // specialization for std::map
  template<typename KeyType, typename ValueType , typename Compare, typename Alloc, typename ElementType, typename AccessTag>
  struct container_access<std::map<KeyType, ValueType, Compare, Alloc>, ElementType, AccessTag>
  {
    typedef KeyType                                             key_type;
    typedef std::map<KeyType, ValueType, Compare, Alloc>        container_type;
    typedef ElementType                                         element_type;
    typedef ValueType                                           value_type;

    static void erase(container_type & container, element_type const & element)
    {
      typename result_of::access_type<element_type, AccessTag>::type access(element);

      typename container_type::iterator it = container.find(access);
      if (it != container.end())
        container.erase(it);
    }

    static typename container_type::pointer find(container_type & container, element_type const & element)
    {
      typename result_of::access_type<element_type, AccessTag>::type access(element);

      typename container_type::iterator it = container.find(access);
      return (it != container.end()) ? &it->second : NULL; // return NULL if not found
    }

    static typename container_type::const_pointer find(container_type const & container, element_type const & element)
    {
      typename result_of::access_type<element_type, AccessTag>::type access(element);

      typename container_type::const_iterator it = container.find(access);
      return (it != container.end()) ? &it->second : NULL; // return NULL if not found
    }

    static typename container_type::reference lookup_unchecked(container_type & container, element_type const & element)
    {
      typename result_of::access_type<element_type, AccessTag>::type access(element);
      return container[access];
    }

    static typename container_type::const_reference lookup_unchecked(container_type const & container, element_type const & element)
    {
      typename result_of::access_type<element_type, AccessTag>::type access(element);
      typename container_type::const_iterator it = container.find(access);

      assert(it != container.end()); // no release-runtime check for accessing elements outside container

      return it->second;
    }
    
    static typename container_type::reference lookup(container_type & container, element_type const & element)
    {
      return lookup_unchecked(container, element); // using unchecked lookup
    }

    static typename container_type::const_reference lookup(container_type const & container, element_type const & element)
    {
      return lookup_unchecked(container, element); // using unchecked lookup
    }

    static void copy(container_type & container, element_type const & src_element, element_type const & dst_element)
    {
      lookup(container, dst_element) = lookup(container, src_element);
    }
    
    static void resize(container_type & container, std::size_t size) {} // not supported
  };



} // namespace viennadata
#endif
