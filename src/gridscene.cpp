#include "gridscene.h"

GridScene::GridScene(QWidget *parent)
{
    parent_ = parent;
    qRegisterMetaType<std::vector<int>>("std::vector<int>");
    ConnectSlots();
}

void GridScene::DrawGrid()
{
    ClearAllData();
    GenerateCoordObstacles();

    qreal rectW = width_grid_/num_column_ < SIZE_SQUARE ? SIZE_SQUARE : width_grid_/num_column_;
    qreal rectH = height_grid_/num_row_< SIZE_SQUARE ? SIZE_SQUARE : height_grid_/num_row_;

    QGraphicsScene::setSceneRect(0, 0, rectW * num_column_, rectH * num_row_);
    std::vector<DataSquare>::iterator it = data_square_.begin();

    for(unsigned int i = 0; i < num_row_; i++)
    {
        for(unsigned int j = 0; j < num_column_; j++)
        {
            DrawSquare(i, j, rectW, rectH, it);
            ++it;
        }
    }

    CreateEdge();
}

void GridScene::DrawGrid(unsigned int num_row, unsigned int num_column, unsigned int width_window, unsigned int height_window)
{
    num_row_ = num_row;
    num_column_ = num_column;
    width_grid_ = width_window * (1 - BORDERPERCENT / 100);
    height_grid_ = height_window * (1 - BORDERPERCENT / 100);

    DrawGrid();
}
void GridScene::SetGridSize(unsigned int num_row, unsigned int num_column, unsigned int width_window, unsigned int height_window)
{
    num_row_ = num_row;
    num_column_ = num_column;
    width_grid_ = width_window * (1 - BORDERPERCENT / 100);
    height_grid_ = height_window * (1 - BORDERPERCENT / 100);
}

void GridScene::SetStatusBar(QStatusBar *status_bar)
{
    status_bar_ = status_bar;
}

void GridScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(is_searching_path){return;}
    QGraphicsItem *item = itemAt(event->scenePos(), QTransform());

    if (QGraphicsRectItem *rect = qgraphicsitem_cast<QGraphicsRectItem *>(item); rect != nullptr)
    {
        QPen pen(Qt::transparent);
        QBrush brushField(Qt::red);
        if(!data_square_[square_addr_[rect]].is_obstacle)
        {
            DrawPoints(rect);
            if(points_.size() == 2)
            {
                status_bar_->showMessage("Идет поиск пути...");
                std::thread thread_path(&GridScene::FindPath, this);
                thread_path.detach();
            }
        }

    }
}

void GridScene::ConnectSlots()
{
    connect(this, &GridScene::DoDrawPath, this, &GridScene::DrawPath);
    connect(this, &GridScene::DoShowMessage, this, &GridScene::ShowMessage);
}

void GridScene::DrawSquare(unsigned int i, unsigned int j, qreal rectW, qreal rectH, std::vector<DataSquare>::iterator it)
{
    QPen pen(Qt::black, 1);
    QBrush brushField(QColor(255,222,179));
    QBrush brushObstacle(QColor(34, 139, 34));

    QRectF rect(j * rectW, i * rectH, rectW, rectH);
    it->coord.x = i;
    it->coord.y = j;
    if(current_num_obsctacles_ < max_num_obstacles_)
    {
        if(IsContainObstacle(i, j))
        {
            current_num_obsctacles_++;
            it->item = addRect(rect, pen, brushObstacle);
            square_addr_[it->item] = GetNumInMatrixByIndices(i,j);
            return;
        }
    }
    it->item = addRect(rect, pen, brushField);
    square_addr_[it->item] = GetNumInMatrixByIndices(i,j);
}

void GridScene::GenerateNumObstacle()
{
    std::random_device rd;
    std::mt19937 g(rd());
    int maxNumObstacles = num_row_ * num_column_ / 2; // количество препятствий не более 1/2 поля

    std::uniform_int_distribution d(1,maxNumObstacles);
    max_num_obstacles_ = d(g);
}

std::vector<unsigned int> GridScene::GenerateRundomIndices()
{
    std::vector<unsigned int> v(num_row_ * num_column_);
    std::random_device rd;
    std::mt19937 g(rd());
    std::iota(v.begin(), v.end(), 0);
    std::shuffle(v.begin(), v.end(), g);
    v.resize(max_num_obstacles_);
    return v;
}

void GridScene::GenerateCoordObstacles()
{
    GenerateNumObstacle();
    std::vector<unsigned int> indx = GenerateRundomIndices();
    data_square_.resize(num_row_*num_column_);
    for(unsigned int i = 0; i < max_num_obstacles_; i++)
    {
        data_square_[indx[i]].is_obstacle = true;
    }
}

