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

#ifndef VIENNADATA_KEY_VALUE_REGISTRATION_HPP
#define VIENNADATA_KEY_VALUE_REGISTRATION_HPP

#include <assert.h>
#include <map>
#include <vector>
#include <list>
#include <set>
#include <iostream>
#include <algorithm>
#include <memory>

#include "viennadata/key_value_erase.hpp"

namespace viennadata
{
  template <typename element_type>
  class key_value_registration
  {
      typedef std::vector<key_value_erase<element_type> >       container_type;
    
    public:
      
      static key_value_registration & instance()
      {
        static key_value_registration * kvr = NULL;
        if (kvr == NULL)
          kvr = new key_value_registration();

        return *kvr;
      }
      
      
      template <typename key_type, typename value_type>
      void add()
      {
        key_value_erase<element_type> kve;
        kve.template add<key_type, value_type>();
        
        //search whether already registered:
        if (std::find(container.begin(), container.end(), kve) == container.end())
          container.push_back(kve);
      }
      
      template <typename key_type, typename value_type>
      void remove()
      {
        typedef typename container_type::iterator   ContIter;
        
        key_value_erase<element_type> kve;
        kve.template add<key_type, value_type>();
        
        ContIter ci = std::find(container.begin(), container.end(), kve);
        if (ci != container.end())
          container.erase(ci);
      }
      
      
      
      template <typename key_type>
      void erase(all const &, element_type const & el) const
      {
        typedef typename container_type::const_iterator   ContIter;
        
        std::cout << "key_value_registration::erase() called!" << std::endl;
        
        key_value_erase<element_type> kve;
        kve.template add<key_type, all>();
        
        for (ContIter it = container.begin();
             it != container.end();
             ++it)
        {
          if (*it == kve)
          {
            std::cout << "FOUND!" << std::endl;
            kve.erase(el);
          }
          else
            std::cout << "not found!" << std::endl;
        }
        
      }
      
    private:
      container_type container;
  }; 
  
}

#endif