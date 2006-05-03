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
#include "getterapod.h"


#include <QObject>
#include <QHttp>
#include <QBuffer>
#include <QImage>
#include <QtGui>
#include "getter.h"

/**
	@author wolfgang loeffler <wolfgang.loeffler@entropia.biz>
 */

GetterAPOD::GetterAPOD( QObject *parent, QString lastmod ) {
  hostname = "antwrp.gsfc.nasa.gov";
  parentObj = parent;
  lastModified = lastmod;
}

QImage GetterAPOD::getImage() { return image; }

void GetterAPOD::update() {
  http = new QHttp( this );
  buffer = new QBuffer( this );
  image = QImage();
  description = "";
  connect( http, SIGNAL( requestFinished( int, bool ) ), this, SLOT( httpRequestFinished( int, bool ) ) );
  connect( http, SIGNAL( dataReadProgress( int, int ) ), parentObj, SLOT( updateDataReadProgress( int, int ) ) );

  QHttpRequestHeader header( "GET", "/apod/astropix.html" );
  header.setValue( "Host", hostname );

  // das geht nicht, da apod's webserver kein "last-modified" schickt :-(
  //header.setValue("If-Modified-Since", lastModified);

  header.setValue( "User-Agent", "qapod" );
  http->setHost( hostname );
  httpIdIndex = http->request( header, 0, buffer );
}

QString GetterAPOD::getDescription() { return description; }
QString GetterAPOD::getLastModified() { return lastModified; }


void GetterAPOD::httpRequestFinished( int id, bool error ) {
  QHttpResponseHeader resp = http->lastResponse ();

  if ( error != 0 ) return ;

  if ( httpIdIndex == id ) { // got index.html
    //lastModified = resp.value( "last-modified" ); // s.o. :-(
    QString s( buffer->buffer().data() );
    int posi = s.indexOf( "<a href=\"image" );
    if ( posi > 0 ) {
      int endpos = s.indexOf( ">", posi );
      if ( endpos > 0 ) {
        QString link = s.mid( posi + 9, endpos - posi - 10 );
        if ( link == lastModified ) {
          qDebug() << "nothing new...";
          emit ( updateFinished( false ) );
          return ;
        } else {
          lastModified = link;
        }


        posi = s.indexOf( "</center>" , endpos );
        if ( posi > 0 ) {
          posi += 9;
          int endpos = s.indexOf( "<b> Tomorrow's picture: </b>", posi );
          if ( endpos > 0 ) {
            description = s.mid( posi, endpos - posi );
          }
        }
        buffer = new QBuffer( this );
        http->setHost( hostname );
        httpIdImage = http->get( "/apod/" + link, buffer );
      }
    }



  } else if ( httpIdImage == id ) { // got image!
    buffer->open( QBuffer::ReadWrite );
    image.loadFromData( buffer->data(), "jpg" );
    qDebug() << "buffer->qba->size=" << buffer->data().size() << " image: h=" << image.height() << " w=" << image.width();
    emit ( updateFinished( true ) );
  }
}




