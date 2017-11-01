#ifndef DATA_STRUCTURE 
#define DATA_STRUCTURE
#include <string>
struct General
{
	General(int w = 0, int z = 0, int m = 0, int y = 0, int t = 0, std::string n = "anonym") :wuli(w), zhili(z), minjie(m), tongshuai(t), yunqi(y), name(n)
	{
		hpadd = 0; mpadd = 0;
	}
	int wuli;
	int zhili;
	int minjie;
	int yunqi;
	int tongshuai;
	int hpadd;
	int mpadd;
	std::string name;
	std::string biography;
	std::string retreate;
	std::string critical;
	std::string img_path;
};

#endif