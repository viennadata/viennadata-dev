

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
class ClassWithID
{
  public:
    ClassWithID(long i) : id(i) {}
    long get_id() const { return id; } 

  private:
    long id;
};

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
    StandardClass std_obj1;
    StandardClass std_obj2;

    ClassWithID    id_obj1(23);
    ClassWithID    id_obj2(42);

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
    viennadata::access<char,double>('c')(std_obj1) = 23.45;
    viennadata::access<char,std::string>('c')(std_obj1) = "Hello";
    viennadata::access<SomeKey,double>(SomeKey(2))(std_obj1) = 23.456;
    viennadata::access<SomeKey,std::string>(SomeKey(2))(std_obj1) = "World";
    viennadata::access<QuickKey,double>(QuickKey(2))(std_obj1) = 23.4567;
    viennadata::access<QuickKey,std::string>(QuickKey(2))(std_obj1) = "!";

    viennadata::access<char,double>('c')(std_obj2) = 30;
    viennadata::access<char,std::string>('c')(std_obj2) = "This";
    viennadata::access<SomeKey,double>(SomeKey(2))(std_obj2) = 300;
    viennadata::access<SomeKey,std::string>(SomeKey(2))(std_obj2) = "is";
    viennadata::access<QuickKey,double>(QuickKey(2))(std_obj2) = 3000;
    viennadata::access<QuickKey,std::string>(QuickKey(2))(std_obj2) = "Sparta!";

    viennadata::access<char,double>('c')(id_obj1) = 1.2;
    viennadata::access<char,std::string>('c')(id_obj1) = "foo";
    viennadata::access<SomeKey,double>(SomeKey(2))(id_obj1) = 3.4;
    viennadata::access<SomeKey,std::string>(SomeKey(2))(id_obj1) = "or";
    viennadata::access<QuickKey,double>(QuickKey(2))(id_obj1) = 5.6;
    viennadata::access<QuickKey,std::string>(QuickKey(2))(id_obj1) = "bar";

    viennadata::access<char,double>('c')(id_obj2) = 9.8;
    viennadata::access<char,std::string>('c')(id_obj2) = "all";
    viennadata::access<SomeKey,double>(SomeKey(2))(id_obj2) = 7.6;
    viennadata::access<SomeKey,std::string>(SomeKey(2))(id_obj2) = "your";
    viennadata::access<QuickKey,double>(QuickKey(2))(id_obj2) = 5.4;
    viennadata::access<QuickKey,std::string>(QuickKey(2))(id_obj2) = "base";

    //modify a little bit:
    viennadata::erase<char,double>('c')(std_obj1);
    viennadata::erase<SomeKey,double>(SomeKey(2))(std_obj2);
    viennadata::erase<QuickKey,double>(QuickKey(2))(id_obj2);

    //read data:
    long error_cnt = 0;

    if (viennadata::access<char,double>('c')(std_obj1) != 0) ++error_cnt;
    if (viennadata::access<char,std::string>('c')(std_obj1) != "Hello") ++error_cnt;
    if (viennadata::access<SomeKey,double>(SomeKey(2))(std_obj1) != 23.456) ++error_cnt;
    if (viennadata::access<SomeKey,std::string>(SomeKey(2))(std_obj1) != "World") ++error_cnt;
    if (viennadata::access<QuickKey,double>(QuickKey(2))(std_obj1) != 23.4567) ++error_cnt;
    if (viennadata::access<QuickKey,std::string>(QuickKey(2))(std_obj1) != "!") ++error_cnt;

    if (viennadata::access<char,double>('c')(std_obj2) != 30) ++error_cnt;
    if (viennadata::access<char,std::string>('c')(std_obj2) != "This") ++error_cnt;
    if (viennadata::access<SomeKey,double>(SomeKey(2))(std_obj2) != 0) ++error_cnt;
    if (viennadata::access<SomeKey,std::string>(SomeKey(2))(std_obj2) != "is") ++error_cnt;
    if (viennadata::access<QuickKey,double>(QuickKey(2))(std_obj2) != 3000) ++error_cnt;
    if (viennadata::access<QuickKey,std::string>(QuickKey(2))(std_obj2) != "Sparta!") ++error_cnt;

    if (viennadata::access<char,double>('c')(id_obj1) != 1.2) ++error_cnt;
    if (viennadata::access<char,std::string>('c')(id_obj1) != "foo") ++error_cnt;
    if (viennadata::access<SomeKey,double>(SomeKey(2))(id_obj1) != 3.4) ++error_cnt;
    if (viennadata::access<SomeKey,std::string>(SomeKey(2))(id_obj1) != "or") ++error_cnt;
    if (viennadata::access<QuickKey,double>(QuickKey(2))(id_obj1) != 5.6) ++error_cnt;
    if (viennadata::access<QuickKey,std::string>(QuickKey(2))(id_obj1) != "bar") ++error_cnt;

    if (viennadata::access<char,double>('c')(id_obj2) != 9.8) ++error_cnt;
    if (viennadata::access<char,std::string>('c')(id_obj2) != "all") ++error_cnt;
    if (viennadata::access<SomeKey,double>(SomeKey(2))(id_obj2) != 7.6) ++error_cnt;
    if (viennadata::access<SomeKey,std::string>(SomeKey(2))(id_obj2) != "your") ++error_cnt;
    if (viennadata::access<QuickKey,double>(QuickKey(2))(id_obj2) != 0) ++error_cnt;
    if (viennadata::access<QuickKey,std::string>(QuickKey(2))(id_obj2) != "base") ++error_cnt;



    if (error_cnt == 0)
      std::cout << "Data access check succeeded!" << std::endl;
    else
      std::cout << "Data access check failed with " << error_cnt << " errors!" << std::endl;
}


#endif

