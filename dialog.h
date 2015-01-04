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
    //obsolete..
    QTimer *timer;

    //accelerometers:
    QwtPlotCurve *curveAccel_X;
    QwtPlotCurve *curveAccel_Y;
    QwtPlotCurve *curveAccel_Z;

    QPolygonF points_Accel_X;
    QPolygonF points_Accel_Y;
    QPolygonF points_Accel_Z;

    QPointF pointF_Accel_X;
    QPointF pointF_Accel_Y;
    QPointF pointF_Accel_Z;

    //magnetometers:
    QwtPlotCurve *curve_Mag_X;
    QwtPlotCurve *curve_Mag_Y;
    QwtPlotCurve *curve_Mag_Z;

    QPolygonF points_Mag_X;
    QPolygonF points_Mag_Y;
    QPolygonF points_Mag_Z;

    QPointF pointF_Mag_X;
    QPointF pointF_Mag_Y;
    QPointF pointF_Mag_Z;

    //gyrometers:
    QwtPlotCurve *curve_Gyro_X;
    QwtPlotCurve *curve_Gyro_Y;
    QwtPlotCurve *curve_Gyro_Z;

    QPolygonF points_Gyro_X;
    QPolygonF points_Gyro_Y;
    QPolygonF points_Gyro_Z;

    QPointF pointF_Gyro_X;
    QPointF pointF_Gyro_Y;
    QPointF pointF_Gyro_Z;



    QSerialPort *serialPort;
    bool begin;


private slots:
    //obsolelte...
//    void updatePlots();
    void readData();

    void on_pbConnect_clicked(bool checked);
    void on_pbRequestData_clicked();

};

#endif // DIALOG_H
