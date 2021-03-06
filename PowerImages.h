#include <avr/pgmspace.h>

#define BatteryIconHeight	 12 // Y Axis
#define BatteryIconWidth	 22 // X Axis
#define BatteryIconXOffset	 105
#define BatteryIconYOffset	 0
#define MosfetIconHeight	 12 // Y Axis
#define MosfetIconWidth	     12 // X Axis
#define MosfetIconXOffset	 90
#define MosfetIconYOffset	 0
#define Fan1IconHeight	 12 // Y Axis
#define Fan1IconWidth	     12 // X Axis
#define Fan1IconXOffset	 78
#define Fan1IconYOffset	 0
static const unsigned char PROGMEM Battery100Percent_bmp[] =
{
0xff,0xff,0xe0,
0x80,0x0,0x30,
0xbb,0xbb,0xb0,
0xaa,0xaa,0xbc,
0xaa,0xaa,0xa4,
0xaa,0xaa,0xa4,
0xaa,0xaa,0xa4,
0xaa,0xaa,0xa4,
0xaa,0xaa,0xbc,
0xbb,0xbb,0xb0,
0x80,0x0,0x30,
0xff,0xff,0xe0
};

static const unsigned char PROGMEM Battery75Percent_bmp[] =
{
0xff,0xff,0xe0,
0x80,0x0,0x30,
0xbb,0xb8,0x30,
0xaa,0xa8,0x3c,
0xaa,0xa8,0x24,
0xaa,0xa8,0x24,
0xaa,0xa8,0x24,
0xaa,0xa8,0x24,
0xaa,0xa8,0x3c,
0xbb,0xb8,0x30,
0x80,0x0,0x30,
0xff,0xff,0xe0
};

static const unsigned char PROGMEM Battery50Percent_bmp[] =
{
0xff,0xff,0xe0,
0x80,0x0,0x30,
0xbb,0x80,0x30,
0xaa,0x80,0x3c,
0xaa,0x80,0x24,
0xaa,0x80,0x24,
0xaa,0x80,0x24,
0xaa,0x80,0x24,
0xaa,0x80,0x3c,
0xbb,0x80,0x30,
0x80,0x0,0x30,
0xff,0xff,0xe0
};

static const unsigned char PROGMEM Battery25Percent_bmp[] =
{
0xff,0xff,0xe0,
0x80,0x0,0x30,
0xb8,0x0,0x30,
0xa8,0x0,0x3c,
0xa8,0x0,0x24,
0xa8,0x0,0x24,
0xa8,0x0,0x24,
0xa8,0x0,0x24,
0xa8,0x0,0x3c,
0xb8,0x0,0x30,
0x80,0x0,0x30,
0xff,0xff,0xe0
};

static const unsigned char PROGMEM Battery0Percent_bmp[] =
{
0xff,0xff,0xe0,
0x80,0x0,0x30,
0x80,0x0,0x30,
0x80,0x0,0x3c,
0x80,0x0,0x24,
0x80,0x0,0x24,
0x80,0x0,0x24,
0x80,0x0,0x24,
0x80,0x0,0x3c,
0x80,0x0,0x30,
0x80,0x0,0x30,
0xff,0xff,0xe0
};

static const unsigned char PROGMEM MOSFETOn_bmp[] =
{
0x0,0x0,
0x4,0x40,
0x4,0x40,
0x7,0xc0,
0x4,0x0,
0x7c,0x0,
0x7c,0x0,
0x4,0x0,
0x7,0xc0,
0x4,0x40,
0x4,0x40,
0x0,0x0
};

static const unsigned char PROGMEM MOSFETOff_bmp[] =
{
0x0,0x0,
0x8,0x10,
0x8,0x10,
0x8,0xf0,
0x8,0x0,
0xf8,0x0,
0xf8,0x0,
0x8,0x0,
0x8,0xf0,
0x8,0x10,
0x8,0x10,
0x0,0x0
};

static const unsigned char PROGMEM FANOn1_bmp[] =
{
0x14,0x0,
0x26,0x0,
0x46,0x0,
0x46,0x0,
0xf,0x0,
0x79,0xf0,
0xf9,0xe0,
0xf,0x0,
0x6,0x20,
0x6,0x20,
0x6,0x40,
0x2,0x80
};

static const unsigned char PROGMEM FANOn2_bmp[] =
{
0x4,0x0,
0x6,0xc0,
0x6,0x20,
0x6,0x10,
0xf,0x0,
0x79,0xf0,
0xf9,0xe0,
0xf,0x0,
0x86,0x0,
0x46,0x0,
0x36,0x0,
0x2,0x0
};

static const unsigned char PROGMEM FANOff_bmp[] =
{
0x4,0x0,
0x6,0x0,
0x6,0x0,
0x6,0x0,
0xf,0x0,
0x79,0xf0,
0xf9,0xe0,
0xf,0x0,
0x6,0x0,
0x6,0x0,
0x6,0x0,
0x2,0x0
};