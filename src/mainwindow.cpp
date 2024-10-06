#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui_(new Ui::MainWindow)
{
    ui_->setupUi(this);
    Init();
}

MainWindow::~MainWindow()
{
    delete view_;
    delete scene_;
    delete ui_;
}

void MainWindow::Init()
{
    scene_ = new GridScene(this);
    scene_->SetStatusBar(ui_->statusbar);
    view_ = new GridView(this);
    ui_->horizontalLayout_3->replaceWidget(ui_->fieldView, view_);
    delete ui_->fieldView;
    view_->setScene(scene_);
    scene_->setSceneRect(0,0,view_->width(),view_->height());

    InitConnectSlots();
}

void MainWindow::InitConnectSlots()
{
    connect(ui_->gen_pB, &QPushButton::clicked, this, &MainWindow::OnGeneratePB);
}

void MainWindow::OnGeneratePB()
{
    DrawField();
}

void MainWindow::DrawField()
{
    scene_->SetGridSize(ui_->width_sB->value(), ui_->height_sB->value(), view_->width(), view_->height());
    scene_->DrawGrid();
}



