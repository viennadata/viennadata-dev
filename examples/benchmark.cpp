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

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <vector>

#include "benchmark-utils.hpp"
#include "viennadata/interface.hpp"

class SlimClass
{
  public:
    SlimClass(double v = 1.0, size_t i = 0) : value_(v), id_(i) {}
    
    double value() const { return value_; }
    size_t id() const { return id_; }
  private:
    double value_;
    size_t id_;
};


template <size_t num_bytes>
class FatClass
{
  public:
    FatClass(double v = 1.0, size_t i = 0) : value_(v), id_(i) {}
    
    double value() const { return value_; }
    size_t id() const { return id_; }
  private:
    double value_;
    char payload[num_bytes];
    size_t id_;
};

struct data_key {};

//configure ViennaData:
namespace viennadata
{
  template <>
  struct object_identifier<SlimClass>
  {
    typedef object_provided_id    tag;
    typedef size_t                  id_type;

    static size_t id(SlimClass const & obj) { return obj.id(); }
  };
  template <long num_bytes>
  struct object_identifier<FatClass<num_bytes> >
  {
    typedef object_provided_id    tag;
    typedef long                   id_type;

    static long id(FatClass<num_bytes> const & obj) { return obj.id(); }
  };

  // tell ViennaData to use type-based dispatch for data_key:
  template <>
  struct dispatch_traits<data_key>
  {
    typedef type_key_dispatch_tag    tag;
  };


  // store doubles using data_key densely:
/*  template <typename T>
  struct storage_traits<T, double, SlimClass>
  {
    typedef dense_data_tag    tag;
  };*/
  
  template <>
  struct storage_traits<data_key, double, SlimClass>
  {
    typedef dense_data_tag    tag;
  };
    
}







template <typename T>
void run(size_t num, size_t repeat, bool use_viennadata = false)
{
  std::vector<T> objects(num);
  std::string key_string = "test";
  std::vector<double> refvector(num);

  std::cout << "Size of type: " << sizeof(T) << std::endl;

  if (use_viennadata)
  {
    viennadata::reserve<std::string, double>(num)(objects[0]);
    viennadata::reserve<long, double>(num)(objects[0]);
    viennadata::reserve<data_key, double>(num)(objects[0]);
  }
  
  for (size_t i=0; i<num; ++i)
  {
    refvector[i] = ::sqrt(i);
    objects[i] = T(::sqrt(i), i);
   
    if (use_viennadata)
    {
      viennadata::access<std::string, double>(key_string)(objects[i]) = objects[i].value();
      viennadata::access<std::string, double>("bla")(objects[i]) = objects[i].value();
      viennadata::access<long, double>(42)(objects[i]) = objects[i].value();
      viennadata::access<long, double>(7)(objects[i]) = objects[i].value();
      viennadata::access<data_key, double>()(objects[i]) = objects[i].value();    
      T dummy = viennadata::access<data_key, T >()(objects[i]);
    }
  }
  
  double result;
  double elapsed;
  Timer t;
  
  //
  //
  
  std::cout << " - Summing vector directly..." << std::endl;
  result = 0.0;
  t.start();
  for (size_t r=0; r<repeat; ++r)
    for (size_t i=0; i<num; ++i)
      result += refvector[i]; 
  elapsed = t.get();
  std::cout << "   Time elapsed: " << elapsed << std::endl;
  std::cout << "   Result: " << result << std::endl;
  
  //
  //
  
  std::cout << " - Testing direct value access..." << std::endl;
  result = 0.0;
  t.start();
  for (size_t r=0; r<repeat; ++r)
    for (size_t i=0; i<num; ++i)
      result += objects[i].value(); 
  elapsed = t.get();
  std::cout << "   Time elapsed: " << elapsed << std::endl;
  std::cout << "   Result: " << result << std::endl;

  if (!use_viennadata)
    return;
  
  //
  //
  
  std::cout << " - Testing ViennaData access (slow)..." << std::endl;
  result = 0.0;
  t.start();
  for (size_t r=0; r<repeat; ++r)
    for (size_t i=0; i<num; ++i)
      result += viennadata::access<std::string, double>(key_string)(objects[i]); 
  elapsed = t.get();
  std::cout << "   Time elapsed: " << elapsed << std::endl;
  std::cout << "   Result: " << result << std::endl;

  //
  //
  
  std::cout << " - Testing ViennaData access (medium)..." << std::endl;
  result = 0.0;
  t.start();
  for (size_t r=0; r<repeat; ++r)
    for (size_t i=0; i<num; ++i)
      result += viennadata::access<long, double>(42)(objects[i]); 
  elapsed = t.get();
  std::cout << "   Time elapsed: " << elapsed << std::endl;
  std::cout << "   Result: " << result << std::endl;

  //
  //
  
  std::cout << " - Testing ViennaData value access (fast)..." << std::endl;
  result = 0.0;
  t.start();
  for (size_t r=0; r<repeat; ++r)
    for (size_t i=0; i<num; ++i)
      result += viennadata::access<data_key, double>()(objects[i]); 
  elapsed = t.get();
  std::cout << "   Time elapsed: " << elapsed << std::endl;
  std::cout << "   Result: " << result << std::endl;
  
  
  
  //clean up:
  for (size_t i=0; i<num; ++i)
  {
    viennadata::erase<std::string, double>()(objects[i]);
    viennadata::erase<long, double>()(objects[i]);
    viennadata::erase<data_key, double>()(objects[i]);
  }
}



int main(int argc, char *argv[])
{
  //long num = 1000000;
  long num = 1000;
  
  std::cout << "--- Test 1: SlimClass ---" << std::endl;
  run<SlimClass>(num, 1000, true);
  std::cout << "--- Test 2: FatClass<10> ---" << std::endl;
  run<FatClass<10> >(num, 1000);
  std::cout << "--- Test 3: FatClass<100> ---" << std::endl;
  run<FatClass<100> >(num, 1000);
  std::cout << "--- Test 4: FatClass<1000> ---" << std::endl;
  run<FatClass<1000> >(num, 1000);
  
  return EXIT_SUCCESS;
}

