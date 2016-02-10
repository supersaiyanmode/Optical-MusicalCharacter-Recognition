#include <sstream>
#include <string>

#include "ImageResize.h"
#include "SImageIO.h"

SDoublePlane get_template(int template_num, double staff_height) {
	std::stringstream ss;
	ss<<"templates/template"<<template_num<<"-"<<int(staff_height *100 / 11.0)<<".png";
	std::cout<<"Loading Template: "<<ss.str()<<std::endl;
	return SImageIO::read_png_file(ss.str().c_str());
}
