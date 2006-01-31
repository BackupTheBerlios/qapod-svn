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
#include "web.h" 
/**
	@author wolfgang loeffler <wolfgang.loeffler@entropia.biz>
 */


GetterEPOD::GetterEPOD( QObject *parent, QString lastmod , QString st ) : Getter( parent, lastmod, st ) {
  hostname = "epod.usra.edu";
}



void GetterEPOD::update() {
  image = QImage();
  description = "";
  QBuffer *buffer = new QBuffer(this);

  
  
  Web *web = new Web( this, hostname, "/", buffer );
  web->start();
  qDebug() << "nach web.start()";
  return;
  ////////////////////////
  
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
    qDebug() << "nothing new..." << " link=" << link << " lastmod=" << lastModified;
    emit ( Getter::updateFinished( false, sourceType ) );
    return ;
  } else {
    qDebug() << "new..." << " link=" << link << " lastmod=" << lastModified;
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
    Web web( this, hostname, "/" + link, buffer );
    web.start();
    web.wait();

    buffer->open( QBuffer::ReadWrite );
    image.loadFromData( buffer->data(), "jpg" );
    emit ( updateFinished( true, sourceType ) );

  }




  qDebug() << "update done";
}

