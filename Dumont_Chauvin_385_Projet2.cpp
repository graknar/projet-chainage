/*
*
* Projet 2 chaînage et maillon
* vecteur de coordonné
* Fait par Chauvin Leeloo et Dumont Thomas
*
*/
#include <iostream>
#include <string>
#include <sstream>
#include <cstddef>
using namespace std;

//définition des structures de donnée comme ils nous ont été défini.
struct t_coord {
double valeur;
int indice; // 1 a D
t_coord * suiv; // coordonnee suivante dans le chainage
};

struct t_vecteur{
int dimension; // positive
double defaut; // valeur par défaut
t_coord * tete; //chainage des coordonnees significative, ordonnees par indice croissant
};

//procédure supplémentaire supprimer qui vise a supprimer l'intégralité d'un vecteur en vu de son initialisation
void supprimer (t_vecteur & vec) {
	t_vecteur temp; // vecteur temporaire pour le parcours du vecteur principale
	if (vec.tete->suiv != nullptr){
        temp = vec;
        temp.tete = temp.tete->suiv;
	    supprimer(temp); // appel récursif
	}
	delete vec.tete->suiv; //destruction du maillon suivant 
	vec.tete = nullptr; // passage de la tête à nul
}

//procédure pour remettre toute les valeurs d'un vecteur comme valeur par défaut défini lors de l'initialisation du vecteur
void vider (t_vecteur vec) {
	t_vecteur temp; // vecteur temporaire pour le parcours du vecteur principale
    if (vec.tete->suiv != nullptr){
        temp = vec;
        temp.tete = temp.tete->suiv;
	    vider(temp); // appel récursif
	}
	vec.tete->valeur = vec.defaut; // remise à la valeur par défaut du vecteur 
}

//procédure de modification du vecteur
void modifier (t_vecteur vec, int ind, double val) {
	t_vecteur temp;
    if (vec.dimension < ind){ // vérification que l'indice n'est pas supérieur a la taille du vecteur
    	cout <<"erreur l'indice renseigne doit etre inferieur ou egal a la dimension du vecteur." << endl;
    }else{
    	if (ind <= 0 ){ //vérification que l'indice est bien supérieur ou égale à 1
    		cout <<"erreur l'indice renseigne doit etre strictement positif." << endl;
    	}else{
    		if (ind != vec.tete->indice){ //va lancer le parcours du vecteur pour l'indice cherché
    			temp=vec;
    			temp.tete = temp.tete->suiv;
    			modifier(temp,ind,val); // appel récursif
    		}else{
    			vec.tete->valeur = val;
    		}
    	}
    }
}

//procédure d'initialisation pour un vecteur, supposé normalement fraichement créé.
void initialiser (t_vecteur & vec, int d, double v){
	if (d > 0){
		supprimer(vec); // suppression du contenu du vecteur, dans le cas ou l'utilisateur aurait donné un vecteur non fraichement crée
		vec.dimension = d;
		vec.defaut = v;
		vec.tete = new t_coord;
		t_coord * temp = vec.tete;
		int i;
		for ( i = 0 ; i < d-2 ; i++){ // parcours de l'intégralité du vecteur sauf son dernier maillon pour l'initialiser de valeur par défaut
			temp->valeur = vec.defaut;
			temp->indice = i+1;
			temp->suiv = new t_coord;
			temp = temp->suiv;
		}
        // dernier maillon ayant n'ayant pas de maillon suivant
		temp->valeur = vec.defaut;
		temp->indice = i+1;
		temp->suiv = nullptr;
	}else{
		cout <<"erreur la dimension n'est pas strictement positive." << endl; //message d'erreur
	}
}

//procédure d'affichage d'un vecteur, prenant un boolean en parametre pour savoir s'il est complet ou non.
void afficher (t_vecteur vec, bool complet){
    if(vec.tete!=nullptr){// il existe au moins un maillon
        t_coord * temp = vec.tete;
        if(complet){// s'il est complet
            cout<<"[";
            for (int i = 0; i < vec.dimension ; i++){ // boucle d'affichage
                cout<<temp->valeur;
                temp = temp->suiv;
                if(i < vec.dimension-1){
                    cout<<" , ";
                }
            }
            cout<<"]"<<endl;
        }else{// il n'est donc pas complet
            bool virg = false;
            cout<<"[";
            for (int i = 0; i < vec.dimension ; i++){ // boucle d'affichage
                if(temp->valeur != vec.defaut){ // test de si la valeur est celle par défaut ou une modifiée
                    if(virg){ // test s'il faut metre la virgule
                        cout<<" , ";
                        virg = false;
                    }
                    cout<<temp->indice<<":"<<temp->valeur;
                    virg = true;
                }
                temp = temp->suiv;//décalage au maillon suivant
            }
            cout<<"]"<<endl;
        }
    }else{
        cout<<"La tête du vecteur est vide"<<endl; //le vecteur est vide et ne contient pas de maillon.
    }
}

