//
// C++ Interface: testbench
//
// Description: 
//
//
// Author:  <>, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//


using Vienna::QuantityManager::QuanKeyPair;
using Vienna::QuantityManager::NoStorageBanks;
using Vienna::QuantityManager::QuantityManager;
using Vienna::QuantityManager::NoID;
using Vienna::QuantityManager::ProvideID;
using Vienna::QuantityManager::SetKeyDispatch;
using Vienna::QuantityManager::KeyWrapper;
using Vienna::QuantityManager::KeyInterface;

class TestClassWithLongBanks 
  : public QuantityManager< TestClassWithLongBanks, NoID, long>
{
  public:
    TestClassWithLongBanks(long member_ = 0) : member(member_) {};

    long getMember() { return member; }

  private:
    long member;

};

class AnotherClassWithLongBanks 
  : public QuantityManager< AnotherClassWithLongBanks, NoID, long>
{
  public:
    AnotherClassWithLongBanks(long member_ = 0) : member(member_) {};

    long getMember() { return member; }

  private:
    long member;

};

class TestClassWithDoubleBanks 
  : public QuantityManager< TestClassWithDoubleBanks, NoID, double>
{
  public:
    TestClassWithDoubleBanks(long member_ = 0) : member(member_) {};

    long getMember() { return member; }

  private:
    long member;

};

class TestWithoutBanks 
  : public QuantityManager< TestWithoutBanks, ProvideID, NoStorageBanks >
{
  public:
    TestWithoutBanks(long member_ = 0) : member(member_) {};

    long getMember() { return member; }

  private:
    long member;

};





struct TestClass1
{
  static void print() { std::cout << "TestClass1"; }
};

struct TestClass2
{
  static void print() { std::cout << "TestClass2"; }
};


struct KeyClass
{
   static void print() { std::cout << "TestKeyClass"; }
};

namespace Vienna{
  namespace QuantityManager{
    template <>
    struct SetKeyDispatch< KeyClass >
    {
      typedef TypeBasedDispatch    ResultType;
    };
  }
}


template <typename TestClassType, typename BankType>
void fillBank_1_impl(TestClassType & test_bank, BankType const * bank_1, BankType const * bank_2)
{

  //keys:
  char key_char_1 = 'c';
  char key_char_2 = 'd';
  long key_long_1 = 1;
//  long key_long_2 = 2;

  std::string key_string_1 = "key1";
  std::string key_string_2 = "key2";

  KeyClass key_keyclass_1;
  KeyClass key_keyclass_2;

  //data:
  double data_double_1 = 100.0;
  double data_double_2 = 200.0;
  double data_double_3 = 300.0;

  std::string data_string_1 = "data1";
  std::string data_string_2 = "data2";

  test_bank.template reserveQuantity<double>( key_char_1 );
  test_bank.template reserveQuantity<std::string>( key_char_1 );
  test_bank.template reserveQuantity<double>( key_char_2 );
  test_bank.template reserveQuantity<std::string>( key_char_2 );
  test_bank.template reserveQuantity<double>( key_long_1 );
  test_bank.template reserveQuantity<std::string>( key_long_1 );
  test_bank.template reserveQuantity<double>( key_string_2 );
  test_bank.template reserveQuantity<std::string>( key_string_2 );
  test_bank.template reserveQuantity<double>( key_keyclass_1 );
  test_bank.template reserveQuantity<std::string>( key_keyclass_2 );

  if (bank_1 != NULL)
    test_bank.setStorageBank( *bank_1 );

  //fill test_bank:
  test_bank.storeQuantity(key_char_1, data_double_1);
  test_bank.storeQuantity(key_char_1, data_string_2);
  test_bank.storeQuantity(key_char_2, data_double_3);
  test_bank.storeQuantity(key_char_2, data_string_1);
  test_bank.storeQuantity(key_long_1, data_double_2);
  test_bank.storeQuantity(key_long_1, data_string_2);
  test_bank.storeQuantity(key_string_2, data_double_3);
  test_bank.storeQuantity(key_string_2, data_string_1);
  test_bank.storeQuantity(key_keyclass_1, data_double_3);
  test_bank.storeQuantity(key_keyclass_2, data_string_1);

  //intentional overwrites of existing quantities:
  test_bank.storeQuantity(key_char_1, data_double_3);
  test_bank.storeQuantity(key_char_1, data_string_1);
  test_bank.storeQuantity(key_char_2, data_double_2);
  test_bank.storeQuantity(key_char_2, data_string_2);
  test_bank.storeQuantity(key_long_1, data_double_3);
  test_bank.storeQuantity(key_long_1, data_string_1);
  test_bank.storeQuantity(key_string_2, data_double_1);
  test_bank.storeQuantity(key_string_2, data_string_2);
  test_bank.storeQuantity(key_keyclass_1, data_double_1);
  test_bank.storeQuantity(key_keyclass_2, data_string_2);

  //switch storage bank
  if (bank_2 != NULL)
    test_bank.setStorageBank( *bank_2 );

  //fill test_longbank_1:
  test_bank.storeQuantity(key_char_1, data_double_3);
  test_bank.storeQuantity(key_char_1, data_string_1);
  test_bank.storeQuantity(key_char_2, data_double_2);
  test_bank.storeQuantity(key_char_2, data_string_2);
  test_bank.storeQuantity(key_long_1, data_double_3);
  test_bank.storeQuantity(key_long_1, data_string_1);
  test_bank.storeQuantity(key_string_2, data_double_1);
  test_bank.storeQuantity(key_string_2, data_string_2);
  test_bank.storeQuantity(key_keyclass_1, data_double_1);
  test_bank.storeQuantity(key_keyclass_2, data_string_2);

  //intentional overwrites of existing quantities:
  test_bank.storeQuantity(key_char_1, data_double_1);
  test_bank.storeQuantity(key_char_1, data_string_2);
  test_bank.storeQuantity(key_char_2, data_double_3);
  test_bank.storeQuantity(key_char_2, data_string_1);
  test_bank.storeQuantity(key_long_1, data_double_2);
  test_bank.storeQuantity(key_long_1, data_string_2);
  test_bank.storeQuantity(key_string_2, data_double_3);
  test_bank.storeQuantity(key_string_2, data_string_1);
  test_bank.storeQuantity(key_keyclass_1, data_double_3);
  test_bank.storeQuantity(key_keyclass_2, data_string_1);
}

