#include "classes.h"

//T_Simulation Methodes

T_Simulation::T_Simulation()
{
    elementsNumber = 3; //default, is changed at will by the signal
    RAB.initialize(&element[0], &element[1]); //associate reaction and element
    RBC.initialize(&element[1], &element[2]);
    Xmax=0;
    Ymax=0;
    debugCalcul=false;
}
void T_Simulation::createErrorMessage(QWidget *parent)
{
    m_parent=parent; //useful for the out of mem error message
    m_errorMessage=new QErrorMessage(parent);
}
void T_Simulation::go()
{
    //check prerequesites, and display an error message if needed
    // (test k!=0; t !=0; dt != 0 )
    QString ErrorMessage;

    if ((element[0].check()||element[1].check()||element[2].check()||((elementsNumber-3)&&element[3].check())))
    {
        ErrorMessage += tr("<br> - Les éléments de la réaction");
    }

    if(RAB.check()||RBC.check()||((elementsNumber-3)&&RCD.check()))
    {
        ErrorMessage += tr("<br> - Les paramètres de raction");
    }

    if(m_Time->check())
    {
        ErrorMessage += tr("<br> - Les paramètres temporels");
    }
    if(ErrorMessage.size()!=0)
    {
        emit MessageStatus(tr("Une erreur est survenue! Vérifiez les paramètres."));
        ErrorMessage= tr("Veuillez vérifier : ") + ErrorMessage;
        m_errorMessage->showMessage(ErrorMessage);
        return;
    }
    element[0].prepare();
    element[1].prepare();
    element[2].prepare();
    element[3].prepare(); //allows to display a curve if element is disabled

    m_Time->initialize();

    emit CalculationProgress(0);
    emit MessageStatus("Calcul en cours...");

    Ymax=0;
    /**/
    if(debugCalcul)
    {
        qDebug()<< "**********Valeurs initiales************\n";
        qDebug()<< element[0].returnCt(0)<<" \t"<<element[1].returnCt(0)<<"\t"<<element[2].returnCt(0);
        qDebug()<< "\n/***********************************\\\n|Début de l'arrive des résultats : |\n\\***********************************/\n";
        qDebug()<< "Durée : "<<m_Time->dureesec()<< " delta : "<<m_Time->deltaTsec()<<" courant : "<<m_Time->actualsec()<<"\n";
    }
    /**/

    try
    {
        while (m_Time->checkLoop()) //temporal check
        {
            Ymax=qMax(Ymax,qMax(qMax(element[0].returnCt(),element[1].returnCt()),qMax(element[2].returnCt(),element[3].returnCt())));
            /**/
            if(debugCalcul)
                qDebug() <<m_Time->actualsec()<< " |\t"<< element[0].returnCt() << '\t' << element[1].returnCt() << '\t' << element[2].returnCt() << '\n';
            /**/
            element[0].step(m_Time, int(m_Time->actualsec()/m_Time->deltaTsec())); //concentration variation of A
            element[1].step(m_Time, int(m_Time->actualsec()/m_Time->deltaTsec())); //concentration variation of B
            element[2].step(m_Time, int(m_Time->actualsec()/m_Time->deltaTsec())); //concentration variation of C
            if (elementsNumber == 4)
                element[3].step(m_Time, int(m_Time->actualsec()/m_Time->deltaTsec())); //concentration variation of D (if needed)


            m_Time->step(); //one step forward time-wise
            emit CalculationProgress(int(100*(m_Time->actualsec()/m_Time->dureesec())));
        }

        Ymax=qMax(Ymax,qMax(qMax(element[0].returnCt(),element[1].returnCt()),qMax(element[2].returnCt(),element[3].returnCt())));
        /**/
        if(debugCalcul)
            qDebug() <<m_Time->actualsec()<< " |\t"<< element[0].returnCt() << '\t' << element[1].returnCt() << '\t' << element[2].returnCt() << '\n';
        /**/
        emit MessageStatus(tr("Calcul terminé"));
        emit CalculationProgress(100);
        Xmax=m_Time->actualsec()-m_Time->deltaTsec(); //we did one more delta(t) to get out of the loop

    }
    catch(std::exception e)
    {
        outOfMemHandler();
    }


}
void T_Simulation::amnesia(){
    element[0].prepare();
    element[1].prepare();
    element[2].prepare();
    element[3].prepare();
    Xmax=0;
    Ymax=0;
}
void T_Simulation::checkboxTriggered(int arg1){
    elementsNumber=3+!!arg1; // arg is either 0 or 2, so !! is 0 or 1

    if(elementsNumber==4)
        RCD.initialize(&element[2],&element[3]);  //associate C and D in RCD
    if(elementsNumber==3)
        RCD.dissociate(&element[2],&element[3]);//delete D in the different pointers

    testForSliders(); //checks if it enables or disables a slider
}
void T_Simulation::setRatio1(int position)
{
    double temp=position;
    temp=temp/300;
    Ratio1=pow(10,(temp)); //K2/K1
    double new_K1=RBC.returnKab()/Ratio1;
    RAB.setKab(new_K1);

    emit newK1(new_K1);
}
void T_Simulation::setRatio2(int position)
{
    double temp=position;
    temp=temp/300; //300 here is completely arbitrary
    Rapport2=pow(10,(temp)); //K3/K2
    double new_K3=RBC.returnKab()*Rapport2;
    RCD.setKab(new_K3);
    emit newK3(new_K3);
}
void T_Simulation::CalculateRatio1()
{
    if(RAB.returnKab()!=0)//K2/K1 <=> RBC/RAB
    {
        Ratio1=RBC.returnKab()/RAB.returnKab();
        emit newRatio1(log(Ratio1)*300);
    }
    if(RBC.returnKab()==0&&RAB.returnKab()==0)
    {
        emit newRatio1(0);
    }
    else
    {
        if(RBC.returnKab()==0)
            emit newRatio1(-1000);
        if(RAB.returnKab()==0)
            emit newRatio1(1000);
    }
}
void T_Simulation::CalculateRatio2()
{
        if(RBC.returnKab()!=0) //K3/K2 <=> RCD/RBC
        {
            Ratio1=RCD.returnKab()/RBC.returnKab();
            emit newRatio2(log(Ratio1)*300);
        }
        if(RBC.returnKab()==0&&RCD.returnKab()==0)
        {
            emit newRatio2(0);
        }
        else
        {
            if(RBC.returnKab()==0)
                emit newRatio2(1000);
            if(RCD.returnKab()==0)
                emit newRatio2(-1000);
        }
    //}
}
void T_Simulation::testForSliders() //tests which slider(s) can be enabled/disabled
{
    if(RBC.returnKab()!=0)
    {
        emit DisableSlider1(false);
    }
    else
    {
        emit DisableSlider1(true);
    }
    //the same for the second slider :
    if(RBC.returnKab()!=0&&elementsNumber==4) //if k2 is valid and there's 4 elements
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
    element[0].prepare();
    element[1].prepare();
    element[2].prepare();
    element[3].prepare();
    emit MessageStatus(tr("Erreur critique : Mémoire insuffisante!"));
    QMessageBox::warning(m_parent,tr("Erreur"),tr("Mémoire vive insuffisante pour continuer.\nVérifiez les paramètres temporels."));
    std::set_new_handler(NULL);
}
T_Simulation::~T_Simulation()
{

}


