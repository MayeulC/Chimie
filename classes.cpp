/* Classes.cpp : contient les m�thodes des classes du projet de fin d'ann�e
*/

#include "classes.h"

//M�thodes pour T_Simulation

T_Simulation::T_Simulation()
{
    nombreElements = 3; //par d�faut, sera chang� au besoin par le signal.
    RAB.initialiser(&element[0], &element[1]); //associe la r�action � l'�l�ment
    RBC.initialiser(&element[1], &element[2]);
    Xmax=0; //V�rifier que cela ne cause pas de bug
    Ymax=0;
    debugCalcul=false;
}
void T_Simulation::creerMessageErreur(QWidget *parent)
{
    m_parent=parent; //sert pour le message d'erreur en cas de d�passement m�moire.
    MessageErreur=new QErrorMessage(parent);
}
void T_Simulation::go()
{
    //v�rifier les conditions, si une est fausse, afficher une boite de dialogue
    // (tester k!=0; T !=0; Dt != 0 )
    std::string ErrorMessage;

    if ((element[0].verifier()||element[1].verifier()||element[2].verifier()||((nombreElements-3)&&element[3].verifier())))
    {
        ErrorMessage+="<br> - Les �l�ments de la r�action";
    }

    if(RAB.verifier()||RBC.verifier()||((nombreElements-3)&&RCD.verifier()))
    {
        ErrorMessage+="<br> - Les param�tres de r�action";
    }

    if(Temps->verifier())
    {
        ErrorMessage+="<br> - Les param�tres temporels";
    }
    if(ErrorMessage.size()!=0)
    {
        emit MessageStatut("Une erreur est survenue! V�rifiez les param�tres.");
        ErrorMessage="Veuillez v�rifier : " + ErrorMessage;
        MessageErreur->showMessage(ErrorMessage.c_str());
        return;
    }
    element[0].preparer();
    element[1].preparer();
    element[2].preparer();
    element[3].preparer();//permet d'effacer la courbe si l'�l�ment a �t� d�sactiv�

    Temps->initialiser();

    emit ProgressionCalcul(0);
    emit MessageStatut("Calcul en cours...");

    Ymax=0;
    /**/
    if(debugCalcul)
    {
        qDebug()<< "**********Valeurs initiales************\n";
        qDebug()<< element[0].returnCt(0)<<" \t"<<element[1].returnCt(0)<<"\t"<<element[2].returnCt(0);
        qDebug()<< "\n/***********************************\\\n|D�but de l'arriv�e des r�sultats : |\n\\***********************************/\n";
        qDebug()<< "Dur�e : "<<Temps->dureesec()<< " delta : "<<Temps->deltaTsec()<<" courant : "<<Temps->courantsec()<<"\n";
    }
    /**/

    try
    {
        while (Temps->checkLoop()) //condition de calcul des concentrations exclusivement temporelle
        {
            Ymax=qMax(Ymax,qMax(qMax(element[0].returnCt(),element[1].returnCt()),qMax(element[2].returnCt(),element[3].returnCt())));
            /**/
            if(debugCalcul)
                qDebug() <<Temps->courantsec()<< " |\t"<< element[0].returnCt() << '\t' << element[1].returnCt() << '\t' << element[2].returnCt() << '\n';
            /**/
            element[0].avancer(Temps, int(Temps->courantsec()/Temps->deltaTsec())); //variation de concentration de A
            element[1].avancer(Temps, int(Temps->courantsec()/Temps->deltaTsec())); //variation de concentration de B
            element[2].avancer(Temps, int(Temps->courantsec()/Temps->deltaTsec())); //variation de concentration de C
            if (nombreElements == 4)
                element[3].avancer(Temps, int(Temps->courantsec()/Temps->deltaTsec())); //�ventuelle variation de concentration de D


            Temps->avancement(); //avancement de l'instant Courant repr�sentant l'avancement de la r�action
            emit ProgressionCalcul(int(100*(Temps->courantsec()/Temps->dureesec())));
        }

        Ymax=qMax(Ymax,qMax(qMax(element[0].returnCt(),element[1].returnCt()),qMax(element[2].returnCt(),element[3].returnCt())));
        /**/
        if(debugCalcul)
            qDebug() <<Temps->courantsec()<< " |\t"<< element[0].returnCt() << '\t' << element[1].returnCt() << '\t' << element[2].returnCt() << '\n';
        /**/
        emit MessageStatut("Calcul termin�");
        emit ProgressionCalcul(100);
        Xmax=Temps->courantsec()-Temps->deltaTsec(); //car on a fait un deltaT de plus pour sortir de la boucle

    }
    catch(std::exception e)
    {
        outOfMemHandler();
    }


}
void T_Simulation::amnesie(){
    element[0].preparer();
    element[1].preparer();
    element[2].preparer();
    element[3].preparer();
    Xmax=0;
    Ymax=0;
}
void T_Simulation::checkboxTriggered(int arg1){
    nombreElements=3+!!arg1; // l'argument renvoie 0 ou 2, donc prendre !! permet d'avoir 0 ou 1

    if(nombreElements==4)
        RCD.initialiser(&element[2],&element[3]);  //associe C et D dans RCD
    if(nombreElements==3)
        RCD.dissocier(&element[2],&element[3]);//supprime la mention de D dans les diff�rents pointeurs.

    testPourSliders();//au cas o� cela permettrait l'activation/la d�sactivation d'un slider
}
void T_Simulation::setRapport1(int position)
{
    double temp=position;
    temp=temp/300;
    Rapport1=pow(10,(temp)); //K2/K1
    double newK1=RBC.returnKab()/Rapport1;
    RAB.setKab(newK1);

    emit nouveauK1(newK1);
}
void T_Simulation::setRapport2(int position)
{
    double temp=position;
    temp=temp/300;                     //au lieu de faire un double(position)/double(300)
    Rapport2=pow(10,(temp)); //K3/K2
    double newK3=RBC.returnKab()*Rapport2;
    RCD.setKab(newK3);
    emit nouveauK3(newK3);
}
void T_Simulation::CalculRapport1()
{
    if(RAB.returnKab()!=0)//K2/K1 soit RBC/RAB
    {
        Rapport1=RBC.returnKab()/RAB.returnKab();
        emit nouveauRapport1(log(Rapport1)*300);
    }
    if(RBC.returnKab()==0&&RAB.returnKab()==0)
    {
        emit nouveauRapport1(0);
    }
    else
    {
        if(RBC.returnKab()==0)
            emit nouveauRapport1(-1000);
        if(RAB.returnKab()==0)
            emit nouveauRapport1(1000);
    }
}
void T_Simulation::CalculRapport2()
{
    //if(nombreElements==4) //en fait c'est plus joli de voir le slider bouger m^eme quand E4 est d�sactiv�
    //{
        if(RBC.returnKab()!=0) //K3/K2 soit RCD/RBC
        {
            Rapport1=RCD.returnKab()/RBC.returnKab();
            emit nouveauRapport2(log(Rapport1)*300);
        }
        if(RBC.returnKab()==0&&RCD.returnKab()==0)
        {
            emit nouveauRapport2(0);
        }
        else
        {
            if(RBC.returnKab()==0)
                emit nouveauRapport2(1000);
            if(RCD.returnKab()==0)
                emit nouveauRapport2(-1000);
        }
    //}
}
void T_Simulation::testPourSliders() //v�rifie quels sliders peuvent �tre activ�s et les (d�s)active
{
    if(RBC.returnKab()!=0) //si [s]k1 et k2[/s] K2 suffit car on modifie K1 en fonction de K2 avec le slider.
    {
        emit DisableSlider1(false);
    }
    else
    {
        emit DisableSlider1(true);
    }
    //et rebelote pour le deuxi�me slider
    if(RBC.returnKab()!=0&&nombreElements==4) //si k2 est valide et qu'il y a 4 �l�ments en jeu
    {
        emit DisableSlider2(false);
    }
    else
    {
        emit DisableSlider2(true);
    }
}
void T_Simulation::outOfMemHandler()
{
    element[0].preparer();
    element[1].preparer();
    element[2].preparer();
    element[3].preparer();
    emit MessageStatut("Erreur critique : M�moire insuffisante!");
    QMessageBox::warning(m_parent,"Erreur","M�moire vive insuffisante pour continuer.\nV�rifiez les param�tres temporels.");
    //MessageErreur->showMessage("M�moire vive insuffisante pour continuer.\nV�rifiez les param�tres temporels.","mem");
    std::set_new_handler(NULL);
}
T_Simulation::~T_Simulation()
{

}