template <typename TestClassType, typename BankType>
void checkBank_1_impl(TestClassType & test_bank, BankType const * bank_1, BankType const * bank_2)
{
  //keys:
  char key_char_1 = 'c';
  char key_char_2 = 'd';
  long key_long_1 = 1;
//  long key_long_2 = 2;

  std::string key_string_1 = "key1";
  std::string key_string_2 = "key2";

  KeyClass key_keyclass_1;
  KeyClass key_keyclass_2;

  //data:
  double data_double_1 = 100.0;
  double data_double_2 = 200.0;
  double data_double_3 = 300.0;

  std::string data_string_1 = "data1";
  std::string data_string_2 = "data2";

  //check results:
  long errors_test_bank = 0;

  //bank 1
  if (bank_1 != NULL)
  {
    test_bank.setStorageBank( *bank_1 );
    if (test_bank.template retrieveQuantity<double>     (key_char_1) != data_double_3) ++errors_test_bank;
    if (test_bank.template retrieveQuantity<std::string>(key_char_1) != data_string_1) ++errors_test_bank;
    if (test_bank.template retrieveQuantity<double>     (key_char_2) != data_double_2) ++errors_test_bank;
    if (test_bank.template retrieveQuantity<std::string>(key_char_2) != data_string_2) ++errors_test_bank;
    if (test_bank.template retrieveQuantity<double>     (key_long_1) != data_double_3) ++errors_test_bank;
    if (test_bank.template retrieveQuantity<std::string>(key_long_1) != data_string_1) ++errors_test_bank;
    if (test_bank.template retrieveQuantity<double>     (key_string_2) != data_double_1) ++errors_test_bank;
    if (test_bank.template retrieveQuantity<std::string>(key_string_2) != data_string_2) ++errors_test_bank;
    if (test_bank.template retrieveQuantity<double>     (key_keyclass_1) != data_double_1) ++errors_test_bank;
    if (test_bank.template retrieveQuantity<std::string>(key_keyclass_2) != data_string_2) ++errors_test_bank;
  }

  if (bank_2 != NULL)
    test_bank.setStorageBank( *bank_2 );
  if (test_bank.template retrieveQuantity<double>     (key_char_1) != data_double_1) ++errors_test_bank;
  if (test_bank.template retrieveQuantity<std::string>(key_char_1) != data_string_2) ++errors_test_bank;
  if (test_bank.template retrieveQuantity<double>     (key_char_2) != data_double_3) ++errors_test_bank;
  if (test_bank.template retrieveQuantity<std::string>(key_char_2) != data_string_1) ++errors_test_bank;
  if (test_bank.template retrieveQuantity<double>     (key_long_1) != data_double_2) ++errors_test_bank;
  if (test_bank.template retrieveQuantity<std::string>(key_long_1) != data_string_2) ++errors_test_bank;
  if (test_bank.template retrieveQuantity<double>     (key_string_2) != data_double_3) ++errors_test_bank;
  if (test_bank.template retrieveQuantity<std::string>(key_string_2) != data_string_1) ++errors_test_bank;
  if (test_bank.template retrieveQuantity<double>     (key_keyclass_1) != data_double_3) ++errors_test_bank;
  if (test_bank.template retrieveQuantity<std::string>(key_keyclass_2) != data_string_1) ++errors_test_bank;

  std::cout << "Data Errors: " << errors_test_bank << std::endl;

  errors_test_bank = 0;
//   if (bank_1 != NULL)
//   {
//     test_bank.setStorageBank( *bank_1 );
//     test_bank.template eraseBank();
// 
//     if (test_bank.template retrieveQuantity<double>     (key_char_1) != double()) ++errors_test_bank;
//     if (test_bank.template retrieveQuantity<std::string>(key_char_1) != std::string()) ++errors_test_bank;
//     if (test_bank.template retrieveQuantity<double>     (key_char_2) != double()) ++errors_test_bank;
//     if (test_bank.template retrieveQuantity<std::string>(key_char_2) != std::string()) ++errors_test_bank;
//     if (test_bank.template retrieveQuantity<double>     (key_long_1) != double()) ++errors_test_bank;
//     if (test_bank.template retrieveQuantity<std::string>(key_long_1) != std::string()) ++errors_test_bank;
//     if (test_bank.template retrieveQuantity<double>     (key_string_2) != double()) ++errors_test_bank;
//     if (test_bank.template retrieveQuantity<std::string>(key_string_2) != std::string()) ++errors_test_bank;
//     if (test_bank.template retrieveQuantity<double>     (key_keyclass_1) != double()) ++errors_test_bank;
//     if (test_bank.template retrieveQuantity<std::string>(key_keyclass_2) != std::string()) ++errors_test_bank;
//   }


  if (bank_2 != NULL)
    test_bank.setStorageBank( *bank_2 );

  test_bank.template eraseQuantity<double>(key_long_1);
  test_bank.template eraseQuantity<char, double>();
  test_bank.template eraseKey<KeyClass>();
  if (test_bank.template retrieveQuantity<double>     (key_char_1) != double()) ++errors_test_bank;
  if (test_bank.template retrieveQuantity<std::string>(key_char_1) != data_string_2) ++errors_test_bank;
  if (test_bank.template retrieveQuantity<double>     (key_char_2) != double()) ++errors_test_bank;
  if (test_bank.template retrieveQuantity<std::string>(key_char_2) != data_string_1) ++errors_test_bank;
  if (test_bank.template retrieveQuantity<double>     (key_long_1) != double()) ++errors_test_bank;
  if (test_bank.template retrieveQuantity<std::string>(key_long_1) != data_string_2) ++errors_test_bank;
  if (test_bank.template retrieveQuantity<double>     (key_string_2) != data_double_3) ++errors_test_bank;
  if (test_bank.template retrieveQuantity<std::string>(key_string_2) != data_string_1) ++errors_test_bank;
  if (test_bank.template retrieveQuantity<double>     (key_keyclass_1) != double()) ++errors_test_bank;
  if (test_bank.template retrieveQuantity<std::string>(key_keyclass_2) != std::string()) ++errors_test_bank;

  std::cout << "Release Errors: " << errors_test_bank << std::endl;

}


