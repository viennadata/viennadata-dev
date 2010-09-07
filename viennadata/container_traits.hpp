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


  template <typename key_type,
            typename value_type,
            typename element_type,
            typename element_identification_tag = typename element_identifier<element_type>::tag,
            typename key_dispatch_tag = typename dispatch_traits<key_type>::tag,
            typename storage_tag = typename storage_traits<key_type, value_type, element_type>::tag>
  struct container_traits
  {
    //this is the default case:
    // - pointer-based element identification
    // - full key dispatch
    // - sparse storage

    // the datatype:
    typedef std::map< const element_type *, std::map< key_type, value_type > >  container_type;

    // the accessors:
    static value_type & access(container_type & cont, element_type const & element, key_type const & key)
    {
       return cont[&element][key];
    }

    static value_type & access(container_type & cont, element_type const & element)
    {
      //not allowed in this case!
      typedef typename container_type::ERROR_ACCESS_WITHOUT_KEY_ARGUMENT_SUPPLIED   error_type;
    }

  };


} // namespace viennadata

#endif
