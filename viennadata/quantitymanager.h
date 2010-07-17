/* =======================================================================
   Copyright (c) 2010, Institute for Microelectronics, TU Vienna.
   http://www.iue.tuwien.ac.at
                             -----------------
                     ViennaData - The Vienna Data Storage Library
                             -----------------

   authors:    Karl Rupp                          rupp@iue.tuwien.ac.at

   license:    MIT (X11), see file LICENSE in the ViennaMesh base directory
======================================================================= */


#ifndef VIENNADATA_QUANTITYMANAGER_GUARD
#define VIENNADATA_QUANTITYMANAGER_GUARD

#include <map>
#include <vector>
#include <list>
#include <set>
#include <iostream>
#include <algorithm>
#include <memory>

#include "forwards.h"
#include "idhandler.h"
#include "quanTraits.h"
#include "QuanKeyPair.h"
#include "storagebank.h"


namespace viennadata
{

  //namespace {

/********************** Quantity Manager ***********************/


    //use a map for storage of data, i.e. object-based keys
    template <typename ElementType,
              typename QuanType,
              typename KeyType>
    class QuanMan_data_holder <ElementType, QuanType, KeyType, FullDispatch>
    {
      public:
        QuanMan_data_holder() {};
        QuanMan_data_holder( QuanMan_data_holder const & other ) : quan_map(other.quan_map) {};

        void operator=(QuanMan_data_holder const & other) { quan_map = other.quan_map; }

        typename QuanTraits<QuanType>::ResultType getQuantity(KeyType const & key) { return quan_map[key]; }

        void       setQuantity(QuanType const & quan, KeyType const & key) { quan_map[key] = quan; }

        void erase(KeyType const & key) { quan_map.erase(key); }

      private:
        std::map<KeyType, QuanType>   quan_map;
    };


    //Direct access:
    template <typename ElementType,
              typename QuanType,
              typename KeyType>
    class QuanMan_data_holder <ElementType, QuanType, KeyType, TypeBasedDispatch>
    {
      public:
        QuanMan_data_holder() {};
        QuanMan_data_holder( QuanMan_data_holder const & other ) : quan(other.quan) {};
        void operator=(QuanMan_data_holder const & other) { quan = other.quan; }

        typename QuanTraits<QuanType>::ResultType getQuantity(KeyType const & key) const { return quan; }
        void       setQuantity(QuanType const & quan_, KeyType const & key) { quan = quan_; }

        void erase(KeyType const & key) { /* do nothing */ }
        void eraseAll(KeyType const & key) { /* do nothing */ }

      private:
        QuanType quan;
    };




    //dispatch with respect to the type of access (here: type KeyType)
    template <typename ElementType,
              typename QuanType,
              typename KeyType,
              typename StorageTag>
    class QuanMan_element_dispatcher< ElementType,
                                      QuanType, 
                                      KeyType,
                                      StorageTag,
                                      SparseAccessTag   >
    {
      typedef QuanMan_data_holder <ElementType, QuanType, KeyType, StorageTag>    MapDataType;
      typedef std::map<ElementType *, MapDataType >                               MapType;

      public:
        typename QuanTraits<QuanType>::ResultType getQuantity(ElementType * el, KeyType const & key) { return map_[el].getQuantity(key); }

        void setQuantity(ElementType *el, QuanType const & quan, KeyType const & key) { map_[el].setQuantity(quan, key); }

        void reserveQuantity(KeyType const & key, long size) { /*do nothing */ }

        void eraseQuantity(ElementType *el, KeyType const & key) { map_[el].erase(key); }
        void clearQuanKeyPair(ElementType *el) { map_.erase(el); }

        bool hasQuantity(ElementType * el, KeyType const & key) { return ( map_.find(el) != map_.end() ); }

        void copyQuantity(ElementType * src, ElementType * dest)
        {
          typedef typename MapType::iterator              MapIteratorType;

          MapIteratorType mit = map_.find(src);

          if ( mit != map_.end() )
          {
            map_[dest] = mit->second;
          }
        }

        void transferQuantity(ElementType *src, ElementType *dest)
        {
          typedef typename MapType::iterator              MapIteratorType;

          MapIteratorType mit = map_.find(src);

          if ( mit != map_.end() )
          {
            map_[dest] = mit->second;
            map_.erase(mit);
          }
        }

      private:
        MapType map_;
    };


