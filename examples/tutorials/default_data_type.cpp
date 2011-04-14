/* =======================================================================
   Copyright (c) 2010, Institute for Microelectronics, TU Vienna.
   http://www.iue.tuwien.ac.at
                             -----------------
                     ViennaData - The Vienna Data Storage Library
                             -----------------

   authors:    Karl Rupp                             rupp@iue.tuwien.ac.at
   authors:    Markus Bina (boost.test interface)    bina@iue.tuwien.ac.at

   license:    MIT (X11), see file LICENSE in the ViennaData base directory
======================================================================= */


#include "viennadata/api.hpp"



//
// Some class defined by the user (can be anything)
//
struct StandardClass { /* possibly sophisticated internals here */ };

struct MyKey {};

namespace viennadata
{
  namespace config
  {
    template <>
    struct default_data_for_key<MyKey>
    {
      typedef double    type;
    };
    
    template <>
    struct key_dispatch<MyKey>
    {
      typedef type_key_dispatch_tag    tag;
    };
  }
}



int main(int argc, char *argv[])
{
    StandardClass obj1;
    StandardClass obj2;

    //
    // Store a bit of data on obj1 and obj2
    //
    viennadata::access<MyKey, double>()(obj1) = 3.1415;
    viennadata::access<MyKey>()(obj1) = 3.1415;

    viennadata::access<MyKey, double>()(obj2) = 2.71;
    viennadata::access<MyKey>()(obj2) = 2.71;

    //
    // Output data:
    //
    std::cout << "--- Data for obj1 ---" << std::endl;
    std::cout << "Data (type 'double') for key 'c' (type 'char'): " << viennadata::access<MyKey, double>()(obj1) << std::endl;
    std::cout << "Data (type 'std::string') for key 'c' (type 'char'): " << viennadata::access<MyKey>()(obj1) << std::endl;
    std::cout << std::endl;
    std::cout << "--- Data for obj2 ---" << std::endl;
    std::cout << "Data (type 'long') for key '42' (type 'long'): " << viennadata::access<MyKey, double>()(obj2) << std::endl;
    std::cout << "Data (type 'std::string') for key '3.1415' (type 'double'): " << viennadata::access<MyKey>()(obj2) << std::endl;

  return EXIT_SUCCESS;
}
