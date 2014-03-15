#include "view.h"

View::View(QWidget *parent)
:QWidget(parent), m_renderer(0)
{
    SDL_FreeSurface(m_renderer); //The documentation says it's 'safe'
    m_renderer = SDLHelpers::createQImageCompliantSurface(SDL_HWSURFACE | SDL_RESIZABLE | SDL_DOUBLEBUF, height(), width());
    m_backgroundColor=0xFFFFFF;
    m_curveColor[0][0]=0xFF0000;
    m_curveColor[1][0]=0x00FF00;
    m_curveColor[2][0]=0x0000FF;
    m_curveColor[3][0]=0x000000;

    m_curveColor[0][1]=0xFF00FF;
    m_curveColor[1][1]=0x00AAFF;
    m_curveColor[2][1]=0xFFAA00;
    m_curveColor[3][1]=0xAAFF00;

    debugDraw=false;
    SDL_FillRect(m_renderer, 0, m_backgroundColor);
    for(int j=0;j<2;j++)
    {
        for(int i=0;i<4;i++) //By default, m_width=1
        {
            m_width[i][j]=1;
        }
    }
}

void View::drawCurve(SDL_Surface *surface) //Draw the curve, SDL_Surface is required to print
{
    int i,j,k;
    QPointF current, next;
    for(i=0;i<2;i++)
    {
        for(k=0;k<4;k++)
        {
            if(m_width[k][i]) //do not draw if width=0
            {
                if(debugDraw)
                    qDebug()<<"courbe "<<k<<"\n";
                for(j=0;j<m_element[i][k].getHistorySize()-1;j++) //-1 to avoid overflow (crashes with std::outOfRange)
                {
                    current=m_element[i][k].returnCt(j);
                    next=m_element[i][k].returnCt(j+1);
                    drawLine(surface, current,next, m_curveColor[k][i], m_width[k][i] );
                }
            }
        }
    }

}
void View::drawLine(SDL_Surface *surface, QPointF start,QPointF stop, Uint32 col=0x000000, int width=1)//draw a curve section
{
    double xConvert= surface->w / (*Xmax);//not getXmas... To scale
    double yConvert= surface->h / (*Ymax);//w <=> width()

    double xstart=start.x()*(xConvert);
    double ystart=start.y()*(yConvert);

    double xend=stop.x()*(xConvert);
    double yend=stop.y()*(yConvert);

    double deltax=xend-xstart, deltay=yend-ystart;


    SDL_Rect rect;
        rect.h=width;
        rect.w=width;

    short int *p,*g;
    double *gstart, *pstart, *gdelta;
    double coef;
    int parcours=1;
    //it is better to move along the big variation and to display values of the little one.


    if(debugDraw)
        qDebug()<<"traçage à "<<start<<"\t et "<<stop;

    if(qAbs(deltax)>=qAbs(deltay))
    {
        if(deltax)
        {
            coef=deltay/deltax;
        }
        else
        {
            coef=0;
        }
        p=&(rect.y);
        g=&(rect.x);
        gstart=&xstart;
        gdelta=&deltax;
        pstart=&ystart;
    }
    else //if(qAbs(deltax)<qAbs(deltay))
    {
        if(deltay)
        {
            coef=deltax/deltay;
        }
        else
        {
            coef=0;
        }
        p=&(rect.x);
        g=&(rect.y);
        gstart=&ystart;
        gdelta=&deltay;
        pstart=&xstart;
    }

    if((*(gdelta))<0) //if reversed
    {
        parcours=-1;
    }

    for(int i=0; i<=qAbs(*(gdelta)); i++)
    {
        *(g) = i * parcours + *(gstart);
        *(p) = coef * i * parcours + *(pstart);
        rect.y=rect.y * (-1) + surface->h; //h <=> height(), put the graph upside
        SDL_FillRect(surface, &rect, col /*SDL_MapRGB(m_renderer->format, 255, 255, 0)*/);
    }
}
void View::print()
{
    QPrinter printer;
    QPrintDialog *dialog = new QPrintDialog(&printer, this);
    dialog->setWindowTitle(tr("Imprimer Courbe"));
    if (dialog->exec() != QDialog::Accepted)
        return;
    QPainter painter;
    painter.begin(&printer);

    SDL_Surface* buffer=SDLHelpers::createQImageCompliantSurface(SDL_HWSURFACE | SDL_RESIZABLE | SDL_DOUBLEBUF, printer.pageRect().width(), printer.pageRect().height());
    SDL_FillRect(buffer, 0, m_backgroundColor);
    drawCurve(buffer);
    QImage im = SDLHelpers::SDLSurfaceToQImage(buffer);
    painter.drawImage(0, 0, im);
    painter.end();

    SDL_FreeSurface(buffer);
}
void  View::clean() //clean...
{
    SDL_FillRect(m_renderer, 0, m_backgroundColor);
}

void View::setWidth(int curve, int simulation, int width)
{
    m_width[curve][simulation]=width;
    clean();
    update();
}
QImage View::getImage()
{
    return SDLHelpers::SDLSurfaceToQImage(m_renderer);
}

void View::paintEvent(QPaintEvent *event)
{
        if(m_renderer)
	{
            drawCurve(m_renderer);

                QImage im = SDLHelpers::SDLSurfaceToQImage(m_renderer);
		
		QPainter p(this);
		p.drawImage(0, 0, im);
	}
}
void View::resizeEvent ( QResizeEvent * event )//called before PaintEvent
{
    SDL_FreeSurface(m_renderer); //to avoid memory leaks
    m_renderer = SDLHelpers::createQImageCompliantSurface(SDL_HWSURFACE | SDL_RESIZABLE | SDL_DOUBLEBUF, width(), height());
    SDL_FillRect(m_renderer, 0, m_backgroundColor);
}
