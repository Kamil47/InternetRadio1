#include "00_settings.h"
#include "audio.h"
//bitmaps for large numbers and colon every number has 40 x 56 pixles
//(0,1,2,3,4,5,6,7,8,9,0,:) size of numbers is reduced
const unsigned char ziffern PROGMEM [11][500]  = {
 
// 'OpenSanseSemi0', 40x56px
 {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xc0, 0x00, 0x00, 0x1f, 
	0xff, 0xf0, 0x00, 0x00, 0x3f, 0xff, 0xf8, 0x00, 0x00, 0x7f, 0xff, 0xfc, 0x00, 0x00, 0xff, 0xff, 
	0xfe, 0x00, 0x01, 0xff, 0x83, 0xff, 0x00, 0x01, 0xfe, 0x00, 0xff, 0x00, 0x03, 0xfc, 0x00, 0x7f, 
	0x80, 0x03, 0xf8, 0x00, 0x7f, 0x80, 0x07, 0xf8, 0x00, 0x3f, 0x80, 0x07, 0xf8, 0x00, 0x3f, 0xc0, 
	0x07, 0xf0, 0x00, 0x1f, 0xc0, 0x07, 0xf0, 0x00, 0x1f, 0xc0, 0x0f, 0xf0, 0x00, 0x1f, 0xc0, 0x0f, 
	0xf0, 0x00, 0x1f, 0xe0, 0x0f, 0xf0, 0x00, 0x1f, 0xe0, 0x0f, 0xe0, 0x00, 0x0f, 0xe0, 0x0f, 0xe0, 
	0x00, 0x0f, 0xe0, 0x0f, 0xe0, 0x00, 0x0f, 0xe0, 0x0f, 0xe0, 0x00, 0x0f, 0xe0, 0x0f, 0xe0, 0x00, 
	0x0f, 0xe0, 0x0f, 0xe0, 0x00, 0x0f, 0xe0, 0x0f, 0xe0, 0x00, 0x0f, 0xe0, 0x0f, 0xe0, 0x00, 0x0f, 
	0xe0, 0x0f, 0xe0, 0x00, 0x0f, 0xe0, 0x0f, 0xe0, 0x00, 0x0f, 0xe0, 0x0f, 0xe0, 0x00, 0x0f, 0xe0, 
	0x0f, 0xe0, 0x00, 0x0f, 0xe0, 0x0f, 0xe0, 0x00, 0x0f, 0xe0, 0x0f, 0xf0, 0x00, 0x1f, 0xe0, 0x0f, 
	0xf0, 0x00, 0x1f, 0xe0, 0x07, 0xf0, 0x00, 0x1f, 0xe0, 0x07, 0xf0, 0x00, 0x1f, 0xc0, 0x07, 0xf0, 
	0x00, 0x1f, 0xc0, 0x07, 0xf8, 0x00, 0x3f, 0xc0, 0x03, 0xf8, 0x00, 0x3f, 0xc0, 0x03, 0xf8, 0x00, 
	0x7f, 0x80, 0x03, 0xfc, 0x00, 0x7f, 0x80, 0x01, 0xfe, 0x00, 0xff, 0x00, 0x01, 0xff, 0x83, 0xff, 
	0x00, 0x00, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x7f, 0xff, 0xfc, 0x00, 0x00, 0x3f, 0xff, 0xf8, 0x00, 
	0x00, 0x1f, 0xff, 0xf0, 0x00, 0x00, 0x07, 0xff, 0xe0, 0x00, 0x00, 0x01, 0xff, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,},
// 'OpenSanseSemi1', 40x56px
{	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xf0, 0x00, 0x00, 0x00, 
	0x0f, 0xf0, 0x00, 0x00, 0x00, 0x3f, 0xf0, 0x00, 0x00, 0x00, 0x7f, 0xf0, 0x00, 0x00, 0x00, 0xff, 
	0xf0, 0x00, 0x00, 0x01, 0xff, 0xf0, 0x00, 0x00, 0x07, 0xff, 0xf0, 0x00, 0x00, 0x0f, 0xff, 0xf0, 
	0x00, 0x00, 0x1f, 0xff, 0xf0, 0x00, 0x00, 0x7f, 0xef, 0xf0, 0x00, 0x00, 0xff, 0xcf, 0xf0, 0x00, 
	0x00, 0x7f, 0x0f, 0xf0, 0x00, 0x00, 0x7e, 0x0f, 0xf0, 0x00, 0x00, 0x3c, 0x0f, 0xf0, 0x00, 0x00, 
	0x18, 0x0f, 0xf0, 0x00, 0x00, 0x00, 0x0f, 0xf0, 0x00, 0x00, 0x00, 0x0f, 0xf0, 0x00, 0x00, 0x00, 
	0x0f, 0xf0, 0x00, 0x00, 0x00, 0x0f, 0xf0, 0x00, 0x00, 0x00, 0x0f, 0xf0, 0x00, 0x00, 0x00, 0x0f, 
	0xf0, 0x00, 0x00, 0x00, 0x0f, 0xf0, 0x00, 0x00, 0x00, 0x0f, 0xf0, 0x00, 0x00, 0x00, 0x0f, 0xf0, 
	0x00, 0x00, 0x00, 0x0f, 0xf0, 0x00, 0x00, 0x00, 0x0f, 0xf0, 0x00, 0x00, 0x00, 0x0f, 0xf0, 0x00, 
	0x00, 0x00, 0x0f, 0xf0, 0x00, 0x00, 0x00, 0x0f, 0xf0, 0x00, 0x00, 0x00, 0x0f, 0xf0, 0x00, 0x00, 
	0x00, 0x0f, 0xf0, 0x00, 0x00, 0x00, 0x0f, 0xf0, 0x00, 0x00, 0x00, 0x0f, 0xf0, 0x00, 0x00, 0x00, 
	0x0f, 0xf0, 0x00, 0x00, 0x00, 0x0f, 0xf0, 0x00, 0x00, 0x00, 0x0f, 0xf0, 0x00, 0x00, 0x00, 0x0f, 
	0xf0, 0x00, 0x00, 0x00, 0x0f, 0xf0, 0x00, 0x00, 0x00, 0x0f, 0xf0, 0x00, 0x00, 0x00, 0x0f, 0xf0, 
	0x00, 0x00, 0x00, 0x0f, 0xf0, 0x00, 0x00, 0x00, 0x0f, 0xf0, 0x00, 0x00, 0x00, 0x0f, 0xf0, 0x00, 
	0x00, 0x00, 0x0f, 0xf0, 0x00, 0x00, 0x00, 0x0f, 0xf0, 0x00, 0x00, 0x00, 0x0f, 0xf0, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,},
 // 'OpenSanseSemi2', 40x56px
 {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0x00, 0x00, 0x00, 0x1f, 
	0xff, 0xe0, 0x00, 0x00, 0x7f, 0xff, 0xf8, 0x00, 0x01, 0xff, 0xff, 0xfc, 0x00, 0x03, 0xff, 0xff, 
	0xfe, 0x00, 0x0f, 0xff, 0xff, 0xff, 0x00, 0x07, 0xfe, 0x03, 0xff, 0x00, 0x03, 0xf8, 0x00, 0xff, 
	0x80, 0x03, 0xe0, 0x00, 0x7f, 0x80, 0x01, 0x80, 0x00, 0x3f, 0x80, 0x00, 0x00, 0x00, 0x3f, 0x80, 
	0x00, 0x00, 0x00, 0x3f, 0xc0, 0x00, 0x00, 0x00, 0x3f, 0xc0, 0x00, 0x00, 0x00, 0x3f, 0xc0, 0x00, 
	0x00, 0x00, 0x3f, 0x80, 0x00, 0x00, 0x00, 0x3f, 0x80, 0x00, 0x00, 0x00, 0x3f, 0x80, 0x00, 0x00, 
	0x00, 0x7f, 0x80, 0x00, 0x00, 0x00, 0x7f, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x01, 
	0xfe, 0x00, 0x00, 0x00, 0x01, 0xfe, 0x00, 0x00, 0x00, 0x03, 0xfc, 0x00, 0x00, 0x00, 0x07, 0xf8, 
	0x00, 0x00, 0x00, 0x0f, 0xf8, 0x00, 0x00, 0x00, 0x1f, 0xf0, 0x00, 0x00, 0x00, 0x3f, 0xe0, 0x00, 
	0x00, 0x00, 0x7f, 0xc0, 0x00, 0x00, 0x00, 0xff, 0x80, 0x00, 0x00, 0x01, 0xff, 0x00, 0x00, 0x00, 
	0x03, 0xfe, 0x00, 0x00, 0x00, 0x07, 0xfc, 0x00, 0x00, 0x00, 0x0f, 0xf8, 0x00, 0x00, 0x00, 0x1f, 
	0xf0, 0x00, 0x00, 0x00, 0x3f, 0xe0, 0x00, 0x00, 0x00, 0x7f, 0xc0, 0x00, 0x00, 0x00, 0xff, 0x00, 
	0x00, 0x00, 0x01, 0xfe, 0x00, 0x00, 0x00, 0x03, 0xfc, 0x00, 0x00, 0x00, 0x07, 0xf8, 0x00, 0x00, 
	0x00, 0x0f, 0xff, 0xff, 0xff, 0xe0, 0x0f, 0xff, 0xff, 0xff, 0xe0, 0x0f, 0xff, 0xff, 0xff, 0xe0, 
	0x0f, 0xff, 0xff, 0xff, 0xe0, 0x0f, 0xff, 0xff, 0xff, 0xe0, 0x0f, 0xff, 0xff, 0xff, 0xe0, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,},
 // 'OpenSanseSemi3', 40x56px
 {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xe0, 0x00, 0x01, 0xff, 
	0xff, 0xf8, 0x00, 0x07, 0xff, 0xff, 0xfc, 0x00, 0x0f, 0xff, 0xff, 0xfe, 0x00, 0x07, 0xff, 0xff, 
	0xff, 0x00, 0x07, 0xfc, 0x03, 0xff, 0x00, 0x03, 0xe0, 0x00, 0xff, 0x80, 0x01, 0x80, 0x00, 0x7f, 
	0x80, 0x01, 0x00, 0x00, 0x3f, 0x80, 0x00, 0x00, 0x00, 0x3f, 0x80, 0x00, 0x00, 0x00, 0x3f, 0x80, 
	0x00, 0x00, 0x00, 0x3f, 0x80, 0x00, 0x00, 0x00, 0x3f, 0x80, 0x00, 0x00, 0x00, 0x3f, 0x80, 0x00, 
	0x00, 0x00, 0x7f, 0x80, 0x00, 0x00, 0x00, 0x7f, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 
	0x01, 0xfe, 0x00, 0x00, 0x00, 0x0f, 0xfc, 0x00, 0x00, 0x1f, 0xff, 0xf8, 0x00, 0x00, 0x1f, 0xff, 
	0xe0, 0x00, 0x00, 0x1f, 0xff, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xe0, 0x00, 0x00, 0x1f, 0xff, 0xfc, 
	0x00, 0x00, 0x1f, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x07, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0x80, 
	0x00, 0x00, 0x00, 0x7f, 0xc0, 0x00, 0x00, 0x00, 0x3f, 0xc0, 0x00, 0x00, 0x00, 0x1f, 0xe0, 0x00, 
	0x00, 0x00, 0x1f, 0xe0, 0x00, 0x00, 0x00, 0x1f, 0xe0, 0x00, 0x00, 0x00, 0x1f, 0xe0, 0x00, 0x00, 
	0x00, 0x1f, 0xe0, 0x00, 0x00, 0x00, 0x1f, 0xe0, 0x00, 0x00, 0x00, 0x3f, 0xe0, 0x00, 0x00, 0x00, 
	0x3f, 0xc0, 0x08, 0x00, 0x00, 0x7f, 0xc0, 0x0e, 0x00, 0x00, 0xff, 0x80, 0x0f, 0xe0, 0x07, 0xff, 
	0x80, 0x0f, 0xff, 0xff, 0xff, 0x00, 0x0f, 0xff, 0xff, 0xfe, 0x00, 0x0f, 0xff, 0xff, 0xfc, 0x00, 
	0x0f, 0xff, 0xff, 0xf0, 0x00, 0x03, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x3f, 0xfe, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,},
// 'OpenSanseSemi4', 40x56px
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xfe, 0x00, 0x00, 0x00, 
	0x03, 0xfe, 0x00, 0x00, 0x00, 0x03, 0xfe, 0x00, 0x00, 0x00, 0x07, 0xfe, 0x00, 0x00, 0x00, 0x0f, 
	0xfe, 0x00, 0x00, 0x00, 0x0f, 0xfe, 0x00, 0x00, 0x00, 0x1f, 0xfe, 0x00, 0x00, 0x00, 0x3f, 0xfe, 
	0x00, 0x00, 0x00, 0x7e, 0xfe, 0x00, 0x00, 0x00, 0x7e, 0xfe, 0x00, 0x00, 0x00, 0xfc, 0xfe, 0x00, 
	0x00, 0x01, 0xfd, 0xfe, 0x00, 0x00, 0x01, 0xf9, 0xfe, 0x00, 0x00, 0x03, 0xf1, 0xfe, 0x00, 0x00, 
	0x07, 0xf1, 0xfe, 0x00, 0x00, 0x07, 0xe1, 0xfe, 0x00, 0x00, 0x0f, 0xc1, 0xfe, 0x00, 0x00, 0x1f, 
	0xc1, 0xfe, 0x00, 0x00, 0x1f, 0x81, 0xfe, 0x00, 0x00, 0x3f, 0x01, 0xfe, 0x00, 0x00, 0x7f, 0x01, 
	0xfe, 0x00, 0x00, 0xfe, 0x01, 0xfe, 0x00, 0x00, 0xfc, 0x01, 0xfe, 0x00, 0x01, 0xfc, 0x01, 0xfe, 
	0x00, 0x03, 0xf8, 0x01, 0xfe, 0x00, 0x03, 0xf0, 0x01, 0xfe, 0x00, 0x07, 0xe0, 0x01, 0xfe, 0x00, 
	0x0f, 0xe0, 0x01, 0xfe, 0x00, 0x0f, 0xc0, 0x01, 0xfe, 0x00, 0x1f, 0x80, 0x01, 0xfe, 0x00, 0x3f, 
	0xff, 0xff, 0xff, 0xf8, 0x3f, 0xff, 0xff, 0xff, 0xf8, 0x3f, 0xff, 0xff, 0xff, 0xf8, 0x3f, 0xff, 
	0xff, 0xff, 0xf8, 0x3f, 0xff, 0xff, 0xff, 0xf8, 0x3f, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x01, 
	0xfe, 0x00, 0x00, 0x00, 0x01, 0xfe, 0x00, 0x00, 0x00, 0x01, 0xfe, 0x00, 0x00, 0x00, 0x01, 0xfe, 
	0x00, 0x00, 0x00, 0x01, 0xfe, 0x00, 0x00, 0x00, 0x01, 0xfe, 0x00, 0x00, 0x00, 0x01, 0xfe, 0x00, 
	0x00, 0x00, 0x01, 0xfe, 0x00, 0x00, 0x00, 0x01, 0xfe, 0x00, 0x00, 0x00, 0x01, 0xfe, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, },
