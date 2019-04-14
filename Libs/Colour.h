
#ifndef _COLOUR_H_
#define _COLOUR_H_

#include <General.h>

struct RGB
{
	u8 R;
	u8 G;
	u8 B;
};

struct HSLA
{
	double Hue;
	double Saturation;
	double Lightness;
	double Amplitude;
	RGB operator()();
};

#define RED (0.0)
#define YELLOW (1.0/6.0)
#define GREEN (2.0/6.0)
#define CYAN (3.0/6.0)
#define BLUE (4.0/6.0)
#define MAGENTA (5.0/6.0)



//double OppCos(double x);
inline double RedVal(double val)
{
	return val = 2.0-abs(6.0*(0.5-abs(val-0.5))), val=MIN(val,1.0), MAX(val,0.0);
}
inline double GreenVal(double val)
{
	return val = 2.0-abs(6.0*val-2.0), val=MIN(val,1.0), MAX(val,0.0);
}
inline double BlueVal(double val)
{
	return val = 2.0-abs(6.0*val-4.0), val=MIN(val,1.0), MAX(val,0.0);
}
inline RGB RGBVal(double hue)
{		
	return RGB{(u8)(255.0*RedVal(hue)),(u8)(255.0*GreenVal(hue)),(u8)(255.0*BlueVal(hue))};
}
inline RGB RGBScale(RGB rgb, double scale)
{
	return RGB{ (u8)(scale*rgb.R), (u8)(scale*rgb.G), (u8)(scale*rgb.B) };
}
inline RGB ColourMix(RGB a, RGB b, double w)
{
	double iw = 1.0-w;
	return {(u8)(a.R*iw + b.R*w), (u8)(a.G*iw + b.G*w), (u8)(a.B*iw + b.B*w)};
}
/*inline RGB ColourMixInt(RGB a, RGB b, i32 w)
{
	double iw = 1000-w;
	return {(u8)((a.R*iw + b.R*w)/1000), (u8)((a.G*iw + b.G*w)/1000), (u8)((a.B*iw + b.B*w)/1000)};
}
inline double Bistable(double x, double a)
{
	return x<0 ? 0 : (x<a && a!=0 ? x/a : 1);
}

inline RGB RGBVal2(HSLA val)
{
	double c = (1.0-abs(2*val.Lightness-1))*val.Saturation;
	double h = val.Hue*6.0;
	double x = c*(1-abs(fmod(h,2)-1));
	double a = val.Amplitude*255.0;
	double r, g, b;
	double m = val.Lightness-c/2.0;
	     
	if		(h <= 1.0) { r=c; g=x; b=0; }
	else if	(h <= 2.0) { r=x; g=c; b=0; }
	else if	(h <= 3.0) { r=0; g=c; b=x; }
	else if	(h <= 4.0) { r=0; g=x; b=c; }
	else if	(h <= 5.0) { r=x; g=0; b=c; }
	else if	(h <= 6.0) { r=c; g=0; b=x; }
	
	return {(u8)((r+m)*a), (u8)((g+m)*a), (u8)((b+m)*a)};
}
*/
#endif