    // Improved quantity manager for the case an element-id exists:
    template <typename ElementType,
              typename QuanType,
              typename KeyType,
              typename StorageTag>
    class QuanMan_element_dispatcher< ElementType,
                                      QuanType, 
                                      KeyType,
                                      StorageTag,
                                      DenseAccessTag   >
    {
      typedef QuanMan_data_holder <ElementType, QuanType, KeyType, StorageTag>    VectorDataType;
      typedef std::vector< VectorDataType >                                       VecType;

      public:
        typename QuanTraits<QuanType>::ResultType getQuantity(ElementType * el, KeyType const & key) { return vec_[el->getID()].getQuantity(key); }

        void setQuantity(ElementType *el, QuanType const & quan, KeyType const & key) { vec_[el->getID()].setQuantity(quan, key); }

        void reserveQuantity(KeyType const & key, long size)
        {
          if (size < 1)
            std::cerr << "FATAL ERROR in QuantityManager: reserveQuantity with non-positive size " << size << std::endl;
          else
            vec_.resize(size);
        }

        void eraseQuantity(ElementType *el, KeyType const & key) { /* do nothing */ }
        void clearQuanKeyPair(ElementType *el) { /* do nothing */ }

        //for compatibility reasons:
        bool hasQuantity(ElementType * el, KeyType const & key) { return vec_.size() > 0; }

        void copyQuantity(ElementType const *src, ElementType *dest) const
        {
          typedef typename VecType::iterator              VecIteratorType;

          VecIteratorType vit = vec_.find(src);

          if ( vit != vec_.end() )
            vec_[dest] = *vit;
        }

        void transferQuantity(ElementType *src, ElementType *dest)
        {
          typedef typename VecType::iterator              VecIteratorType;

          VecIteratorType vit = vec_.find(src);

          if ( vit != vec_.end() )
          {
            vec_[dest] = *vit;
            vec_.erase(vit);
          }
        }

      private:
        VecType vec_;
    };