//M�thodes pour T_Element

T_Element::T_Element() //constructeur pour T_Element inutilis� (cas d'une r�action � 3 �l�ments)
{
    *m_Name = '\0';
     m_C0 = 0;
     m_Ct = 0;
     m_vC[0] = 0;
     m_vC[1] = 0;
     m_Reaction[0] = NULL;
     m_Reaction[1] = NULL;
     m_TCt.push_back(QPointF(0,m_C0));
     m_sizeTCt = 1;
}
void T_Element::associerReaction(T_Reaction *R, int Sens){ //associe une réaction �  un élément. Si l'élément est réactif, la réaction sera ajoutée en 0. Si l'élément est produit, la réaction sera ajoutée en 1

    if (Sens == 1) //l'élément est réactif, donc la réaction est écrite �  sa droite (A = B)
    {
            m_Reaction[1] = R;
    }
    if (Sens == -1) //l'élément est produit, donc la réaction est �  sa gauche (B = A)
    {
        m_Reaction[0] = R;
    }
    if ((Sens != 1) && (Sens != -1)) //ERREUR
    {
            qDebug()<<"erreur : Vous n'avez pas envoy� un argument 'sens' valide �  T_Reaction::associerReaction "<<endl;
    }
}
void T_Element::dissocierReaction(/*T_Reaction *R,*/ int Sens)//supprime l'�l�ment de la r�action
{
        if (Sens == 1) //l'�l�ment �tait un r�actif
    {
            m_Reaction[1] = NULL;
    }
    if (Sens == -1) //l'�l�ment �tait un produit
    {
        m_Reaction[0] = NULL;
    }
    if ((Sens != 1) && (Sens != -1)) //ERREUR
    {
        qDebug()<<"param�tre sens invalide dans la fonction dissocierReaction."<<endl;
    }
}
QPointF T_Element::returnCt(int i)
{
    return m_TCt.at(i);
}
double T_Element::returnCt()
{
    return m_Ct;
}
int T_Element::tailleHistorique()
{
    return m_sizeTCt;
}
void T_Element::avancer(T_Times* T, int instant)
{
//un backup se trouve dans le dossier "avant SDL"... au cas o�
    double variation=0;

    if(m_Reaction[1]!=NULL&&m_Reaction[0]!=NULL) //dans le cas d'une r�action "Centrale
    {
        variation  = ( 1)*(m_Reaction[0]->returnKab()*m_Reaction[0]->getElementA()->returnCt(instant).y() - m_Reaction[1]->returnKab()*m_TCt[instant].y()); //sens direct
        variation += (-1)*(m_Reaction[0]->returnKba()*m_TCt[instant].y() - m_Reaction[1]->returnKba()*m_Reaction[1]->getElementB()->returnCt(instant).y()); //sens indirect //les parenth�ses ont �t� rajout�es apr�s, il faut v�rifier que c'est bon.
    }

    if(m_Reaction[1]!=NULL&&m_Reaction[0]==NULL) //Dans le cas d'une r�action uniquement "� droite"
    {
        variation  = (-1)*m_Reaction[1]->returnKab()*m_TCt[instant].y();
        variation += ( 1)*m_Reaction[1]->returnKba()*m_Reaction[1]->getElementB()->returnCt(instant).y();
    }

    if(m_Reaction[0]!=NULL&&m_Reaction[1]==NULL) //Dans le cas d'une r�action uniquement "� gauche"
    {
        variation  = ( 1)*m_Reaction[0]->returnKab()*m_Reaction[0]->getElementA()->returnCt(instant).y();
        variation += (-1)*m_Reaction[0]->returnKba()*m_TCt[instant].y();
    }


    m_Ct+=variation * (T->deltaTsec());
    m_TCt.push_back(QPointF(T->courantsec()+T->deltaTsec(),m_Ct)); //le +dT est l� pour essayer de r�soudre le bug du premier point
    m_sizeTCt++;
//    std::cout<<endl<<"fin de T_Element::avancer";
}
void T_Element::preparer() // sert � remettre le tableau � l'instant 0
{
    std::vector<QPointF> temp;
    m_TCt.swap(temp);           //car m_TCt.shrink_to_fit() fait partie du standard C++11

    m_TCt.push_back(QPointF(0,m_C0));
    m_sizeTCt = 1;
    m_Ct=m_C0;
}
void T_Element::reserver(int nombre)
{
    m_TCt.reserve(nombre);
}
void T_Element::setC0(double newC0)
{
   m_C0 = newC0;
}
double T_Element::getC0()
{
    return m_C0;
}
int T_Element::verifier()
{
    if (/*m_Name[0]!='\0'           &&      //Je m'en suis rendu compte : on ne r�gle pas le nom
   */   m_C0>=0                    &&        //la concentration initiale peut parfaitement etre nulle...
        //m_Ct!=0                &&
        ((m_Reaction[0]!=NULL)||(m_Reaction[1]!=NULL))   &&
        m_TCt[0].y()>=0               &&
        m_sizeTCt!=0)
        return 0;
    return 1;
}
T_Element::~T_Element()//destructeur pour T_Element (inutilis� dans notre cas)
{
}

