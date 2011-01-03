/* =======================================================================
   Copyright (c) 2010, Institute for Microelectronics, TU Vienna.
   http://www.iue.tuwien.ac.at
                             -----------------
                     ViennaData - The Vienna Data Storage Library
                             -----------------

   authors:    Markus Bina                     bina@iue.tuwien.ac.at

   license:    MIT (X11), see file LICENSE in the ViennaMesh base directory
======================================================================= */

#ifndef VLOG_GUARD
#define VLOG_GUARD

// give true to enable debug messages on std::clog
#define VIENNADATA_DEBUG false

namespace viennadata {

class nullstream
{
   public:
   nullstream() {}
};

#if VIENNADATA_DEBUG
  static std::ostream & vlog = std::clog; // debugging ON
#else
  static const nullstream vlog; // debugging OFF

const nullstream& nullOp() { return vlog; }

const nullstream& operator<< (nullstream& out, bool& val ) { return nullOp(); }
const nullstream& operator<< (nullstream& out, short& val ) { return nullOp(); }
const nullstream& operator<< (nullstream& out, unsigned short& val ) { return nullOp(); }
const nullstream& operator<< (nullstream& out, int& val ) { return nullOp(); }
const nullstream& operator<< (nullstream& out, unsigned int& val ) { return nullOp(); }
const nullstream& operator<< (nullstream& out, long& val ) { return nullOp(); }
const nullstream& operator<< (nullstream& out, unsigned long& val ) { return nullOp(); }
const nullstream& operator<< (nullstream& out, float& val ) { return nullOp(); }
const nullstream& operator<< (nullstream& out, double& val ) { return nullOp(); }
const nullstream& operator<< (nullstream& out, long double& val ) { return nullOp(); }
const nullstream& operator<< (nullstream& out, void* val ) { return nullOp(); }

const nullstream& operator<< (const nullstream & out, std::ostream& x ) { return nullOp(); }

const nullstream& operator<< (const nullstream& out, const nullstream& x) { return nullOp(); }

const nullstream& operator<< (const nullstream& out, char* x ) { return nullOp(); }
const nullstream& operator<< (const nullstream& out, char c ) { return nullOp(); }
const nullstream& operator<< (const nullstream& out, signed char c ) { return nullOp(); }
const nullstream& operator<< (const nullstream& out, unsigned char c ) { return nullOp(); }

const nullstream& operator<< (const nullstream& out, const char* s ) { return nullOp(); }
const nullstream& operator<< (const nullstream& out, const signed char* s ) { return nullOp(); }
const nullstream& operator<< (const nullstream& out, const unsigned char* s ) { return nullOp(); }

// type of std::cout
typedef std::basic_ostream<char, std::char_traits<char> > CoutType;

// function signature of std::endl
typedef CoutType& (*StandardEndLine)(CoutType&);

// operator<< to take in std::endl
const nullstream& operator<<(const nullstream out, StandardEndLine manip)
{
  return nullOp();
}
#endif


}

#endif
