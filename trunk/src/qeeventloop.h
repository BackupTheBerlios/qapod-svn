/***************************************************************************
*   Copyright (C) 2005 by wolfgang loeffler   *
*   wolfgang.loeffler@entropia.biz   *
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
#ifndef QEEVENTLOOP_H
#define QEEVENTLOOP_H

#include <QThread>

/**
	@author wolfgang loeffler <wolfgang.loeffler@entropia.biz>
*/
class QeEventLoop : public QThread {
  public:
    //! Constructor
    QeEventLoop( void );
    //! Destructor
    ~QeEventLoop( void );
  protected:
    //! The Thread method, it just calls QThread::exec()
    void run();
    bool started;
};


#endif
