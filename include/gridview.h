#ifndef GRIDVIEW_H
#define GRIDVIEW_H
#include <QGraphicsView>
#include <QWheelEvent>

class GridView : public QGraphicsView
{
    Q_OBJECT

public:
    GridView(QWidget *parent = 0);

protected:
#if QT_CONFIG(wheelevent)
    void wheelEvent(QWheelEvent *event) override;
#endif
    void ScaleView(qreal scale_factor);
private:
    QWidget *parent_;
};

#endif // GRIDVIEW_H