template <typename TestClassType, typename BankType>
void transferChecks(TestClassType & test_bank_src,
                    TestClassType & test_bank_dest,
                    BankType const * bank_1,
                    BankType const * bank_2)
{

  //reset candidates:
  test_bank_src.releaseAll();
  test_bank_dest.releaseAll();

  fillBank_1_impl(test_bank_src, bank_1, bank_2);
  test_bank_src.transferAllTo(test_bank_dest);
  checkBank_1_impl(test_bank_dest, bank_1, bank_2);

  test_bank_src.releaseAll();
  fillBank_1_impl(test_bank_src, bank_1, bank_2);
  test_bank_src.copyAllTo(test_bank_dest);
  checkBank_1_impl(test_bank_dest, bank_1, bank_2);

}

void testBasicOperations()
{

  // (initial) test objects:
  TestClassWithLongBanks test_longbank_1(1);
  TestClassWithLongBanks test_longbank_2(2);

  AnotherClassWithLongBanks test_longbank_3(3);
  AnotherClassWithLongBanks test_longbank_4(4);

  TestClassWithDoubleBanks test_doublebank_1(1);
  TestClassWithDoubleBanks test_doublebank_2(2);
  TestClassWithDoubleBanks test_doublebank_3(3);

  TestWithoutBanks  test_nobank_1(1);
  TestWithoutBanks  test_nobank_2(2);
  TestWithoutBanks  test_nobank_3(3);

  //banks:
  long long_bank_1 = 4;
  long long_bank_2 = 3;
  double double_bank_1 = 1.0;
  double double_bank_2 = 1.0;

  fillBank_1_impl(test_longbank_1, &long_bank_1, &long_bank_2);
  fillBank_1_impl(test_longbank_2, &long_bank_2, &long_bank_1);
  fillBank_1_impl(test_longbank_3, &long_bank_1, &long_bank_2);
  fillBank_1_impl(test_longbank_4, &long_bank_2, &long_bank_1);

  fillBank_1_impl(test_doublebank_1, &double_bank_1, &double_bank_2);
  fillBank_1_impl(test_doublebank_2, &double_bank_2, &double_bank_1);
  fillBank_1_impl(test_doublebank_3, &double_bank_1, &double_bank_2);

  fillBank_1_impl<TestWithoutBanks, NoStorageBanks>(test_nobank_1, NULL, NULL);
  fillBank_1_impl<TestWithoutBanks, NoStorageBanks>(test_nobank_2, NULL, NULL);
  fillBank_1_impl<TestWithoutBanks, NoStorageBanks>(test_nobank_3, NULL, NULL);

  std::cout << "Testing test_longbank_1..." << std::endl;
  checkBank_1_impl(test_longbank_1, &long_bank_1, &long_bank_2);
  std::cout << "Testing test_longbank_2..." << std::endl;
  checkBank_1_impl(test_longbank_2, &long_bank_2, &long_bank_1);
  std::cout << "Testing test_longbank_3..." << std::endl;
  checkBank_1_impl(test_longbank_3, &long_bank_1, &long_bank_2);
  std::cout << "Testing test_longbank_4..." << std::endl;
  checkBank_1_impl(test_longbank_4, &long_bank_2, &long_bank_1);

  std::cout << "Testing test_doublebank_1..." << std::endl;
  checkBank_1_impl(test_doublebank_1, &double_bank_1, &double_bank_2);
  std::cout << "Testing test_doublebank_2..." << std::endl;
  checkBank_1_impl(test_doublebank_2, &double_bank_2, &double_bank_1);
  std::cout << "Testing test_doublebank_3..." << std::endl;
  checkBank_1_impl(test_doublebank_3, &double_bank_1, &double_bank_2);

  std::cout << "Testing test_nobank_1..." << std::endl;
  checkBank_1_impl<TestWithoutBanks, NoStorageBanks>(test_nobank_1, NULL, NULL);
  std::cout << "Testing test_nobank_2..." << std::endl;
  checkBank_1_impl<TestWithoutBanks, NoStorageBanks>(test_nobank_2, NULL, NULL);
  std::cout << "Testing test_nobank_3..." << std::endl;
  checkBank_1_impl<TestWithoutBanks, NoStorageBanks>(test_nobank_3, NULL, NULL);


  //transfer checks:
  std::cout << "Testing copy&transfer..." << std::endl;
  transferChecks(test_longbank_1, test_longbank_2, &long_bank_1, &long_bank_2);
  transferChecks(test_longbank_2, test_longbank_1, &long_bank_1, &long_bank_2);
  transferChecks(test_longbank_3, test_longbank_4, &long_bank_1, &long_bank_2);
  transferChecks(test_longbank_4, test_longbank_3, &long_bank_1, &long_bank_2);

  transferChecks(test_doublebank_1, test_doublebank_2, &double_bank_1, &double_bank_2);
  transferChecks(test_doublebank_2, test_doublebank_3, &double_bank_1, &double_bank_2);
  transferChecks(test_doublebank_3, test_doublebank_1, &double_bank_1, &double_bank_2);

  transferChecks(test_doublebank_1, test_doublebank_2, &double_bank_1, &double_bank_2);
  transferChecks(test_doublebank_2, test_doublebank_3, &double_bank_1, &double_bank_2);
  transferChecks(test_doublebank_3, test_doublebank_1, &double_bank_1, &double_bank_2);

}









