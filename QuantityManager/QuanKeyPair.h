//
// C++ Interface: virtualID
//
// Description: 
//
//
// Author:  <>, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//


  //purpose: type erase so that a particular QuantityManager is able to delete all quantities associated with a particular object
  //concepts used: type erasure, cf. Alexandrescu

  template <typename QuanManType>
  class QuanKeyPair;

  template <typename QuanManType, typename QuanType, typename KeyType>
  struct QuanKeyWrapper;

  class KeyInterface
  {
    public:
      virtual ~KeyInterface() {};

      virtual bool operator==(KeyInterface const &) const = 0;
  };


  template <typename QuanManType>
  class QuanKeyPairInterface
  {
    public:
      virtual ~QuanKeyPairInterface() {};

      //virtual void print() const = 0;
      virtual void release(QuanManType & src) const = 0;   //releases all data for that particular quantity-key pairing (considers types only!)
      virtual void eraseQuantity(QuanManType & src) const = 0;   //releases all data for that particular quantity-key pairing on a particular element
      virtual void copyTo(QuanManType & src, QuanManType & dest) const = 0;
      virtual void transferTo(QuanManType & src, QuanManType & dest) const = 0;

      virtual bool operator==(QuanKeyPairInterface const &) const = 0;
      virtual bool sameKey(KeyInterface const &) const = 0;
  };

  template <typename KeyType>
  class KeyWrapper : public KeyInterface
  {
      typedef KeyWrapper< KeyType >   SelfType;

    public:
      KeyWrapper(){};
  
      bool operator==(KeyInterface const & rhs) const
      {
        return dynamic_cast< SelfType const *>( &rhs ) != NULL;
      }
  };


  template <typename QuanManType, typename QuanType, typename KeyType>
  class QuanKeyPairWrapper : public QuanKeyPairInterface<QuanManType>
  {

    typedef QuanKeyPairWrapper<QuanManType, QuanType, KeyType>    SelfType;

    public:
      QuanKeyPairWrapper() : pKeyInterface( new KeyWrapper<KeyType>() ) {} ;

      //void print() const {};

      void release(QuanManType & src) const
      {
        src.template releaseQuantity<KeyType, QuanType>();
      }

      void eraseQuantity(QuanManType & src) const
      {
        src.template eraseQuantity<KeyType, QuanType>();
      }

      void copyTo(QuanManType & src, QuanManType & dest) const
      {
        src.template copyPairTo<KeyType, QuanType>(dest);
      }

      void transferTo(QuanManType & src, QuanManType & dest) const
      {
        src.template transferPairTo<KeyType, QuanType>(dest);
      }
  
      //comparison with same type:
      bool operator==(QuanKeyPairInterface<QuanManType> const & rhs) const
      {
        //std::cout << "Entering virtual comparison" << std::endl;
        //return rhs.compare(*this);
//         SelfType const *p_subclass = dynamic_cast< SelfType const *>( &rhs );
//         return p_subclass != NULL;
        return dynamic_cast< SelfType const *>( &rhs ) != NULL;
      }

      bool sameKey(KeyInterface const & rhs) const
      {
        return *pKeyInterface == rhs;
      }

    private:
      //wrapper for QuanType
      //wrapper for KeyType
      KeyInterface * pKeyInterface;

  };


  template <typename QuanManType>
  class QuanKeyPair
  {
    public:

      template <typename QuanType, typename KeyType>
      QuanKeyPair(QuanType const & qt, KeyType const & kt) : pQuanInterface(new QuanKeyPairWrapper<QuanManType, QuanType, KeyType>()),
                                                             isOwner(true) {};

      //copy ctor:
      QuanKeyPair(const QuanKeyPair & other)
      {
        //std::cout << "QuanKeyPair copy constructor called for " << this << " with other=" << &other << "!" << std::endl;
        pQuanInterface = other.pQuanInterface;
        isOwner = true;
        other.isOwner = false;
      }

      ~QuanKeyPair()
      {
        if (isOwner)
        {
          delete pQuanInterface;
          //std::cout << "Deleting pQuanInterface" << std::endl;
        }
      }


      bool operator==(QuanKeyPair<QuanManType> const & other) const
      {
        //std::cout << "Comparing in QuanKeyPair" << std::endl;
        return *pQuanInterface == *(other.pQuanInterface);
      }

      void release(QuanManType & src) { pQuanInterface->release(src); }
      void eraseQuantity(QuanManType & src) { pQuanInterface->eraseQuantity(src); }

      void copyTo(QuanManType & src, QuanManType & dest) { pQuanInterface->copyTo(src, dest); }

      void transferTo(QuanManType & src, QuanManType & dest) { pQuanInterface->transferTo(src, dest); }

      bool sameKey(KeyInterface const & rhs) const
      {
        return pQuanInterface->sameKey(rhs);
      }

    private:
      QuanKeyPairInterface<QuanManType> * pQuanInterface;
      mutable bool isOwner;
  };
