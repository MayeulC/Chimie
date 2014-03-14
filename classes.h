#ifndef DEF_CLASSES
#define DEF_CLASSES

#include <vector> //requis pour le tableau dynamique
#include <math.h> //requis pour les sliders logarithmiques avec pow() et log()
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
                T_Element();                                       //constructeur pour T_Element inutilis� (cas d'une r�action � 3 �l�ments)
                void initialiser();                                //initialisation d'un T_Element post-d�claration
                void associerReaction(T_Reaction *R, int Sens);    //couplage avec une T_Reaction en tant que r�actif (Sens = 1) ou produit (Sens = -1)
                void dissocierReaction(/*T_Reaction *R,*/ int Sens);
                QPointF returnCt(int i);                            //retour de la concentration enregistr�e � la case i du tableau historique
                double returnCt();                                 //retour de la derni�re concentration enregistr�e dans le tableau historique
                int tailleHistorique();                            //retourne m_sizeTCt
                void avancer(T_Times *T, int instant);             //calcul de la concentration de T_Element pour l'instant T+dt, � partir de la concentration en T
                int verifier();
                void preparer();                                    //pr�pare pour la nouvelle r�action
                void reserver(int nombre);                                    //r�serve la place en m�moire, jette une exception si impossible
                void setC0(double newC0);                           //change la concentration initiale
                double getC0();
                ~T_Element();                                       //destructeur pour T_Element (inutilis� dans notre cas)

    private:
                char m_Name[10];                                   //contient le nom de l'�l�ment (10 caracteres)
                double m_C0;                                       //concentration initilale
                double m_Ct;                                       //concentration courante
                double m_vC[2];                                    //vitesse de variation de concentration
                T_Reaction* m_Reaction[2];                         //pointeurs vers les r�actions �ventuelles de l'�l�ment
                std::vector<QPointF> m_TCt;                         //tableau r�pertoriant les concentrations au cours du temps
                int m_sizeTCt;                                     //taille du tableau m_TCt
};

class T_Reaction{
        public :
                T_Reaction();                                      //constructeur pour T_Reaction inutilis� (cas d'une r�action � 3 �l�ments)
                T_Reaction(T_Element *A, T_Element *B);            //constructeur pour T_Reaction utilis� dont on conna�t les �l�ments lors de la d�claration
                T_Reaction(T_Element *A, T_Element *B, double K1, double K2);  //constructeur pour T_Reaction utilis� dont on conna�t les �l�ments lors de la d�claration et les constantes cin�tiques
                void initialiser(T_Element *A, T_Element *B);      //initialisation d'un T_Element post-d�claration
                void dissocier(T_Element *A, T_Element *B);        //si on supprime D de la r�action, il faut r�agir en cons�quence.
                T_Element* getElementA();                          //retour du T_Element m_A de la T_Reaction
                T_Element* getElementB();
                double returnKab();                                //retour de la constante cin�tique de T_Reaction (A -> B)
                double returnKba();                                //retour de la constante cin�tique de T_Reaction (B -> A)
                int verifier();                                    //v�rifie que la r�action est correctement param�tr�e
                void setKab(double Kab);
                void setKba(double Kba);
                ~T_Reaction();

        private :
                T_Element *m_A;                                    //premier  �l�ment de la r�action (A)
                T_Element *m_B;                                    //deuxi�me �l�ment de la r�action (B)
                double m_Kab;                                      //constante cin�tique de la r�action A->B
                double m_Kba;                                      //constante cin�tique de la r�action B->A
};

class T_Times{
    public :
        T_Times();
        double dureesec();                                 //retour de la duree de l'exp�rience    en secondes
        double deltaTsec();                                //retour de l'intervalle de temps       en secondes
        double courantsec();                               //retour de la duree de l'exp�rience    en secondes
        void avancement();                                 //incr�mentation de l'instant courant pour l'avanc�e r�actionnelle
        bool checkLoop();                                  //v�ification de l'instant courant par rapport � la dur�e de l'exp�rience
        void initTemps();                                  //initialisation des param�tres temporels par l'utilisateur
        int verifier();                                    //v�rifie que les param�tres temporels de la r�action ne sont pas fantaisistes
        void initialiser();                                //met l'instant actuel � 0 en pr�vision d'une nouvelle r�action
        void setDuree(double DeltaT);                      //modifie la dur�� de l'exp�rience
        void setDeltaT(double DeltaT);                     //modifie l'intervalle de temps utilis� pour l'intervalle
        ~T_Times();

    private :
        double m_Duree;                                    //dur�e totale de l'exp�rience
        double m_DeltaT;                                   //intervalle de temps s�parant deux mesures
        double m_Courant;                                  //instant courant
};

class T_Simulation : public QObject  //il vaut mieux le mettre en bas car cette classe se base sur les autres
{
Q_OBJECT
        public:
            T_Simulation();
            void creerMessageErreur(QWidget* parent=0);     //permet de d�finir le parent du message que l'on cr��e
            void go();
            void amnesie();                                 //On repart de 0 (ou presque) : avant la simulation
            void checkboxTriggered(int arg1);
            void setRapport1(int position);                 //position est la position du slider; varie de -1000 � +1000
            void setRapport2(int position);
            void CalculRapport1();
            void CalculRapport2();
            void testPourSliders();                         //v�rifie si il est possible d'activer les sliders
            void outOfMemHandler();                         //d�charge la m�moire vive au besoin.

            ~T_Simulation();

        public:
            T_Element* getElements()    {return element;}
            void setTemps(T_Times* T)   {Temps=T;}
            void setDebug(bool arg)     {debugCalcul=arg;}

            double getXmax(){return Xmax;}
            double getYmax(){return Ymax;}

            void setCa(double arg1){element[0].setC0(arg1);}
            void setCb(double arg1){element[1].setC0(arg1);}
            void setCc(double arg1){element[2].setC0(arg1);}
            void setCd(double arg1){element[3].setC0(arg1);}

            void setKab(double arg1){RAB.setKab(arg1);CalculRapport1();testPourSliders();}
            void setKba(double arg1){RAB.setKba(arg1);}
            void setKbc(double arg1){RBC.setKab(arg1);CalculRapport1();CalculRapport2();testPourSliders();}
            void setKcb(double arg1){RBC.setKba(arg1);}
            void setKcd(double arg1){RCD.setKab(arg1);CalculRapport2();testPourSliders();}
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
            void ProgressionCalcul(int);
            void MessageStatut(QString);
            void nouveauRapport1(int);
            void nouveauRapport2(int);
            void DisableSlider1(bool);
            void DisableSlider2(bool);
            void nouveauK1(double);
            void nouveauK3(double);

        private:
            double Xmax, Ymax;              //servira pour le trac�.
            T_Element element[4];           //on pr�pare 4 lments A, B, C, D...
            int nombreElements;
            T_Reaction RAB, RBC, RCD;       //de m�me on pr�pare 3 r�actions...
            bool debugCalcul;
            double Rapport1, Rapport2;      //K2/K1 et K3/K2
            T_Times *Temps;
            QErrorMessage *MessageErreur;
            QWidget* m_parent;


};
#endif
