#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QGraphicsScene>

#include "gridscene.h"
#include "gridview.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui_;
    GridScene *scene_;
    GridView *view_;
    void Init();
    void InitConnectSlots();
    void DrawField();
public slots:
    void OnGeneratePB();
};
#endif // MAINWINDOW_H
