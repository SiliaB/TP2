//
// Created by Antoine Hintzy on 16/03/2020.
//

#include "Graphe.h"
#include <fstream>
#include <queue>
#include <stack>

Graphe::Graphe(std::string cheminFichierGraphe) {
    std::ifstream ifs{cheminFichierGraphe};
    if(!ifs) {
        throw std::runtime_error("Impossible d'ouvrir " + cheminFichierGraphe);
    }
    ifs >> m_estOriente;
    if(ifs.fail()) {
        throw std::runtime_error("Problème de lecture de l'orientation du graphe.");
    }
    int ordre;
    ifs >> ordre;
    if(ifs.fail()) {
        throw std::runtime_error("Problème de lecture de l'ordre du graphe.");
    }
    int taille;
    ifs >> taille;
    if(ifs.fail()) {
        throw std::runtime_error("Problème de lecture de la taille du graphe.");
    }
    for(int i=0; i<ordre; ++i) {
        m_sommets.push_back(new Sommet(i));
    }
    int num1, num2;
    for(int i=0; i<taille; ++i) {
        ifs >> num1 >> num2;
        if(ifs.fail()) {
            throw std::runtime_error("Problème de lecture d'un.e arc/arête.");
        }
        m_sommets[num1]->addSuccesseur(m_sommets[num2]);
        if(!m_estOriente && num1 < num2) {
            m_sommets[num2]->addSuccesseur(m_sommets[num1]);


        }
    }
}

Graphe::~Graphe() {
    for(auto addrSommet : m_sommets) {
        delete addrSommet;
    }
}

void Graphe::afficher() const {
    std::cout << std::endl << "Graphe ";
    std::cout << (m_estOriente ? "orienté" : "non orienté") << std::endl;
    std::cout << "Ordre : " << m_sommets.size() << std::endl;
    std::cout << "Liste d'adjacences : " << std::endl;
    for(auto addrSommet : m_sommets) {
        addrSommet->afficher();
        std::cout << std::endl;
    }

}

// Algo parcoursBFS(s0)
std::vector<int> Graphe::BFS(int numero_s0) const {
    // 1. Tous les sommets sont blancs (non découverts)
    std::vector<int> couleurs((int) m_sommets.size(), 0);
    // 2. Créer une file vide
    std::queue<const Sommet*> file;
    std::vector<int> predecesseurs((int) m_sommets.size(), -1);
    // 3. Enfiler s0 ; s0 devient gris
    file.push(m_sommets[numero_s0]);
    couleurs[numero_s0] = 1; // 0 = blanc, 1 = gris, 2 = noir
    const Sommet* s;
    // 4. Tant que la file n’est pas vide :
    while(!file.empty()) {
        // 4.1 Défiler le prochain sommet s de la file (Traitement éventuel sur s)
        s = file.front();
        file.pop();
        // 4.2 Pour chaque successeur s’ blanc (non découvert) de s :
        for(auto succ : s->getSuccesseurs()) {
            if(couleurs[succ->getNumero()] == 0) { // blanc
                // 4.2.1 Enfiler s’; s’ devient gris
                file.push(succ);
                couleurs[succ->getNumero()] = 1; // gris
                // 4.2.2 Noter que s est le prédécesseur de s’
                predecesseurs[succ->getNumero()] = s->getNumero();
            }
        }
        // 4.3 s devient noir (exploré)
        couleurs[s->getNumero()] = 2;
    }
    return predecesseurs;
}

std::vector<int> Graphe::DFS(int numero_s0) const {
    // 1. Tous les sommets sont blancs (non découverts)
    std::vector<int> couleurs((int) m_sommets.size(), 0);
    // 2. Créer une file vide
    std::stack<const Sommet*> stack;
    std::vector<int> predecesseurs((int) m_sommets.size(), -1);
    // 3. Enfiler s0 ; s0 devient gris
    stack.push(m_sommets[numero_s0]);
    couleurs[numero_s0] = 1; // 0 = blanc, 1 = gris, 2 = noir
    const Sommet* s;
    // 4. Tant que la file n’est pas vide :
    while(!stack.empty()) {
        // 4.1 Défiler le prochain sommet s de la file (Traitement éventuel sur s)
        s = stack.top();
        stack.pop();
        // 4.2 Pour chaque successeur s’ blanc (non découvert) de s :
        for(auto succ : s->getSuccesseurs()) {
            if(couleurs[succ->getNumero()] == 0) { // blanc
                // 4.2.1 Enfiler s’; s’ devient gris
                stack.push(succ);
                couleurs[succ->getNumero()] = 1; // gris
                // 4.2.2 Noter que s est le prédécesseur de s’
                predecesseurs[succ->getNumero()] = s->getNumero();
            }
        }
        // 4.3 s devient noir (exploré)
        couleurs[s->getNumero()] = 2;
    }
    return predecesseurs;
}


