#include "leveleditor.h"
#include <QtWidgets/QApplication>
#include "GlobalSys.h"

int main(int argc, char *argv[])
{
	g_pGlobalSys = new GlobalSys;
	g_pGlobalSys->assetPath = "D:/SLGSanguo/Data/Assets/";
	QApplication a(argc, argv);
	LevelEditor w;
	w.show();
	return a.exec();
}
