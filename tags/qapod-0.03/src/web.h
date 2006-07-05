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
#ifndef WEB_H
#define WEB_H

#include <QtGui>
#include <QtNetwork>
#include "qeeventloop.h"

/**
	@author wolfgang loeffler <wolfgang.loeffler@entropia.biz>
*/

class WebObj: public QObject {
    Q_OBJECT
  public:
    WebObj( QObject *parent, QString hostname, QString location, QBuffer *buffer );
    ~WebObj();
  public slots:
    void starte();
  private slots:
    void httpRequestFinished( int httpid, bool error ) ;
  public:
    void run();
  public:
    QBuffer *buff;

  private:
    QHttp *http;
    int id;
    QString host;
    QString loc;

};

class Web : public QObject {
    Q_OBJECT
  public:
    Web( QObject *parent, QString hostname, QString location, QBuffer *buffer );
    ~Web();
    QeEventLoop *webEL;
  private:
    WebObj *webObj;
};



#endif
