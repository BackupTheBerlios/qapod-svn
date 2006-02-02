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
#ifndef GETTER_H
#define GETTER_H

#include <QObject>
#include <QImage>
#include <QString>
#include <QThread>

/**
	@author wolfgang loeffler <wolfgang.loeffler@entropia.biz>
*/
class Getter : virtual public QObject{
    Q_OBJECT
  public:
    Getter( QObject *parent, QString lastmod, QString st );
    QImage getImage() { return QImage(); };
    QString getDescription() { return description; }
    QString getLastModified() { return lastModified; }
    
  public slots:
    virtual void update() {};

  signals:
    void updateFinished(bool havenew, const QString &st);

  public slots:

  public:
    QObject *parentObj;
    QString lastModified;
    QString sourceType;
    QString description;
    QImage image;

};

#endif
