#ifndef _SineTable_h_
#define _SineTable_h_

/* constants */
#define TableSize	201
#define pi 			3.141592653
#define TWO_PI 		6.283185306
#define HALF_PI 	1.5707963265

/* the LUT */
extern const float arctantab[2001];

/* public f(x) */
extern float lookup_arctan(float x);


#endif	// _ArcSineTable_h_

