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
                T_Element();                                       //constructeur pour T_Element inutilisé (cas d'une réaction à 3 éléments)
                void initialiser();                                //initialisation d'un T_Element post-déclaration
                void associerReaction(T_Reaction *R, int Sens);    //couplage avec une T_Reaction en tant que réactif (Sens = 1) ou produit (Sens = -1)
                void dissocierReaction(/*T_Reaction *R,*/ int Sens);
                QPointF returnCt(int i);                            //retour de la concentration enregistrée à la case i du tableau historique
                double returnCt();                                 //retour de la dernière concentration enregistrée dans le tableau historique
                int tailleHistorique();                            //retourne m_sizeTCt
                void avancer(T_Times *T, int instant);             //calcul de la concentration de T_Element pour l'instant T+dt, à partir de la concentration en T
                int verifier();
                void preparer();                                    //prépare pour la nouvelle réaction
                void reserver(int nombre);                                    //réserve la place en mémoire, jette une exception si impossible
                void setC0(double newC0);                           //change la concentration initiale
                double getC0();
                ~T_Element();                                       //destructeur pour T_Element (inutilisé dans notre cas)

    private:
                char m_Name[10];                                   //contient le nom de l'élément (10 caracteres)
                double m_C0;                                       //concentration initilale
                double m_Ct;                                       //concentration courante
                double m_vC[2];                                    //vitesse de variation de concentration
                T_Reaction* m_Reaction[2];                         //pointeurs vers les réactions éventuelles de l'élément
                std::vector<QPointF> m_TCt;                         //tableau répertoriant les concentrations au cours du temps
                int m_sizeTCt;                                     //taille du tableau m_TCt
};

class T_Reaction{
        public :
                T_Reaction();                                      //constructeur pour T_Reaction inutilisé (cas d'une réaction à 3 éléments)
                T_Reaction(T_Element *A, T_Element *B);            //constructeur pour T_Reaction utilisé dont on connaît les éléments lors de la déclaration
                T_Reaction(T_Element *A, T_Element *B, double K1, double K2);  //constructeur pour T_Reaction utilisé dont on connaît les éléments lors de la déclaration et les constantes cinétiques
                void initialiser(T_Element *A, T_Element *B);      //initialisation d'un T_Element post-déclaration
                void dissocier(T_Element *A, T_Element *B);        //si on supprime D de la réaction, il faut réagir en conséquence.
                T_Element* getElementA();                          //retour du T_Element m_A de la T_Reaction
                T_Element* getElementB();
                double returnKab();                                //retour de la constante cinétique de T_Reaction (A -> B)
                double returnKba();                                //retour de la constante cinétique de T_Reaction (B -> A)
                int verifier();                                    //vérifie que la réaction est correctement paramétrée
                void setKab(double Kab);
                void setKba(double Kba);
                ~T_Reaction();

        private :
                T_Element *m_A;                                    //premier  élément de la réaction (A)
                T_Element *m_B;                                    //deuxième élément de la réaction (B)
                double m_Kab;                                      //constante cinétique de la réaction A->B
                double m_Kba;                                      //constante cinétique de la réaction B->A
};

class T_Times{
    public :
        T_Times();
        double dureesec();                                 //retour de la duree de l'expérience    en secondes
        double deltaTsec();                                //retour de l'intervalle de temps       en secondes
        double courantsec();                               //retour de la duree de l'expérience    en secondes
        void avancement();                                 //incrémentation de l'instant courant pour l'avancée réactionnelle
        bool checkLoop();                                  //véification de l'instant courant par rapport à la durée de l'expérience
        void initTemps();                                  //initialisation des paramètres temporels par l'utilisateur
        int verifier();                                    //vérifie que les paramètres temporels de la réaction ne sont pas fantaisistes
        void initialiser();                                //met l'instant actuel à 0 en prévision d'une nouvelle réaction
        void setDuree(double DeltaT);                      //modifie la duréé de l'expérience
        void setDeltaT(double DeltaT);                     //modifie l'intervalle de temps utilisé pour l'intervalle
        ~T_Times();

    private :
        double m_Duree;                                    //durée totale de l'expérience
        double m_DeltaT;                                   //intervalle de temps séparant deux mesures
        double m_Courant;                                  //instant courant
};

class T_Simulation : public QObject  //il vaut mieux le mettre en bas car cette classe se base sur les autres
{
Q_OBJECT
        public:
            T_Simulation();
            void creerMessageErreur(QWidget* parent=0);     //permet de définir le parent du message que l'on créée
            void go();
            void amnesie();                                 //On repart de 0 (ou presque) : avant la simulation
            void checkboxTriggered(int arg1);
            void setRapport1(int position);                 //position est la position du slider; varie de -1000 à +1000
            void setRapport2(int position);
            void CalculRapport1();
            void CalculRapport2();
            void testPourSliders();                         //vérifie si il est possible d'activer les sliders
            void outOfMemHandler();                         //décharge la mémoire vive au besoin.

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
            double Xmax, Ymax;              //servira pour le tracé.
            T_Element element[4];           //on prépare 4 lments A, B, C, D...
            int nombreElements;
            T_Reaction RAB, RBC, RCD;       //de même on prépare 3 réactions...
            bool debugCalcul;
            double Rapport1, Rapport2;      //K2/K1 et K3/K2
            T_Times *Temps;
            QErrorMessage *MessageErreur;
            QWidget* m_parent;


};
#endif
