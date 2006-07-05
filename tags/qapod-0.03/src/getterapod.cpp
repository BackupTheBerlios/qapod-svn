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
#include "web.h"

/**
	@author wolfgang loeffler <wolfgang.loeffler@entropia.biz>
 */

GetterAPOD::GetterAPOD( QObject *parent, QSettings *settings, const QString& st ) : Getter( parent, settings , st ) {}

void GetterAPOD::update() {
  QString hostname = "antwrp.gsfc.nasa.gov";
  image = QImage();
  description = "";
  QBuffer *buffer = new QBuffer( this );
  Web( 0, hostname, "/apod/astropix.html", buffer );

  updateResult = "Error parsing astropix.html!";
  QString s( buffer->buffer().data() );
  int posi = s.indexOf( "<a href=\"image" );
  if ( posi <= 0 ) return ;
  int endpos = s.indexOf( "\"", posi+10 );
  if ( endpos <= 0 ) return ;
  QString link = s.mid( posi + 9, endpos - posi - 9 );
  qDebug() << "link=" << link;

  if ( link == lastModified ) {
    updateResult = "No new image!";
    emit ( updateDone( false, sourceType ) );
    return ;
  } else {
    lastModified = link;
  }

  if ( link.endsWith(".gif") ) {
    updateResult = "Image is gif, maybe animated, skipped!";
    emit ( updateDone( false, sourceType ) );
    return ;
  }

  posi = s.indexOf( "</center>" , endpos );
  if ( posi > 0 ) {
    posi += 9;
    int endpos = s.indexOf( "<b> Tomorrow's picture: </b>", posi );
    if ( endpos > 0 ) {
      description = s.mid( posi, endpos - posi );
    }
  }

  if ( link != "" ) {
    buffer = new QBuffer(this);
    Web( this, hostname, "/apod/" + link, buffer );

    buffer->open( QBuffer::ReadWrite );
    image.loadFromData( buffer->data(), "jpg" );
    updateResult = "Image updated!";
    emit ( updateDone( true, sourceType ) );
  }
  emit ( updateDone( false, sourceType ) );
  qDebug() << "update apod done";

}
