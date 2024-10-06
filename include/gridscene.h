#ifndef GRIDSCENE_H
#define GRIDSCENE_H

#include <QStatusBar>
#include <QGraphicsView>
#include <QMessageBox>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsItem>

#include <vector>
#include <random>
#include <iterator>
#include <iostream>
#include <thread>

#include "graph_algorithm.h"

#define BORDERPERCENT 10 // насколько уменьшить границы по сравнению с окном
#define SIZE_SQUARE 50

struct Coord
{
    unsigned int x = 0;
    unsigned int y = 0;
};

struct Point
{
    QGraphicsRectItem *rect;
    QGraphicsEllipseItem *ellipse;
};

struct DataSquare
{
    Coord coord;
    QGraphicsRectItem *item = nullptr;
    bool is_obstacle = false;
};

class GridScene : public QGraphicsScene
{
     Q_OBJECT
public:
    GridScene(QWidget *parent = 0);
    void DrawGrid();
    void DrawGrid(unsigned int num_row, unsigned int num_column, unsigned int width_window, unsigned int height_window);
    void SetGridSize(unsigned int num_row, unsigned int num_column, unsigned int width_window, unsigned int height_window);
    void SetStatusBar(QStatusBar *status_bar);

private:
    unsigned int num_row_ = 1;
    unsigned int num_column_ = 1;
    unsigned int width_grid_ = 1;
    unsigned int height_grid_ = 1;
    unsigned int max_num_obstacles_ = 0;
    unsigned int current_num_obsctacles_ = 0;
    std::vector<DataSquare> data_square_;
    std::unordered_map<QGraphicsRectItem *, unsigned int> square_addr_;
    std::vector<std::vector<unsigned int>> adj_;
    std::vector<Point> points_;
    std::vector<QGraphicsLineItem *> path_;
    std::atomic_bool is_searching_path = false;
    QWidget *parent_;
    QStatusBar *status_bar_;
public slots:
    void DrawPath(std::vector<int> path);
    void ShowMessage(QString text);
signals:
    void DoDrawPath(std::vector<int> path);
    void DoShowMessage(QString text);
private:

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

    void ConnectSlots();

    void DrawSquare(unsigned int i, unsigned int j, qreal rectW, qreal rectH, std::vector<DataSquare>::iterator it);
    void GenerateNumObstacle();
    std::vector<unsigned int> GenerateRundomIndices();
    void GenerateCoordObstacles();
    bool IsContainObstacle(unsigned int x, unsigned int y);

    void CreateEdge();
    std::vector<unsigned int> GetAdjacentElements(Coord coord);
    void DrawPoints(QGraphicsRectItem *rectF);
    void FindPath();
    void ClearPathInField();
    unsigned int GetNumInMatrixByIndices(unsigned int x, unsigned int y);
    Coord GetCoordInMatrixByNum(unsigned int num);
    void ClearAllData();
};

#endif // GRIDSCENE_H
