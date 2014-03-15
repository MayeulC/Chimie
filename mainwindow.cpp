#include "mainwindow.h"
#include "aboutwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Time.initTime();
    ui->verticalLayout_2->addWidget(&m_graph);
    m_graph.setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    Xmax=0;
    Ymax=0;
    simulation[0].createErrorMessage(this); //So that the window can't be bring foreground
    simulation[1].createErrorMessage(this); //while a dialog box is opened

    element[0]=simulation[0].getElements();
    element[1]=simulation[1].getElements();
   m_graph.setElements(element);
   m_graph.setScale(&Xmax,&Ymax);

    //default colors on the buttons :
   for(int j=0;j<2;j++)
   {
       for(int i=0; i<4;i++)
        {
            QColor col=m_graph.getColor(i,j);
            QString qss = QString("background-color: %1").arg(col.name());
            if(i==0&&j==0) //A macro should work, too...
                ui->Color0->setStyleSheet(qss);
            if(i==1&&j==0)
                ui->Color1->setStyleSheet(qss);
            if(i==2&&j==0)
                ui->Color2->setStyleSheet(qss);
            if(i==3&&j==0)
                ui->Color3->setStyleSheet(qss);
            //simulation #2
            if(i==0&&j==1)
                ui->Color0_2->setStyleSheet(qss);
            if(i==1&&j==1)
                ui->Color1_2->setStyleSheet(qss);
            if(i==2&&j==1)
                ui->Color2_2->setStyleSheet(qss);
            if(i==3&&j==1)
                ui->Color3_2->setStyleSheet(qss);
        }
   }

    simulation[0].setTemps(&Time); //Doing this, we update QErrorMessage's parent, too.
    simulation[1].setTemps(&Time);

    m_realTime=false;
    m_errorMessage=new QErrorMessage(this);


    connect(&simulation[0],SIGNAL(MessageStatus(QString)),this,SIGNAL(MessageStatus(QString)));
    connect(&simulation[1],SIGNAL(MessageStatus(QString)),this,SIGNAL(MessageStatus(QString)));
    connect(&simulation[0],SIGNAL(CalculationProgress(int)),this,SIGNAL(CalculationProgress(int)));
    connect(&simulation[1],SIGNAL(CalculationProgress(int)),this,SIGNAL(CalculationProgress(int)));

    //now, the sliders...
    connect(&simulation[0],SIGNAL(newRatio1(int)),ui->horizontalSlider,SLOT(setValue(int)));
    connect(&simulation[1],SIGNAL(newRatio1(int)),ui->horizontalSlider,SLOT(setValue(int)));
    connect(&simulation[0],SIGNAL(newRatio2(int)),ui->horizontalSlider_2,SLOT(setValue(int)));
    connect(&simulation[1],SIGNAL(newRatio2(int)),ui->horizontalSlider_2,SLOT(setValue(int)));

    connect(&simulation[0],SIGNAL(DisableSlider1(bool)),ui->horizontalSlider,SLOT(setDisabled(bool)));
    connect(&simulation[1],SIGNAL(DisableSlider1(bool)),ui->horizontalSlider,SLOT(setDisabled(bool)));
    connect(&simulation[0],SIGNAL(DisableSlider2(bool)),ui->horizontalSlider_2,SLOT(setDisabled(bool)));
    connect(&simulation[1],SIGNAL(DisableSlider2(bool)),ui->horizontalSlider_2,SLOT(setDisabled(bool)));

    connect(&simulation[0],SIGNAL(newK1(double)),ui->k1DoubleSpinBox,SLOT(setValue(double)));
    connect(&simulation[1],SIGNAL(newK1(double)),ui->k1DoubleSpinBox_2,SLOT(setValue(double)));
    connect(&simulation[0],SIGNAL(newK3(double)),ui->k3DoubleSpinBox,SLOT(setValue(double)));
    connect(&simulation[1],SIGNAL(newK3(double)),ui->k3DoubleSpinBox_2,SLOT(setValue(double)));

    emit MessageStatus(tr("Prêt."));
}

void MainWindow::connectSliders() //current tab is sufficient
{
    connect(ui->horizontalSlider,SIGNAL(valueChanged(int)),this,SLOT(on_horizontalSlider_valueChanged(int)));
    connect(ui->horizontalSlider_2,SIGNAL(valueChanged(int)),this,SLOT(on_horizontalSlider_2_valueChanged(int)));
}
void MainWindow::disconnectSliders() //current simulation is sufficient too
{
    disconnect(ui->horizontalSlider,SIGNAL(valueChanged(int)),this,SLOT(on_horizontalSlider_valueChanged(int)));
    disconnect(ui->horizontalSlider_2,SIGNAL(valueChanged(int)),this,SLOT(on_horizontalSlider_2_valueChanged(int)));
}
QString MainWindow::changeColor(int curve, int sim)
{
    QRgb col;
    col= QColorDialog::getRgba(m_graph.getColor(curve,sim));
    m_graph.setColor(curve,sim,col);
    QColor col2=col;
    m_graph.update();
    return QString("background-color: %1").arg(col2.name());
}
MainWindow::~MainWindow()
{
    delete ui;
}




