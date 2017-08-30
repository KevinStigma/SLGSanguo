#include "DataStructure.h"
PlotFrame::PlotFrame(const PlotFrame& f)
{ 
	img_items.assign(f.img_items.begin(), f.img_items.end());
	if (f.dialog_box)
	{
		dialog_box = new DialogBox();
		*dialog_box = *f.dialog_box;
	}
	else
		dialog_box = NULL;
}

PlotFrame& PlotFrame::operator = (PlotFrame f)
{
	img_items.assign(f.img_items.begin(), f.img_items.end());
	dialog_box = new DialogBox();
	*dialog_box = *f.dialog_box;
	return (*this);
}