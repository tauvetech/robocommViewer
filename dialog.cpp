#include "dialog.h"
#include "ui_dialog.h"

#include <qwt_plot_marker.h>
#include <qwt_plot_canvas.h>
#include <QMessageBox>

#include <unistd.h>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    begin=false;

    //configuration du port serie...
    serialPort = new QSerialPort(this);
    serialPort->setPortName(ui->comboBox->currentText());
    serialPort->setBaudRate(QSerialPort::Baud115200);
    serialPort->setDataBits(QSerialPort::Data8);
    serialPort->setParity(QSerialPort::NoParity);
    serialPort->setStopBits(QSerialPort::OneStop);
    serialPort->setFlowControl(QSerialPort::NoFlowControl);

    connect(serialPort, SIGNAL(readyRead()), this, SLOT(readData()));

    ui->qwtPlotAccel->setTitle("Accelerometer");
    ui->qwtPlotAccel->setCanvasBackground(QColor(Qt::white));
    ui->qwtPlotAccel->setAutoReplot(true);
   // ui->qwtPlotAccel->setMargin(5);

    //starting point:
    ui->qwtPlotAccel->setAxisScale( QwtPlot::yLeft, -2.0, 2.0 );
    ui->qwtPlotAccel->setAxisScale( QwtPlot::xBottom, 0.0, 100.0 );

    // legend
    QwtLegend *legend = new QwtLegend;
    legend->setFrameStyle(QFrame::Box|QFrame::Sunken);
    ui->qwtPlotAccel->insertLegend(legend, QwtPlot::LeftLegend);

    // axis
    ui->qwtPlotAccel->setAxisTitle(QwtPlot::xBottom, "X (nb samples)");
    ui->qwtPlotAccel->setAxisTitle(QwtPlot::yLeft, "Y (g)");
    ui->qwtPlotAccel->setLineWidth( 1 );

//    QwtPlotCanvas *canvas = new QwtPlotCanvas();
//    canvas->setFrameStyle( QFrame::Box | QFrame::Plain );
//    ui->qwtPlotAccel->setCanvas(canvas);


//    timer = new QTimer();
//    timer->setInterval(200);
//    connect(timer,SIGNAL(timeout()),this,SLOT(updatePlots()));
    //timer->start();

    curveAccel_X = new QwtPlotCurve();
    curveAccel_X->setTitle( "AccelX" );
    curveAccel_X->setPen( Qt::blue, 1 ),
    curveAccel_X->setRenderHint( QwtPlotItem::RenderAntialiased, true );

    curveAccel_X->attach( ui->qwtPlotAccel );

    curveAccel_Y = new QwtPlotCurve();
    curveAccel_Y->setTitle( "AccelY" );
    curveAccel_Y->setPen( Qt::red, 1 ),
    //curveAccelY->setRenderHint( QwtPlotItem::RenderAntialiased, true );

    curveAccel_Y->attach( ui->qwtPlotAccel );

    curveAccel_Z = new QwtPlotCurve();
    curveAccel_Z->setTitle( "AccelZ" );
    curveAccel_Z->setPen( Qt::green, 1 ),
    //curveAccelY->setRenderHint( QwtPlotItem::RenderAntialiased, true );

    curveAccel_Z->attach( ui->qwtPlotAccel );

