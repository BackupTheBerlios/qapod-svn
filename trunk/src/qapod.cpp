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


#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>

#include "MainWindow.h"
#include <QApplication>

void myMessageOutput(QtMsgType type, const char *msg)
    {
    	FILE *fp = fopen("qapod-debug.txt", "w");
        switch (type) {
        case QtDebugMsg:
            fprintf(fp, "Debug: %s\n", msg);
            break;
        case QtWarningMsg:
            fprintf(fp, "Warning: %s\n", msg);
            break;
        case QtCriticalMsg:
            fprintf(fp, "Critical: %s\n", msg);
            break;
        case QtFatalMsg:
            fprintf(fp, "Fatal: %s\n", msg);
            abort();
        }
        fclose(fp);
    }
    

int main( int argc, char *argv[] ) {
#ifdef WIN32
  qInstallMsgHandler(myMessageOutput);
#endif
  QApplication app( argc, argv );
  app.setStyle("plastique");
  MainWindow mw(argc, argv);
  mw.show();
  return app.exec();
}
