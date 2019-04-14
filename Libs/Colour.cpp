
#include "Colour.h"


RGB HSLA::operator()()
{
	double c = (1.0-abs(2*Lightness-1))*Saturation;
	double h = Hue*6.0;
	double x = c*(1-abs(fmod(h,2)-1));
	double r=0, g=0, b=0;
	     
	if		(0.0 <= h && h <= 1.0) { r=c; g=x; b=0; }
	else if	(1.0 <  h && h <= 2.0) { r=x; g=c; b=0; }
	else if	(2.0 <  h && h <= 3.0) { r=0; g=c; b=x; }
	else if	(3.0 <  h && h <= 4.0) { r=0; g=x; b=c; }
	else if	(4.0 <  h && h <= 5.0) { r=x; g=0; b=c; }
	else if	(5.0 <  h && h <= 6.0) { r=c; g=0; b=x; }
	double m = Lightness-c/2.0;
	return {(u8)(255.0*(r+m)*Amplitude), (u8)(255.0*(g+m)*Amplitude), (u8)(255.0*(b+m)*Amplitude)};
}
