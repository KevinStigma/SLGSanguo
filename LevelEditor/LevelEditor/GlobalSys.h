#ifndef GLOBALSYS_H
#define GLOBALSYS_H
#include <string>
class GlobalSys
{
public:
	std::string assetPath;
};

extern GlobalSys* g_pGlobalSys;
#endif