void Graphe::ComposanteConnexe() const {
    int i=-1;
    int j=0;
    // 1. Tous les sommets sont blancs (non découverts)
    std::vector<int> couleurs((int) m_sommets.size(), 0);
    // 2. Créer une file vide
    std::stack<const Sommet*> stack;
    // 3. Enfiler s0 ; s0 devient gris
    for (size_t sommet =0;sommet<m_sommets.size();sommet++) {

        if (couleurs[sommet] == 0){
            ++i;

            stack.push(m_sommets[sommet]);
            couleurs[sommet] = 1; // 0 = blanc, 1 = gris, 2 = noir
            const Sommet* s;
            // 4. Tant que la file n’est pas vide :
            while(!stack.empty()) {
                // 4.1 Défiler le prochain sommet s de la file (Traitement éventuel sur s)
                s = stack.top();
                stack.pop();
                // 4.2 Pour chaque successeur s’ blanc (non découvert) de s :
                for (auto succ : s->getSuccesseurs()) {
                    if (couleurs[succ->getNumero()] == 0) { // blanc
                        // 4.2.1 Enfiler s’; s’ devient gris
                        stack.push(succ);
                        couleurs[succ->getNumero()] = 1; // gris
                        // 4.2.2 Noter que s est le prédécesseur de s’
                        if ((i==0)&&(j==0)){
                            std::cout<<"Composante connexe 1 : "<<sommet<<" ";
                            j=1;
                        }
                        if (i==0){
                            std::cout<<succ->getNumero()<<" ";
                        }
                        if ((i==1)&&(j==1)){
                            std::cout<<std::endl;
                            std::cout<<"Composante connexe 2 : "<<sommet<<" ";
                            j=0;
                        }
                        if (i==1){
                            std::cout<<succ->getNumero()<<" ";
                        }


                    }
                }
                // 4.3 s devient noir (exploré)
                couleurs[s->getNumero()] = 2;
            }
        }


    }


}


/*void Graphe::Cycle() const {
    int i=-1;
    int j=0;
    int k=0;
    // 1. Tous les sommets sont blancs (non découverts)
    std::vector<int> couleurs((int) m_sommets.size(), 0);
    // 2. Créer une file vide
    std::stack<const Sommet*> stack;
    // 3. Enfiler s0 ; s0 devient gris
    for (size_t sommet =0;sommet<m_sommets.size();sommet++) {

        if (couleurs[sommet] == 0){
            ++i;

            stack.push(m_sommets[sommet]);
            couleurs[sommet] = 1; // 0 = blanc, 1 = gris, 2 = noir
            const Sommet* s;
            // 4. Tant que la file n’est pas vide :
            while(!stack.empty()) {
                // 4.1 Défiler le prochain sommet s de la file (Traitement éventuel sur s)
                s = stack.top();
                stack.pop();
                // 4.2 Pour chaque successeur s’ blanc (non découvert) de s :
                for (auto succ : s->getSuccesseurs()) {
                    if (couleurs[succ->getNumero()] == 0) { // blanc
                        // 4.2.1 Enfiler s’; s’ devient gris
                        stack.push(succ);
                        couleurs[succ->getNumero()] = 1; // gris
                        // 4.2.2 Noter que s est le prédécesseur de s’
                        if ((i==0)&&(j==0)){
                            std::cout<<"Cycle : ";
                            j=1;
                        }
                        if ((i==0)&&(succ->getSuccesseurs()%2==1)){
                            k++;
                            //On verifie dans une composante connexe si l'ordre
                            // de 2 sommet ou plus sont impairs, si ils le sont
                            //alors on a pas de cycle
                            if (k==2){
                                std::cout<<"Il n'y a pas de cycle";
                            }
                            else
                            {
                                std::cout<<succ->getNumero()<<" ";
                            }
                        }


                    }
                }
                // 4.3 s devient noir (exploré)
                couleurs[s->getNumero()] = 2;
            }
        }


    }


}
*/



