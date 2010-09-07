
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

//#include "QuantityManager/quantitymanager.h"
//#include "testbench.h"

/*
remove  <key_type, value_type>(key)(obj);
data    <key_type, value_type>(key)(obj) = value;
value = data     <key_type, value_type>(key)(obj);
transfer<key_type, value_type>(key)(obj_src, obj_dest); //think about it!
transfer<key_type, value_type>()(obj_src, obj_dest);

transfer<key_type, all       >()(obj_src, obj_dest);
transfer<all,      value_type>()(obj_src, obj_dest);
transfer<all,      all       >()(obj_src, obj_dest);
*/


#include "viennadata/interface.hpp"

/*
struct my_class {};

void new_interface()
{
  int key1 = 3;
  double value1 = 3.1415;
  char key2 = 4;
  double * value2 = 0;
  
  my_class obj;
  
  viennadata::data<int, double>(key1)(obj) = value1;
  
  
}
*/

class SomeClass {};




int main(int argc, char *argv[])
{

  //testBasicOperations();
  //testStorageKeyWrappers();

  char key_char_1 = 'c';
  char key_char_2 = 'd';
  long key_long_1 = 42;

  SomeClass obj;

  std::cout << "Setting some sample values:" << std::endl;
  viennadata::access<char, double>(key_char_1)(obj) = 1.0;
  viennadata::access<char, double>(key_char_2)(obj) = 2.0;
  viennadata::access<long, std::string>(key_long_1)(obj) = "Hello World!";


  std::cout << "Output:" << std::endl;
  std::cout << viennadata::access<char, double>(key_char_1)(obj) << std::endl;
  std::cout << viennadata::access<char, double>(key_char_2)(obj) << std::endl;
  std::cout << viennadata::access<long, std::string>(key_long_1)(obj) << std::endl;

  return EXIT_SUCCESS;
}
