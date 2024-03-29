#include <iostream>
#include <fstream>
#include <random>
#include <ctime>
#include <vector>
#include <cmath>
#include <windows.h>

using namespace std;

int getRandomNumber(int min, int max)
{
	static const double fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1.0);
	// ���������� ������������ ��������� ����� � ����� ���������
	return static_cast<int>(rand()* fraction *(max - min + 1) + min);
}

int randome(int n){
	double a = n/RAND_MAX; 
	int z = a+1; // ������ ����������� ������ ������� �� ���� ���������
	int b = getRandomNumber(1, z); // �������� �������� ������� ����� �������
	int c = ((RAND_MAX*(b-1))+rand())%(n+1); //�������� ����� � ��� �����
	return c;
}

int powmod (int a, int b, int p) {
	int res = 1;
	while (b)
		if (b & 1)
			res = int (res * 1ll * a % p),  --b;
		else
			a = int (a * 1ll * a % p),  b >>= 1;
	return res;
}
 
int generator (int k) {
	vector<int> fact;
	int phi = k-1;
	int n = k-1;
	for (int i=2; i*i<=n; ++i)
		if (n % i == 0) {
			fact.push_back (i);
			while (n % i == 0)
				n /= i;
		}
	if (n > 1)
		fact.push_back (n);
 
	for (int res=2; res<=k; ++res) {
		bool ok = true;
		for (size_t i=0; i<fact.size() && ok; ++i)
			ok &= powmod (res, phi / fact[i], k) != 1;
		if (ok)  return res;
	}
	return -1;
}

unsigned int  Erastophen(unsigned int x)
{
	static std::vector<unsigned int> cache;
	static unsigned int max_prime = 0;
	if(max_prime == 0) {
		cache.push_back(2);
		cache.push_back(3);
		max_prime = 3;
	}
	if(x >= max_prime) { // ��� � ����
		unsigned int limit = (x < 1000) ? 1024 : x /4 * 5; // x+20%

		unsigned int start = max_prime + 2; // ������ ���� ������
		unsigned int* bitmap = (unsigned int*) calloc((limit - start) / 64 + 1, sizeof(unsigned int));
		limit -= start;

		if(max_prime > 3) { // ������������� ���������� ��������
			unsigned int* end = &cache[cache.size() - 1];
			for(unsigned int* cur = &cache[1];cur <= end; cur++) {
				unsigned int i = *cur * *cur; 
				if(i > limit + start) break;
				unsigned int step = *cur << 1;
				if(i < start) { // ������������ ��� cur*cur+2N*cur
					i = start - (start - i) % step;
					if(i < start) i += step;
				}
				i -= start;
				//printf("start=%d i=%d max=%d\n", start, i, max_prime);
				for(; i < limit; i += step) { // ����������� ������� max_prime
					bitmap[i >> 6] |= (1 << ((i >> 1) & 31));
				}
			}
		}

		bool need_fill = true;
		while(need_fill) {
			if(max_prime >= 65536 || max_prime * max_prime >= limit + start) {
				need_fill = false; // ������ ��������� �� ����
			} else {
				unsigned int step = max_prime << 1;
				for(unsigned int i = max_prime * max_prime - start; i < limit; i += step) { // ����������� ������� max_prime
					bitmap[i >> 6] |= (1 << ((i >> 1) & 31));
				}
			}
			// ���������� �������� ������
			for(unsigned int i = max_prime + 2 - start; i < limit; i += 2) {
				if((bitmap[i >> 6] & (1 << ((i >> 1) & 31))) == 0) {
					cache.push_back(i + start);
					if(need_fill) {
						break;
					}
				}
			}
			max_prime = cache.back();
		}
		free(bitmap);
	}

	static unsigned int prev_id = 0;
	static unsigned int prev_prime = 0;
	if(x < 2) {
		if(x == 0) { // ������������ ������
			max_prime = 0;
			cache.clear();
		}
		prev_id = 0;
		prev_prime = 2;
	} else if(x == prev_prime) {
		prev_id++;
		prev_prime = cache[prev_id];
	} else { // ����� � ����
		unsigned int* min = &cache[0];
		unsigned int* max = &cache[cache.size() - 1];
		while(max - min > 1) {
			unsigned int* mid = min + (max - min) / 2;
			if (*mid > x) {
				max = mid;
			} else {
				min = mid;
			}
		}
		prev_id = max - &cache[0];
		prev_prime = *max;
	}
	return prev_prime;
}

