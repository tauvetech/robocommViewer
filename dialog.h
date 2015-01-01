#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QTimer>
#include <QDebug>

#include <qwt_legend.h>
#include <qwt_plot_curve.h>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

private:
    Ui::Dialog *ui;
    QTimer *timer;
    QwtPlotCurve *curve;
    QPolygonF points;
    QPointF pointF;

private slots:
    void updatePlots();

};

#endif // DIALOG_H
