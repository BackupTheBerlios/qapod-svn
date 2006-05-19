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

#include <QtGui>

#define FILEFORMAT "yyyyMMddhhmmss"


/**
	@author wolfgang loeffler <wolfgang.loeffler@entropia.biz>
*/
class Getter : virtual public QObject{
    Q_OBJECT
  public:
    Getter( QObject *parent, QSettings *settings, const QString& st );
    QImage getImage() { return image; };
    const QString& getDescription() { return description; }
    const QString& getLastModified() { return lastModified; }
    
  public slots:
    virtual void update() {};
    void updateIsDone(bool havenew, const QString& pod);

  signals:
    void updateFinished(bool havenew, const QString& fn);
    void updateProgress(const QString& st, int bytesRead, int totalBytes );
    void updateDone(bool havenew, const QString& pod);

  public slots:

  public:
    QObject *parentObj;
    QSettings *settings;
    QString sourceType;
    QString lastModified;
    QString description;
    QImage image;

};

#endif
