#ifndef GRAPHSETTINGS_H
#define GRAPHSETTINGS_H

#include <QDialog>
#include <QColorDialog>
#include "view.h"

namespace Ui {
    class GraphSettings;
}

class GraphSettings : public QDialog
{
    Q_OBJECT

public:
    explicit GraphSettings(View* graphique, QWidget *parent = 0);
    ~GraphSettings();

private slots:
    void on_couleurDeFond_clicked();

    void on_buttonBox_accepted();

    void on_horizontalSlider_1_valueChanged(int value){m_graph->setWidth(0,0,value);}
    void on_horizontalSlider_2_valueChanged(int value){m_graph->setWidth(1,0,value);}
    void on_horizontalSlider_3_valueChanged(int value){m_graph->setWidth(2,0,value);}
    void on_horizontalSlider_4_valueChanged(int value){m_graph->setWidth(3,0,value);}

    void on_horizontalSlider_5_valueChanged(int value){m_graph->setWidth(0,1,value);}
    void on_horizontalSlider_6_valueChanged(int value){m_graph->setWidth(1,1,value);}
    void on_horizontalSlider_7_valueChanged(int value){m_graph->setWidth(2,1,value);}
    void on_horizontalSlider_8_valueChanged(int value){m_graph->setWidth(3,1,value);}
private:
    Ui::GraphSettings *ui;
    View* m_graph;
};

#endif // GRAPHSETTINGS_H