    template <typename ElementType, typename IDHandler, typename StorageBankType>
    class SingleBankQuantityManager 
        : public IDHandler,
          public StorageBankInterface< SingleBankQuantityManager<ElementType, IDHandler, StorageBankType>,
                                       ElementType,
                                       StorageBankType >
    {
        typedef SingleBankQuantityManager<  ElementType,
                                            IDHandler, 
                                            StorageBankType >         SelfType;

        typedef QuantityManager<ElementType, IDHandler, StorageBankType>        QuantityManagerType;

        typedef StorageBankInterface< SelfType,
                                      ElementType,
                                      StorageBankType >     StorageBank;

      public:

        /////////////////// Data handling member functions ///////////////////////////
        template <typename QuanType, typename KeyType>
        typename QuanTraits<QuanType>::ResultType retrieveQuantity(KeyType const & key)
        {
          //std::cout << "retrieveQuantity using object " << this << std::endl;
          return getManager<QuanType, KeyType>().getQuantity(this, key);
        }

        //light-weight variant of retrieveQuantity:
        //Only check, whether something is actually stored for this key.
        //Prevents growth of map if every element has to check for a sparse boolean flag.
        //Does not work with DenseStorageTag (because call is unintentional for such a case)
        template <typename KeyType, typename QuanType>
        bool hasQuantity(KeyType const & key)
        {
          return getManager<QuanType, KeyType>().hasQuantity(this, key);
        }

        template <typename KeyType, typename QuanType>
        void storeQuantity(KeyType const & key, QuanType const & quan)
        {
          getManager<QuanType, KeyType>().setQuantity(this, quan, key);
          //std::cout << "storeQuantity using object " << this << std::endl;
        }

        template <typename QuanType, typename KeyType>
        void reserveQuantity(KeyType const & key, long size = -1)
        {
          //std::cout << "Creating new ID" << std::endl;
          QuanKeyPair<SelfType> qkp(QuanType(), key);
          //std::cout << "ID-address: " << &qkp << std::endl;

          //std::cout << "Searching for existing pair" << std::endl;
          if ( std::find(StorageBank::quankeylist.begin(), StorageBank::quankeylist.end(), qkp) == StorageBank::quankeylist.end() )
          {
            //std::cout << "QuanMan adds pair <"; TypeNamePrinter<QuanType>::apply(); std::cout << ","; TypeNamePrinter<KeyType>::apply(); std::cout << ">." << std::endl;
            StorageBank::quankeylist.push_back( qkp );
            //std::cout << "DONE at address" << &(quankeylist.back()) << std::endl;
          }

          //std::cout << "Reserving Quantity of size " << size << " for seg " << CurrentSegmentHolder<DomainConfiguration>::p_seg << std::endl;
          getManager<QuanType, KeyType>().reserveQuantity(key, size);
        }


        /////////////////// data removal member functions ///////////////////////////

        void releaseBank()
        {
          typedef typename StorageBank::ContainerType::iterator      ListIterator;

          for (ListIterator lit = StorageBank::quankeylist.begin();
                lit != StorageBank::quankeylist.end();
                ++lit)
          {
              lit->release(*this);
          }
        }


        template <typename KeyType>
        void releaseKey()
        {
          typedef typename StorageBank::ContainerType::iterator      ListIterator;

          KeyInterface * pKI = new KeyWrapper<KeyType>();

          for (ListIterator lit = StorageBank::quankeylist.begin();
                lit != StorageBank::quankeylist.end();
                ++lit)
          {
            //std::cout << "Releasing pair..." << std::endl;
            if( lit->sameKey(*pKI) )
              lit->release(*this);
          }
        }

        // deletes all data for <KeyType, QuanType> in current storage bank:
        template <typename KeyType, typename QuanType>
        void releaseQuantity()
        {
          getManager<QuanType, KeyType>().clearQuanKeyPair(this);

          //finally, storage is freed.
          getQuanManSelector<QuanType, KeyType>().eraseQuanMan( StorageBankHolder<ElementType, StorageBankType>::getCurrentBank() );
        }

        template <typename KeyType>
        void eraseKey()
        {
          typedef typename StorageBank::ContainerType::iterator      ListIterator;

          KeyInterface * pKI = new KeyWrapper<KeyType>();

          for (ListIterator lit = StorageBank::quankeylist.begin();
                lit != StorageBank::quankeylist.end();
                ++lit)
          {
            //std::cout << "Releasing pair..." << std::endl;
            if( lit->sameKey(*pKI) )
              lit->eraseQuantity(*this);
          }
        }

        template <typename KeyType>
        void eraseKey(KeyType const & key)
        {
          //TODO: Implementation
        }

        template <typename KeyType, typename QuanType>
        void eraseQuantity()
        {
          getManager<QuanType, KeyType>().clearQuanKeyPair(this);
        }

        //erase a particular quantity identified by key:
        template <typename QuanType, typename KeyType>    //note: reversed template argument order necessary here!
        void eraseQuantity(KeyType const & key)
        {
          getManager<QuanType, KeyType>().eraseQuantity(this, key);
        }

        void eraseBank()
        {
            //TODO: Implementation
        }


        ////////// Member functions for data transfer ////////////////////////////
        void copyBankTo(SelfType & other)
        {
          typedef typename StorageBank::ContainerType::iterator      ListIterator;

          for (ListIterator lit = StorageBank::quankeylist.begin();
                lit != StorageBank::quankeylist.end();
                ++lit)
          {
            //std::cout << "Copying pair..." << std::endl;
            lit->copyTo(*this, other);
          }
        }

        template <typename KeyType, typename QuanType>
        void copyPairTo(SelfType & other)
        {
          //std::cout << "QuanMan copies pair <"; 
          //TypeNamePrinter<QuanType>::apply(); std::cout << ","; TypeNamePrinter<KeyType>::apply();
          //std::cout << ">." << std::endl;

          typedef typename StorageBankHolder<ElementType, StorageBankType>::iterator    StorageBankIterator;

          const StorageBankType & backupBank = StorageBankHolder<ElementType, StorageBankType>::getCurrentBank();

          //iterate over all banks:
          for (StorageBankIterator sbi = StorageBankHolder<ElementType, StorageBankType>::begin();
                                  sbi != StorageBankHolder<ElementType, StorageBankType>::end();
                                  ++sbi)
          {
            setStorageBank(*sbi);
            getManager<QuanType, KeyType>().copyQuantity(this, &other);
          }

          setStorageBank(backupBank);
        }


        // transfer
        void transferBankTo(SelfType & other)
        {
          typedef typename std::list< QuanKeyPair<SelfType> >::iterator      ListIterator;

          for (ListIterator lit = StorageBank::quankeylist.begin();
                lit != StorageBank::quankeylist.end();
                ++lit)
          {
            //std::cout << "Transfering pair..." << std::endl;
            lit->transferTo(*this, other);
          }
        }

        template <typename KeyType, typename QuanType>
        void transferPairTo(SelfType & other)
        {
          //std::cout << "QuanMan transfers pair <"; 
          //TypeNamePrinter<QuanType>::apply(); std::cout << ","; TypeNamePrinter<KeyType>::apply();
          //std::cout << ">." << std::endl;

          typedef typename StorageBankHolder<ElementType, StorageBankType>::iterator    StorageBankIterator;

          const StorageBankType & backupBank = StorageBankHolder<ElementType, StorageBankType>::getCurrentBank();

          //iterate over all banks:
          for (StorageBankIterator sbi = StorageBankHolder<ElementType, StorageBankType>::begin();
                                  sbi != StorageBankHolder<ElementType, StorageBankType>::end();
                                  ++sbi)
          {
            setStorageBank(*sbi);
            //std::cout << "Transfer on storage bank " << &(*sbi) << std::endl;
            getManager<QuanType, KeyType>().transferQuantity(this, &other);
          }

          setStorageBank(backupBank);
        }

      private:

        template <typename QuanType, typename KeyType>
        QuantityManagerSelector< ElementType,
                                 IDHandler,
                                 StorageBankType,
                                 QuanType,
                                 KeyType > &
        getQuanManSelector() const
        {
          //Meyers' singleton for retrieving the correct storage bank
          static QuantityManagerSelector< ElementType,
                                          IDHandler,
                                          StorageBankType,
                                          QuanType,
                                          KeyType>              qms;
          return qms;
        }

        template <typename QuanType, typename KeyType>
        typename QuantityManagerSelector< ElementType,
                                          IDHandler,
                                          StorageBankType,
                                          QuanType,
                                          KeyType >::QuanManType & 
        getManager() const
        {

          //std::cout << "Address of QuantityManager: " << &quanMan << std::endl;
          return getQuanManSelector<QuanType, KeyType>().getQuantityManager();
        }

    };


