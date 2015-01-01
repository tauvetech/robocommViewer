#include "dialog.h"
#include "ui_dialog.h"

#include <qwt_plot_marker.h>
#include <qwt_plot_canvas.h>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    ui->qwtPlotAccel->setTitle("Accelerometer");
    ui->qwtPlotAccel->setCanvasBackground(QColor(Qt::white));

    ui->qwtPlotAccel->setAutoReplot(true);
   // ui->qwtPlotAccel->setMargin(5);

    //ui->qwtPlotAccel->setAxisScale( QwtPlot::yLeft, 0.0, 100.0 );
    ui->qwtPlotAccel->setAxisScale( QwtPlot::xBottom, 0.0, 100.0 );

    // legend
    QwtLegend *legend = new QwtLegend;
    legend->setFrameStyle(QFrame::Box|QFrame::Sunken);
    ui->qwtPlotAccel->insertLegend(legend, QwtPlot::BottomLegend);

    // axis
    ui->qwtPlotAccel->setAxisTitle(QwtPlot::xBottom, "X Axis (time)");
    ui->qwtPlotAccel->setAxisTitle(QwtPlot::yLeft, "Y Axis (scale)");

    ui->qwtPlotAccel->setLineWidth( 1 );

//    QwtPlotCanvas *canvas = new QwtPlotCanvas();
//    canvas->setFrameStyle( QFrame::Box | QFrame::Plain );
//    ui->qwtPlotAccel->setCanvas(canvas);


    timer = new QTimer();
    timer->setInterval(200);
    connect(timer,SIGNAL(timeout()),this,SLOT(updatePlots()));
    timer->start();

    curve = new QwtPlotCurve();
    curve->setTitle( "Pixel Count" );
    curve->setPen( Qt::blue, 2 ),
    curve->setRenderHint( QwtPlotItem::RenderAntialiased, true );

    curve->attach( ui->qwtPlotAccel );
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
pointF.setX(val);
pointF.setY(val);
points.append(pointF);

if (val>50)
{
    ui->qwtPlotAccel->setAxisScale( QwtPlot::xBottom, val-50, val+50 );
}


    //curve->setData(QPointF(val++,val++));
    curve->setSamples( points );

    ui->qwtPlotAccel->replot();
}
