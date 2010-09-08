

/* =======================================================================
   Copyright (c) 2010, Institute for Microelectronics, TU Vienna.
   http://www.iue.tuwien.ac.at
                             -----------------
                     ViennaData - The Vienna Data Storage Library
                             -----------------

   authors:    Karl Rupp                          rupp@iue.tuwien.ac.at

   license:    MIT (X11), see file LICENSE in the ViennaData base directory
======================================================================= */

#ifndef VIENNADATA_TESTBENCH_HPP
#define VIENNADATA_TESTBENCH_HPP


struct StandardClass {};
struct ClassWithID { long get_id() const { return 42; } };

// make ID of ClassWithID known to ViennaData:
namespace viennadata
{
  template <>
  struct element_identifier<ClassWithID>
  {
    typedef element_provided_id    tag;

    static long id(ClassWithID const & cwid) { return cwid.get_id(); }
  };
}


class SomeKey
{
  public:
    SomeKey(long i) : id(i) {}

    bool operator<(SomeKey const & other) const { return id < other.id; }

  private:
    long id;
};


class QuickKey
{
  public:
    QuickKey(long i) : id(i) {}

    bool operator<(QuickKey const & other) const { return id < other.id; }

  private:
    long id;
};

// tell ViennaData to use QuickKey:
namespace viennadata
{
  template <>
  struct dispatch_traits<QuickKey>
  {
    typedef type_key_dispatch_tag    tag;
  };
}


// store doubles on ClassWithID densely:
namespace viennadata
{
  template <typename key_type>
  struct storage_traits<key_type, double, ClassWithID>
  {
    typedef dense_data_tag    tag;
  };
}





// Checks all data for being stored and retrieved properly.
void check_data_access()
{
    StandardClass std_obj;
    ClassWithID    id_obj;

    //reserve data:
    viennadata::reserve<char,     double,      StandardClass>(123);
    viennadata::reserve<char,     std::string, StandardClass>(123);
    viennadata::reserve<SomeKey,  double,      StandardClass>(123);
    viennadata::reserve<SomeKey,  std::string, StandardClass>(123);
    viennadata::reserve<QuickKey, double,      StandardClass>(123);
    viennadata::reserve<QuickKey, std::string, StandardClass>(123);

    viennadata::reserve<char,     double,      ClassWithID>(123);
    viennadata::reserve<char,     std::string, ClassWithID>(123);
    viennadata::reserve<SomeKey,  double,      ClassWithID>(123);
    viennadata::reserve<SomeKey,  std::string, ClassWithID>(123);
    viennadata::reserve<QuickKey, double,      ClassWithID>(123);
    viennadata::reserve<QuickKey, std::string, ClassWithID>(123);

    //write data:
    viennadata::access<char,double>('c')(std_obj) = 23.45;
    viennadata::access<char,std::string>('c')(std_obj) = "Hello";
    viennadata::access<SomeKey,double>(SomeKey(2))(std_obj) = 23.456;
    viennadata::access<SomeKey,std::string>(SomeKey(2))(std_obj) = "World";
    viennadata::access<QuickKey,double>(QuickKey(2))(std_obj) = 23.4567;
    viennadata::access<QuickKey,std::string>(QuickKey(2))(std_obj) = "!";


    viennadata::access<char,double>('c')(id_obj) = 1.2;
    viennadata::access<char,std::string>('c')(id_obj) = "foo";
    viennadata::access<SomeKey,double>(SomeKey(2))(id_obj) = 3.4;
    viennadata::access<SomeKey,std::string>(SomeKey(2))(id_obj) = "or";
    viennadata::access<QuickKey,double>(QuickKey(2))(id_obj) = 5.6;
    viennadata::access<QuickKey,std::string>(QuickKey(2))(id_obj) = "bar";

    //read data:
    long error_cnt = 0;

    if (viennadata::access<char,double>('c')(std_obj) != 23.45) ++error_cnt;
    if (viennadata::access<char,std::string>('c')(std_obj) != "Hello") ++error_cnt;
    if (viennadata::access<SomeKey,double>(SomeKey(2))(std_obj) != 23.456) ++error_cnt;
    if (viennadata::access<SomeKey,std::string>(SomeKey(2))(std_obj) != "World") ++error_cnt;
    if (viennadata::access<QuickKey,double>(QuickKey(2))(std_obj) != 23.4567) ++error_cnt;
    if (viennadata::access<QuickKey,std::string>(QuickKey(2))(std_obj) != "!") ++error_cnt;

    if (viennadata::access<char,double>('c')(id_obj) != 1.2) ++error_cnt;
    if (viennadata::access<char,std::string>('c')(id_obj) != "foo") ++error_cnt;
    if (viennadata::access<SomeKey,double>(SomeKey(2))(id_obj) != 3.4) ++error_cnt;
    if (viennadata::access<SomeKey,std::string>(SomeKey(2))(id_obj) != "or") ++error_cnt;
    if (viennadata::access<QuickKey,double>(QuickKey(2))(id_obj) != 5.6) ++error_cnt;
    if (viennadata::access<QuickKey,std::string>(QuickKey(2))(id_obj) != "bar") ++error_cnt;



    if (error_cnt == 0)
      std::cout << "Data access check succeeded!" << std::endl;
    else
      std::cout << "Data access check failed with " << error_cnt << " errors!" << std::endl;
}


#endif

