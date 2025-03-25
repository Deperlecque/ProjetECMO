#define SYSTEME_H

class CSysteme {
private:


public:

	CSysteme();
	~CSysteme();
	void main();
	void initialiserSysteme();
	void mettreAjour();
	void envoyerTrameJson(float valeur);
	char traitementTrame();

};

