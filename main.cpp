
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>

//#include "QuantityManager/quantitymanager.h"
#include "testbench.h"

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







int main(int argc, char *argv[])
{

  testBasicOperations();
  //testStorageKeyWrappers();

  return EXIT_SUCCESS;
}