//M�thodes pour T_Reaction

T_Reaction::T_Reaction()
{
    m_A = NULL;
    m_B = NULL;
    m_Kab = 0;
    m_Kba = 0;
}
T_Reaction::T_Reaction(T_Element *A, T_Element *B)//constructeur surcharg� 1, utilit� � d�finir
{
        m_A = A;
        m_B = B;

        m_A->associerReaction(this, 1);
        m_B->associerReaction(this, -1);
}
void T_Reaction::initialiser(T_Element *A, T_Element *B)
{
        m_A = A;
        m_B = B;

        //il faudra bien s�r reparam�trer K d�s que celui ci change...

        m_A->associerReaction(this, 1);
        m_B->associerReaction(this, -1);
}
void T_Reaction::dissocier(T_Element *A, T_Element *B)
{
        m_A = NULL;
        m_B = NULL;

        //il faudra bien s�r reparam�trer K d�s que celui ci change...

        A->dissocierReaction(/*this,*/ 1);
        B->dissocierReaction(/*this,*/ -1);
}
T_Reaction::T_Reaction(T_Element *A, T_Element *B, double K1, double K2)//constructeur surcharg� 2, qui prend K en param�tre
{
    m_A = A;
    m_B = B;
    m_Kab = K1;
    m_Kba = K2;
    m_A->associerReaction(this, 1); //la r�action intervient chez les deux �l�ments
    m_B->associerReaction(this, -1);
}
T_Element* T_Reaction::getElementA()
{
    return m_A;
}
T_Element* T_Reaction::getElementB()
{
    return m_B;
}
double T_Reaction::returnKab(){
    return m_Kab;
}
double T_Reaction::returnKba(){
    return m_Kba;
}
int T_Reaction::verifier()
{
    if(m_A!=NULL && m_B!=NULL && ( m_Kab!=0 ||m_Kba!=0))
        return 0;
    return 1;
}
void T_Reaction::setKab(double Kab)
{
    m_Kab=Kab;
}
void T_Reaction::setKba(double Kba)
{
    m_Kba=Kba;
}
T_Reaction::~T_Reaction() //destructeur pour T_Reaction (inutilis� dans notre cas)
{
}