// 'OpenSanseSemi5', 40x56px
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xfe, 0x00, 0x01, 0xff, 0xff, 0xfe, 0x00, 0x01, 0xff, 
	0xff, 0xfe, 0x00, 0x01, 0xff, 0xff, 0xfe, 0x00, 0x01, 0xff, 0xff, 0xfe, 0x00, 0x01, 0xff, 0xff, 
	0xfe, 0x00, 0x01, 0xfc, 0x00, 0x00, 0x00, 0x01, 0xfc, 0x00, 0x00, 0x00, 0x01, 0xfc, 0x00, 0x00, 
	0x00, 0x01, 0xfc, 0x00, 0x00, 0x00, 0x01, 0xf8, 0x00, 0x00, 0x00, 0x01, 0xf8, 0x00, 0x00, 0x00, 
	0x03, 0xf8, 0x00, 0x00, 0x00, 0x03, 0xf8, 0x00, 0x00, 0x00, 0x03, 0xf8, 0x00, 0x00, 0x00, 0x03, 
	0xf8, 0x00, 0x00, 0x00, 0x03, 0xf8, 0x00, 0x00, 0x00, 0x03, 0xfb, 0xff, 0x00, 0x00, 0x03, 0xff, 
	0xff, 0xe0, 0x00, 0x03, 0xff, 0xff, 0xf8, 0x00, 0x03, 0xff, 0xff, 0xfc, 0x00, 0x03, 0xff, 0xff, 
	0xfe, 0x00, 0x03, 0xff, 0xff, 0xff, 0x00, 0x00, 0xe0, 0x07, 0xff, 0x80, 0x00, 0x00, 0x00, 0xff, 
	0x80, 0x00, 0x00, 0x00, 0x7f, 0xc0, 0x00, 0x00, 0x00, 0x3f, 0xc0, 0x00, 0x00, 0x00, 0x3f, 0xc0, 
	0x00, 0x00, 0x00, 0x1f, 0xe0, 0x00, 0x00, 0x00, 0x1f, 0xe0, 0x00, 0x00, 0x00, 0x1f, 0xe0, 0x00, 
	0x00, 0x00, 0x1f, 0xe0, 0x00, 0x00, 0x00, 0x1f, 0xe0, 0x00, 0x00, 0x00, 0x1f, 0xe0, 0x00, 0x00, 
	0x00, 0x1f, 0xc0, 0x00, 0x00, 0x00, 0x1f, 0xc0, 0x00, 0x00, 0x00, 0x3f, 0xc0, 0x00, 0x00, 0x00, 
	0x3f, 0xc0, 0x04, 0x00, 0x00, 0x7f, 0x80, 0x07, 0x00, 0x01, 0xff, 0x80, 0x07, 0xf0, 0x07, 0xff, 
	0x00, 0x07, 0xff, 0xff, 0xfe, 0x00, 0x07, 0xff, 0xff, 0xfc, 0x00, 0x07, 0xff, 0xff, 0xf8, 0x00, 
	0x07, 0xff, 0xff, 0xf0, 0x00, 0x01, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x1f, 0xfc, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,},
