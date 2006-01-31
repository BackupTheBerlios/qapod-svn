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
#ifndef GETTEREPOD_H
#define GETTEREPOD_H

#include <QObject>
#include <QHttp>
#include <QBuffer>
#include <QImage>
#include "getter.h"

/**
	@author wolfgang loeffler <wolfgang.loeffler@entropia.biz>
*/
class GetterEPOD : public Getter {
    Q_OBJECT
  public:
    GetterEPOD( QObject *parent, QString lastmod , QString st ) ;
    void update() ;
//  signals:
//    void updateFinished(bool havenew);
    
  private:
    QString hostname;
};

#endif

