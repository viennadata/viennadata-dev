/* =======================================================================
   Copyright (c) 2010, Institute for Microelectronics, TU Vienna.
   http://www.iue.tuwien.ac.at
                             -----------------
                     ViennaData - The Vienna Data Storage Library
                             -----------------

   authors:    Karl Rupp                          rupp@iue.tuwien.ac.at

   license:    MIT (X11), see file LICENSE in the ViennaData base directory
======================================================================= */

#ifndef VIENNADATA_DISPATCH_TRAITS_HPP
#define VIENNADATA_DISPATCH_TRAITS_HPP

#include "viennadata/forwards.h"

namespace viennadata
{

  template <typename key_type>
  struct dispatch_traits
  {
    typedef full_key_dispatch_tag    tag;
  };

  //add here some custom element identifier
} //namespace viennadata


#endif

