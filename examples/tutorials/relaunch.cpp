
#include <vector>
#include <iostream>
#include <cstdlib>

#include "viennadata/api.hpp"


struct test_tag;

struct test_struct
{
    typedef test_tag tag;
    typedef int id_type;

    test_struct() {}
    test_struct(int idd) : id_(idd) {}

    id_type id() const { return id_; }

    int id_;
};




struct test_tag2;

struct test_struct2
{
    typedef test_tag2 tag;
    typedef int id_type;

    test_struct2() {}
    test_struct2(int idd) : id_(idd) {}

    id_type id() const { return id_; }

    int id_;
};


struct my_trivial_key
{
    bool operator=(const my_trivial_key &) const { return true; }
};

bool operator<(my_trivial_key const &, my_trivial_key const &)
{
    return false;
}



int main()
{
  typedef viennameta::make_typemap<
    viennadata::default_tag, viennameta::static_pair<viennadata::std_map_tag, viennadata::pointer_access_tag>,
    viennameta::static_pair< test_struct, viennameta::static_pair<int, double> >, viennameta::static_pair<viennadata::std_deque_tag, viennadata::id_access_tag>
  >::type container_config;

  viennadata::storage<container_config> my_storage;

  std::vector<test_struct> elements;
  std::vector<test_struct2> elements2;

  int num_elements = 10;

  for (int i = 0; i < num_elements; ++i)
  {
    elements.push_back(test_struct(i));
    elements2.push_back(test_struct2(i));
  }


  {
    double val = 0.0;
    for (std::vector<test_struct>::iterator it = elements.begin(); it != elements.end(); ++it, val += 1.0)
      viennadata::access<int, double>(my_storage, 0, *it) = val;
  }

  {
    double val = 0.0;
    for (std::vector<test_struct2>::iterator it = elements2.begin(); it != elements2.end(); ++it, val += 1.0)
      viennadata::access<int, double>(my_storage, 0, *it) = val;
  }

  test_struct2 tmp(10);

  viennadata::result_of::container_type< viennadata::storage<container_config>, double, double, test_struct2>::type & c =
      viennadata::container<double, double, test_struct2>(my_storage, 0);


  std::cout << "\nErase Test\n" << std::endl;

  viennadata::access< double, double >(my_storage, 0, tmp) = 1;
  viennadata::access< int, double >(my_storage, 0, tmp) = 2;
  viennadata::access< double, int >(my_storage, 0, tmp) = 3;

  std::cout << viennadata::find<double, double>(my_storage, 0, tmp) << std::endl;
  std::cout << viennadata::find<int, double>(my_storage, 0, tmp) << std::endl;
  std::cout << viennadata::find<double, int>(my_storage, 0, tmp) << std::endl;

//         viennadata::erase<viennadata::all, viennadata::all>(my_storage, tmp);
//         viennadata::erase<viennadata::all, int>(my_storage, tmp);

  std::cout << "Container size = " << c.size() << std::endl;
  viennadata::erase<double, viennadata::all>(my_storage, 0, tmp);

  std::cout << viennadata::find<double, double>(my_storage, 0, tmp) << std::endl;
  std::cout << viennadata::find<int, double>(my_storage, 0, tmp) << std::endl;
  std::cout << viennadata::find<double, int>(my_storage, 0, tmp) << std::endl;


  std::cout << "Container size = " << c.size() << std::endl;


  std::cout << "\nAccessor Test\n" << std::endl;

  viennadata::result_of::accessor<viennadata::storage<container_config>, double, double, test_struct2>::type
      accessor = viennadata::accessor<double, double, test_struct2>(my_storage, 0);

  accessor(tmp) = 3.14;
  std::cout << "Accessor: " << accessor(tmp) << std::endl;
  std::cout << "Accessor find: " << accessor.find(tmp) << std::endl;
  accessor.erase(tmp);
  std::cout << "Accessor find: " << accessor.find(tmp) << std::endl;


  std::cout << "\nCopy Test\n" << std::endl;
  viennadata::access< double, double >(my_storage, 0, tmp) = 1;
  viennadata::access< int, double >(my_storage, 0, tmp) = 2;
  viennadata::access< double, int >(my_storage, 0, tmp) = 3;

  std::cout << viennadata::access<double, double>(my_storage, 0, tmp) << std::endl;
  std::cout << viennadata::access<int, double>(my_storage, 0, tmp) << std::endl;
  std::cout << viennadata::access<double, int>(my_storage, 0, tmp) << std::endl;

  test_struct2 tmp1(11);

  viennadata::copy<double, viennadata::all>(my_storage, 0, tmp, tmp1);

  std::cout << viennadata::access<double, double>(my_storage, 0, tmp) << std::endl;
  std::cout << viennadata::access<int, double>(my_storage, 0, tmp) << std::endl;
  std::cout << viennadata::access<double, int>(my_storage, 0, tmp) << std::endl;

  std::cout << viennadata::access<double, double>(my_storage, 0, tmp1) << std::endl;
  std::cout << viennadata::access<int, double>(my_storage, 0, tmp1) << std::endl;
  std::cout << viennadata::access<double, int>(my_storage, 0, tmp1) << std::endl;


  std::cout << "\nTrivial Key\n" << std::endl;
  viennadata::access< my_trivial_key, double >(my_storage, my_trivial_key(), tmp) = 42;
  std::cout << "my trivial key = " << viennadata::access< my_trivial_key, double >(my_storage, my_trivial_key(), tmp) << std::endl;

  return EXIT_SUCCESS;
}