/*******************************\
|  Under are Qt semi-automated  |
|    signal-slot connections    |
\*******************************/


void MainWindow::on_Button_Launch_clicked()
{
    //check if it's tab 0 or 1
    int Current=ui->tabWidget->currentIndex();
    Xmax=simulation[!(Current)].getXmax();
    Ymax=simulation[!(Current)].getYmax();
    simulation[Current].go();
    submissionYmax(simulation[Current].getYmax());
    submissionXmax(simulation[Current].getXmax());
    m_graph.clean();
    m_graph.update();
}
void MainWindow::on_Button_Erase_clicked()
{
    simulation[ui->tabWidget->currentIndex()].amnesia();
    Xmax=simulation[!ui->tabWidget->currentIndex()].getXmax();
    Ymax=simulation[!ui->tabWidget->currentIndex()].getYmax();
    m_graph.clean();
    m_graph.update();
}


//File Menu :
void MainWindow::on_actionQuit_triggered()
{
    exit(EXIT_SUCCESS);
}
void MainWindow::on_actionPrint_triggered()
{
    m_graph.print();
}
//Edit Menu :
void MainWindow::on_actionCopy_Graph_triggered()
{
    QApplication::clipboard()->setImage(m_graph.getImage());
}
void MainWindow::on_action_Import_triggered()
{
    int Current=ui->tabWidget->currentIndex(); //TODO : operator = overload

    disconnectSliders();
    if(Current)//if tab 1
    {
        ui->CaDoubleSpinBox_2->setValue(ui->CaDoubleSpinBox->value());
        ui->CbDoubleSpinBox_2->setValue(ui->CbDoubleSpinBox->value());
        ui->CcDoubleSpinBox_2->setValue(ui->CcDoubleSpinBox->value());
        ui->CdDoubleSpinBox_2->setValue(ui->CdDoubleSpinBox->value());

        ui->k1DoubleSpinBox_2->setValue(ui->k1DoubleSpinBox->value());
        ui->k2DoubleSpinBox_2->setValue(ui->k2DoubleSpinBox->value());
        ui->k3DoubleSpinBox_2->setValue(ui->k3DoubleSpinBox->value());
        ui->kminus1DoubleSpinBox_2->setValue(ui->kminus1DoubleSpinBox->value());
        ui->kminus2DoubleSpinBox_2->setValue(ui->kminus2DoubleSpinBox->value());
        ui->kminus3DoubleSpinBox_2->setValue(ui->kminus3DoubleSpinBox->value());
    }
    else
    {
        ui->CaDoubleSpinBox->setValue(ui->CaDoubleSpinBox_2->value());
        ui->CbDoubleSpinBox->setValue(ui->CbDoubleSpinBox_2->value());
        ui->CcDoubleSpinBox->setValue(ui->CcDoubleSpinBox_2->value());
        ui->CdDoubleSpinBox->setValue(ui->CdDoubleSpinBox_2->value());

        ui->k1DoubleSpinBox->setValue(ui->k1DoubleSpinBox_2->value());
        ui->k2DoubleSpinBox->setValue(ui->k2DoubleSpinBox_2->value());
        ui->k3DoubleSpinBox->setValue(ui->k3DoubleSpinBox_2->value());
        ui->kminus1DoubleSpinBox->setValue(ui->kminus1DoubleSpinBox_2->value());
        ui->kminus2DoubleSpinBox->setValue(ui->kminus2DoubleSpinBox_2->value());
        ui->kminus3DoubleSpinBox->setValue(ui->kminus3DoubleSpinBox_2->value());
    }
    connectSliders();

}

//Display Menu :
void MainWindow::on_action_Update_Graph_triggered()
{
    m_graph.clean();
    m_graph.update();
}
void MainWindow::on_actionOptions_triggered()
{
    GraphSettings settings(&m_graph);
    settings.exec();
}
void MainWindow::on_action_RealTime_triggered(bool checked)
{
    m_realTime=checked;
    if(checked)
    {
        m_errorMessage->showMessage(tr("Attention :\nFonctionnalité potentiellement instable et très gourmande en ressources"));
    }
}

//About Menu :
void MainWindow::on_actionAbout_triggered()
{
    AboutWindow About;
    About.exec();
}