bool GridScene::IsContainObstacle(unsigned int x, unsigned int y)
{
    unsigned int num_el = GetNumInMatrixByIndices(x,y);
    if(data_square_[num_el].is_obstacle)
    {
        return true;
    }

    return false;
}

void GridScene::CreateEdge()
{
    adj_.clear();
    adj_.resize(data_square_.size());
    for(unsigned int i = 0; i < adj_.size(); i++)
    {
        if(!data_square_[i].is_obstacle)
        {
            Coord coord = GetCoordInMatrixByNum(i);
            adj_[i] = GetAdjacentElements(coord);
        }
    }
}

std::vector<unsigned int> GridScene::GetAdjacentElements(Coord coord)
{
    std::vector<unsigned int> result;
    unsigned int num_el;

    if(coord.x > 0)
    {
        num_el =  GetNumInMatrixByIndices(coord.x-1, coord.y);
        if(!data_square_[num_el].is_obstacle)
        {
            result.push_back(num_el);
        }
    }
    if(coord.x < (num_row_ - 1))
    {
        num_el = GetNumInMatrixByIndices(coord.x+1, coord.y);
        if(!data_square_[num_el].is_obstacle)
        {
            result.push_back(num_el);
        }
    }
    if(coord.y > 0)
    {
        num_el = GetNumInMatrixByIndices(coord.x, coord.y - 1);
        if(!data_square_[num_el].is_obstacle)
        {
            result.push_back(num_el);
        }
    }
    if(coord.y < (num_row_ - 1))
    {
        num_el = GetNumInMatrixByIndices(coord.x, coord.y + 1);
        if(!data_square_[num_el].is_obstacle)
        {
            result.push_back(num_el);
        }
    }
    std::sort(result.begin(), result.end());
    return std::move(result);
}

void GridScene::DrawPoints(QGraphicsRectItem *rectF)
{
    if(points_.size() == 2)
    {
        ClearPathInField();
    }
    QPen pen(Qt::transparent);
    QBrush brushField(Qt::red);
    qreal radius = rectF->rect().width() > rectF->rect().height()? rectF->rect().height() : rectF->rect().width();
    radius *= 0.1;
    qreal left = rectF->rect().x() + rectF->rect().width() / 2 - radius / 2;
    qreal top = rectF->rect().y() + rectF->rect().height() / 2 - radius / 2;
    QRectF rectEllipse(left, top, radius, radius);
    QGraphicsEllipseItem *ellipse = addEllipse(rectEllipse, pen, brushField);
    if(points_.size() < 2)
    {
        points_.push_back(Point{.rect = rectF, .ellipse = ellipse});
    }
}

void GridScene::FindPath()
{
    is_searching_path = true;
    GraphAlgorithm algo;
    std::vector<int> path = algo.GetShortestPath(adj_, square_addr_[points_[0].rect], square_addr_[points_[1].rect]);
    is_searching_path = false;
    status_bar_->clearMessage();
    if(path.size() == 1)
    {
        emit DoShowMessage("Пути между точками нет");
    }
    else
    {
        emit DoDrawPath(path);
    }
}

void GridScene::DrawPath(std::vector<int> path)
{
    for(unsigned int p = 0 ; p < path.size() - 1; p++)
    {
        QPen pen(Qt::red, 5);
        QRectF rectFStart = data_square_[path[p]].item->rect();
        QRectF rectFStop = data_square_[path[p+1]].item->rect();
        qreal x_start = rectFStart.x() + rectFStart.width() / 2;
        qreal y_start = rectFStart.y() + rectFStart.height() / 2;

        qreal x_stop = rectFStop.x() + rectFStop.width() / 2;
        qreal y_stop = rectFStop.y() + rectFStop.height() / 2;

        QGraphicsLineItem *line = addLine(x_start, y_start, x_stop, y_stop, pen);
        path_.push_back(line);
    }
}

void GridScene::ShowMessage(QString text)
{
    QMessageBox msgBox(parent_);
    msgBox.setText(text);
    msgBox.exec();
}

void GridScene::ClearPathInField()
{
    for(auto &point: points_)
    {
        removeItem(point.ellipse);
    }

    for(auto &p: path_)
    {
        removeItem(p);
    }
    points_.clear();
    path_.clear();
}

void GridScene::ClearAllData()
{
    QGraphicsScene::clear();
    data_square_.clear();
    square_addr_.clear();
    adj_.clear();
    points_.clear();
    path_.clear();
    current_num_obsctacles_ = 0;
    max_num_obstacles_= 0;
}

unsigned int GridScene::GetNumInMatrixByIndices(unsigned int x, unsigned int y)
{
    return x * num_column_ + y;
}

Coord GridScene::GetCoordInMatrixByNum(unsigned int num)
{
    Coord coord;
    coord.x = num / num_column_;
    coord.y = num - coord.x * num_column_;
    return coord;
}