    //exterior interface: default case is to provide storage banks
    template <typename ElementType, typename IDHandler, typename StorageBankType>
    class QuantityManager 
         : public SingleBankQuantityManager<ElementType, IDHandler, StorageBankType>
    {
        typedef SingleBankQuantityManager<ElementType, IDHandler, StorageBankType>        BaseType;
        typedef QuantityManager<ElementType, IDHandler, StorageBankType>                  SelfType;
        typedef StorageBankInterface< SelfType, ElementType, StorageBankType >            StorageBank;

      public:
        ~QuantityManager()
        {
          //std::cout << "DTOR of QuantityManager called!()" << std::endl;
          releaseAll(); //all storage elements of that particular object have to be removed, otherwise memory leak.
        }

        //remove all quantities from this object
        void releaseAll()
        {
          typedef typename StorageBankHolder<ElementType, StorageBankType>::iterator    StorageBankIterator;

          //iterate over all banks:
          for (StorageBankIterator sbi = StorageBankHolder<ElementType, StorageBankType>::begin();
                                  sbi != StorageBankHolder<ElementType, StorageBankType>::end();
                                  ++sbi)
          {
            BaseType::setStorageBank(*sbi);
            BaseType::releaseBank();
          }

          //TODO: Which storage bank is NOW in use?
        }
        
        //release all quantities of type key
        template <typename KeyType>
        void releaseKeyAllBanks()
        {
          typedef typename StorageBank::ContainerType::iterator      ListIterator;

          typedef typename StorageBankHolder<ElementType, StorageBankType>::iterator    StorageBankIterator;

          const StorageBankType & oldBank = StorageBankHolder<ElementType, StorageBankType>::getCurrentBank();

          //iterate over all banks:
          for (StorageBankIterator sbi = StorageBankHolder<ElementType, StorageBankType>::begin();
                                  sbi != StorageBankHolder<ElementType, StorageBankType>::end();
                                  ++sbi)
          {
            BaseType::setStorageBank(*sbi);
            BaseType::template releaseKey<KeyType>();
          }
          BaseType::setStorageBank(oldBank);
        }

        template <typename KeyType, typename QuanType>
        void releasePairAllBanks()
        {
          typedef typename StorageBankHolder<ElementType, StorageBankType>::iterator    StorageBankIterator;

          const StorageBankType & oldBank = StorageBankHolder<ElementType, StorageBankType>::getCurrentBank();

          //iterate over all banks:
          for (StorageBankIterator sbi = StorageBankHolder<ElementType, StorageBankType>::begin();
                                  sbi != StorageBankHolder<ElementType, StorageBankType>::end();
                                  ++sbi)
          {
            BaseType::setStorageBank(*sbi);
            BaseType::template releasePair<KeyType, QuanType>();
          }

          //restore previous bank:
          //TODO: Think about else-branch
          BaseType::setStorageBank(oldBank);
        }

        //erase a particular quantity identified by key:
        template <typename QuanType, typename KeyType>    //note: reversed template argument order necessary here!
        void eraseQuantityAllBanks()
        {
          typedef typename StorageBankHolder<ElementType, StorageBankType>::iterator    StorageBankIterator;

          const StorageBankType & oldBank = StorageBankHolder<ElementType, StorageBankType>::getCurrentBank();

          //iterate over all banks:
          for (StorageBankIterator sbi = StorageBankHolder<ElementType, StorageBankType>::begin();
                                  sbi != StorageBankHolder<ElementType, StorageBankType>::end();
                                  ++sbi)
          {
            BaseType::setStorageBank(*sbi);
            BaseType::template eraseQuantity<KeyType>();
          }

          //restore previous bank:
          BaseType::setStorageBank(oldBank);
        }

        template <typename QuanType, typename KeyType>    //note: reversed template argument order necessary here!
        void eraseQuantityAllBanks(KeyType const & key)
        {
          typedef typename StorageBankHolder<ElementType, StorageBankType>::iterator    StorageBankIterator;

          const StorageBankType & oldBank = StorageBankHolder<ElementType, StorageBankType>::getCurrentBank();

          //iterate over all banks:
          for (StorageBankIterator sbi = StorageBankHolder<ElementType, StorageBankType>::begin();
                                  sbi != StorageBankHolder<ElementType, StorageBankType>::end();
                                  ++sbi)
          {
            BaseType::setStorageBank(*sbi);
            BaseType::template eraseQuantity<KeyType>(key);
          }

          //restore previous bank:
          BaseType::setStorageBank(oldBank);
        }

        template <typename KeyType>
        void eraseKeyAllBanks()
        {
          typedef typename StorageBankHolder<ElementType, StorageBankType>::iterator    StorageBankIterator;

          const StorageBankType & oldBank = StorageBankHolder<ElementType, StorageBankType>::getCurrentBank();

          //iterate over all banks:
          for (StorageBankIterator sbi = StorageBankHolder<ElementType, StorageBankType>::begin();
                                  sbi != StorageBankHolder<ElementType, StorageBankType>::end();
                                  ++sbi)
          {
            BaseType::setStorageBank(*sbi);
            BaseType::template eraseKey<KeyType>();
          }

          //restore previous bank:
          BaseType::setStorageBank(oldBank);
        }

        template <typename KeyType>
        void eraseKeyAllBanks(KeyType const & key)
        {
          typedef typename StorageBankHolder<ElementType, StorageBankType>::iterator    StorageBankIterator;

          const StorageBankType & oldBank = StorageBankHolder<ElementType, StorageBankType>::getCurrentBank();

          //iterate over all banks:
          for (StorageBankIterator sbi = StorageBankHolder<ElementType, StorageBankType>::begin();
                                  sbi != StorageBankHolder<ElementType, StorageBankType>::end();
                                  ++sbi)
          {
            BaseType::setStorageBank(*sbi);
            BaseType::template eraseKey<KeyType>(key);
          }

          //restore previous bank:
          BaseType::setStorageBank(oldBank);
        }

        void eraseAll()
        {
          typedef typename StorageBankHolder<ElementType, StorageBankType>::iterator    StorageBankIterator;

          const StorageBankType & oldBank = StorageBankHolder<ElementType, StorageBankType>::getCurrentBank();

          //iterate over all banks:
          for (StorageBankIterator sbi = StorageBankHolder<ElementType, StorageBankType>::begin();
                                  sbi != StorageBankHolder<ElementType, StorageBankType>::end();
                                  ++sbi)
          {
            BaseType::setStorageBank(*sbi);
            BaseType::template eraseBank();
          }

          //restore previous bank:
          BaseType::setStorageBank(oldBank);
        }


        // copy
        void copyAllTo(SelfType & other)
        {
          typedef typename StorageBankHolder<ElementType, StorageBankType>::iterator    StorageBankIterator;

          const StorageBankType & oldBank = StorageBankHolder<ElementType, StorageBankType>::getCurrentBank();

          //iterate over all banks:
          for (StorageBankIterator sbi = StorageBankHolder<ElementType, StorageBankType>::begin();
                                  sbi != StorageBankHolder<ElementType, StorageBankType>::end();
                                  ++sbi)
          {
            BaseType::setStorageBank(*sbi);
            BaseType::copyBankTo(other);
          }

          //restore previous bank:
          BaseType::setStorageBank(oldBank);
        }

        // transfer:
        void transferAllTo(SelfType & other)
        {
          typedef typename StorageBankHolder<ElementType, StorageBankType>::iterator    StorageBankIterator;

          const StorageBankType & oldBank = StorageBankHolder<ElementType, StorageBankType>::getCurrentBank();

          //iterate over all banks:
          for (StorageBankIterator sbi = StorageBankHolder<ElementType, StorageBankType>::begin();
                                  sbi != StorageBankHolder<ElementType, StorageBankType>::end();
                                  ++sbi)
          {
            BaseType::setStorageBank(*sbi);
            BaseType::transferBankTo(other);
          }

          //restore previous bank:
          BaseType::setStorageBank(oldBank);
        }
    };

    //exterior interface: special case: no storage banks are required
    template <typename ElementType, typename IDHandler>
    class QuantityManager<ElementType, IDHandler, NoStorageBanks>
         : public SingleBankQuantityManager<ElementType, IDHandler, NoStorageBanks>
    {
        typedef SingleBankQuantityManager<ElementType, IDHandler, NoStorageBanks>        BaseType;
        typedef QuantityManager<ElementType, IDHandler, NoStorageBanks>                  SelfType;

      public:
        ~QuantityManager()
        {
          //std::cout << "DTOR of QuantityManager called!()" << std::endl;
          releaseAll(); //all storage elements of that particular object have to be removed, otherwise memory leak.
        }

        //remove all quantities from this object
        void releaseAll()
        {
            BaseType::releaseBank();
        }

        void eraseAll()
        {
            BaseType::eraseBank();
        }

        // copy
        void copyAllTo(SelfType & other)
        {
            BaseType::copyBankTo(other);
        }

        // transfer:
        void transferAllTo(SelfType & other)
        {
            BaseType::transferBankTo(other);
        }
    };

} //namespace Vienna

#endif
