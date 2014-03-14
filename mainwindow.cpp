#include "mainwindow.h"
#include "fenapropos.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Temps.initTemps();
    ui->verticalLayout_2->addWidget(&graphe);
    //graphe.setParent(ui->groupBoxGraphique);
    graphe.setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    //graphe.IamYourGrandFather(this);
    Xmax=0;
    Ymax=0;
    simulation[0].creerMessageErreur(this); //juste pour pouvoir empêcher l'utilisateur de cliquer sur la fenêtre
    simulation[1].creerMessageErreur(this); //lorqu'une boite de dialogue est ouverte.

    element[0]=simulation[0].getElements();
    element[1]=simulation[1].getElements();
   graphe.setElements(element);
   graphe.setEchelle(&Xmax,&Ymax);

    //on met les couleurs par défaut des boutons :
   for(int j=0;j<2;j++)
   {
       for(int i=0; i<4;i++)
        {
            QColor col=graphe.getCouleur(i,j);
            QString qss = QString("background-color: %1").arg(col.name());
            if(i==0&&j==0) //on aurait pu le faire avec le préprocesseur aussi...
                ui->Couleur0->setStyleSheet(qss);
            if(i==1&&j==0)
                ui->Couleur1->setStyleSheet(qss);
            if(i==2&&j==0)
                ui->Couleur2->setStyleSheet(qss);
            if(i==3&&j==0)
                ui->Couleur3->setStyleSheet(qss);
            //passage à la simulation #2
            if(i==0&&j==1)
                ui->Couleur0_2->setStyleSheet(qss);
            if(i==1&&j==1)
                ui->Couleur1_2->setStyleSheet(qss);
            if(i==2&&j==1)
                ui->Couleur2_2->setStyleSheet(qss);
            if(i==3&&j==1)
                ui->Couleur3_2->setStyleSheet(qss);
        }
   }

    simulation[0].setTemps(&Temps); //on va en profiter pour mettre le parent du QErrorMessage à jour
    simulation[1].setTemps(&Temps);

    tempsReel=false;
    m_messageErreur=new QErrorMessage(this);


    connect(&simulation[0],SIGNAL(MessageStatut(QString)),this,SIGNAL(MessageStatut(QString)));
    connect(&simulation[1],SIGNAL(MessageStatut(QString)),this,SIGNAL(MessageStatut(QString)));
    connect(&simulation[0],SIGNAL(ProgressionCalcul(int)),this,SIGNAL(ProgressionCalcul(int)));
    connect(&simulation[1],SIGNAL(ProgressionCalcul(int)),this,SIGNAL(ProgressionCalcul(int)));

    //et maintenant, les sliders...
    connect(&simulation[0],SIGNAL(nouveauRapport1(int)),ui->horizontalSlider,SLOT(setValue(int)));
    connect(&simulation[1],SIGNAL(nouveauRapport1(int)),ui->horizontalSlider,SLOT(setValue(int)));
    connect(&simulation[0],SIGNAL(nouveauRapport2(int)),ui->horizontalSlider_2,SLOT(setValue(int)));
    connect(&simulation[1],SIGNAL(nouveauRapport2(int)),ui->horizontalSlider_2,SLOT(setValue(int)));

    connect(&simulation[0],SIGNAL(DisableSlider1(bool)),ui->horizontalSlider,SLOT(setDisabled(bool)));
    connect(&simulation[1],SIGNAL(DisableSlider1(bool)),ui->horizontalSlider,SLOT(setDisabled(bool)));
    connect(&simulation[0],SIGNAL(DisableSlider2(bool)),ui->horizontalSlider_2,SLOT(setDisabled(bool)));
    connect(&simulation[1],SIGNAL(DisableSlider2(bool)),ui->horizontalSlider_2,SLOT(setDisabled(bool)));

    connect(&simulation[0],SIGNAL(nouveauK1(double)),ui->k1DoubleSpinBox,SLOT(setValue(double)));
    connect(&simulation[1],SIGNAL(nouveauK1(double)),ui->k1DoubleSpinBox_2,SLOT(setValue(double)));
    connect(&simulation[0],SIGNAL(nouveauK3(double)),ui->k3DoubleSpinBox,SLOT(setValue(double)));
    connect(&simulation[1],SIGNAL(nouveauK3(double)),ui->k3DoubleSpinBox_2,SLOT(setValue(double)));

    emit MessageStatut("Prêt.");
}

