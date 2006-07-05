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
#include "getter.h"

#include <QtGui>

Getter::Getter( QObject *parent, QSettings *settings, const QString&  st ) : QObject( parent ) {
  parentObj = parent;
  this->settings = settings;
  sourceType = st;
  
  emit( updateProgress(st, 0, 100) );
  
  
  lastModified = settings->value( "apod:lastmodified:" + sourceType ).toString();
  connect( this, SIGNAL( updateDone( bool, const QString& ) ), this, SLOT( updateIsDone( bool, const QString& ) ) );
  QMetaObject::invokeMethod( this, "update", Qt::QueuedConnection );
}

void Getter::updateIsDone( bool havenew, const QString& pod ) {
  QString fn = "";
  if ( havenew ) {
    settings->setValue( "apod:lastmodified:" + pod, lastModified );
    settings->sync();
    QDateTime now = QDateTime::currentDateTime();
    fn = settings->value( "imagelocation" ).toString() + "/" + now.toString( FILEFORMAT ) + pod;
    QImage img = image;
    img = img.scaled( settings->value( "bgwidth" ).toInt() ,
                      settings->value( "bgheight" ).toInt() ,
                      Qt::KeepAspectRatio );

    img.save( fn + ".jpg", "jpg", 80 );
    img = img.scaled( settings->value( "thumbwidth" ).toInt() ,
                      settings->value( "thumbheight" ).toInt() ,
                      Qt::KeepAspectRatio );
    img.save( fn + "-thumb.jpg", "jpg", 80 );
    QFile dfile( fn + ".txt" );
    if ( dfile.open( QFile::WriteOnly | QFile::Truncate ) ) {
      QTextStream out( &dfile );
      out << qPrintable( description );
    } else qDebug() << "unable to write to descrfile " << qPrintable( fn ) << ".txt";
  }
  emit( updateProgress(pod, 100, 100) );
  emit( updateFinished( havenew, fn, pod + ": " + updateResult ) );
}