// 'OpenSanseSemi6', 40x56px
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0x00, 0x00, 0x00, 0x7f, 0xff, 0x00, 0x00, 0x01, 
	0xff, 0xff, 0x00, 0x00, 0x07, 0xff, 0xff, 0x00, 0x00, 0x0f, 0xff, 0xff, 0x00, 0x00, 0x1f, 0xff, 
	0xff, 0x00, 0x00, 0x3f, 0xfc, 0x01, 0x00, 0x00, 0x7f, 0xe0, 0x00, 0x00, 0x00, 0xff, 0x80, 0x00, 
	0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x01, 0xfe, 0x00, 0x00, 0x00, 0x01, 0xfc, 0x00, 0x00, 0x00, 
	0x03, 0xf8, 0x00, 0x00, 0x00, 0x03, 0xf8, 0x00, 0x00, 0x00, 0x07, 0xf0, 0x00, 0x00, 0x00, 0x07, 
	0xf0, 0x00, 0x00, 0x00, 0x07, 0xf0, 0x00, 0x00, 0x00, 0x07, 0xf0, 0x3f, 0xc0, 0x00, 0x07, 0xe1, 
	0xff, 0xf8, 0x00, 0x0f, 0xe3, 0xff, 0xfc, 0x00, 0x0f, 0xe7, 0xff, 0xff, 0x00, 0x0f, 0xef, 0xff, 
	0xff, 0x00, 0x0f, 0xff, 0xff, 0xff, 0x80, 0x0f, 0xff, 0x80, 0xff, 0xc0, 0x0f, 0xfe, 0x00, 0x7f, 
	0xc0, 0x0f, 0xfc, 0x00, 0x3f, 0xe0, 0x0f, 0xf8, 0x00, 0x1f, 0xe0, 0x0f, 0xf0, 0x00, 0x0f, 0xe0, 
	0x0f, 0xf0, 0x00, 0x0f, 0xe0, 0x0f, 0xe0, 0x00, 0x0f, 0xe0, 0x0f, 0xe0, 0x00, 0x0f, 0xe0, 0x0f, 
	0xe0, 0x00, 0x0f, 0xe0, 0x0f, 0xf0, 0x00, 0x0f, 0xe0, 0x07, 0xf0, 0x00, 0x0f, 0xe0, 0x07, 0xf0, 
	0x00, 0x0f, 0xe0, 0x07, 0xf0, 0x00, 0x0f, 0xe0, 0x07, 0xf8, 0x00, 0x1f, 0xe0, 0x03, 0xfc, 0x00, 
	0x1f, 0xc0, 0x03, 0xfc, 0x00, 0x3f, 0xc0, 0x01, 0xfe, 0x00, 0x7f, 0xc0, 0x01, 0xff, 0x81, 0xff, 
	0x80, 0x00, 0xff, 0xff, 0xff, 0x00, 0x00, 0x7f, 0xff, 0xff, 0x00, 0x00, 0x3f, 0xff, 0xfe, 0x00, 
	0x00, 0x1f, 0xff, 0xf8, 0x00, 0x00, 0x07, 0xff, 0xf0, 0x00, 0x00, 0x00, 0xff, 0x80, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, },
