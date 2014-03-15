#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QClipboard>
#include "classes.h"
#include "aboutwindow.h"
#include "view.h"
#include "graphsettings.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void submissionYmax(double testYmax){Ymax=qMax(Ymax,testYmax);}
    void submissionXmax(double testXmax){Xmax=qMax(Xmax,testXmax);}
    void disconnectSliders(); //disconnect the sliders from the signals to change the spinbox value without touching the sliders.
    void connectSliders();
    QString changeColor(int curve, int sim); //returns the button CSS stylesheet. Used in the slot connections.
    ~MainWindow();

private slots:
    void on_Button_Launch_clicked();
    void on_Button_Erase_clicked();

    void on_tDoubleSpinBox_valueChanged(double arg1){Time.setDuree(arg1);}
    void on_dTDoubleSpinBox_valueChanged(double arg1){Time.setDeltaT(arg1);}

    //File Menu :
    void on_actionQuit_triggered();
    void on_actionPrint_triggered();

    //Edit Menu :
    void on_actionCopy_Graph_triggered();
    void on_action_Import_triggered();

    //Display Menu :
    void on_action_Update_Graph_triggered();
    void on_actionOptions_triggered();
    void on_action_RealTime_triggered(bool checked);

    //Help Menu :
    void on_actionAbout_triggered();
    //Debug Menu :
        void on_actionDebugCurves_triggered(bool checked);
        void on_actionDebugCalcul_triggered(bool checked);

    //sliders :
    void on_horizontalSlider_valueChanged(int position);
    void on_horizontalSlider_2_valueChanged(int position);

    /////////////////////////////////////////////////////////
    //Tabs, now :
    void on_tabWidget_currentChanged(int index);

    //watch out, there is many connections ahead :
    //Tab 1 :
        void on_CaDoubleSpinBox_valueChanged(double arg1){simulation[0].setCa(arg1);}
        void on_CbDoubleSpinBox_valueChanged(double arg1){simulation[0].setCb(arg1);}
        void on_CcDoubleSpinBox_valueChanged(double arg1){simulation[0].setCc(arg1);}
        void on_CdDoubleSpinBox_valueChanged(double arg1){simulation[0].setCd(arg1);}

        void on_k1DoubleSpinBox_valueChanged(double arg1);//plus aussi simple avec les sliders : on va dans le cpp
        void on_k2DoubleSpinBox_valueChanged(double arg1);
        void on_k3DoubleSpinBox_valueChanged(double arg1);
        void on_kminus1DoubleSpinBox_valueChanged(double arg1){simulation[0].setKba(arg1);}
        void on_kminus2DoubleSpinBox_valueChanged(double arg1){simulation[0].setKcb(arg1);}
        void on_kminus3DoubleSpinBox_valueChanged(double arg1){simulation[0].setKdc(arg1);}

        void on_Color0_clicked();
        void on_Color1_clicked();
        void on_Color2_clicked();
        void on_Color3_clicked();

        void on_enableElement4CheckBox_stateChanged(int arg1){simulation[0].checkboxTriggered(arg1);}

    //and again :
    //Tab 2 :
        void on_CaDoubleSpinBox_2_valueChanged(double arg1){simulation[1].setCa(arg1);}
        void on_CbDoubleSpinBox_2_valueChanged(double arg1){simulation[1].setCb(arg1);}
        void on_CcDoubleSpinBox_2_valueChanged(double arg1){simulation[1].setCc(arg1);}
        void on_CdDoubleSpinBox_2_valueChanged(double arg1){simulation[1].setCd(arg1);}

        void on_k1DoubleSpinBox_2_valueChanged(double arg1);//meme chose que pour le premier onglet, les slots sont dans le cpp
        void on_k2DoubleSpinBox_2_valueChanged(double arg1);
        void on_k3DoubleSpinBox_2_valueChanged(double arg1);
        void on_kminus1DoubleSpinBox_2_valueChanged(double arg1){simulation[1].setKba(arg1);}
        void on_kminus2DoubleSpinBox_2_valueChanged(double arg1){simulation[1].setKcb(arg1);}
        void on_kminus3DoubleSpinBox_2_valueChanged(double arg1){simulation[1].setKdc(arg1);}

        void on_Color0_2_clicked();
        void on_Color1_2_clicked();
        void on_Color2_2_clicked();
        void on_Color3_2_clicked();

        void on_enableElement4CheckBox_2_stateChanged(int arg1){simulation[1].checkboxTriggered(arg1);}
    //done!

        void on_actionDebugCurves_triggered();

signals:
    void CalculationProgress(int);
    void MessageStatus(QString);

private:
    Ui::MainWindow *ui;
    T_Times Time;
    View m_graph;
    T_Simulation simulation[2];
    T_Element* element[2];
    bool m_realTime;
    double Xmax,Ymax;
    QErrorMessage* m_errorMessage; //For "realtime" functionality, that can crash the app.
};

#endif // MAINWINDOW_H