class DummyQuanMan
{
  public:
    template <typename QuanType, typename KeyType>
    void releaseQuantity() {}

    template <typename QuanType, typename KeyType>
    void eraseQuantity() {}

    template <typename QuanType, typename KeyType>
    void copyPairTo(DummyQuanMan & ) {}

    template <typename QuanType, typename KeyType>
    void transferPairTo(DummyQuanMan & ) {}

};




void testStorageKeyWrappers()
{
  DummyQuanMan dqm;
  long quan1 = 3;
  double quan2 = 2.0;
  char quan3 = '1';

  double key1 = 1.0;
  char key2 = '0';
  long key3 = 0;

  QuanKeyPair<DummyQuanMan> qkp11(quan1, key1);
  QuanKeyPair<DummyQuanMan> qkp12(quan1, key2);
  QuanKeyPair<DummyQuanMan> qkp13(quan1, key3);
  QuanKeyPair<DummyQuanMan> qkp21(quan2, key1);
  QuanKeyPair<DummyQuanMan> qkp22(quan2, key2);
  QuanKeyPair<DummyQuanMan> qkp23(quan2, key3);
  QuanKeyPair<DummyQuanMan> qkp31(quan3, key1);
  QuanKeyPair<DummyQuanMan> qkp32(quan3, key2);
  QuanKeyPair<DummyQuanMan> qkp33(quan3, key3);

  std::cout << "<quan1, key1>: " << (qkp11 == qkp11) << " " << (qkp11 == qkp12) << " " << (qkp11 == qkp13) << " "
                                 << (qkp11 == qkp21) << " " << (qkp11 == qkp22) << " " << (qkp11 == qkp23) << " "
                                 << (qkp11 == qkp31) << " " << (qkp11 == qkp32) << " " << (qkp11 == qkp33) << std::endl;

  std::cout << "<quan1, key2>: " << (qkp12 == qkp11) << " " << (qkp12 == qkp12) << " " << (qkp12 == qkp13) << " "
                                 << (qkp12 == qkp21) << " " << (qkp12 == qkp22) << " " << (qkp12 == qkp23) << " "
                                 << (qkp12 == qkp31) << " " << (qkp12 == qkp32) << " " << (qkp12 == qkp33) << std::endl;

  std::cout << "<quan1, key3>: " << (qkp13 == qkp11) << " " << (qkp13 == qkp12) << " " << (qkp13 == qkp13) << " "
                                 << (qkp13 == qkp21) << " " << (qkp13 == qkp22) << " " << (qkp13 == qkp23) << " "
                                 << (qkp13 == qkp31) << " " << (qkp13 == qkp32) << " " << (qkp13 == qkp33) << std::endl;



  std::cout << "<quan2, key1>: " << (qkp21 == qkp11) << " " << (qkp21 == qkp12) << " " << (qkp21 == qkp13) << " "
                                 << (qkp21 == qkp21) << " " << (qkp21 == qkp22) << " " << (qkp21 == qkp23) << " "
                                 << (qkp21 == qkp31) << " " << (qkp21 == qkp32) << " " << (qkp21 == qkp33) << std::endl;

  std::cout << "<quan2, key2>: " << (qkp22 == qkp11) << " " << (qkp22 == qkp12) << " " << (qkp22 == qkp13) << " "
                                 << (qkp22 == qkp21) << " " << (qkp22 == qkp22) << " " << (qkp22 == qkp23) << " "
                                 << (qkp22 == qkp31) << " " << (qkp22 == qkp32) << " " << (qkp22 == qkp33) << std::endl;

  std::cout << "<quan2, key3>: " << (qkp23 == qkp11) << " " << (qkp23 == qkp12) << " " << (qkp23 == qkp13) << " "
                                 << (qkp23 == qkp21) << " " << (qkp23 == qkp22) << " " << (qkp23 == qkp23) << " "
                                 << (qkp23 == qkp31) << " " << (qkp23 == qkp32) << " " << (qkp23 == qkp33) << std::endl;



  std::cout << "<quan3, key1>: " << (qkp31 == qkp11) << " " << (qkp31 == qkp12) << " " << (qkp31 == qkp13) << " "
                                 << (qkp31 == qkp21) << " " << (qkp31 == qkp22) << " " << (qkp31 == qkp23) << " "
                                 << (qkp31 == qkp31) << " " << (qkp31 == qkp32) << " " << (qkp31 == qkp33) << std::endl;

  std::cout << "<quan3, key2>: " << (qkp32 == qkp11) << " " << (qkp32 == qkp12) << " " << (qkp32 == qkp13) << " "
                                 << (qkp32 == qkp21) << " " << (qkp32 == qkp22) << " " << (qkp32 == qkp23) << " "
                                 << (qkp32 == qkp31) << " " << (qkp32 == qkp32) << " " << (qkp32 == qkp33) << std::endl;

  std::cout << "<quan3, key3>: " << (qkp33 == qkp11) << " " << (qkp33 == qkp12) << " " << (qkp33 == qkp13) << " "
                                 << (qkp33 == qkp21) << " " << (qkp33 == qkp22) << " " << (qkp33 == qkp23) << " "
                                 << (qkp33 == qkp31) << " " << (qkp33 == qkp32) << " " << (qkp33 == qkp33) << std::endl;


  std::cout << "Testing for key key1:" << std::endl;
  //KeyWrapper<double> kw1();
  KeyInterface * pKI = new KeyWrapper<double>();

  std::cout << "<quan1, key1>: " << qkp11.sameKey(*pKI) << std::endl;
  std::cout << "<quan1, key2>: " << qkp12.sameKey(*pKI) << std::endl;
  std::cout << "<quan1, key3>: " << qkp13.sameKey(*pKI) << std::endl;

  std::cout << "<quan2, key1>: " << qkp21.sameKey(*pKI) << std::endl;
  std::cout << "<quan2, key2>: " << qkp22.sameKey(*pKI) << std::endl;
  std::cout << "<quan2, key3>: " << qkp23.sameKey(*pKI) << std::endl;

  std::cout << "<quan3, key1>: " << qkp31.sameKey(*pKI) << std::endl;
  std::cout << "<quan3, key2>: " << qkp32.sameKey(*pKI) << std::endl;
  std::cout << "<quan3, key3>: " << qkp33.sameKey(*pKI) << std::endl;
  delete pKI;


  std::cout << "Testing for key key2:" << std::endl;
  //KeyWrapper<double> kw1();
  pKI = new KeyWrapper<char>();

  std::cout << "<quan1, key1>: " << qkp11.sameKey(*pKI) << std::endl;
  std::cout << "<quan1, key2>: " << qkp12.sameKey(*pKI) << std::endl;
  std::cout << "<quan1, key3>: " << qkp13.sameKey(*pKI) << std::endl;

  std::cout << "<quan2, key1>: " << qkp21.sameKey(*pKI) << std::endl;
  std::cout << "<quan2, key2>: " << qkp22.sameKey(*pKI) << std::endl;
  std::cout << "<quan2, key3>: " << qkp23.sameKey(*pKI) << std::endl;

  std::cout << "<quan3, key1>: " << qkp31.sameKey(*pKI) << std::endl;
  std::cout << "<quan3, key2>: " << qkp32.sameKey(*pKI) << std::endl;
  std::cout << "<quan3, key3>: " << qkp33.sameKey(*pKI) << std::endl;
  delete pKI;


  std::cout << "Testing for key key3:" << std::endl;
  //KeyWrapper<double> kw1();
  pKI = new KeyWrapper<long>();

  std::cout << "<quan1, key1>: " << qkp11.sameKey(*pKI) << std::endl;
  std::cout << "<quan1, key2>: " << qkp12.sameKey(*pKI) << std::endl;
  std::cout << "<quan1, key3>: " << qkp13.sameKey(*pKI) << std::endl;

  std::cout << "<quan2, key1>: " << qkp21.sameKey(*pKI) << std::endl;
  std::cout << "<quan2, key2>: " << qkp22.sameKey(*pKI) << std::endl;
  std::cout << "<quan2, key3>: " << qkp23.sameKey(*pKI) << std::endl;

  std::cout << "<quan3, key1>: " << qkp31.sameKey(*pKI) << std::endl;
  std::cout << "<quan3, key2>: " << qkp32.sameKey(*pKI) << std::endl;
  std::cout << "<quan3, key3>: " << qkp33.sameKey(*pKI) << std::endl;
  delete pKI;


}



void testCopyTransfer()
{







}