// 'OpenSanseSemi7', 40x56px
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xff, 0xf8, 0x07, 0xff, 
	0xff, 0xff, 0xf8, 0x07, 0xff, 0xff, 0xff, 0xf8, 0x07, 0xff, 0xff, 0xff, 0xf8, 0x07, 0xff, 0xff, 
	0xff, 0xf8, 0x07, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x07, 0xf8, 0x00, 0x00, 0x00, 0x07, 
	0xf0, 0x00, 0x00, 0x00, 0x0f, 0xf0, 0x00, 0x00, 0x00, 0x0f, 0xe0, 0x00, 0x00, 0x00, 0x1f, 0xe0, 
	0x00, 0x00, 0x00, 0x1f, 0xe0, 0x00, 0x00, 0x00, 0x1f, 0xc0, 0x00, 0x00, 0x00, 0x3f, 0xc0, 0x00, 
	0x00, 0x00, 0x3f, 0x80, 0x00, 0x00, 0x00, 0x7f, 0x80, 0x00, 0x00, 0x00, 0x7f, 0x00, 0x00, 0x00, 
	0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x00, 0x00, 0x00, 0x01, 0xfe, 0x00, 0x00, 0x00, 0x01, 
	0xfc, 0x00, 0x00, 0x00, 0x03, 0xfc, 0x00, 0x00, 0x00, 0x03, 0xfc, 0x00, 0x00, 0x00, 0x07, 0xf8, 
	0x00, 0x00, 0x00, 0x07, 0xf8, 0x00, 0x00, 0x00, 0x07, 0xf0, 0x00, 0x00, 0x00, 0x0f, 0xf0, 0x00, 
	0x00, 0x00, 0x0f, 0xe0, 0x00, 0x00, 0x00, 0x1f, 0xe0, 0x00, 0x00, 0x00, 0x1f, 0xc0, 0x00, 0x00, 
	0x00, 0x3f, 0xc0, 0x00, 0x00, 0x00, 0x3f, 0xc0, 0x00, 0x00, 0x00, 0x7f, 0x80, 0x00, 0x00, 0x00, 
	0x7f, 0x80, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x01, 0xfe, 
	0x00, 0x00, 0x00, 0x01, 0xfe, 0x00, 0x00, 0x00, 0x01, 0xfc, 0x00, 0x00, 0x00, 0x03, 0xfc, 0x00, 
	0x00, 0x00, 0x03, 0xfc, 0x00, 0x00, 0x00, 0x07, 0xf8, 0x00, 0x00, 0x00, 0x07, 0xf8, 0x00, 0x00, 
	0x00, 0x0f, 0xf0, 0x00, 0x00, 0x00, 0x0f, 0xf0, 0x00, 0x00, 0x00, 0x1f, 0xe0, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, },