void MainWindow::connexionSliders() //m^eme chose que pour la déco : l'onglet actif suffit
{
    connect(ui->horizontalSlider,SIGNAL(valueChanged(int)),this,SLOT(on_horizontalSlider_valueChanged(int)));
    connect(ui->horizontalSlider_2,SIGNAL(valueChanged(int)),this,SLOT(on_horizontalSlider_2_valueChanged(int)));
}
void MainWindow::deconnexionSliders() //il suffit de déconnecter la simulation actuelle
{
    disconnect(ui->horizontalSlider,SIGNAL(valueChanged(int)),this,SLOT(on_horizontalSlider_valueChanged(int)));
    disconnect(ui->horizontalSlider_2,SIGNAL(valueChanged(int)),this,SLOT(on_horizontalSlider_2_valueChanged(int)));
}
QString MainWindow::changerCouleur(int courbe, int sim)
{
    QRgb col;
    col= QColorDialog::getRgba(graphe.getCouleur(courbe,sim));
    graphe.setCouleur(courbe,sim,col);
    QColor col2=col;
    graphe.update();
    return QString("background-color: %1").arg(col2.name());
}
MainWindow::~MainWindow()
{
    delete ui;
}




/*******************************\
|Dessous viennent les connexions|
| signal-slot semi-automatisées |
|           de Qt               |
\*******************************/


void MainWindow::on_Boutton_Calculer_clicked()
{
    //vérifier si il s'agit de l'onglet 0 ou 1
    int Courant=ui->tabWidget->currentIndex();
    Xmax=simulation[!(Courant)].getXmax();
    Ymax=simulation[!(Courant)].getYmax();
    simulation[Courant].go();
    soumissionYmax(simulation[Courant].getYmax());
    soumissionXmax(simulation[Courant].getXmax());
    graphe.effacer();
    graphe.update();
}
void MainWindow::on_Boutton_Effacer_clicked()
{
    //redessiner();
    //mettre des conditions sur la visibilité des onglets
    /*if(ui->tabWidget->currentIndex()==0)
    {
        simulation[0].amnesie();
        Xmax=simulation[1].getXmax();
        Ymax=simulation[1].getYmax();
    }*/

    simulation[ui->tabWidget->currentIndex()].amnesie();
    Xmax=simulation[!ui->tabWidget->currentIndex()].getXmax();
    Ymax=simulation[!ui->tabWidget->currentIndex()].getYmax();
    graphe.effacer();
    graphe.update();
}


//Menu Fichier :
void MainWindow::on_actionQuitter_triggered()
{
    exit(EXIT_SUCCESS);
}
void MainWindow::on_actionImprimer_triggered()
{
    graphe.imprimer();
}
//Menu Edition :
void MainWindow::on_actionCopier_Graphique_triggered()
{
    QApplication::clipboard()->setImage(graphe.getImage());
}
void MainWindow::on_action_Importer_triggered() //l'opérateur '=' ne fonctionne pas ici.
{
    int Courant=ui->tabWidget->currentIndex();

    //on prend le taureau par les cornes et on se jette à l'eau :
    deconnexionSliders();
    if(Courant)//si onglet 1
    {
        ui->CaDoubleSpinBox_2->setValue(ui->CaDoubleSpinBox->value());
        ui->CbDoubleSpinBox_2->setValue(ui->CbDoubleSpinBox->value());
        ui->CcDoubleSpinBox_2->setValue(ui->CcDoubleSpinBox->value());
        ui->CdDoubleSpinBox_2->setValue(ui->CdDoubleSpinBox->value());

        ui->k1DoubleSpinBox_2->setValue(ui->k1DoubleSpinBox->value());
        ui->k2DoubleSpinBox_2->setValue(ui->k2DoubleSpinBox->value());
        ui->k3DoubleSpinBox_2->setValue(ui->k3DoubleSpinBox->value());
        ui->kmoins1DoubleSpinBox_2->setValue(ui->kmoins1DoubleSpinBox->value());
        ui->kmoins2DoubleSpinBox_2->setValue(ui->kmoins2DoubleSpinBox->value());
        ui->kmoins3DoubleSpinBox_2->setValue(ui->kmoins3DoubleSpinBox->value());
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
        ui->kmoins1DoubleSpinBox->setValue(ui->kmoins1DoubleSpinBox_2->value());
        ui->kmoins2DoubleSpinBox->setValue(ui->kmoins2DoubleSpinBox_2->value());
        ui->kmoins3DoubleSpinBox->setValue(ui->kmoins3DoubleSpinBox_2->value());
    }
    connexionSliders();

}
//Menu Affichage :
void MainWindow::on_action_Actualiser_Graphique_triggered()
{
    graphe.effacer();
    graphe.update();
}
void MainWindow::on_actionOptions_triggered()
{
    ReglagesCourbes reglages(&graphe);
    reglages.exec();
}
void MainWindow::on_action_Temps_R_el_triggered(bool checked)
{
    tempsReel=checked;
    if(checked)
    {
        m_messageErreur->showMessage("Attention :\nFonctionnalité potentiellement instable et très gourmande en ressources");
    }
}
//Menu A propos :
void MainWindow::on_actionA_propos_triggered()
{
    FenApropos Apropos;
    Apropos.exec();
}
//Sous-menu Debug :
    void MainWindow::on_actionDebugCourbes_triggered(bool checked)
{
    if(checked)
        qDebug()<<"Debug Courbes Activé\n";
    if(!checked)
        qDebug()<<"Debug Courbes Désactivé\n";
    graphe.setDebug(checked);
}
    void MainWindow::on_actionDebugCalcul_triggered(bool checked)
    {
        if(checked)
            qDebug()<<"Debug Calcul Activé\n";
        if(!checked)
            qDebug()<<"Debug Calcul Désactivé\n";
        simulation[0].setDebug(checked);
        simulation[1].setDebug(checked);
    }

