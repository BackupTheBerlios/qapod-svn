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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_MainWindow.h"
#include "getterapod.h"
#include "getter.h"
#include <QString>
#include <QImage>
#include <QSettings>
#include <QFileDialog>
#include <QDateTime>
#include <QTextStream>
#include <QFile>
#include <QStringList>
#include <QStringListModel>
#include <QItemDelegate>
#include <QAbstractItemDelegate>
#include <QPainter>

#define FILEFORMAT "yyyyMMddhhmmss"
#define ICONWIDTH 100
#define ICONHEIGHT 100


class MyModel : public QAbstractListModel {
    Q_OBJECT
  public:
    MyModel( QObject *parent = 0 );
    int rowCount( const QModelIndex &parent = QModelIndex() ) const;
    void addItem( QString sss );
    QImage getImage( QString name );
    void clearAll();
    QVariant data( const QModelIndex &index, int role ) const;

  private:
    QStringList nameList;
};

class MyDelegate: public QAbstractItemDelegate {
    Q_OBJECT
  public:
    MyDelegate( QObject * parent = 0 );
    void paint( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    QSize sizeHint( const QStyleOptionViewItem & option,
                    const QModelIndex & index ) const;
    void setDir( QString dir );
  private:
    QString basedir;
};

class MainWindow : public QMainWindow {
    Q_OBJECT

  public:
    MainWindow(int argc, char *argv[], QWidget *parent = 0 );

  private slots:
    void saveSettings();
    void browseImageLocation();
    void updateImage(QString st = "");
    void updateAll();
    void updateList();
    void listViewclicked( const QModelIndex & index );
    void deleteSelected();
    void setAsBackground();
    void setBackground(QString name);
    void installAutostart();
    void installDesktop();
    void removeDesktop();
    void removeAutostart();
    void anchorClicked ( const QUrl &link);

  public slots:
    void updateImageDone(bool havenew, QString pod);
    void updateDataReadProgress( int bytesRead, int totalBytes );

  private:
    Ui::MainWindow ui;
    QSettings *settings;
    Getter *getter;
    MyModel model;
    MyDelegate delegate;
    bool modeAuto;
    QStringList getters;
};















#endif