//Magnetometer//////////////////////////////////////////////////////////////////////////
    ui->qwtPlotMag->setTitle("Magnetometer");
    ui->qwtPlotMag->setCanvasBackground(QColor(Qt::white));
    ui->qwtPlotMag->setAutoReplot(true);

    //starting point:
    //ui->qwtPlotMag->setAxisScale( QwtPlot::yLeft, -4.0, 4.0 );
    ui->qwtPlotMag->setAxisScale( QwtPlot::xBottom, 0.0, 100.0 );

    // legend
    QwtLegend *legend_Mag = new QwtLegend;
    legend_Mag->setFrameStyle(QFrame::Box|QFrame::Sunken);
    ui->qwtPlotMag->insertLegend(legend_Mag, QwtPlot::LeftLegend);

    // axis
    ui->qwtPlotMag->setAxisTitle(QwtPlot::xBottom, "X (nb samples)");
    ui->qwtPlotMag->setAxisTitle(QwtPlot::yLeft, "Y");
    ui->qwtPlotMag->setLineWidth( 1 );

    curve_Mag_X = new QwtPlotCurve();
    curve_Mag_X->setTitle( "MagX" );
    curve_Mag_X->setPen( Qt::blue, 1 ),
    curve_Mag_X->setRenderHint( QwtPlotItem::RenderAntialiased, true );

    curve_Mag_X->attach( ui->qwtPlotMag );

    curve_Mag_Y = new QwtPlotCurve();
    curve_Mag_Y->setTitle( "MagY" );
    curve_Mag_Y->setPen( Qt::red, 1 ),
    //curveAccelY->setRenderHint( QwtPlotItem::RenderAntialiased, true );

    curve_Mag_Y->attach( ui->qwtPlotMag );

    curve_Mag_Z = new QwtPlotCurve();
    curve_Mag_Z->setTitle( "MagZ" );
    curve_Mag_Z->setPen( Qt::green, 1 ),
    //curveAccelY->setRenderHint( QwtPlotItem::RenderAntialiased, true );

    curve_Mag_Z->attach( ui->qwtPlotMag );
//////////////////////////////////////////////////////////////////////////////////////

//Gyrometer//////////////////////////////////////////////////////////////////////////
    ui->qwtPlotGyro->setTitle("Gyrometer");
    ui->qwtPlotGyro->setCanvasBackground(QColor(Qt::white));
    ui->qwtPlotGyro->setAutoReplot(true);

    //starting point:
    //ui->qwtPlotGyro->setAxisScale( QwtPlot::yLeft, -2.0, 2.0 );
    ui->qwtPlotGyro->setAxisScale( QwtPlot::xBottom, 0.0, 100.0 );

    // legend
    QwtLegend *legend_Gyro = new QwtLegend;
    legend_Gyro->setFrameStyle(QFrame::Box|QFrame::Sunken);
    ui->qwtPlotGyro->insertLegend(legend_Gyro, QwtPlot::LeftLegend);

    // axis
    ui->qwtPlotGyro->setAxisTitle(QwtPlot::xBottom, "X (nb samples)");
    ui->qwtPlotGyro->setAxisTitle(QwtPlot::yLeft, "Y");
    ui->qwtPlotGyro->setLineWidth( 1 );

    curve_Gyro_X = new QwtPlotCurve();
    curve_Gyro_X->setTitle( "GyroX" );
    curve_Gyro_X->setPen( Qt::blue, 1 ),
    curve_Gyro_X->setRenderHint( QwtPlotItem::RenderAntialiased, true );

    curve_Gyro_X->attach( ui->qwtPlotGyro );

    curve_Gyro_Y = new QwtPlotCurve();
    curve_Gyro_Y->setTitle( "GyroY" );
    curve_Gyro_Y->setPen( Qt::red, 1 ),
    //curveAccelY->setRenderHint( QwtPlotItem::RenderAntialiased, true );

    curve_Gyro_Y->attach( ui->qwtPlotGyro );

    curve_Gyro_Z = new QwtPlotCurve();
    curve_Gyro_Z->setTitle( "GyroZ" );
    curve_Gyro_Z->setPen( Qt::green, 1 ),
    //curveAccelY->setRenderHint( QwtPlotItem::RenderAntialiased, true );

    curve_Gyro_Z->attach( ui->qwtPlotGyro );
//////////////////////////////////////////////////////////////////////////////////////


//    // ...a horizontal line at y = 0...
//   QwtPlotMarker *mY = new QwtPlotMarker();
//   mY->setLabel(QString::fromLatin1("y = 0"));
//   mY->setLabelAlignment(Qt::AlignRight|Qt::AlignTop);
//   mY->setLineStyle(QwtPlotMarker::HLine);
//   mY->setYValue(0.0);
//   mY->attach(ui->qwtPlotAccel);

