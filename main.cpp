
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

//#include "QuantityManager/quantitymanager.h"

/*
remove  <key_type, value_type>(key)(obj);
data    <key_type, value_type>(key)(obj) = value;
value = data     <key_type, value_type>(key)(obj);
reserve<key_type, value_type>()(obj)
transfer<key_type, value_type>(key)(obj_src, obj_dest); //think about it!
transfer<key_type, value_type>()(obj_src, obj_dest);

transfer<key_type, all       >()(obj_src, obj_dest);
transfer<all,      value_type>()(obj_src, obj_dest);
transfer<all,      all       >()(obj_src, obj_dest); */


#include "viennadata/interface.hpp"
#include "tests/testbench.h"


int main(int argc, char *argv[])
{
  check_data_access();

  return EXIT_SUCCESS;
}
