// reading a text file
#include <iostream>
#include <fstream>
#include <string>
using namespace std;
int days;

void rewrite() {

	string line;
	string saveLine;
	ifstream myfile("test.txt");
	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			int count = 0;
			int separator = 0;
			while (line[count] != '\0') {
				if (line[count] == ',') {
					count++;
					separator++;
					continue;
				}
				if (separator == 1) {
					saveLine += line[count];
				}

				count++;
			}
			saveLine += '\n';
		}
		myfile.close();
	}
	else cout << "Unable to open file";

	ofstream file;
	file.open("data.txt");
	file << saveLine;
	file.close();



}

double EMA(double *tab, int N)
{

	if (days == 12) {
		int aasdf = 9;
	}

	double ema = 0;
	double up = 0;
	double down = 0;
	double alfa = 0;
	if (N >= days) N = days - 1;//zzaklada ze przy wzorze na alfa N jest zalezne od wartosci liczbzy dni <26
	if (N != 1) {
		alfa = 2 / (double)(N - 1);
	}

	double count = 1;
	for (int a = 0; a <= N; a++) {
		double liczba = count*tab[days - a - 1];
		up += count*tab[days - a - 1];//
		down += count;
		count *= (1 - alfa);
	}
	ema = up / down;
	return ema;
}

void  fillArray(string newName, double * tab, int size)
{
	string line;
	
	ifstream myfile(newName);
	
	int count = 0;
	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			if (count >= 1000) {
				break;
			}
			else {
				tab[count++] = std::stod(line);
			}
			
		}
		myfile.close();
	}
	else cout << "Unable to open file!!!!!!!!";
	

}

double invest(double *tab, double* MACD, double* SIGNAL,double money) {

	int day = 0;//od zaer indexowane
	double oldSignal=NULL;
	double oldMacd = NULL;
	double stock = 0;

	for (int day = 0; day < 1000; day++)
	{
		if (day < 26) {
			continue;
		}
		else
		{
			if (oldSignal == NULL || oldMacd == NULL) {//przy pierwszej inicjalizacji
				oldSignal = SIGNAL[day];
				oldMacd = MACD[day];
				continue;
			}
			else
			{
				if (oldMacd > oldSignal && SIGNAL[day] > MACD[day] && stock >0) {
					//sprzedaj
					money = tab[day] * stock;
					stock = 0;

				}
				else if (oldMacd < oldSignal && SIGNAL[day] < MACD[day] && stock == 0) {
					//kup
					stock = money / tab[day];
					money = 0;

				}
				oldSignal = SIGNAL[day];
				oldMacd = MACD[day];
			}
		}

	}
	if (money == 0) {
		money = tab[day] * stock;
	}


	return money;
}

int main(int argc, char* argv[])
{
	double money = 1000;
	string newName;
	if (argc == 2) {
		cout << argv[1];
		newName = argv[1];
	}
	else newName = "data.txt";

	//rewrite();   //sluzy do zmiany danych
	
	days = 1;

	
	double tab[1000];
	double MACD[1000];
	double SIGNAL[1000];
	
	fillArray(newName, tab, 1000);

	for (int a = 0; a < 1000; a++) {
		double Ema12 = EMA(tab, 12);
		double Ema26 = EMA(tab, 26);
		MACD[a] = Ema12 - Ema26;
		days++;
	}
	ofstream file;
	file.open("MACD.txt");

	for (int a = 0; a < 1000; a++)
	{
		cout << MACD[a] << endl;
		file << MACD[a];
		file << '\n';
	}
	file.close();

	ofstream sig;
	sig.open("SIGNAL.txt");
	days = 1;
	for (int a = 0; a < 1000; a++)
	{
		double ret = EMA(MACD, 9);
		SIGNAL[days - 1] = ret;
		sig << ret;
		sig << '\n';
		days++;
	}
	sig.close();

	cout << "stan konta ";
	cout<<invest(tab, MACD, SIGNAL, money) << endl;
	cin >> days;

	return 0;
}