//T_Element Methodes

T_Element::T_Element()
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
void T_Element::associateReaction(T_Reaction *R, int Direction){ //associate a reaction to an element. If the element is consumed, the reaction will be added at 0. If consumed, at 1.

    if (Direction == 1) //the element is consumed, so the reaction is on the right (A = B)
    {
            m_Reaction[1] = R;
    }
    if (Direction == -1) //the element is produced, so the reaction is on the left (B = A)
    {
        m_Reaction[0] = R;
    }
    if ((Direction != 1) && (Direction != -1)) //ERROR
    {
            qDebug()<<"erreur : Vous n'avez pas envoy un argument 'sens' valide   T_Reaction::associerReaction "<<endl;
    }
}
void T_Element::dissociateReaction(int Direction)//delete the element from the reaction
{
        if (Direction == 1) //the element was consumed
    {
            m_Reaction[1] = NULL;
    }
    if (Direction == -1) //the element was produced
    {
        m_Reaction[0] = NULL;
    }
    if ((Direction != 1) && (Direction != -1)) //ERROR
    {
        qDebug()<<"paramètre sens invalide dans la fonction dissocierReaction."<<endl;
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
int T_Element::getHistorySize()
{
    return m_sizeTCt;
}
void T_Element::step(T_Times* T, int instant)
{
    double variation=0;

    if(m_Reaction[1]!=NULL&&m_Reaction[0]!=NULL) //"middle" reaction
    {
        variation  = ( 1)*(m_Reaction[0]->returnKab()*m_Reaction[0]->getElementA()->returnCt(instant).y() - m_Reaction[1]->returnKab()*m_TCt[instant].y()); //sens direct
        variation += (-1)*(m_Reaction[0]->returnKba()*m_TCt[instant].y() - m_Reaction[1]->returnKba()*m_Reaction[1]->getElementB()->returnCt(instant).y()); //sens indirect //les parenthses ont t rajoutes aprs, il faut vrifier que c'est bon.
    }

    if(m_Reaction[1]!=NULL&&m_Reaction[0]==NULL) //"right" reaction
    {
        variation  = (-1)*m_Reaction[1]->returnKab()*m_TCt[instant].y();
        variation += ( 1)*m_Reaction[1]->returnKba()*m_Reaction[1]->getElementB()->returnCt(instant).y();
    }

    if(m_Reaction[0]!=NULL&&m_Reaction[1]==NULL) //"left" reaction
    {
        variation  = ( 1)*m_Reaction[0]->returnKab()*m_Reaction[0]->getElementA()->returnCt(instant).y();
        variation += (-1)*m_Reaction[0]->returnKba()*m_TCt[instant].y();
    }


    m_Ct+=variation * (T->deltaTsec());
    m_TCt.push_back(QPointF(T->actualsec()+T->deltaTsec(),m_Ct)); //+dT to solve a bug for the first point
    m_sizeTCt++;
}
void T_Element::prepare() // reset to time=0
{
    std::vector<QPointF> temp;
    m_TCt.swap(temp);           //because m_TCt.shrink_to_fit() is C++11 only

    m_TCt.push_back(QPointF(0,m_C0));
    m_sizeTCt = 1;
    m_Ct=m_C0;
}
void T_Element::reserve(int nombre)
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
int T_Element::check()
{
    if (m_C0>=0                    &&
        ((m_Reaction[0]!=NULL)||(m_Reaction[1]!=NULL))   &&
        m_TCt[0].y()>=0               &&
        m_sizeTCt!=0)
        return 0;
    return 1;
}
T_Element::~T_Element()
{
}

//T_Reaction Methodes

T_Reaction::T_Reaction()
{
    m_A = NULL;
    m_B = NULL;
    m_Kab = 0;
    m_Kba = 0;
}
T_Reaction::T_Reaction(T_Element *A, T_Element *B)
{
        m_A = A;
        m_B = B;

        m_A->associateReaction(this, 1);
        m_B->associateReaction(this, -1);
}
void T_Reaction::initialize(T_Element *A, T_Element *B)
{
        m_A = A;
        m_B = B;

        //don't forget to set K as soon as it changes...

        m_A->associateReaction(this,  1);
        m_B->associateReaction(this, -1);
}
void T_Reaction::dissociate(T_Element *A, T_Element *B)
{
        m_A = NULL;
        m_B = NULL;

        //the same...

        A->dissociateReaction( 1);
        B->dissociateReaction(-1);
}
T_Reaction::T_Reaction(T_Element *A, T_Element *B, double K1, double K2)
{
    m_A = A;
    m_B = B;
    m_Kab = K1;
    m_Kba = K2;
    m_A->associateReaction(this, 1); //the reaction operates on the two elements
    m_B->associateReaction(this, -1);
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
int T_Reaction::check()
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
T_Reaction::~T_Reaction()
{
}

//T_Times Methodes

T_Times::T_Times():m_Duration(0), m_DeltaT(0), m_Actual(0){}
double T_Times::dureesec()
{
    return m_Duration;
}
double T_Times::deltaTsec()
{
    return m_DeltaT;
}
double T_Times::actualsec()
{
    return m_Actual;
}
void T_Times::step()
{
    m_Actual += m_DeltaT;
}
bool T_Times::checkLoop()
{
    if (m_Actual > m_Duration)
        return false;
    return true;
}
void T_Times::initTime()
{
    m_Duration = 0; //of course, check that these parameters have changed before launching the simulation...
    m_DeltaT = 0; //idem : otherwise we divide by 0... It's bad!
    m_Actual = 0;
}
int T_Times::check()
{
    if(m_Duration>0&&m_DeltaT>0&&m_Actual>=0&&m_Duration>=m_DeltaT)
        return 0;
    return 1;
}
void T_Times::setDuree(double Duration)
{
    m_Duration=Duration;
}
void T_Times::setDeltaT(double DeltaT)
{
    m_DeltaT=DeltaT;
}
void T_Times::initialize()
{
    m_Actual=0;
}
T_Times::~T_Times()
{
}
