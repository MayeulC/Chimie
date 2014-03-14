#include "vue.h"

Vue::Vue(QWidget *parent)
:QWidget(parent), renderer(0) //Constructeur
{
    //renderer = SDLHelpers::createQImageCompliantSurface(SDL_SWSURFACE, height(), width());
    SDL_FreeSurface(renderer); //au cas où, la doc dit que c'est 'safe'
    renderer = SDLHelpers::createQImageCompliantSurface(SDL_HWSURFACE | SDL_RESIZABLE | SDL_DOUBLEBUF, height(), width());
    //couleur=qRgb(255,0,0);
    couleurFond=0xFFFFFF;
    couleurCourbe[0][0]=0xFF0000;
    couleurCourbe[1][0]=0x00FF00;
    couleurCourbe[2][0]=0x0000FF;
    couleurCourbe[3][0]=0x000000;

    couleurCourbe[0][1]=0xFF00FF;
    couleurCourbe[1][1]=0x00AAFF;
    couleurCourbe[2][1]=0xFFAA00;
    couleurCourbe[3][1]=0xAAFF00;

    debugTracage=false;
    SDL_FillRect(renderer, 0, couleurFond);
    for(int j=0;j<2;j++)
    {
        for(int i=0;i<4;i++) //mettre l'épaisseur par défaut à 1
        {
            m_epaisseur[i][j]=1;
        }
    }
}

void Vue::tracerCourbe(SDL_Surface *surface) //Trace la courbe, le SDL_Surface est requis pour l'impression
{
    /* et là on dessine les courbes sur la surface renderer*/
    int i,j,k;
    QPointF pointActuel, pointSuivant;
    //qDebug()<<element[0].tailleHistorique()<<" "<<element[1].tailleHistorique();
    for(i=0;i<2;i++)
    {
        for(k=0;k<4;k++)
        {
            if(m_epaisseur[k][i]) //si l'épaisseur est nulle on ne traçe pas
            {
                if(debugTracage)
                    qDebug()<<"courbe "<<k<<"\n";
                for(j=0;j<element[i][k].tailleHistorique()-1;j++) //-1 pour éviter de dépasser, ce qui fait crasher avec un std::outOfRange
                {
                    pointActuel=element[i][k].returnCt(j);
                    pointSuivant=element[i][k].returnCt(j+1);
                    tracerLigne(surface, pointActuel,pointSuivant, couleurCourbe[k][i], m_epaisseur[k][i] );
                }
            }
        }
    }

}
void Vue::tracerLigne(SDL_Surface *surface, QPointF depart,QPointF arrivee, Uint32 col=0x000000, int epaisseur=1)//trace une portion de courbe
{
    double xConvert= surface->w / (*Xmax);//et non pas getXmas... Pour une histoire d'échelle
    double yConvert= surface->h / (*Ymax);//w <=> width()

    double xstart=depart.x()*(xConvert);
    double ystart=depart.y()*(yConvert);

    double xend=arrivee.x()*(xConvert);
    double yend=arrivee.y()*(yConvert);

    /*if(xstart>xend)
    {
        double temp=xstart;
        xstart=xend;
        xend=temp;
    }
    if(ystart>yend) //cause d'un bug
    {
        double temp=ystart;
        ystart=yend;
        yend=temp;
    }*/
    double deltax=xend-xstart, deltay=yend-ystart;


    SDL_Rect rect;
        rect.h=epaisseur;
        rect.w=epaisseur;

    short int *p,*g;
    double *gstart, *pstart, *gdelta;
    //double *gend, *pend;
    double coef;
    int parcours=1;
    //il est mieux de parcourir la grande variation et d'afficher les points de la
    //petite variation

    if(debugTracage)
        qDebug()<<"traçage à "<<depart<<"\t et "<<arrivee;

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
        //gend=&xend;
        pstart=&ystart;
        //pend=&yend;
    }
    else//if(qAbs(deltax)<qAbs(deltay))
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
        //gend=&yend;
        pstart=&xstart;
        //pend=&xend;
    }

    if((*(gdelta))<0) //si le parcours est inversé
    {
        /*double *tmp;
        tmp=gstart;
        gstart=gend;
        gend=tmp;
        tmp=pstart;
        pstart=pend;
        pend=tmp;
        if //si delta négatif...*/
        parcours=-1;
    }

    for(int i=0; i<=qAbs(*(gdelta)); i++)
    {
        //dessiner à (x, coef*x + ystart)
        *(g) = i * parcours + *(gstart);
        *(p) = coef * i * parcours + *(pstart);
        rect.y=rect.y * (-1) + surface->h; //h <=> height(), remet le graphe à l'endroit
        SDL_FillRect(surface, &rect, col /*SDL_MapRGB(renderer->format, 255, 255, 0)*/);
    }
}
void Vue::imprimer() //pas testé sur une vraie imprimante, imprime le graphique. Penser aux marges
{
    QPrinter printer;

    QPrintDialog *dialog = new QPrintDialog(&printer, this);
    dialog->setWindowTitle(tr("Imprimer Courbe"));
    if (dialog->exec() != QDialog::Accepted)
        return;
    QPainter painter;
    painter.begin(&printer);

    //
    SDL_Surface* buffer=SDLHelpers::createQImageCompliantSurface(SDL_HWSURFACE | SDL_RESIZABLE | SDL_DOUBLEBUF, printer.pageRect().width(), printer.pageRect().height());
    SDL_FillRect(buffer, 0, couleurFond);
    tracerCourbe(buffer);
    QImage im = SDLHelpers::SDLSurfaceToQImage(buffer);
    painter.drawImage(0, 0, im);
    painter.end();

    SDL_FreeSurface(buffer);
}
void  Vue::effacer() //efface...
{
    SDL_FillRect(renderer, 0, couleurFond);
}

void Vue::setEpaisseur(int courbe, int simulation, int epaisseur)
{
    m_epaisseur[courbe][simulation]=epaisseur;
    effacer();
    update();
}
QImage Vue::getImage()
{
    return SDLHelpers::SDLSurfaceToQImage(renderer);
}

void Vue::paintEvent(QPaintEvent *event)
{
        if(renderer)
	{
                /*tracerCourbe(renderer,
			SDL_MapRGBA(renderer->format,
                                qRed(couleur), qGreen(couleur), qBlue(couleur), qAlpha(couleur)));
                */
            //GrandFather->redessiner();
            tracerCourbe(renderer);

		QImage im = SDLHelpers::SDLSurfaceToQImage(renderer);
		
		QPainter p(this);
		p.drawImage(0, 0, im);
	}
}
void Vue::resizeEvent ( QResizeEvent * event )//est appelé avant le PaintEvent
{
    SDL_FreeSurface(renderer); //évite d'énormes fuites mémoire
    renderer = SDLHelpers::createQImageCompliantSurface(SDL_HWSURFACE | SDL_RESIZABLE | SDL_DOUBLEBUF, width(), height());
    SDL_FillRect(renderer, 0, couleurFond);
    //qDebug()<<"Nouvelle taille : "<<height()<<" * "<<width();
}

/* //je la laisse, elle est sympa
void Vue::IamYourGrandFather( MainWindow *me)
{
    GrandFather=me;
}*/

