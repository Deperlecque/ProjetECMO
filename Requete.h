#define REQUETE_H

class CRequete {
private:


public:

	CRequete();
	~CRequete();
	bool connectionBdd();
	bool deconnectionBdd();
	void envoyerRequete(float valeur);

};

