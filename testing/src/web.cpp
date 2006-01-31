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
#include "web.h"

Web::Web( QObject *parent, QString hostname, QString location , QBuffer *buffer ) : QObject( parent ) {
  host = hostname;
  loc = location;
  buff = buffer;
}

void Web::start() {
  http = new QHttp( this );
  connect( http, SIGNAL( requestFinished( int, bool ) ), this, SLOT( httpRequestFinished( int, bool ) ) , Qt::QueuedConnection );
  http->setHost( host );
  id = http->get( "/" + loc , buff );

//  exec();
}

void Web::httpRequestFinished( int httpid, bool error ) {
  qDebug() << "finished: httpid=" << httpid << " id=" << id;
  sleep( 2 );
  qDebug() << "finished: httpid=" << httpid << " id=" << id;

  if ( httpid != id ) {} else {
    qDebug() << "REALLY.";
  }
}

Web::~Web() {}


QeEventLoop::QeEventLoop( void ) : QThread() {
  start();
}

QeEventLoop::~QeEventLoop( void ) {}


void QeEventLoop::run( void ) {
  exec();
}
