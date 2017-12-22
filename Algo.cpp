#include <iostream>
#include <string>
#include <sstream>
#include <cstddef>
using namespace std;

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

void supprimer (t_vecteur & vec) {
	t_vecteur temp;
	if (vec.tete->suiv != nullptr){
        temp = vec;
        temp.tete = temp.tete->suiv;
	    supprimer(temp);
	}
	delete vec.tete;
	vec.tete = nullptr;
}

void vider (t_vecteur vec) {
	t_vecteur temp;
    if (vec.tete->suiv != nullptr){
        temp = vec;
        temp.tete = temp.tete->suiv;
	    vider(temp);
	}
	vec.tete->valeur = vec.defaut;
}


void modifier (t_vecteur vec, int ind, double val) {
	t_vecteur temp;
    if (vec.dimension < ind)
    {
    	cout <<"erreur l'indice renseigne doit etre inferieur ou egal a la dimension du vecteur." << endl;
    }else
    {
    	if (ind <= 0)
    	{
    		cout <<"erreur l'indice renseigne doit etre strictement positif." << endl;
    	}else
    	{
    		if (ind != vec.tete->indice)
    		{
    			temp=vec;
    			temp.tete = temp.tete->suiv;
    			modifier(temp,ind,val);
    		}else
    		{
    			vec.tete->valeur = val;
    		}
    	}
    }
}

void initialiser (t_vecteur & vec, int d, double v){
	if (d > 0)
	{
		supprimer(vec);
		vec.dimension = d;
		vec.defaut = v;
		vec.tete = new t_coord;
		t_coord * temp = vec.tete;
		int i;
		for ( i = 0 ; i < d-2 ; i++)
		{
			temp->valeur = vec.defaut;
			temp->indice = i+1;
			temp->suiv = new t_coord;
			temp = temp->suiv;
		}
		temp->valeur = vec.defaut;
		temp->indice = i+1;
		temp->suiv = nullptr;
	}else
	{
		cout <<"erreur la dimension n'est pas strictement positive." << endl;
	}
}

t_vecteur significatif(t_vecteur vec, bool complet){
	if(!complet)
	{
	    t_coord * memo = nullptr;
	    t_coord * actu;
	    int comp = 0;
	    for(int i = 0 ; i < vec.dimension ; i++){
            if(vec.tete->valeur != vec.defaut)
            {
                comp ++;
                if(memo==nullptr)
                {
                        memo = vec.tete;
                }
                vec.tete->indice = comp;
                actu = vec.tete;
                vec.tete = vec.tete->suiv;
            }else
            {
                actu->suiv = vec.tete->suiv;
                vec.tete = vec.tete->suiv;
            }
        }
        vec.tete = memo;
        vec.dimension = comp;
        return vec;
	}else
	{
		return vec;
	}
}

void afficher (t_vecteur vec, bool complet){
    t_coord * temp = vec.tete;
    if(complet)
    {
		cout<<"[";
		for (int i = 0; i < vec.dimension ; i++){
			cout<<temp->valeur;
			temp = temp->suiv;
			if(i < vec.dimension-1)
			{
				cout<<" , ";
			}
		}
		cout<<"]"<<endl;
	}else
	{
		bool virg = false;
		cout<<"[";
		for (int i = 0; i < vec.dimension ; i++){
			if(temp->valeur != vec.defaut)
			{
				if(virg){
					cout<<" , ";
					virg = false;
				}
				cout<<temp->indice<<":"<<temp->valeur;
				virg = true;
			}
			temp = temp->suiv;
		}
		cout<<"]"<<endl;
	}
}

void saisir (t_vecteur vec){
	string a;
	for (int i = 0; i < vec.dimension ; i++){
			if(vec.tete->valeur == vec.defaut)
			{
				bool valid;
				double a;
				do
				{
					valid = false;
					cout << "Saisir la valeur de la coordonnee d'indice: "<< i+1 << endl;
					cin >> a;
					if(cin.fail())
					{
						valid = cin.fail();
						cin.clear();
						cin.ignore();
						cout << "la valeur de la coordonnee doit etre un reel."<< endl;
					}
				}while(valid);
				vec.tete->valeur = a;
				vec.tete = vec.tete->suiv;
			}
	}
}

t_vecteur somme (t_vecteur a, t_vecteur b, bool completA,bool completB){
    if(!completA)
    {
        a = significatif(a,false);
    }
    if(!completB)
    {
        b = significatif(b,false);
    }
	t_vecteur c;
	if(a.dimension == b.dimension && a.dimension != 0)
	{
		c.tete = new t_coord();
		c.dimension = a.dimension;
		c.defaut = a.defaut + b.defaut;
		t_coord * temp = c.tete;
		for(int i = 0 ; i < a.dimension ; i++){
			temp->indice = a.tete->indice;
			temp->valeur = a.tete->valeur + b.tete->valeur;
			temp->suiv = new t_coord();
			temp = temp->suiv;
			a.tete = a.tete->suiv;
			b.tete = b.tete->suiv;
		}
	}else
	{
	    cout<<"les vecteurs doivent etre de meme dimension et non nule pour que leur somme ait un sens"<<endl;
		c.tete = nullptr;
		c.dimension = 0;
		c.defaut = 0;
	}
	return c;
}

double produit (t_vecteur a, t_vecteur b, bool completA,bool completB){
    if(!completA)
    {
        a = significatif(a,false);
    }
    if(!completB)
    {
        b = significatif(b,false);
    }
	double p;
	if(a.dimension == b.dimension && a.dimension != 0)
	{
		p = a.tete->valeur * b.tete->valeur;
		if(a.dimension != a.tete->indice)
		{
			a.tete = a.tete->suiv;
			b.tete = b.tete->suiv;
			p = p + produit(a,b,completA,completB);
		}
	}else
	{
		cout << "les vecteurs doivent etre de meme dimension et non nule pour que leur produit scalaire ait un sens. Attention la fonction retourne 0 par defaut !" << endl;
		p = 0;
	}
	return p;

}

int main()
{
    t_vecteur a;
    a.tete = nullptr;
    initialiser(a,3,2);
    t_vecteur b;
    b.tete = new t_coord;
    initialiser(b,3,2);
    cout<<"";
    afficher(somme(a,b,false,false),true);
    return 0;
}
