#ifndef DEF_CLASSES
#define DEF_CLASSES

#include <vector> //required for the history
#include <math.h> //required for the logarithmic sliders with pow() and log()
#include <QPoint>
#include <QErrorMessage>
#include <QMessageBox>
#include <QDebug>


class T_Element;
class T_Reaction;
class T_Times;
class T_Simulation;


class T_Element{
        public:
                T_Element();
                void initialize();                                //initialisation d'un T_Element post-dclaration
                void associateReaction(T_Reaction *R, int Sens);    //couplage avec une T_Reaction en tant que ractif (Sens = 1) ou produit (Sens = -1)
                void dissociateReaction(/*T_Reaction *R,*/ int Sens);
                QPointF returnCt(int i);                            //retour de la concentration enregistre  la case i du tableau historique
                double returnCt();                                 //retour de la dernire concentration enregistre dans le tableau historique
                int getHistorySize();                            //retourne m_sizeTCt
                void step(T_Times *T, int instant);             //calcul de la concentration de T_Element pour l'instant T+dt,  partir de la concentration en T
                int check();
                void prepare();                                    //prpare pour la nouvelle raction
                void reserve(int nombre);                                    //rserve la place en mmoire, jette une exception si impossible
                void setC0(double newC0);                           //change la concentration initiale
                double getC0();
                ~T_Element();                                       //destructeur pour T_Element (inutilis dans notre cas)

    private:
                char m_Name[10];                                   //element name (10 char) [unused atm]
                double m_C0;                                       //initilal concentration
                double m_Ct;                                       //actual concentration
                double m_vC[2];                                    //concentration rate of change
                T_Reaction* m_Reaction[2];                         //pointers to the reactions, if relevant
                std::vector<QPointF> m_TCt;                         //history of concentrations
                int m_sizeTCt;                                     //size of this history
};

class T_Reaction{
        public :
                T_Reaction();                                      //unused
                T_Reaction(T_Element *A, T_Element *B);            //used : we already know the elements
                T_Reaction(T_Element *A, T_Element *B, double K1, double K2);  //used : we know the elements and the constants
                void initialize(T_Element *A, T_Element *B);      //initialization of the reaction
                void dissociate(T_Element *A, T_Element *B);        //used if we delete D from the reaction
                T_Element* getElementA();
                T_Element* getElementB();
                double returnKab();                                //returns T_Reaction kinetic constant (A -> B)
                double returnKba();                                //returns T_Reaction kinetic constant (B -> A)
                int check();                                    //check that the parameters are good
                void setKab(double Kab);
                void setKba(double Kba);
                ~T_Reaction();

        private :
                T_Element *m_A;                                    //first element in the reaction (A)
                T_Element *m_B;                                    //second element (B)
                double m_Kab;                                      //kinetic constant A->B
                double m_Kba;                                      //kinetic constant B->A
};

class T_Times{
    public :
        T_Times();
        double dureesec();                                //experience duration (seconds)
        double deltaTsec();                               //time interval used for calculations (seconds)
        double actualsec();                               //time actually elapsed since the begining of the experience
        void step();                                      //increments actual time by deltat to actually advance the reaction
        bool checkLoop();                                 //checks that we still have time until experience completion
        void initTime();                                  //initialization of the temporal parameters
        int check();                                      //checks that the parameters are okay
        void initialize();                                //actual=0 for a new calculation
        void setDuree(double DeltaT);                     //changes experience duration
        void setDeltaT(double DeltaT);                    //changes interval used for calculations
        ~T_Times();

    private :
        double m_Duration;                                //Total experience duration
        double m_DeltaT;                                  //time interval between two measures
        double m_Actual;                                  //current time
};

class T_Simulation : public QObject
{
Q_OBJECT
        public:
            T_Simulation();
            void createErrorMessage(QWidget* parent=0);     //allows us to define its parent
            void go();
            void amnesia();                                 //Let's start again from scratch the simulation!
            void checkboxTriggered(int arg1);
            void setRatio1(int position);                   //position is the slider's; between -1000 and +1000
            void setRatio2(int position);
            void CalculateRatio1();
            void CalculateRatio2();
            void testForSliders();                         //check if it is possible to enable the sliders
            void outOfMemHandler();                        //unload the history from RAM if needed

            ~T_Simulation();

        public:
            T_Element* getElements()    {return element;}
            void setTemps(T_Times* T)   {m_Time=T;}
            void setDebug(bool arg)     {debugCalcul=arg;}

            double getXmax(){return Xmax;}
            double getYmax(){return Ymax;}

            void setCa(double arg1){element[0].setC0(arg1);}
            void setCb(double arg1){element[1].setC0(arg1);}
            void setCc(double arg1){element[2].setC0(arg1);}
            void setCd(double arg1){element[3].setC0(arg1);}

            void setKab(double arg1){RAB.setKab(arg1);CalculateRatio1();testForSliders();}
            void setKba(double arg1){RAB.setKba(arg1);}
            void setKbc(double arg1){RBC.setKab(arg1);CalculateRatio1();CalculateRatio2();testForSliders();}
            void setKcb(double arg1){RBC.setKba(arg1);}
            void setKcd(double arg1){RCD.setKab(arg1);CalculateRatio2();testForSliders();}
            void setKdc(double arg1){RCD.setKba(arg1);}

            double getCa(){return element[0].getC0();}
            double getCb(){return element[1].getC0();}
            double getCc(){return element[2].getC0();}
            double getCd(){return element[3].getC0();}

            double getKab(){return RAB.returnKab();}
            double getKba(){return RAB.returnKba();}
            double getKbc(){return RBC.returnKab();}
            double getKcb(){return RBC.returnKba();}
            double getKcd(){return RCD.returnKab();}
            double getKdc(){return RCD.returnKba();}

        signals:
            void CalculationProgress(int);
            void MessageStatus(QString);
            void newRatio1(int);
            void newRatio2(int);
            void DisableSlider1(bool);
            void DisableSlider2(bool);
            void newK1(double);
            void newK3(double);

        private:
            double Xmax, Ymax;              //Useful for the drawing
            T_Element element[4];           //We have 4 elements A, B, C, D...
            int elementsNumber;
            T_Reaction RAB, RBC, RCD;       //... and three reactions
            bool debugCalcul;
            double Ratio1, Rapport2;      //K2/K1 and K3/K2
            T_Times *m_Time;
            QErrorMessage *m_errorMessage;
            QWidget* m_parent;


};
#endif