int power(int a, int b, int n){// a^b mod n
    int tmp=a;
    int sum=tmp;
    for(int i=1;i<b;i++){
        for(int j=1;j<a;j++){
            sum+=tmp;
            if(sum>=n){
                sum-=n;
            }
        }
        tmp=sum;
    }
    return tmp;
}
 
int mul(int a, int b, int n){// a*b mod n
    int sum=0;
     
    for(int i=0;i<b;i++){
        sum+=a;
         
        if(sum>=n){
            sum-=n;
        }
    }
 
    return sum;
}

void crypt(int p,int g,int x, string inFileName,string outFileName){
	ofstream infa(inFileName.c_str());
	ifstream inf(inFileName.c_str());
	ofstream outf(outFileName.c_str());
	ofstream of("x.txt");
	cout<<"������� ��, ��� ����� �����������: ";
    char s[256];
    cin.getline(s,256);
    infa << s;
    infa.close(); 
    int y=power(g,x,p);
    cout<<"�������� ���� (p,g,y)="<<"("<<p<<","<<g<<","<<y<<")"<<endl;
    of << x;
    of.close(); 
    cout<<"������������� �����:"<<endl;
    
    while(inf.good()){
        int m=inf.get();
        if(m>0){             
            int k=rand()%(p-2)+1; // 1 < k < (p-1)
            int a= power(g,k,p);
            int b= mul(power(y,k,p),m,p);
 //         cout<<a<<" "<<b<<" ";
            outf << a <<" "<< b <<" ";
            char z = a;
            char s = b;
            cout << z <<" "<< s <<" ";
        }
    }
    
    cout<<endl;
    
	inf.close();
    outf.close();
  
}
 
void decrypt(int p,int x,string inFileName,string outFileName){
    ifstream inf(inFileName.c_str());
    ofstream outf(outFileName.c_str());
    cout<<"������������ �����:"<<endl;
    while(inf.good()){
        int a=0;
        int b=0;
        inf>>a;
        inf>>b;
         
        if(a!=0&&b!=0){
            int deM=mul(b,power(a,p-1-x,p),p);
            char m=static_cast<char>(deM);
            outf<<m;
            cout<<m;
        }
    }
    cout<<endl;
    
    inf.close();
    outf.close();
   
}

int main(){
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
//  srand(time(NULL));
    srand(static_cast<unsigned int>(time(0)));
    setlocale(LC_ALL, "Rus");
//  random_device rd;
//	mt19937 gen(rd());
//	uniform_int_distribution<> dis(1,100000);
	int a = randome(50000);/*dis(gen);*/
//	cout<<a;
//	uniform_int_distribution<> dis1(1,a);
	int p = getRandomNumber(10000, a);/*dis1(gen);*/
	p = Erastophen(p);
//	uniform_int_distribution<> dis2(1,p-1);
	int x = getRandomNumber(10000, p-1);/*dis2(gen);*/
	int v;
	crypt(p,generator(p),x,"in.txt","out_crypt.txt");
	do{
	 	cout << "������� ��� �������� ����: ";
        cin >> v;
        if(v!=x){
        	cout<<"�������� �������� ����"<<endl;
        }
    } while (v!=x);		
	decrypt(p,x,"out_crypt.txt","out_decrypt.txt");
	
	

    return 0;
}