//procédure de saisie d'un vecteur
void saisir (t_vecteur vec){
    t_coord * Tempo;
    Tempo= vec.tete;
	for (int i = 0; i < vec.dimension ; i++){ // parcours de tout le vecteur
        if(Tempo->valeur == vec.defaut){ // test si la valeur est par défaut ou non, si ce n'est pas le cas on ne va pas effacer une valeur déjà remplit
            bool valid;
            double a;
            do{
                valid = false;
                cout << "Saisir la valeur de la coordonnee d'indice: "<< i+1 << endl; // demande à l'utilisateur de faire une saisie
                cin >> a;
                if(cin.fail()){//gestion de l'erreur de saisie de l'utilisateur
                    valid = cin.fail();
                    cin.clear();
                    cin.ignore();
                    cout << "la valeur de la coordonnee doit etre un reel."<< endl; //indication a l'utilisateur que la saisie est incorrect
                    }
            }while(valid); // boucle de test de validité
            Tempo->valeur = a;
            Tempo = Tempo->suiv;
        }
    }
}

//fonction d'opération somme retournant un vecteur
t_vecteur somme (t_vecteur a, t_vecteur b){
    t_vecteur c;
    if(a.dimension == b.dimension && a.dimension > 0){ // les vecteurs sont de même dimension et supérieur à zéro
        c.tete = new t_coord(); //initialisation d'un vecteur de stockage qu'on retournera 
        c.dimension = a.dimension;
        c.defaut = a.defaut + b.defaut;
        if(a.defaut!=a.tete->valeur  && b.defaut != b.tete->valeur){ // test si les valeurs ont été remplis où si ce sont des valeurs par défaut.
            c.tete->indice = a.tete->indice;
            c.tete->valeur = a.tete->valeur + b.tete->valeur;
            c.tete->suiv = new t_coord();
            a.tete = a.tete->suiv;
            b.tete = b.tete->suiv;
            c.tete->suiv = (somme(a,b)).tete ;// appel récursif
        }else{
            cout<<"Les vecteurs ne contiennent plus de valeurs"<<endl; //si ce sont des valeurs par défaut un suppose qu'il n'y en a plus derrière.
            delete c.tete;
            c.tete = nullptr;      
        }
    }else{
        cout<<"les vecteurs doivent etre de meme dimension et non nulle pour que leur somme ait un sens"<<endl; // gestion de dimension nulle ou de dimension différente
        c.tete = nullptr;
        c.dimension = 0;
        c.defaut = 0;
    }
    return c;
}

//fonction de calcule du produit de deux vecteur
double produit (t_vecteur a, t_vecteur b){
	double p;
	if(a.dimension == b.dimension && a.dimension > 0){ // les vecteurs sont de même dimension et supérieur à zéro
		if(a.defaut != a.tete->valeur && b.defaut != b.tete->valeur)// test si les valeurs ont été remplis où si ce sont des valeurs par défaut.
		{
			p = a.tete->valeur * b.tete->valeur;
			a.tete = a.tete->suiv;
			b.tete = b.tete->suiv;
			p = p + produit(a,b);// appel récursif
		}else{
            cout<<"Les vecteurs ne contiennent plus de valeurs"<<endl; //si ce sont des valeurs par défaut un suppose qu'il n'y en a plus derrière.
            p=0;
        }
	}else{
		cout << "les vecteurs doivent etre de meme dimension et non nulle pour que leur produit scalaire ait un sens. Attention la fonction retourne 0 par defaut !" << endl; // gestion de dimension nulle ou de dimension différente
		p = 0;
	}
	return p;
}

//fonction d'execution 
int main()
{
    t_vecteur a; 
    a.tete = nullptr;
    initialiser(a,3,2); //construction d'un vecteur avec une tête nulle
    t_vecteur b;
    b.tete = new t_coord;
    initialiser(b,3,2); //construction d'un vecteur avec une tête pointant vers une case mémoire non défini
    cout<<""; // Grand incompréension de notre part, mais sans lui il existe une erreur
    afficher(somme(a,b),true); // Affichage de la somme des deux vecteurs, supposé comme complet, permettant de gérer nos différents cas d'erreur.
    return 0;
}
