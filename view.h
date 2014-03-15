#ifndef VIEW_H
#define VIEW_H

#include "tools.h"
#include "classes.h"
#include <QPrinter>
#include <QPrintDialog>
#include <QPainter>


class View : public QWidget
{
public:
        View(QWidget *parent=0);
        void drawCurve(SDL_Surface *s);
        void drawLine(SDL_Surface *screen, QPointF start,QPointF stop, Uint32 col,int width);
        void print();
        void clean();

        void setWidth(int curve, int simulation, int width);
        QImage getImage(); //this is more constructImage();

        void setElements(T_Element* element[2]){m_element=element;}
        void setScale(double* Xmaxi, double* Ymaxi){Xmax=Xmaxi;Ymax=Ymaxi;}
        void setDebug(bool enabled)                                 {debugDraw=enabled;}

        int getWidth(int curve, int simulation)                {return m_width[curve][simulation];}
        void setColor(int curve,int simulation, QRgb parametre)  {m_curveColor[curve][simulation]=parametre;}
        QRgb getColor(int curve, int simulation)                 {return m_curveColor[curve][simulation];}
        void setBackgroundColor(QRgb param)                             {m_backgroundColor=param;}
        QRgb getBackgroundColor()                                       {return m_backgroundColor;}

public slots:

protected:
        virtual void paintEvent(QPaintEvent *e);
        virtual void resizeEvent ( QResizeEvent * event );

	
private:
        SDL_Surface *m_renderer;
        T_Element** m_element;
        double *Xmax, *Ymax;
        Uint32 m_backgroundColor;
        Uint32 m_curveColor[4][2];
        int m_width[4][2];
        bool debugDraw;
};
#endif
