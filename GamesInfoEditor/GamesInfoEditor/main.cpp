#include "gamesinfoeditor.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	GamesInfoEditor w;
	w.show();
	return a.exec();
}