//    // ...a vertical line at x = 2 * pi
//   QwtPlotMarker *mX = new QwtPlotMarker();
//   mX->setLabel(QString::fromLatin1("x = 2 pi"));
//   mX->setLabelAlignment(Qt::AlignLeft | Qt::AlignBottom);
//   mX->setLabelOrientation(Qt::Vertical);
//   mX->setLineStyle(QwtPlotMarker::VLine);
//   mX->setLinePen(QPen(Qt::black, 0, Qt::DashDotLine));
//   mX->setXValue(2.0 * M_PI);
//   mX->attach(ui->qwtPlotAccel);

}

Dialog::~Dialog()
{
    delete ui;
}

//void Dialog::updatePlots()
//{
//    qDebug()<<"on updatePlots()";
//static int val=0;
//val++;
////pointF.setX(val);
////pointF.setY(val);
////points.append(pointF);

////if (val>50)
////{
////    ui->qwtPlotAccel->setAxisScale( QwtPlot::xBottom, val-50, val+50 );
////}


//    //curve->setData(QPointF(val++,val++));
//    curve->setSamples( points );

//    ui->qwtPlotAccel->replot();
//}


void Dialog::on_pbConnect_clicked(bool checked)
{
    qDebug()<<"on_pushButton_clicked " << checked ;
    if (checked == true)
    {
        // ouverture du port:
        if (serialPort->open(QIODevice::ReadWrite))
        {
            qDebug()<<"open serial port ok!";
        }
        else
        {
            QMessageBox::critical(this, tr("Error"), serialPort->errorString());

            qDebug()<<"pb open serial port";
        }
        ui->pbConnect->setText("Disconnect");
    }
    if (checked == false)
    {
        // fermeture du port
        if (serialPort->isOpen())
        {
            serialPort->close();
            qDebug()<<"closing serial port";
        }
        ui->pbConnect->setText(("Connect"));
    }
}