// 'OpenSanseSemi8', 40x56px
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x80, 0x00, 0x00, 0x07, 0xff, 0xf0, 0x00, 0x00, 0x1f, 
	0xff, 0xfc, 0x00, 0x00, 0x7f, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 0xff, 0x80, 0x00, 0xff, 0xff, 
	0xff, 0x80, 0x01, 0xff, 0x80, 0xff, 0xc0, 0x01, 0xfe, 0x00, 0x3f, 0xc0, 0x03, 0xfc, 0x00, 0x1f, 
	0xc0, 0x03, 0xfc, 0x00, 0x1f, 0xe0, 0x03, 0xfc, 0x00, 0x1f, 0xe0, 0x03, 0xf8, 0x00, 0x1f, 0xe0, 
	0x03, 0xfc, 0x00, 0x1f, 0xe0, 0x01, 0xfc, 0x00, 0x1f, 0xc0, 0x01, 0xfc, 0x00, 0x1f, 0xc0, 0x01, 
	0xfe, 0x00, 0x3f, 0xc0, 0x00, 0xff, 0x00, 0x7f, 0x80, 0x00, 0xff, 0xc0, 0xff, 0x00, 0x00, 0x7f, 
	0xf3, 0xfe, 0x00, 0x00, 0x3f, 0xff, 0xfc, 0x00, 0x00, 0x1f, 0xff, 0xf8, 0x00, 0x00, 0x07, 0xff, 
	0xe0, 0x00, 0x00, 0x07, 0xff, 0xe0, 0x00, 0x00, 0x0f, 0xff, 0xf8, 0x00, 0x00, 0x3f, 0xff, 0xfe, 
	0x00, 0x00, 0x7f, 0xef, 0xff, 0x00, 0x00, 0xff, 0x83, 0xff, 0x80, 0x01, 0xff, 0x00, 0xff, 0xc0, 
	0x03, 0xfe, 0x00, 0x3f, 0xc0, 0x03, 0xfc, 0x00, 0x1f, 0xe0, 0x07, 0xf8, 0x00, 0x0f, 0xf0, 0x07, 
	0xf0, 0x00, 0x0f, 0xf0, 0x07, 0xf0, 0x00, 0x07, 0xf0, 0x07, 0xf0, 0x00, 0x07, 0xf0, 0x07, 0xf0, 
	0x00, 0x07, 0xf0, 0x07, 0xf0, 0x00, 0x07, 0xf0, 0x07, 0xf0, 0x00, 0x07, 0xf0, 0x07, 0xf8, 0x00, 
	0x0f, 0xf0, 0x07, 0xfc, 0x00, 0x1f, 0xf0, 0x03, 0xfe, 0x00, 0x3f, 0xe0, 0x03, 0xff, 0x80, 0xff, 
	0xe0, 0x01, 0xff, 0xff, 0xff, 0xc0, 0x00, 0xff, 0xff, 0xff, 0x80, 0x00, 0x7f, 0xff, 0xff, 0x00, 
	0x00, 0x3f, 0xff, 0xfe, 0x00, 0x00, 0x0f, 0xff, 0xf8, 0x00, 0x00, 0x01, 0xff, 0xc0, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, },
