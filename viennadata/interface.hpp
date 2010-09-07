/* =======================================================================
   Copyright (c) 2010, Institute for Microelectronics, TU Vienna.
   http://www.iue.tuwien.ac.at
                             -----------------
                     ViennaData - The Vienna Data Storage Library
                             -----------------

   authors:    Karl Rupp                          rupp@iue.tuwien.ac.at

   license:    MIT (X11), see file LICENSE in the ViennaData base directory
======================================================================= */

#include <map>

namespace viennadata
{

  template <typename key_type,
            typename value_type,
            typename element_type>
  class data_archive
  {
    private:
      data_archive() {};
      data_archive(data_archive const & other) {}
      ~data_archive()
      {
        //if (da != NULL) delete da;
      }

      std::map< const element_type *, std::map< key_type, value_type > >  container; 

    public:
      static data_archive & instance()
      {
        static data_archive * da = NULL;
        if (da == NULL)
          da = new data_archive();

        return *da;
      }

      value_type & operator()(element_type const & el, key_type const & key)
      {
        return container[&el][key];
      }
  };


  
  template <typename key_type, typename value_type>
  class data_getter
  {
    public:
      data_getter(key_type const & key) : key_(key) {}
      
      template <typename element_type>
      value_type & operator()(element_type const & el)
      {
        //std::cout << "Get data from element" << std::endl;
        return data_archive<key_type, value_type, element_type>::instance()(el, key_);
      }
      
    private:
      key_type const & key_;
    
  };
  
  
  // data getter and setter:
  template <typename key_type, typename value_type>
  data_getter<key_type, value_type> access(key_type const & key)
  {
    return data_getter<key_type, value_type>(key);
  }
  
    
  
}
