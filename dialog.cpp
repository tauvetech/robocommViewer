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

    //configuration du port serie...
    serialPort = new QSerialPort(this);
    serialPort->setPortName("/dev/ttyACM0");
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

    ui->qwtPlotAccel->setAxisScale( QwtPlot::yLeft, -2.0, 2.0 );
    ui->qwtPlotAccel->setAxisScale( QwtPlot::xBottom, 0.0, 100.0 );

    // legend
    QwtLegend *legend = new QwtLegend;
    legend->setFrameStyle(QFrame::Box|QFrame::Sunken);
    ui->qwtPlotAccel->insertLegend(legend, QwtPlot::LeftLegend);

    // axis
    ui->qwtPlotAccel->setAxisTitle(QwtPlot::xBottom, "X Axis ");
    ui->qwtPlotAccel->setAxisTitle(QwtPlot::yLeft, "Y Axis");

    ui->qwtPlotAccel->setLineWidth( 1 );

//    QwtPlotCanvas *canvas = new QwtPlotCanvas();
//    canvas->setFrameStyle( QFrame::Box | QFrame::Plain );
//    ui->qwtPlotAccel->setCanvas(canvas);


    timer = new QTimer();
    timer->setInterval(200);
    connect(timer,SIGNAL(timeout()),this,SLOT(updatePlots()));
    //timer->start();

    curve = new QwtPlotCurve();
    curve->setTitle( "AccelX" );
    curve->setPen( Qt::blue, 1 ),
    curve->setRenderHint( QwtPlotItem::RenderAntialiased, true );

    curve->attach( ui->qwtPlotAccel );

    curveAccelY = new QwtPlotCurve();
    curveAccelY->setTitle( "AccelY" );
    curveAccelY->setPen( Qt::red, 1 ),
    //curveAccelY->setRenderHint( QwtPlotItem::RenderAntialiased, true );

    curveAccelY->attach( ui->qwtPlotAccel );

    curveAccelZ = new QwtPlotCurve();
    curveAccelZ->setTitle( "AccelZ" );
    curveAccelZ->setPen( Qt::green, 1 ),
    //curveAccelY->setRenderHint( QwtPlotItem::RenderAntialiased, true );

    curveAccelZ->attach( ui->qwtPlotAccel );


//        points << QPointF( 0.0, 4.4 ) << QPointF( 1.0, 3.0 )
//            << QPointF( 2.0, 4.5 ) << QPointF( 3.0, 6.8 )
//            << QPointF( 4.0, 7.9 ) << QPointF( 5.0, 7.1 );

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

void Dialog::updatePlots()
{
    qDebug()<<"on updatePlots()";
static int val=0;
val++;
//pointF.setX(val);
//pointF.setY(val);
//points.append(pointF);

//if (val>50)
//{
//    ui->qwtPlotAccel->setAxisScale( QwtPlot::xBottom, val-50, val+50 );
//}


    //curve->setData(QPointF(val++,val++));
    curve->setSamples( points );

    ui->qwtPlotAccel->replot();
}


void Dialog::on_pbConnect_clicked(bool checked)
{
    qDebug()<<"on_pushButton_clicked " << checked ;
    if (checked == true)
    {
        //TODO ouverture du port:
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
        //TODO fermeture du port
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

    double x_axis;
    double accel_y_axis;
    double accel_z_axis;

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
          x_axis = ((double)received_value-32768.0)/(double)(32768/4);
          qDebug()<< "valeur de x_axis:" << x_axis;

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

          pointF.setX(stamps);
          pointF.setY(x_axis);
          points.append(pointF);
          curve->setSamples( points );

          pointF_Accel_Y.setX(stamps);
          pointF_Accel_Y.setY(accel_y_axis);
          points_Accel_Y.append(pointF_Accel_Y);
          curveAccelY->setSamples( points_Accel_Y );

          pointF_Accel_Z.setX(stamps);
          pointF_Accel_Z.setY(accel_z_axis);
          points_Accel_Z.append(pointF_Accel_Z);
          curveAccelZ->setSamples( points_Accel_Z );


          ui->qwtPlotAccel->repaint();

 usleep(10000);
     serialPort->write("readAccel\n");
}


void Dialog::on_pbRequestData_clicked()
{
    serialPort->write("\n");
    serialPort->write("promptOff\n");
    serialPort->write("resetConfig\n");
    serialPort->write("roboveroConfig\n");
    serialPort->flush();
    serialPort->write("configAccel 1 1 1 1 32 4\n");
    serialPort->write("configMag 1 0 96 19\n");
    serialPort->write("configGyro 1 1 1 1  64 fa\n");

    serialPort->write("readAccel\n");
}


