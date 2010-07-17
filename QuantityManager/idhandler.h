/***************************************************************************
 *   Copyright (C) 2007 by Karl Rupp   *
 *   elfem@karlrupp.net   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/


  /******************* ID Handling *******************************/

#ifndef ELFEM_IDHANDLER_GUARD
#define ELFEM_IDHANDLER_GUARD

  struct NoID
  {
    NoID() {};
    //NoID(long id) {};

    //for compatibility:
    void setID(long id) { };
    const NoID * getID() const { return this; };

  };

  class ProvideID
  {
    public:
      ProvideID() : id_(-1) {};
      //ProvideID(long id): id_(id) {}

      long getID() const { return id_; };
      void setID(long id) { id_ = id; };

    protected:
      long id_;
  };

#endif
