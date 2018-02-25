#define MAGIC_TYPE 'J'
/*1. Define command : _IO(type,nr) */
#define LED_ON _IO(MAGIC_TYPE,0)
#define LED_OFF _IO(MAGIC_TYPE,1)
