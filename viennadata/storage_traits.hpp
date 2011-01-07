/* =======================================================================
   Copyright (c) 2010, Institute for Microelectronics, TU Vienna.
   http://www.iue.tuwien.ac.at
                             -----------------
                     ViennaData - The Vienna Data Storage Library
                             -----------------

   authors:    Karl Rupp                          rupp@iue.tuwien.ac.at

   license:    MIT (X11), see file LICENSE in the ViennaData base directory
======================================================================= */

#ifndef VIENNADATA_STORAGE_TRAITS_HPP
#define VIENNADATA_STORAGE_TRAITS_HPP

#include "viennadata/forwards.h"

namespace viennadata
{

  /** @brief A traits class that specifies for whether data should be stored in a dense (i.e. vector) or a sparse (i.e. map) manner.
   * 
   * For dense storage it is required that ViennaData knows how to get ids from the elements (see element_identifier)
   * 
   * @tparam key_type      The type of the key used for access
   * @tparam value_type    Type of the data that is stored for the element
   * @tparam element_type  The type of the object the data is associated with
   */
  template <typename key_type,
            typename value_type,
            typename element_type>
  struct storage_traits
  {
    typedef sparse_data_tag    tag;
  };

  //add here some custom element identifier
  
  
} //namespace viennadata


#endif

