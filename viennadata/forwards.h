/* =======================================================================
   Copyright (c) 2010, Institute for Microelectronics, TU Vienna.
   http://www.iue.tuwien.ac.at
                             -----------------
                     ViennaData - The Vienna Data Storage Library
                             -----------------

   authors:    Karl Rupp                          rupp@iue.tuwien.ac.at

   license:    MIT (X11), see file LICENSE in the ViennaMesh base directory
======================================================================= */


  /******************* ID Handling *******************************/

#ifndef VIENNADATA_FORWARDS_H
#define VIENNADATA_FORWARDS_H

#include <map>
#include <vector>
#include <list>
#include <set>
#include <iostream>
#include <algorithm>
#include <memory>



namespace viennadata
{
  
    ////////////////////// tags ////////////////////////
        //forward declaration for disabling storage banks:
    struct no_storage_bank_tag;

    //element identification:
    struct pointer_based_id {};  //use object address as ID


    //storage policy tag:
    struct dense_data_tag {};      //for a property that is accessed on every element *and* is required to be fast
    struct sparse_data_tag {};     //properties that will be accessed on some elements only. Default policy.

    
    //key dispatch based on type only
    struct full_key_dispatch_tag {};
    struct type_key_dispatch_tag {};

    
    // add further performance-critical keys here

  
} //namespace

#endif
