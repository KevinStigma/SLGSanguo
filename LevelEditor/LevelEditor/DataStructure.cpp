#include "DataStructure.h"
PlotFrame::PlotFrame(PlotFrame& f)
{ 
	image_items.assign(f.image_items.begin(), f.image_items.end());
	dialog_box = new DialogBox();
	*dialog_box = *f.dialog_box;
}

PlotFrame& PlotFrame::operator = (PlotFrame f)
{
	image_items.assign(f.image_items.begin(), f.image_items.end());
	dialog_box = new DialogBox();
	*dialog_box = *f.dialog_box;
	return (*this);
}