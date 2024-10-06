#include "mainwindow.h"
#include <QApplication>
#include <QSettings>
#include <QDesktopWidget>
#include <QFileInfo>

#define FILENAME_SETTINGS "settings.ini"
void InitPosition(MainWindow &w);
void SavePosition(MainWindow &w);

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    InitPosition(w);
    w.show();
    int res = a.exec();
    SavePosition(w);
    return res;
}
void InitPosition(MainWindow &w)
{
    if(QFileInfo::exists(FILENAME_SETTINGS))
    {
        QSettings settings(FILENAME_SETTINGS, QSettings::IniFormat);
        settings.beginGroup("AppPosition");
        QRect rect = settings.value("geometry").toRect();
        w.setGeometry(rect);
    }
    else
    {
        QDesktopWidget *desktop = QApplication::desktop();
        int width = w.frameGeometry().width();
        int height = w.frameGeometry().height();
        int screenWidth = desktop->width();
        int screenHeight = desktop->height();
        w.setGeometry((screenWidth/2)-(width/2),(screenHeight/2)-(height/2),width,height);
    }
}

void SavePosition(MainWindow &w)
{
    QSettings settings(FILENAME_SETTINGS, QSettings::IniFormat);
    settings.beginGroup( "AppPosition" );
    settings.setValue("geometry", w.geometry());
    settings.endGroup();
}
