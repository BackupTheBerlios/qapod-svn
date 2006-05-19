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
#include "getterepod.h"


#include <QObject>
#include <QHttp>
#include <QBuffer>
#include <QImage>
#include <QtGui>
#include "getter.h"

/**
	@author wolfgang loeffler <wolfgang.loeffler@entropia.biz>
 */

GetterEPOD::GetterEPOD( QObject *parent, QString lastmod ) {
  parentObj = parent;
  lastModified = lastmod;
  hostname = "epod.usra.edu";
}

QImage GetterEPOD::getImage() { return image; }

void GetterEPOD::update() {
  http = new QHttp( this );
  buffer = new QBuffer( this );
  image = QImage();
  description = "";
  connect( http, SIGNAL( requestFinished( int, bool ) ), this, SLOT( httpRequestFinished( int, bool ) ) );
  connect( http, SIGNAL( dataReadProgress( int, int ) ), parentObj, SLOT( updateDataReadProgress( int, int ) ) );

  QHttpRequestHeader header( "GET", "/" );
  header.setValue( "Host", hostname );

  // das geht nicht, da apod's webserver kein "last-modified" schickt :-(
  header.setValue( "If-Modified-Since", lastModified );

  header.setValue( "User-Agent", "qapod" );
  http->setHost( hostname );
  httpIdIndex = http->request( header, 0, buffer );
}

QString GetterEPOD::getDescription() { return description; }
QString GetterEPOD::getLastModified() { return lastModified; }


void GetterEPOD::httpRequestFinished( int id, bool error ) {
  QHttpResponseHeader resp = http->lastResponse ();

  if ( error != 0 ) return ;

  if ( httpIdIndex == id ) { // got index.html
    //QString newLastModified = resp.value( "last-modified" ); // s.o. :-(

    // title
    QString s( buffer->buffer().data() );
    description = "";
    int posi = s.indexOf( "<H2>" );
    if ( posi > 0 ) {
      int endpos = s.indexOf( "</H2>", posi );
      if ( endpos > 0 ) {
        description = s.mid( posi, endpos - posi );
      }
    }
    // link
    QString link = "";
    posi = s.indexOf( "<A HREF= \"archive" );
    if ( posi > 0 ) {
      int endpos = s.indexOf( "TARGET=\"_window\">", posi );
      if ( endpos > 0 ) {
        link = s.mid( posi + 10, endpos - posi - 12 );
        qDebug() << "link=" << link;
      }
    }

    if ( link == lastModified ) {
      qDebug() << "nothing new...";
      emit ( updateFinished( false ) );
      return ;
    } else {
      lastModified = link;
    }

    // descr
    posi = s.indexOf( "</A>", posi );
    if ( posi > 0 ) {
      int endpos = s.indexOf( "Coming soon...", posi );
      if ( endpos > 0 ) {
        description += s.mid( posi + 4, endpos - posi - 5 );
      }
    }
    if ( link != "" ) {
      buffer = new QBuffer( this );
      http->setHost( hostname );
      httpIdImage = http->get( "/" + link, buffer );
    }

  } else if ( httpIdImage == id ) { // got image!
    buffer->open( QBuffer::ReadWrite );
    image.loadFromData( buffer->data(), "jpg" );
    emit ( updateFinished( true ) );
  }
}


