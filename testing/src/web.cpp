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
  webEL = new WebEL();
  WebObj *webObj = new WebObj( 0, hostname, location, buffer );
  webObj->moveToThread( webEL );
  webObj->run();
  while ( webEL->isRunning() ) {
//    sleep( 1 ); Sleep is not portable
    webEL->wait();
  }
}

void WebObj::run() {
  QMetaObject::invokeMethod( this, "starte", Qt::QueuedConnection );
  //  exec();
}

WebObj::WebObj( QObject *parent, QString hostname, QString location , QBuffer *buffer ) : QObject( parent ) {
  host = hostname;
  loc = location;
  buff = buffer;
}

void WebObj::starte() {
  http = new QHttp( this );
  connect( http, SIGNAL( requestFinished( int, bool ) ), this, SLOT( httpRequestFinished( int, bool ) ) , Qt::QueuedConnection );
  http->setHost( host );
  id = http->get( "/" + loc , buff );
  //qDebug() << "getting " << host + "/" + loc;
}

void WebObj::httpRequestFinished( int httpid, bool error ) {
  if ( httpid != id ) {
    //qDebug() << "id=" << id << " httpid=" << httpid;
  } else {
    this->thread()->quit(); // Quit stops nicely, terminate kills
  }
}


QeEventLoop::QeEventLoop( void ) : QThread() {
  started = false;
  start();
  while ( !started )
    msleep( 100 );
}

QeEventLoop::~QeEventLoop( void ) {}


void QeEventLoop::run( void ) {
  started = true;
  exec();
}
