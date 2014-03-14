#ifndef VUE_H
#define VUE_H

#include "tools.h"
#include "classes.h"
#include <QPrinter>
#include <QPrintDialog>
#include <QPainter>


class Vue : public QWidget
{
public:
        Vue(QWidget *parent=0);
        void tracerCourbe(SDL_Surface *s);
        void tracerLigne(SDL_Surface *screen, QPointF depart,QPointF arrivee, Uint32 col,int epaisseur);
        void imprimer();
        void effacer();

        void setEpaisseur(int courbe, int simulation, int epaisseur);
        QImage getImage(); //c'est plutot construireImage...

        void setElements(T_Element* elem[2]){element=elem;}
        void setEchelle(double* Xmaxi, double* Ymaxi){Xmax=Xmaxi;Ymax=Ymaxi;}
        void setDebug(bool enabled)                                 {debugTracage=enabled;}

        int getEpaisseur(int courbe, int simulation)                {return m_epaisseur[courbe][simulation];}
        void setCouleur(int courbe,int simulation, QRgb parametre)  {couleurCourbe[courbe][simulation]=parametre;}
        QRgb getCouleur(int courbe, int simulation)                 {return couleurCourbe[courbe][simulation];}
        void setCouleurFond(QRgb param)                             {couleurFond=param;}
        QRgb getCouleurFond()                                       {return couleurFond;}


        //void IamYourGrandFather( MainWindow * me); //nooooooo

public slots:


protected:
	virtual void paintEvent(QPaintEvent *e);
    virtual void resizeEvent ( QResizeEvent * event );

	
private:
        SDL_Surface *renderer;
        T_Element** element;
        double *Xmax, *Ymax;
        Uint32 couleurFond;
        Uint32 couleurCourbe[4][2];
        int m_epaisseur[4][2];
        bool debugTracage;
};
#endif
