#include <iostream>
#include <stdlib.h>
#include <unistd.h>
using namespace std;

class Figure
{
	protected:
		char pos[2];
		
	public:
		Figure( char*);
		
		virtual int bad_move(char*)=0;
		 int deskout(char*);//проверка на выход за пределы
		int show_desk();//прорисовка доски 
		
		operator char*();
		Figure& operator=( char*);//нужен в перегрузке >> для присваивания новых знач позиции
		int operator==( char*);
		int operator!=(char*);
		void strategic_move();
	
		friend std::istream& operator>>(std::istream&, Figure&);
		friend std::ostream& operator<<(std::ostream&, Figure&);
		
};

//РЕАЛИЗАЦИЯ МЕТОДОВ


Figure::Figure( char* p){
	pos[0] = p[0]; 
	pos[1] = p[1]; 
}

Figure::operator char*(){
	return pos;
	}

int Figure :: deskout( char* p){
	return((p[0] > 'h') || (p[0] < 'a') || (p[1] < '1') || (p[1] > '8'));
}

Figure& Figure :: operator=( char* p){
	pos[0] = p[0]; 
	pos[1] = p[1];
	return(*this);
}

int Figure :: operator==( char* p){
	return((pos[0] == p[0]) && (pos[1] == p[1]));
}

int Figure :: operator!=(char* p){
	return((pos[0] != p[0]) || (pos[1] != p[1]));
}

std::ostream& operator<<(std::ostream& out, Figure& f){
	return out << f.pos[0] << f.pos[1];
}

std::istream& operator>>(std::istream& in, Figure& f){
	char s[2];
	std::cin.unsetf(std::ios::skipws);
	in >> s[0] >> s[1];
	in.ignore(64, '\n');
	if((f.bad_move(s) == 1) || (f == s)) 
		in.clear(std::ios::failbit | in.rdstate());
	f = s;
	return in;
}

void Figure :: strategic_move(){
	if (( pos[0] % 2 != 0) && (pos[1]  % 2 != 0)){
		++pos[0];
		++pos[1];
	}
	else if ( pos[0]  % 2 != 0)
		++pos[0];
	else if ( pos[1]  % 2 != 0)
		++pos[1];
	else
		if (pos[1]!='8')
			++pos[1];
		else 
			++pos[0];
	cout<<pos[0]<<pos[1]<<endl;
}

int Figure :: show_desk()
{
	int i, j;
	char s[2];
	char* mark = (char *)".K";
	char m;
	cout << "  a b c d e f g h\n";
	for(i=8; i > 0; i--)
	{
		cout << i << ' ';
		s[1] = '0' + i;
		for(j=0; j < 8; j++)
		{
			s[0] = 'a' + j;
			m = (*this != s) ? mark[0] : mark[1]; 
			cout << m << ' ';
		}
		cout << i << "\n";
	}
	cout << "  a b c d e f g h\n";
	return(0);
}

//-----------------Класс короля--------------------------------------
class King : public Figure{
	public:
		King(char* p) : Figure(p){};
		int bad_move(char* );
};
//РЕАЛИЗАЦИЯ МЕТОДОВ
int King :: bad_move(char* p){
	if (deskout(p) > 0)
		return (1); 
	int x = p[0] - pos[0];
	int y = p[1] - pos[1];
	if (( x == 0)&&( y == 1)||( x == 1 )&&( y == 0 )||( x==1 )&&( y ==1 ))
		return (0);
	return (1);
}

//-----------------Интерфейс игрока----------------------------------

class GenericPlayer{
  protected:
    King* king;
    const char* name; 
  public:
    GenericPlayer(King& k)
	{
 		king = &k; 
	}; 
	const char* getName(){return name;}
    virtual int move() = 0; 
    int query(); // запрос хода
	virtual ~GenericPlayer(){};
}; 

//РЕАЛИЗАЦИЯ МЕТОДОВ
int GenericPlayer :: query() 
{
	cout << name << ": ";
}



//--------------------Классы Человека и PC------------------

class Human : public GenericPlayer {
  public:
    Human(King& k,const char* n) : GenericPlayer(k) {name = n;};
    int move(); 
	
}; 

class Pc : public GenericPlayer {
  public:
    Pc(King& k,const char* n) : GenericPlayer(k) {name = n;};
    int move(); 
}; 

//РЕАЛИЗАЦИЯ МЕТОДОВ
int Human::move() {
	if(!(std::cin >> (*king)))
		return (0);
}
int Pc::move() {
	king->strategic_move();
	
	return (1);
	}
	 
//-----------------------Main()-----------------------------------

int main()
{
	King king((char* )"a1");
	GenericPlayer* g[] = {new Human(king,"Человек"),new Pc(king,"Компьютер")};

	king.show_desk();
	
	int i = 0;
	int player_index;

	while (king != (char* )"h8"){
		player_index=i;
		g[i]->query();
	 	if (g[i]->move() == 0){
			cout <<"\nВы ввели не верный ход\n";
			return (0);
		}
		if (i==1)
			sleep(1);
		king.show_desk();
		if(++i > 1){
    		i = 0;
		}
	}
	cout << g[player_index]->getName() << " победил\n";

	delete g[0];
	delete g[1];
	return (0); 
}
