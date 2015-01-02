#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QTimer>
#include <QDebug>

#include <qwt_legend.h>
#include <qwt_plot_curve.h>

#include <QtSerialPort/QSerialPort>

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
    QwtPlotCurve *curveAccelY;
    QwtPlotCurve *curveAccelZ;
    QPolygonF points;
    QPolygonF points_Accel_Y;
    QPolygonF points_Accel_Z;

    QPointF pointF;
    QPointF pointF_Accel_Y;
    QPointF pointF_Accel_Z;

    QSerialPort *serialPort;
private slots:
    void updatePlots();
    void readData();

    void on_pbConnect_clicked(bool checked);
    void on_pbRequestData_clicked();

};

#endif // DIALOG_H