//Sliders :
void MainWindow::on_horizontalSlider_valueChanged(int position)//le slider a bougé, on modifie le rapport
{
    int Courant=ui->tabWidget->currentIndex();
    disconnect(ui->k1DoubleSpinBox,SIGNAL(valueChanged(double)),this,SLOT(on_k1DoubleSpinBox_valueChanged(double)));
    disconnect(ui->k1DoubleSpinBox_2,SIGNAL(valueChanged(double)),this,SLOT(on_k1DoubleSpinBox_2_valueChanged(double)));
    simulation[Courant].setRapport1(position);
    connect(ui->k1DoubleSpinBox,SIGNAL(valueChanged(double)),this,SLOT(on_k1DoubleSpinBox_valueChanged(double)));
    connect(ui->k1DoubleSpinBox_2,SIGNAL(valueChanged(double)),this,SLOT(on_k1DoubleSpinBox_2_valueChanged(double)));
    if(tempsReel)
    {
        simulation[Courant].go();
        graphe.effacer();
        graphe.update();
    }
}
void MainWindow::on_horizontalSlider_2_valueChanged(int position)
{
    int Courant=ui->tabWidget->currentIndex();
    disconnect(ui->k3DoubleSpinBox,SIGNAL(valueChanged(double)),this,SLOT(on_k3DoubleSpinBox_valueChanged(double)));
    disconnect(ui->k3DoubleSpinBox_2,SIGNAL(valueChanged(double)),this,SLOT(on_k3DoubleSpinBox_2_valueChanged(double)));
    simulation[Courant].setRapport2(position);
    connect(ui->k3DoubleSpinBox,SIGNAL(valueChanged(double)),this,SLOT(on_k3DoubleSpinBox_valueChanged(double)));
    disconnect(ui->k3DoubleSpinBox_2,SIGNAL(valueChanged(double)),this,SLOT(on_k3DoubleSpinBox_2_valueChanged(double)));
    if(tempsReel)
    {
        simulation[Courant].go();
        graphe.effacer();
        graphe.update();
    }
}

//onglets :
void MainWindow::on_tabWidget_currentChanged(int index) //permet de mettre à jour les sliders au changement d'onglet
{
    deconnexionSliders(); //sinon les approximations vont faire changer K1 et K3
    simulation[index].CalculRapport1();
    simulation[index].CalculRapport2();
    simulation[index].testPourSliders();
    connexionSliders();
}

void MainWindow::on_Couleur0_clicked(){ui->Couleur0->setStyleSheet(changerCouleur(0, 0));}
void MainWindow::on_Couleur1_clicked(){ui->Couleur1->setStyleSheet(changerCouleur(1, 0));}
void MainWindow::on_Couleur2_clicked(){ui->Couleur2->setStyleSheet(changerCouleur(2, 0));}
void MainWindow::on_Couleur3_clicked(){ui->Couleur3->setStyleSheet(changerCouleur(3, 0));}

void MainWindow::on_Couleur0_2_clicked(){ui->Couleur0_2->setStyleSheet(changerCouleur(0, 1));}
void MainWindow::on_Couleur1_2_clicked(){ui->Couleur1_2->setStyleSheet(changerCouleur(1, 1));}
void MainWindow::on_Couleur2_2_clicked(){ui->Couleur2_2->setStyleSheet(changerCouleur(2, 1));}
void MainWindow::on_Couleur3_2_clicked(){ui->Couleur3_2->setStyleSheet(changerCouleur(3, 1));}

void MainWindow::on_k1DoubleSpinBox_valueChanged(double arg1)
{
    deconnexionSliders();
    simulation[0].setKab(arg1);
    connexionSliders();
}
void MainWindow::on_k2DoubleSpinBox_valueChanged(double arg1)
{
    deconnexionSliders();
    simulation[0].setKbc(arg1);
    connexionSliders();
}
void MainWindow::on_k3DoubleSpinBox_valueChanged(double arg1)
{
    deconnexionSliders();
    simulation[0].setKcd(arg1);
    connexionSliders();
}
void MainWindow::on_k1DoubleSpinBox_2_valueChanged(double arg1)
{
    deconnexionSliders();
    simulation[1].setKab(arg1);
    connexionSliders();
}
void MainWindow::on_k2DoubleSpinBox_2_valueChanged(double arg1)
{
    deconnexionSliders();
    simulation[1].setKbc(arg1);
    connexionSliders();
}
void MainWindow::on_k3DoubleSpinBox_2_valueChanged(double arg1)
{
    deconnexionSliders();
    simulation[1].setKcd(arg1);
    connexionSliders();
}

