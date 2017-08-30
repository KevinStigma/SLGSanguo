#ifndef DATA_STRUCTURE_LEVELEDITOR
#define DATA_STRUCTURE_LEVELEDITOR

#ifndef SAFE_DELETE
#define SAFE_DELETE(p) {if(p){delete (p);(p)=NULL;}}
#endif

#include <string>
#include <vector>
#include <set>

struct Position
{
	int x;
	int y;
	Position(int a = 0, int b = 0):x(a),y(b){}
};

struct ImageItem
{
	Position pos;
	std::string img_paths;
	int type;
	bool mirrored;
};

struct WorldMapFrame
{
	std::string txt;
	std::vector<ImageItem> img_items;
};

enum DialogPos{LEFT_UP,RIGHT_UP,LEFT_DOWN,RIGHT_DOWN};
struct DialogBox
{
	std::string character_name;
	std::string txt;
	std::string character_path;
	DialogPos pos;
};

struct PlotFrame
{
	std::vector<ImageItem> img_items;
	DialogBox* dialog_box;
	PlotFrame(){ dialog_box = NULL; }
	~PlotFrame(){ SAFE_DELETE(dialog_box); }
	PlotFrame(const PlotFrame& f);
	PlotFrame& operator = (PlotFrame f);
};

#endif