//M�thodes pour T_Times

T_Times::T_Times():m_Duree(0), m_DeltaT(0), m_Courant(0){}
double T_Times::dureesec()
{
    return m_Duree;
}
double T_Times::deltaTsec()
{
    return m_DeltaT;
}
double T_Times::courantsec()
{
    return m_Courant;
}
void T_Times::avancement()
{
    m_Courant += m_DeltaT;
}
bool T_Times::checkLoop()
{
    if (m_Courant >/*=*/ m_Duree) //petite modification ici pour tester. temporaire?
        return false;
    return true;
}
void T_Times::initTemps()
{
    m_Duree = 0; //il faudra bien s^ur tester si ces param�tres ont �t� chang�s avant de lancer la r�action...
    m_DeltaT = 0; //idem : sinon division par 0... C'est mal!
    m_Courant = 0;
}
int T_Times::verifier()
{
    if(m_Duree>0&&m_DeltaT>0&&m_Courant>=0&&m_Duree>=m_DeltaT)
        return 0;
    return 1;
}
void T_Times::setDuree(double Duree)
{
    m_Duree=Duree;
}
void T_Times::setDeltaT(double DeltaT)
{
    m_DeltaT=DeltaT;
}
void T_Times::initialiser()
{
    m_Courant=0;
}
T_Times::~T_Times() //destructeur pour T_Times (inutilis� dans notre cas)
{
}
