#include <iostream>
#include <algorithm>
#include <string>
#include <sstream>
#include <fstream>

using namespace std;

class Dikdortgen {
public:
	int x_sol;
	int yukseklik;
	int x_sag;

};

class Ip {	//maximumdaki çizilen ip
public:
	Ip(int x_sol = 0, int yukseklik = 0);
	friend class Zirve;
private:
	int x_sol;
	int yukseklik;
};

class Zirve {  //max yükseklik
public:
	Zirve(int toplam);
	~Zirve();
	void ekle(Ip *obje); //etrafı iple çevreliyoruz
	Zirve *Birlesim(Zirve *other);
	void yazdir();
private:
	Ip *dizi;
	int kapasite; //dizideki toplam eleman sayısı
	int cizgiSayisi;
};

Zirve *zirveBul(Dikdortgen dizi[], int sol, int boy);
int main()
{
	constexpr char DELIMITER{ ',' };

	Dikdortgen koordinatlar[500];
	string okuma; //dosyadaki verileri stringe aktardım
	ifstream dosya("dikdörtgen.csv");

	int x{ 0 };
	if (dosya.is_open()) {
		while (getline(dosya, okuma)) {
			stringstream reader(okuma);		// dosyayi okumayi saglayan
			Dikdortgen temp;
			reader >> temp.yukseklik; reader.ignore(1, DELIMITER);
			reader >> temp.x_sag; reader.ignore(1, DELIMITER);
			koordinatlar[x] = temp;
			++x;
		}
		Zirve *ptr = zirveBul(koordinatlar, 0, x);
		(*ptr).yazdir();
		return 0;
	}
	else
	{
		cout << "!!!Dosya Bulunamadi!!!" << endl;
		return -1;
	}
}

Ip::Ip(int x_sol, int yukseklik) {
	(*this).x_sol = x_sol;
	(*this).yukseklik = yukseklik;
}

Zirve:: ~Zirve()
{
	delete[] dizi;
}

Zirve::Zirve(int toplam) : kapasite{ toplam }, cizgiSayisi{ 0 }{
	dizi = new Ip[toplam];
}

void Zirve::ekle(Ip *obje) {
	if (dizi[cizgiSayisi - 1].yukseklik == (*obje).yukseklik && 0 < cizgiSayisi)
		return;
	if (dizi[cizgiSayisi - 1].x_sol == (*obje).x_sol && 0 < cizgiSayisi) {

		dizi[cizgiSayisi - 1].yukseklik = max(dizi[cizgiSayisi - 1].yukseklik, (*obje).yukseklik);
		return;
	}
	dizi[cizgiSayisi] = *obje;
	++cizgiSayisi;
}

Zirve *Zirve::Birlesim(Zirve * other)
{

	Zirve *sonuc = new Zirve((*this).cizgiSayisi + (*other).cizgiSayisi);

	int boy_1{ 0 }, boy_2{ 0 };	//mevcut olan iki zirvenin boyu
	int i{ 0 }, j{ 0 };

	while (i < (*this).cizgiSayisi && j < (*other).cizgiSayisi) {
		if ((*this).dizi[i].x_sol < (*other).dizi[j].x_sol)
		{
			int x_1 = (*this).dizi[i].x_sol;
			boy_1 = (*this).dizi[i].yukseklik;

			int maxBoy{ max(boy_1, boy_2) };

			(*sonuc).ekle(new Ip(x_1, maxBoy));
			++i;
		}
		else
		{
			int x_2 = (*other).dizi[j].x_sol;
			boy_2 = (*other).dizi[j].yukseklik;
			int maxBoy{ max(boy_1, boy_2) };
			(*sonuc).ekle(new Ip(x_2, maxBoy));
			++j;
		}
	}

	for (i; i < (*this).cizgiSayisi; i++) {
		(*sonuc).ekle(&dizi[i]);
	}

	for (j; j < (*other).cizgiSayisi; j++) {
		(*sonuc).ekle(&(*other).dizi[j]);
	}

	return sonuc;
}

Zirve *zirveBul(Dikdortgen dizi[], int sol, int boy) //D&C kısmını uyguladım
{
	if (sol == boy) {
		Zirve *sonuc = new Zirve(2);
		(*sonuc).ekle(new Ip(dizi[sol].x_sol, dizi[sol].yukseklik));
		(*sonuc).ekle(new Ip(dizi[sol].x_sag, 0));
		return sonuc;
	}

	int orta{ (sol + boy) / 2 };


	Zirve *ilk = zirveBul(dizi, sol, orta);
	Zirve *son = zirveBul(dizi, orta + 1, boy);
	Zirve *sonuc = (*ilk).Birlesim(son);


	delete ilk;
	delete son;

	return sonuc;
}

void Zirve::yazdir()
{
	for (int i = 0; i < cizgiSayisi; i++) {
		cout << " (" << dizi[i].x_sol << ", "
			<< dizi[i].yukseklik << "), ";
	}
}
