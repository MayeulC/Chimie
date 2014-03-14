#include "reglagescourbes.h"
#include "ui_reglagescourbes.h"

ReglagesCourbes::ReglagesCourbes(Vue* graphique, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReglagesCourbes)
{
    graphe=graphique;
    ui->setupUi(this);
    QColor col2=graphe->getCouleurFond();
    ui->couleurDeFond->setStyleSheet(QString("background-color: %1").arg(col2.name()));

    ui->horizontalSlider_1->setValue(graphe->getEpaisseur(0,0));
    ui->horizontalSlider_2->setValue(graphe->getEpaisseur(1,0));
    ui->horizontalSlider_3->setValue(graphe->getEpaisseur(2,0));
    ui->horizontalSlider_4->setValue(graphe->getEpaisseur(3,0));

    ui->horizontalSlider_5->setValue(graphe->getEpaisseur(0,1));
    ui->horizontalSlider_6->setValue(graphe->getEpaisseur(1,1));
    ui->horizontalSlider_7->setValue(graphe->getEpaisseur(2,1));
    ui->horizontalSlider_8->setValue(graphe->getEpaisseur(3,1));
}

ReglagesCourbes::~ReglagesCourbes()
{
    delete ui;
}

void ReglagesCourbes::on_couleurDeFond_clicked()
{
    QRgb col;
    col= QColorDialog::getRgba(graphe->getCouleurFond());
    graphe->setCouleurFond(col);
    QColor col2=col;
    graphe->effacer();
    graphe->update();
    ui->couleurDeFond->setStyleSheet(QString("background-color: %1").arg(col2.name()));
}

void ReglagesCourbes::on_buttonBox_accepted()
{
    close();
}

