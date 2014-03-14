#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QClipboard>
#include "classes.h"
#include "fenapropos.h"
#include "vue.h"
#include "reglagescourbes.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void soumissionYmax(double testYmax){Ymax=qMax(Ymax,testYmax);}
    void soumissionXmax(double testXmax){Xmax=qMax(Xmax,testXmax);}
    void deconnexionSliders(); //dÃ©connecte les sliders des signaux pour pouvoir modifier la spinbox sans toucher au slider.
    void connexionSliders();
    QString changerCouleur(int courbe, int sim); //permet d'allÃ©ger l'Ã©criture des slots. le Qstring sert Ã  la feuille de style css du bouton.
    ~MainWindow();

private slots:
    void on_Boutton_Calculer_clicked();
    void on_Boutton_Effacer_clicked();

    void on_tDoubleSpinBox_valueChanged(double arg1){Temps.setDuree(arg1);}
    void on_dTDoubleSpinBox_valueChanged(double arg1){Temps.setDeltaT(arg1);}

    //Menu Fichier :
    void on_actionQuitter_triggered();
    void on_actionImprimer_triggered();

    //menu Edition :
    void on_actionCopier_Graphique_triggered();
    void on_action_Importer_triggered();

    //Menu Affichage :
    void on_action_Actualiser_Graphique_triggered();
    void on_actionOptions_triggered();
    void on_action_Temps_R_el_triggered(bool checked);

    //Menu Aide:
    void on_actionA_propos_triggered();
    //Sous-menu Debug :
        void on_actionDebugCourbes_triggered(bool checked);
        void on_actionDebugCalcul_triggered(bool checked);

    //sliders :
    void on_horizontalSlider_valueChanged(int position);
    void on_horizontalSlider_2_valueChanged(int position);

    /////////////////////////////////////////////////////////
    //On passe aux onglets :
    void on_tabWidget_currentChanged(int index);

    //attention, il y a un paquet de connections qui arrivent :
    //Onglet 1 :
        void on_CaDoubleSpinBox_valueChanged(double arg1){simulation[0].setCa(arg1);}
        void on_CbDoubleSpinBox_valueChanged(double arg1){simulation[0].setCb(arg1);}
        void on_CcDoubleSpinBox_valueChanged(double arg1){simulation[0].setCc(arg1);}
        void on_CdDoubleSpinBox_valueChanged(double arg1){simulation[0].setCd(arg1);}

        void on_k1DoubleSpinBox_valueChanged(double arg1);//plus aussi simple avec les sliders : on va dans le cpp
        void on_k2DoubleSpinBox_valueChanged(double arg1);
        void on_k3DoubleSpinBox_valueChanged(double arg1);
        void on_kmoins1DoubleSpinBox_valueChanged(double arg1){simulation[0].setKba(arg1);}
        void on_kmoins2DoubleSpinBox_valueChanged(double arg1){simulation[0].setKcb(arg1);}
        void on_kmoins3DoubleSpinBox_valueChanged(double arg1){simulation[0].setKdc(arg1);}

        void on_Couleur0_clicked();
        void on_Couleur1_clicked();
        void on_Couleur2_clicked();
        void on_Couleur3_clicked();

        void on_activerElement4CheckBox_stateChanged(int arg1){simulation[0].checkboxTriggered(arg1);}

    //et rebelote :
    //Onglet 2 :
        void on_CaDoubleSpinBox_2_valueChanged(double arg1){simulation[1].setCa(arg1);}
        void on_CbDoubleSpinBox_2_valueChanged(double arg1){simulation[1].setCb(arg1);}
        void on_CcDoubleSpinBox_2_valueChanged(double arg1){simulation[1].setCc(arg1);}
        void on_CdDoubleSpinBox_2_valueChanged(double arg1){simulation[1].setCd(arg1);}

        void on_k1DoubleSpinBox_2_valueChanged(double arg1);//meme chose que pour le premier onglet, les slots sont dans le cpp
        void on_k2DoubleSpinBox_2_valueChanged(double arg1);
        void on_k3DoubleSpinBox_2_valueChanged(double arg1);
        void on_kmoins1DoubleSpinBox_2_valueChanged(double arg1){simulation[1].setKba(arg1);}
        void on_kmoins2DoubleSpinBox_2_valueChanged(double arg1){simulation[1].setKcb(arg1);}
        void on_kmoins3DoubleSpinBox_2_valueChanged(double arg1){simulation[1].setKdc(arg1);}

        void on_Couleur0_2_clicked();
        void on_Couleur1_2_clicked();
        void on_Couleur2_2_clicked();
        void on_Couleur3_2_clicked();

        void on_activerElement4CheckBox_2_stateChanged(int arg1){simulation[1].checkboxTriggered(arg1);}
    //ouf!

signals:
    void ProgressionCalcul(int);
    void MessageStatut(QString);

private:
    Ui::MainWindow *ui;
    T_Times Temps;
    //std::vector<JeuDeCourbes> memoire;
    Vue graphe;
    T_Simulation simulation[2];
    T_Element* element[2];
    bool tempsReel;
    double Xmax,Ymax;
    QErrorMessage* m_messageErreur; //pour la fonctionnalité "temps réel" qui peut générer des crash.
};

#endif // MAINWINDOW_H
