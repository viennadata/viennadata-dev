/* =======================================================================
   Copyright (c) 2010, Institute for Microelectronics, TU Vienna.
   http://www.iue.tuwien.ac.at
                             -----------------
                     ViennaData - The Vienna Data Storage Library
                             -----------------

   authors:    Karl Rupp                             rupp@iue.tuwien.ac.at

   license:    MIT (X11), see file LICENSE in the ViennaData base directory
======================================================================= */

#include <string>
#include <iostream>

#include "viennadata/api.hpp"


//
// Tutorial: Basic use of viennadata::erase()
//


//
// Some class defined by the user (can be anything)
//
struct StandardClass { /* possibly sophisticated internals here */ };


//
// Helper function for printing data stored on objects:
//
void print(StandardClass const & obj)
{
    std::cout << "Key 'c', data type double: " << viennadata::access<char, double>('c')(obj) << std::endl;
    std::cout << "Key 'd', data type double: " << viennadata::access<char, double>('d')(obj) << std::endl;
    std::cout << "Key 'c', data type std::string: " << viennadata::access<char, std::string>('c')(obj) << std::endl;
    std::cout << "Key 'some_character', data type char: " << viennadata::access<std::string, char>("some_character")(obj) << std::endl;
}

int main(int /*argc*/, char ** /*argv*/)
{
    StandardClass obj1;

    //
    // Store some data for obj1
    //
    viennadata::access<char, double>('c')(obj1) = 3.1415;                  //using key of type char, data of type double
    viennadata::access<char, double>('d')(obj1) = 2.71;                    //using key of type char, data of type double
    viennadata::access<char, std::string>('c')(obj1) = "Hello";            //using key of type char, data of type std::string
    viennadata::access<std::string, char>("some_character")(obj1) = 'a';   //using key of type std::string, data of type char

    //
    // Output data for obj1:
    //
    std::cout << "--- Data for obj1 ---" << std::endl;
    print(obj1);
    
    //
    // Copying 'double' data with key 'c' to obj2
    //
    std::cout << std::endl;
    std::cout << "Erasing 'double' data with key 'c'... " << std::endl;
    viennadata::erase<char, double>('c')(obj1);
    std::cout << std::endl;
    
    std::cout << "--- Data for obj1 ---" << std::endl;
    print(obj1);
    

    //
    // Copying 'double' data with all keys of type char to obj2
    //
    std::cout << std::endl;
    std::cout << "Erasing 'double' data with all keys of type 'char'... " << std::endl;
    viennadata::erase<char, double>()(obj1);
    std::cout << std::endl;
    
    std::cout << "--- Data for obj1 ---" << std::endl;
    print(obj1);
    
    //
    // Copying all data that is stored using keys of type 'char' to obj2
    //
    std::cout << std::endl;
    std::cout << "Erasing all data that is stored using keys of type 'char'... " << std::endl;
    viennadata::erase<char, viennadata::all>()(obj1);
    std::cout << std::endl;
    
    std::cout << "--- Data for obj1 ---" << std::endl;
    print(obj1);

    //
    // Copying all data with all keys of type char to obj2
    //
    std::cout << std::endl;
    std::cout << "Erasing all data that is stored using any key for obj1... " << std::endl;
    viennadata::erase<viennadata::all, viennadata::all>()(obj1);
    std::cout << std::endl;
    
    std::cout << "--- Data for obj2 ---" << std::endl;
    print(obj1);
    
  return EXIT_SUCCESS;
}