//Debug Menu :
    void MainWindow::on_actionDebugCurves_triggered(bool checked)
{
    if(checked)
        qDebug()<<"Debug Courbes Activé\n";
    if(!checked)
        qDebug()<<"Debug Courbes Dsactivé\n";
    m_graph.setDebug(checked);
}
    void MainWindow::on_actionDebugCalcul_triggered(bool checked)
    {
        if(checked)
            qDebug()<<"Debug Calcul Activé\n";
        if(!checked)
            qDebug()<<"Debug Calcul Dsactivé\n";
        simulation[0].setDebug(checked);
        simulation[1].setDebug(checked);
    }

//Sliders :
void MainWindow::on_horizontalSlider_valueChanged(int position) //slider has moved, let's change the ratio
{
    int Current=ui->tabWidget->currentIndex();
    disconnect(ui->k1DoubleSpinBox,SIGNAL(valueChanged(double)),this,SLOT(on_k1DoubleSpinBox_valueChanged(double)));
    disconnect(ui->k1DoubleSpinBox_2,SIGNAL(valueChanged(double)),this,SLOT(on_k1DoubleSpinBox_2_valueChanged(double)));
    simulation[Current].setRatio1(position);
    connect(ui->k1DoubleSpinBox,SIGNAL(valueChanged(double)),this,SLOT(on_k1DoubleSpinBox_valueChanged(double)));
    connect(ui->k1DoubleSpinBox_2,SIGNAL(valueChanged(double)),this,SLOT(on_k1DoubleSpinBox_2_valueChanged(double)));
    if(m_realTime)
    {
        simulation[Current].go();
        m_graph.clean();
        m_graph.update();
    }
}
void MainWindow::on_horizontalSlider_2_valueChanged(int position)
{
    int Current=ui->tabWidget->currentIndex();
    disconnect(ui->k3DoubleSpinBox,SIGNAL(valueChanged(double)),this,SLOT(on_k3DoubleSpinBox_valueChanged(double)));
    disconnect(ui->k3DoubleSpinBox_2,SIGNAL(valueChanged(double)),this,SLOT(on_k3DoubleSpinBox_2_valueChanged(double)));
    simulation[Current].setRatio2(position);
    connect(ui->k3DoubleSpinBox,SIGNAL(valueChanged(double)),this,SLOT(on_k3DoubleSpinBox_valueChanged(double)));
    disconnect(ui->k3DoubleSpinBox_2,SIGNAL(valueChanged(double)),this,SLOT(on_k3DoubleSpinBox_2_valueChanged(double)));
    if(m_realTime)
    {
        simulation[Current].go();
        m_graph.clean();
        m_graph.update();
    }
}

//tabs :
void MainWindow::on_tabWidget_currentChanged(int index) //update sliders on tab change
{
    disconnectSliders(); //if we don't do this, approximations will change K1 and K3
    simulation[index].CalculateRatio1();
    simulation[index].CalculateRatio2();
    simulation[index].testForSliders();
    connectSliders();
}

void MainWindow::on_Color0_clicked(){ui->Color0->setStyleSheet(changeColor(0, 0));}
void MainWindow::on_Color1_clicked(){ui->Color1->setStyleSheet(changeColor(1, 0));}
void MainWindow::on_Color2_clicked(){ui->Color2->setStyleSheet(changeColor(2, 0));}
void MainWindow::on_Color3_clicked(){ui->Color3->setStyleSheet(changeColor(3, 0));}

void MainWindow::on_Color0_2_clicked(){ui->Color0_2->setStyleSheet(changeColor(0, 1));}
void MainWindow::on_Color1_2_clicked(){ui->Color1_2->setStyleSheet(changeColor(1, 1));}
void MainWindow::on_Color2_2_clicked(){ui->Color2_2->setStyleSheet(changeColor(2, 1));}
void MainWindow::on_Color3_2_clicked(){ui->Color3_2->setStyleSheet(changeColor(3, 1));}

void MainWindow::on_k1DoubleSpinBox_valueChanged(double arg1)
{
    disconnectSliders();
    simulation[0].setKab(arg1);
    connectSliders();
}
void MainWindow::on_k2DoubleSpinBox_valueChanged(double arg1)
{
    disconnectSliders();
    simulation[0].setKbc(arg1);
    connectSliders();
}
void MainWindow::on_k3DoubleSpinBox_valueChanged(double arg1)
{
    disconnectSliders();
    simulation[0].setKcd(arg1);
    connectSliders();
}
void MainWindow::on_k1DoubleSpinBox_2_valueChanged(double arg1)
{
    disconnectSliders();
    simulation[1].setKab(arg1);
    connectSliders();
}
void MainWindow::on_k2DoubleSpinBox_2_valueChanged(double arg1)
{
    disconnectSliders();
    simulation[1].setKbc(arg1);
    connectSliders();
}
void MainWindow::on_k3DoubleSpinBox_2_valueChanged(double arg1)
{
    disconnectSliders();
    simulation[1].setKcd(arg1);
    connectSliders();
}


void MainWindow::on_actionDebugCurves_triggered()
{

}
