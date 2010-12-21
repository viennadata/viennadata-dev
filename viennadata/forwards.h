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
    struct pointer_based_id {};      //use object address as ID
    struct element_provided_id {};   //object provides some ID with values in 0...N


    //storage policy tag:
    struct dense_data_tag {};      //for a property that is accessed on every element *and* is required to be fast
    struct sparse_data_tag {};     //properties that will be accessed on some elements only. Default policy.

    
    
    struct full_key_dispatch_tag {};
    struct type_key_dispatch_tag {};  //key dispatch based on type only

    
    // add further performance-critical keys here
    
    
    
    //////////////////// function declarations ///////////////////////
    
    template <typename key_type, typename value_type>
    class data_accessor_with_key;
    
    template <typename key_type, typename value_type>
    class data_accessor_no_key;
    
    
    template <typename key_type, typename value_type>
    data_accessor_with_key<key_type, value_type> access(key_type const & key);

    template <typename key_type, typename value_type>
    data_accessor_no_key<key_type, value_type> access();
  
    
    
    ////////////////////// other stuff ////////////////////////
    template <typename key_type,
              typename value_type,
              typename element_type>
    class data_container;
    
    struct all {};   //denotes all types
    
    template <typename element_type_src>
    class key_value_copy_construction;
    
    template <typename element_type_src>
    class key_value_copy_construction_wrapper;
  
    template <typename T>
    struct error_indicator {};
  
    
  
} //namespace

#endif
