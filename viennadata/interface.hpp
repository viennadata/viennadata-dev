/* =======================================================================
   Copyright (c) 2010, Institute for Microelectronics, TU Vienna.
   http://www.iue.tuwien.ac.at
                             -----------------
                     ViennaData - The Vienna Data Storage Library
                             -----------------

   authors:    Karl Rupp                          rupp@iue.tuwien.ac.at

   license:    MIT (X11), see file LICENSE in the ViennaData base directory
======================================================================= */


namespace viennadata
{
  double i;
  
  template <typename key_type, typename value_type>
  class data_getter
  {
    public:
      data_getter(key_type const & key) : key_(key) {}
      
      template <typename ElementType>
      value_type & operator()(ElementType const & el)
      {
        std::cout << "Get data from element" << std::endl;
        return i;
      }
      
    private:
      key_type const & key_;
    
  };
  
  
  // data getter and setter:
  template <typename key_type, typename value_type>
  data_getter<key_type, value_type> data(key_type const & key)
  {
    return data_getter<key_type, value_type>(key);
  }
  
  
  
  
  
  
  
  
  
}
