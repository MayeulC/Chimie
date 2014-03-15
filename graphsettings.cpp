#include "graphsettings.h"
#include "ui_graphsettings.h"

GraphSettings::GraphSettings(View* graph, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GraphSettings)
{
    m_graph=graph;
    ui->setupUi(this);
    QColor col2=m_graph->getBackgroundColor();
    ui->couleurDeFond->setStyleSheet(QString("background-color: %1").arg(col2.name()));

    ui->horizontalSlider_1->setValue(m_graph->getWidth(0,0));
    ui->horizontalSlider_2->setValue(m_graph->getWidth(1,0));
    ui->horizontalSlider_3->setValue(m_graph->getWidth(2,0));
    ui->horizontalSlider_4->setValue(m_graph->getWidth(3,0));

    ui->horizontalSlider_5->setValue(m_graph->getWidth(0,1));
    ui->horizontalSlider_6->setValue(m_graph->getWidth(1,1));
    ui->horizontalSlider_7->setValue(m_graph->getWidth(2,1));
    ui->horizontalSlider_8->setValue(m_graph->getWidth(3,1));
}

GraphSettings::~GraphSettings()
{
    delete ui;
}

void GraphSettings::on_couleurDeFond_clicked()
{
    QRgb col;
    col= QColorDialog::getRgba(m_graph->getBackgroundColor());
    m_graph->setBackgroundColor(col);
    QColor col2=col;
    m_graph->clean();
    m_graph->update();
    ui->couleurDeFond->setStyleSheet(QString("background-color: %1").arg(col2.name()));
}

void GraphSettings::on_buttonBox_accepted()
{
    close();
}

