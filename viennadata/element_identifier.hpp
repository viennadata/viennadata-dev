/* =======================================================================
   Copyright (c) 2010, Institute for Microelectronics, TU Vienna.
   http://www.iue.tuwien.ac.at
                             -----------------
                     ViennaData - The Vienna Data Storage Library
                             -----------------

   authors:    Karl Rupp                          rupp@iue.tuwien.ac.at

   license:    MIT (X11), see file LICENSE in the ViennaData base directory
======================================================================= */

#ifndef VIENNADATA_ELEMENT_IDENTIFIER_HPP
#define VIENNADATA_ELEMENT_IDENTIFIER_HPP

#include "viennadata/forwards.h"

namespace viennadata
{

  template <typename element_type>
  struct element_identifier
  {
    typedef pointer_based_id    tag;
  };

  //add here some custom element identifier

} //namespace viennadata


#endif