// 'OpenSanseSemi9', 40x56px
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0x00, 0x00, 0x00, 0x07, 0xff, 0xe0, 0x00, 0x00, 0x1f, 
	0xff, 0xf8, 0x00, 0x00, 0x3f, 0xff, 0xfc, 0x00, 0x00, 0x7f, 0xff, 0xfe, 0x00, 0x00, 0xff, 0xff, 
	0xff, 0x00, 0x01, 0xff, 0x81, 0xff, 0x80, 0x03, 0xfe, 0x00, 0x7f, 0x80, 0x03, 0xfc, 0x00, 0x3f, 
	0xc0, 0x03, 0xf8, 0x00, 0x1f, 0xc0, 0x07, 0xf8, 0x00, 0x1f, 0xe0, 0x07, 0xf0, 0x00, 0x0f, 0xe0, 
	0x07, 0xf0, 0x00, 0x0f, 0xe0, 0x07, 0xf0, 0x00, 0x0f, 0xf0, 0x07, 0xf0, 0x00, 0x07, 0xf0, 0x07, 
	0xf0, 0x00, 0x07, 0xf0, 0x07, 0xf0, 0x00, 0x07, 0xf0, 0x07, 0xf0, 0x00, 0x07, 0xf0, 0x07, 0xf0, 
	0x00, 0x0f, 0xf0, 0x07, 0xf8, 0x00, 0x0f, 0xf0, 0x07, 0xf8, 0x00, 0x1f, 0xf0, 0x03, 0xfc, 0x00, 
	0x3f, 0xf0, 0x03, 0xfe, 0x00, 0x7f, 0xf0, 0x03, 0xff, 0x01, 0xff, 0xf0, 0x01, 0xff, 0xff, 0xff, 
	0xf0, 0x00, 0xff, 0xff, 0xf7, 0xf0, 0x00, 0x7f, 0xff, 0xe7, 0xf0, 0x00, 0x3f, 0xff, 0xc7, 0xf0, 
	0x00, 0x1f, 0xff, 0x87, 0xf0, 0x00, 0x03, 0xfc, 0x07, 0xe0, 0x00, 0x00, 0x00, 0x0f, 0xe0, 0x00, 
	0x00, 0x00, 0x0f, 0xe0, 0x00, 0x00, 0x00, 0x0f, 0xe0, 0x00, 0x00, 0x00, 0x1f, 0xc0, 0x00, 0x00, 
	0x00, 0x1f, 0xc0, 0x00, 0x00, 0x00, 0x3f, 0xc0, 0x00, 0x00, 0x00, 0x7f, 0x80, 0x00, 0x00, 0x00, 
	0x7f, 0x80, 0x00, 0x00, 0x01, 0xff, 0x00, 0x00, 0x00, 0x03, 0xfe, 0x00, 0x00, 0x80, 0x1f, 0xfc, 
	0x00, 0x00, 0xff, 0xff, 0xfc, 0x00, 0x00, 0xff, 0xff, 0xf0, 0x00, 0x00, 0xff, 0xff, 0xe0, 0x00, 
	0x00, 0xff, 0xff, 0xc0, 0x00, 0x00, 0xff, 0xfe, 0x00, 0x00, 0x00, 0xff, 0xf0, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, },
// 'OpenSansSemDvojbodka', 40x56px
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x7c, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 
	0x00, 0x00, 0x01, 0xff, 0x00, 0x00, 0x00, 0x01, 0xff, 0x80, 0x00, 0x00, 0x01, 0xff, 0x80, 0x00, 
	0x00, 0x01, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x00, 0x00, 0x00, 
	0x00, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x00, 0x00, 0x00, 0x00, 
	0xff, 0x00, 0x00, 0x00, 0x01, 0xff, 0x00, 0x00, 0x00, 0x01, 0xff, 0x80, 0x00, 0x00, 0x01, 0xff, 
	0x80, 0x00, 0x00, 0x01, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x00, 
	0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, },
  };