void Dialog::readData()
{

        static unsigned long stamps=0;
        stamps++;


        double accel_x_axis;
        double accel_y_axis;
        double accel_z_axis;


        double mag_x_axis;
        double mag_y_axis;
        double mag_z_axis;

        double gyro_x_axis;
        double gyro_y_axis;
        double gyro_z_axis;


        qDebug()<<"on slot readData!";
        //     QByteArray data = serialPort->readLine();
        //     qDebug()<< "first data:" << data;
        //     qDebug()<<"size of data" << data.size();
        //     QByteArray data2 = serialPort->readLine();
        //     qDebug()<< "second line data2:" <<data2;
        //     qDebug()<<"size of data2" <<data2.size();
        //     QByteArray data3 = serialPort->readLine();
        //     qDebug()<< "third line data3:" <<data3;
        //     qDebug()<<"size of data3" <<data3.size();


/*****************************************************************************************/
/*        Accelerometer                                                                   */
/*****************************************************************************************/


        QByteArray data = serialPort->readLine();
        qDebug()<< data;
        unsigned long received_value =0;

        for (int i=0; i<data.size();i++)
        {
            if(data.at(i) >= '0' && data.at(i) <= '9')
                received_value = received_value*16+(data.at(i)-'0');
            else if(data.at(i) >= 'a' && data.at(i) <= 'f')
                received_value = received_value*16+(data.at(i)-'a'+10);
            else if(data.at(i) >= 'A' && data.at(i) <= 'F')
                received_value = received_value*16+(data.at(i)-'A'+10);
            else if(data.at(i) == '\n')
                ;
        }
        qDebug()<< "valeur de uLong:" << received_value;
        accel_x_axis = ((double)received_value-32768.0)/(double)(32768/4);
        qDebug()<< "valeur de x_axis:" << accel_x_axis;

        //     qDebug()<<"size of data" << data.size();
        QByteArray data2 = serialPort->readLine();
        qDebug()<< data2;
        unsigned long received_value2 =0;
        for (int i=0; i<data2.size();i++)
        {
            if(data2.at(i) >= '0' && data2.at(i) <= '9')
                received_value2 = received_value2*16+(data2.at(i)-'0');
            else if(data2.at(i) >= 'a' && data2.at(i) <= 'f')
                received_value2 = received_value2*16+(data2.at(i)-'a'+10);
            else if(data2.at(i) >= 'A' && data2.at(i) <= 'F')
                received_value2 = received_value2*16+(data2.at(i)-'A'+10);
            else if(data2.at(i) == '\n')
                ;
        }
        accel_y_axis = ((double)received_value2-32768.0)/(double)(32768/4);
        //     qDebug()<<"size of data2" <<data2.size();
        QByteArray data3 = serialPort->readLine();
        qDebug()<<data3;

        unsigned long received_value3 =0;
        for (int i=0; i<data3.size();i++)
        {
            if(data3.at(i) >= '0' && data3.at(i) <= '9')
                received_value3 = received_value3*16+(data3.at(i)-'0');
            else if(data3.at(i) >= 'a' && data3.at(i) <= 'f')
                received_value3 = received_value3*16+(data3.at(i)-'a'+10);
            else if(data3.at(i) >= 'A' && data3.at(i) <= 'F')
                received_value3 = received_value3*16+(data3.at(i)-'A'+10);
            else if(data3.at(i) == '\n')
                ;
        }
        accel_z_axis = ((double)received_value3-32768.0)/(double)(32768/4);

        //          qDebug()<<data3.at(1);
        //     qDebug()<<"size of data3" <<data3.size();

        if (stamps>100)
        {
            ui->qwtPlotAccel->setAxisScale( QwtPlot::xBottom, stamps-100, stamps );
        }

        pointF_Accel_X.setX(stamps);
        pointF_Accel_X.setY(accel_x_axis);
        points_Accel_X.append(pointF_Accel_X);
        curveAccel_X->setSamples( points_Accel_X );

        pointF_Accel_Y.setX(stamps);
        pointF_Accel_Y.setY(accel_y_axis);
        points_Accel_Y.append(pointF_Accel_Y);
        curveAccel_Y->setSamples( points_Accel_Y );

        pointF_Accel_Z.setX(stamps);
        pointF_Accel_Z.setY(accel_z_axis);
        points_Accel_Z.append(pointF_Accel_Z);
        curveAccel_Z->setSamples( points_Accel_Z );


/*****************************************************************************************/
/*        Magnetometer                                                                   */
/*****************************************************************************************/

        QByteArray dataMagX = serialPort->readLine();
        qDebug()<< dataMagX;
        unsigned long received_valueMagX =0;

        for (int i=0; i<dataMagX.size();i++)
        {
            if(dataMagX.at(i) >= '0' && dataMagX.at(i) <= '9')
                received_valueMagX = received_valueMagX*16+(dataMagX.at(i)-'0');
            else if(dataMagX.at(i) >= 'a' && dataMagX.at(i) <= 'f')
                received_valueMagX = received_valueMagX*16+(dataMagX.at(i)-'a'+10);
            else if(dataMagX.at(i) >= 'A' && dataMagX.at(i) <= 'F')
                received_valueMagX = received_valueMagX*16+(dataMagX.at(i)-'A'+10);
            else if(dataMagX.at(i) == '\n')
                ;
        }
        qDebug()<< "valeur de uLong:" << received_valueMagX;
        mag_x_axis = (((double)received_valueMagX-4096)/(double)(4096/25))*100;
        qDebug()<< "valeur de mag_x_axis:" << mag_x_axis;

        //     qDebug()<<"size of data" << data.size();
        QByteArray dataMagY = serialPort->readLine();
        qDebug()<< dataMagY;
        unsigned long received_valueMagY =0;
        for (int i=0; i<dataMagY.size();i++)
        {
            if(dataMagY.at(i) >= '0' && dataMagY.at(i) <= '9')
                received_valueMagY = received_valueMagY*16+(dataMagY.at(i)-'0');
            else if(dataMagY.at(i) >= 'a' && dataMagY.at(i) <= 'f')
                received_valueMagY = received_valueMagY*16+(dataMagY.at(i)-'a'+10);
            else if(dataMagY.at(i) >= 'A' && dataMagY.at(i) <= 'F')
                received_valueMagY = received_valueMagY*16+(dataMagY.at(i)-'A'+10);
            else if(dataMagY.at(i) == '\n')
                ;
        }
        mag_y_axis = (((double)received_valueMagY-4096)/(double)(4096/25))*100;
        //     qDebug()<<"size of data2" <<data2.size();
        QByteArray dataMagZ = serialPort->readLine();
        qDebug()<<dataMagZ;

        unsigned long received_valueMagZ =0;
        for (int i=0; i<dataMagZ.size();i++)
        {
            if(dataMagZ.at(i) >= '0' && dataMagZ.at(i) <= '9')
                received_valueMagZ = received_valueMagZ*16+(data3.at(i)-'0');
            else if(dataMagZ.at(i) >= 'a' && dataMagZ.at(i) <= 'f')
                received_valueMagZ = received_valueMagZ*16+(dataMagZ.at(i)-'a'+10);
            else if(dataMagZ.at(i) >= 'A' && dataMagZ.at(i) <= 'F')
                received_valueMagZ = received_valueMagZ*16+(dataMagZ.at(i)-'A'+10);
            else if(dataMagZ.at(i) == '\n')
                ;
        }
        mag_z_axis = (((double)received_valueMagZ-4096)/(double)(4096/25))*100;

        //          qDebug()<<data3.at(1);
        //     qDebug()<<"size of data3" <<data3.size();

        if (stamps>100)
        {
            ui->qwtPlotMag->setAxisScale( QwtPlot::xBottom, stamps-100, stamps );
        }

        pointF_Mag_X.setX(stamps);
        pointF_Mag_X.setY(mag_x_axis);
        points_Mag_X.append(pointF_Mag_X);
        curve_Mag_X->setSamples( points_Mag_X );

        pointF_Mag_Y.setX(stamps);
        pointF_Mag_Y.setY(mag_y_axis);
        points_Mag_Y.append(pointF_Mag_Y);
        curve_Mag_Y->setSamples( points_Mag_Y );

        pointF_Mag_Z.setX(stamps);
        pointF_Mag_Z.setY(mag_z_axis);
        points_Mag_Z.append(pointF_Mag_Z);
        curve_Mag_Z->setSamples( points_Mag_Z );
/////////////////////////////////////////////////////////////////////////////////////////////////////


        /*****************************************************************************************/
        /*        Gyrometer                                                                   */
        /*****************************************************************************************/

                QByteArray dataGyroX = serialPort->readLine();
                qDebug()<< dataGyroX;
                unsigned long received_valueGyroX =0;

                for (int i=0; i<dataGyroX.size();i++)
                {
                    if(dataGyroX.at(i) >= '0' && dataGyroX.at(i) <= '9')
                        received_valueGyroX = received_valueGyroX*16+(dataGyroX.at(i)-'0');
                    else if(dataGyroX.at(i) >= 'a' && dataGyroX.at(i) <= 'f')
                        received_valueGyroX = received_valueGyroX*16+(dataGyroX.at(i)-'a'+10);
                    else if(dataGyroX.at(i) >= 'A' && dataGyroX.at(i) <= 'F')
                        received_valueGyroX = received_valueGyroX*16+(dataGyroX.at(i)-'A'+10);
                    else if(dataGyroX.at(i) == '\n')
                        ;
                }
                qDebug()<< "valeur de uLong:" << received_valueGyroX;
                //x_axis = ((double)ReceiveULong(serial_if)-32768)/(double)(32768/0xFA);
                gyro_x_axis = ((double)received_valueGyroX-32768)/(double)(32768/250);
                qDebug()<< "valeur de gyro_x_axis:" << gyro_x_axis;


                QByteArray dataGyroY = serialPort->readLine();
                qDebug()<< dataGyroY;
                unsigned long received_valueGyroY =0;
                for (int i=0; i<dataGyroY.size();i++)
                {
                    if(dataGyroY.at(i) >= '0' && dataGyroY.at(i) <= '9')
                        received_valueGyroY = received_valueGyroY*16+(dataGyroY.at(i)-'0');
                    else if(dataGyroY.at(i) >= 'a' && dataGyroY.at(i) <= 'f')
                        received_valueGyroY = received_valueGyroY*16+(dataGyroY.at(i)-'a'+10);
                    else if(dataGyroY.at(i) >= 'A' && dataGyroY.at(i) <= 'F')
                        received_valueGyroY = received_valueGyroY*16+(dataGyroY.at(i)-'A'+10);
                    else if(dataGyroY.at(i) == '\n')
                        ;
                }
                gyro_y_axis = ((double)received_valueGyroY-32768)/(double)(32768/250);

                //     qDebug()<<"size of data2" <<data2.size();
                QByteArray dataGyroZ = serialPort->readLine();
                qDebug()<<dataGyroZ;

                unsigned long received_valueGyroZ =0;
                for (int i=0; i<dataGyroZ.size();i++)
                {
                    if(dataGyroZ.at(i) >= '0' && dataGyroZ.at(i) <= '9')
                        received_valueGyroZ = received_valueGyroZ*16+(dataGyroZ.at(i)-'0');
                    else if(dataGyroZ.at(i) >= 'a' && dataGyroZ.at(i) <= 'f')
                        received_valueGyroZ = received_valueGyroZ*16+(dataGyroZ.at(i)-'a'+10);
                    else if(dataGyroZ.at(i) >= 'A' && dataGyroZ.at(i) <= 'F')
                        received_valueGyroZ = received_valueGyroZ*16+(dataGyroZ.at(i)-'A'+10);
                    else if(dataGyroZ.at(i) == '\n')
                        ;
                }
                gyro_z_axis = ((double)received_valueGyroZ-32768)/(double)(32768/250);

                //          qDebug()<<data3.at(1);
                //     qDebug()<<"size of data3" <<data3.size();

                if (stamps>100)
                {
                    ui->qwtPlotGyro->setAxisScale( QwtPlot::xBottom, stamps-100, stamps );
                }

                pointF_Gyro_X.setX(stamps);
                pointF_Gyro_X.setY(gyro_x_axis);
                points_Gyro_X.append(pointF_Gyro_X);
                curve_Gyro_X->setSamples( points_Gyro_X );

                pointF_Gyro_Y.setX(stamps);
                pointF_Gyro_Y.setY(gyro_y_axis);
                points_Gyro_Y.append(pointF_Gyro_Y);
                curve_Gyro_Y->setSamples( points_Gyro_Y );

                pointF_Gyro_Z.setX(stamps);
                pointF_Gyro_Z.setY(gyro_z_axis);
                points_Gyro_Z.append(pointF_Gyro_Z);
                curve_Gyro_Z->setSamples( points_Gyro_Z );
        /////////////////////////////////////////////////////////////////////////////////////////////////////



        ui->qwtPlotAccel->repaint();
        ui->qwtPlotMag->repaint();
        ui->qwtPlotGyro->repaint();

        usleep(10000);


        serialPort->write("readAccel\n");
        serialPort->write("readMag\n");
        serialPort->write("readGyro\n");

}




void Dialog::on_pbRequestData_clicked()
{
    serialPort->write("\n");
    serialPort->write("promptOff\n");
    serialPort->write("resetConfig\n");
    serialPort->write("roboveroConfig\n");
    qDebug()<<"on_pbRequestData_clicked before usleep 1s";
    usleep(1000000);
    qDebug()<<"on_pbRequestData_clicked after usleep 1s";
    serialPort->flush();
    serialPort->write("configAccel 1 1 1 1 32 4\n");
    serialPort->write("configMag 1 0 96 19\n");
    serialPort->write("configGyro 1 1 1 1  64 fa\n");

    serialPort->write("readAccel\n");
    serialPort->write("readMag\n");
    serialPort->write("readGyro\n");
}


