#ifndef REGLAGESCOURBES_H
#define REGLAGESCOURBES_H

#include <QDialog>
#include <QColorDialog>
#include "vue.h"

namespace Ui {
    class ReglagesCourbes;
}

class ReglagesCourbes : public QDialog
{
    Q_OBJECT

public:
    explicit ReglagesCourbes(Vue* graphique, QWidget *parent = 0);
    ~ReglagesCourbes();

private slots:
    void on_couleurDeFond_clicked();

    void on_buttonBox_accepted();

    void on_horizontalSlider_1_valueChanged(int value){graphe->setEpaisseur(0,0,value);}
    void on_horizontalSlider_2_valueChanged(int value){graphe->setEpaisseur(1,0,value);}
    void on_horizontalSlider_3_valueChanged(int value){graphe->setEpaisseur(2,0,value);}
    void on_horizontalSlider_4_valueChanged(int value){graphe->setEpaisseur(3,0,value);}

    void on_horizontalSlider_5_valueChanged(int value){graphe->setEpaisseur(0,1,value);}
    void on_horizontalSlider_6_valueChanged(int value){graphe->setEpaisseur(1,1,value);}
    void on_horizontalSlider_7_valueChanged(int value){graphe->setEpaisseur(2,1,value);}
    void on_horizontalSlider_8_valueChanged(int value){graphe->setEpaisseur(3,1,value);}
private:
    Ui::ReglagesCourbes *ui;
    Vue* graphe;
};

#endif // REGLAGESCOURBES_H
