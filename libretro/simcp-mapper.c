#include "libretro.h"

#include "libretro-simcp.h"

#include "graph.h"
#include "vkbd.h"

#ifdef PS3PORT
#include "sys/sys_time.h"
#include "sys/timer.h"
#define usleep  sys_timer_usleep
#else
#include <sys/types.h>
#include <sys/time.h>
#include <time.h>
#endif

extern const char keyboard_translation[320];

extern char * filebrowser(const char *path_and_name);

unsigned short int bmp[TEX_WIDTH * TEX_HEIGHT];

int NPAGE=-1, KCOL=1, BKGCOLOR=0, MAXPAS=6;
int SHIFTON=-1,MOUSEMODE=-1,SHOWKEY=-1,PAS=2,STATUTON=-1;
extern int RVSYNC;

short signed int SNDBUF[1024*2];
int snd_sampler = 22050 / 60;
char RPATH[512];

int gmx=320,gmy=240; //gui mouse unused

unsigned char MXjoy0; // joy 
int touch=-1; // gui mouse btn unused
int fmousex,fmousey; // emu mouse unused
int pauseg=0; //enter_gui
int SND=1; //SOUND ON/OFF
int NUMjoy=1; //used for Joy  1/2 flag

static int mbt[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

long frame=0;
unsigned long  Ktime=0 , LastFPSTime=0;

int BOXDEC= 32+2;
int STAT_BASEY=CROP_HEIGHT;

static retro_input_state_t input_state_cb;
static retro_input_poll_t input_poll_cb;

void retro_set_input_state(retro_input_state_t cb)
{
   	input_state_cb = cb;
}

void retro_set_input_poll(retro_input_poll_t cb)
{
   	input_poll_cb = cb;
}

void texture_init(){

	memset(bmp, 0, sizeof(bmp));
}

void Emu_init(){
	
}

void Emu_uninit(){


}

int RVSYNC=0;

void retroloop(){}
void loadfirst(){}
void retrostop(){}
void retro_reset_msx(){}
void retro_savestate_msx(){}

void retro_key_down(unsigned char retrok){
 
	//XKBD_SET(retrok); 
Keyboard_SetKey (retrok,  true,16,retrok);
}
 
void retro_key_up(unsigned char retrok){
 
	//XKBD_RES(retrok);
Keyboard_SetKey (retrok, false,16,retrok);
}


#ifdef AND
#define DEFAULT_PATH "/mnt/sdcard/fmsx/"
#else
#define DEFAULT_PATH "/"
#endif

#define MDEBUG
#ifdef MDEBUG
#define mprintf printf
#else
#define mprintf(...) 
#endif

long GetTicks(void)
{
#ifndef _ANDROID_

#ifdef PS3PORT

	//#warning "GetTick PS3\n"

	unsigned long	ticks_micro;
	uint64_t secs;
	uint64_t nsecs;

	sys_time_get_current_time(&secs, &nsecs);
	ticks_micro =  secs * 1000000UL + (nsecs / 1000);

	return ticks_micro;
#else
   	struct timeval tv;
   	gettimeofday (&tv, NULL);
   	return tv.tv_sec*1000000 + tv.tv_usec;
#endif

#else

    	struct timespec now;
    	clock_gettime(CLOCK_MONOTONIC, &now);
    	return now.tv_sec*1000000 + now.tv_nsec/1000;
#endif
                                                                              
} 


void retro_fillsample(signed short *buf,int sz){

	signed short *prt=buf,val;
	int i;

	for(i=0;i<sz;i++){
		val=*(prt++);		
		retro_audio_cb( val,val);
	}
}

//DBG LOAD DSK UNUSED
void enter_gui(){	
	
	char dskimg[512]="\0";
	static int inbrowser=1;
	int ret=0;	

	sprintf(dskimg,"%s\0",filebrowser(DEFAULT_PATH));

	if(strcmp(dskimg,"EMPTY\0")==0){
		mprintf("Cancel Fileselect(%s)\n",dskimg);
		inbrowser=0;	
		pauseg=0;
		Screen_SetFullUpdate();

		return;		
	}
	else if(strcmp(dskimg,"NO CHOICE\0")==0){
			
	}
	else{
		mprintf("Ok Fileselect(%s)\n",dskimg);		
	
	  	//loadadsk(dskimg);

		inbrowser=0;
		pauseg=0;
		Screen_SetFullUpdate();

		return;
	}		
	
}

int tomx=320,tomy=200;

void Print_Statut(){

	DrawFBoxBmp(bmp,0,CROP_HEIGHT,CROP_WIDTH,STAT_YSZ,RGB565(0,0,0));

	Draw_text(bmp,STAT_DECX+40 ,10,0xffff,0x8080,1,2,40,(SND>0?"SND":"  "));
        Draw_text(bmp,STAT_DECX+80 ,10/*STAT_BASEY*/,0xffff,0x8080,1,2,40,(RVSYNC>0?"SYNC ON ":"SYNC OFF"));
	Draw_text(bmp,STAT_DECX+120,10,0xffff,0x8080,1,2,40,"JOY%c",NUMjoy>0?'1':'2');
        Draw_text(bmp,STAT_DECX+40 ,20,0xffff,0x8080,1,2,40,"mx:%d my:%d (%d,%d)",tomx,tomy,fmousex,fmousey);

}

/*
L2  STATUS ON/OFF
R2  SND ON/OFF
L   VKBD ON/OFF
R   RESET
SEL VSYNC ON/OFF
STR mousemode
A   FIREA/VKBD KEY
B   FIREB
X   ??
Y   fMSX Menu
*/


void Screen_SetFullUpdate(){	
	//memset(bmp, 0, sizeof(bmp));
}

void retro_mouse(int a,int b){    
	
	tomx+=a;
	if(tomx>639)tomx=639;
	if(tomx<0)tomx=0;

	tomy+=b;
	if(tomy>479)tomy=479;
	if(tomy<0)tomy=0;

	//to8_HandleMouseMotion(tomx,tomy);
}

/*
void mbup(int x,int y){

	GUI::SendMessage(GM_BUTTONUP, x, y);
}

void mbdown(int x,int y){

	GUI::SendMessage(GM_BUTTONDOWN, x, y); 

}

int GUI_ACTIVE(){
	if (GUI::IsActive())return 1;
	return 0;
}
*/

void retro_mouse_but0(int a,int x,int y){
	if(a!=1) mbup( x, y);
	else  mbdown( x, y);
}

void retro_mouse_but1(int a,int x,int y){
	

}

void UnInitOSGLU(){}

int InitOSGLU(void)
{
 	Smain(1,NULL);
}

/*
void retro_key_down(unsigned char retrok){
 
	 
}
 
void retro_key_up(unsigned char retrok){
 
	 
}
*/

void update_input(void)
{
	int i;	
	//   RETRO      B    Y    SLT  STA  UP   DWN  LEFT RGT  A    X    L    R    L2   R2   L3   R3
        //   INDEX      0    1    2    3    4    5    6    7    8    9    10   11   12   13   14   15
	static int vbt[16]={0x20,0x39,0x01,0x3B,0x01,0x02,0x04,0x08,0x10,0x6D,0x15,0x31,0x24,0x1F,0x6E,0x6F};
	static int oldi=-1;
	static int vkx=0,vky=0;

 	MXjoy0=0;
	if(oldi!=-1){retro_key_up(oldi);oldi=-1;}

   	input_poll_cb();

        if (input_state_cb(0, RETRO_DEVICE_KEYBOARD, 0, RETROK_F11) || input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_Y) )
		pauseg=4; //fMSX menu

        //if (input_state_cb(0, RETRO_DEVICE_KEYBOARD, 0, RETROK_F10)  )
	//	pauseg=1; // DBG LOADFILE


	i=10;//show vkey toggle
	if ( input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, i) && mbt[i]==0 )
	    	mbt[i]=1;
	else if ( mbt[i]==1 && ! input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, i) ){
	   	mbt[i]=0;
		SHOWKEY=-SHOWKEY;
		Screen_SetFullUpdate();
	}

	i=2;//SELECT
	if ( input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, i) && mbt[i]==0 )
	    	mbt[i]=1;
	else if ( mbt[i]==1 && ! input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, i) ){
	   	mbt[i]=0;
		RVSYNC=!RVSYNC;				
	}

	i=3;//START
	if ( input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, i) && mbt[i]==0 )
	    	mbt[i]=1;
	else if ( mbt[i]==1 && ! input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, i) ){
	   	mbt[i]=0;
		MOUSEMODE=-MOUSEMODE;
		
	}

/*		
        i=0;//BTN B
	if ( input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, i) && mbt[i]==0 )
	    	mbt[i]=1;
	else if ( mbt[i]==1 && ! input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, i) ){
	   	mbt[i]=0;		
	}
*/
        i=9;//BTN X
	if ( input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, i) && mbt[i]==0 )
	    	mbt[i]=1;
	else if ( mbt[i]==1 && ! input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, i) ){
	   	mbt[i]=0;
		browsedsk1();
	}

	i=12;//show/hide statut
	if ( input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, i) && mbt[i]==0 )
	    	mbt[i]=1;
	else if ( mbt[i]==1 && ! input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, i) ){
	   	mbt[i]=0;
		STATUTON=-STATUTON;
		Screen_SetFullUpdate();
	}

	i=13;//snd on/off
	if ( input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, i) && mbt[i]==0 )
	    	mbt[i]=1;
	else if ( mbt[i]==1 && ! input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, i) ){
	   	mbt[i]=0;
		SND=-SND;		
	}

	i=11;//reset
	if ( input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, i) && mbt[i]==0 )
	    	mbt[i]=1;
	else if ( mbt[i]==1 && ! input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, i) ){
	   	mbt[i]=0;		
		retro_reset_msx();
	}

	if(SHOWKEY==1){

		static int vkflag[5]={0,0,0,0,0};		
		
		if ( input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP) && vkflag[0]==0 )
		    	vkflag[0]=1;
		else if (vkflag[0]==1 && ! input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP) ){
		   	vkflag[0]=0;
			vky -= 1; 
		}

		if ( input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN) && vkflag[1]==0 )
		    	vkflag[1]=1;
		else if (vkflag[1]==1 && ! input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN) ){
		   	vkflag[1]=0;
			vky += 1; 
		}

		if ( input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT) && vkflag[2]==0 )
		    	vkflag[2]=1;
		else if (vkflag[2]==1 && ! input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT) ){
		   	vkflag[2]=0;
			vkx -= 1;
		}

		if ( input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT) && vkflag[3]==0 )
		    	vkflag[3]=1;
		else if (vkflag[3]==1 && ! input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT) ){
		   	vkflag[3]=0;
			vkx += 1;
		}

		if(vkx<0)vkx=9;
		if(vkx>9)vkx=0;
		if(vky<0)vky=4;
		if(vky>4)vky=0;

		virtual_kdb(bmp,vkx,vky);

		i=8;
		if(input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, i)  && vkflag[4]==0) 	
			vkflag[4]=1;
		else if( !input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, i)  && vkflag[4]==1) {

			vkflag[4]=0;
			i=check_vkey2(vkx,vky);

			if(i==-2){
				NPAGE=-NPAGE;oldi=-1;							
				//Screen_SetFullUpdate();				
			}
			else if(i==-1)oldi=-1;
			else if(i==-3){//KDB bgcolor
				//Screen_SetFullUpdate();
				KCOL=-KCOL;
				oldi=-1;
			}
			else if(i==-4){//VKbd show/hide 			
				
				oldi=-1;
				Screen_SetFullUpdate();
				SHOWKEY=-SHOWKEY;
			}
			else if(i==-5){//Change Joy
				NUMjoy=-NUMjoy;
				oldi=-1;
			}
			else if(i==-6){//EXIT				
				oldi=-1;
				retro_deinit();
				exit(0);
			}
			else if(i==-7){//savestate 
				//retro_savestate_msx("DEFAULT.STA");
				oldi=-1;
			}
			else {	

				if(i<200/*128*/){
					oldi=i;
					retro_key_down(i);
				}
				else{

					i=keyboard_translation[i];
					if(i==0x05){
											
						if(SHIFTON == 1)retro_key_up(i);
						else retro_key_down(i);
	
						SHIFTON=-SHIFTON;	
						//Screen_SetFullUpdate();
						
						oldi=-1;
					}
					else {
						oldi=i;
						retro_key_down(i);
					}
				}
			}				

		}

         	if(STATUTON==1)Print_Statut();

		return ;
	}

   	static int mbL=0,mbR=0;
	int mouse_l;
	int mouse_r;
  	int16_t mouse_x;
   	int16_t mouse_y;
  
	if(MOUSEMODE==-1){ //Joy mode

		for(i=4;i<9;i++)if( input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, i) )MXjoy0 |= vbt[i]; // Joy press
		//btn 2
		if( input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, 0) )MXjoy0 |=0x20;
		retro_joy0(MXjoy0);

	   	mouse_x = input_state_cb(0, RETRO_DEVICE_MOUSE, 0, RETRO_DEVICE_ID_MOUSE_X);
	   	mouse_y = input_state_cb(0, RETRO_DEVICE_MOUSE, 0, RETRO_DEVICE_ID_MOUSE_Y);
	   	mouse_l    = input_state_cb(0, RETRO_DEVICE_MOUSE, 0, RETRO_DEVICE_ID_MOUSE_LEFT);
	   	mouse_r    = input_state_cb(0, RETRO_DEVICE_MOUSE, 0, RETRO_DEVICE_ID_MOUSE_RIGHT);

		fmousex=mouse_x;
  		fmousey=mouse_y;

	}
	else {  //Mouse mode
		fmousex=fmousey=0;

		if (input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT))fmousex += PAS;

		if (input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT))fmousex -= PAS;

		if (input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN))fmousey += PAS;

		if (input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP))fmousey -= PAS;

		mouse_l=input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A);
		mouse_r=input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B);
       }


	retro_mouse(fmousex, fmousey);

	if(mbL==0 && mouse_l){
		mbL=1;		
		retro_mouse_but0(1,tomx, tomy);
	}
	else if(mbL==1 && !mouse_l) {

 		
		retro_mouse_but0(0,tomx, tomy);
                mbL=0;
	}

	if(mbR==0 && mouse_r){
		mbR=1;
		retro_mouse_but1(1,tomx, tomy);
	}
	else if(mbR==1 && !mouse_r) {

 		retro_mouse_but1(0,tomx, tomy);
                mbR=0;
	}

	
    
	if(STATUTON==1)Print_Statut();

	return ;

}

int update_input_gui()
{
	int ret=0;	
	static int dskflag[7]={0,0,0,0,0,0,0};// UP -1 DW 1 A 2 B 3 LFT -10 RGT 10 X 4	

   	input_poll_cb();	
		
	if ( input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP) && dskflag[0]==0 ){
	    	dskflag[0]=1;
		//ret= -1; 
	}
	else if (dskflag[0]==1 && ! input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP) ){
	   	dskflag[0]=0;
		ret= -1; 
	}//dskflag[0]=0;

	if ( input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN) && dskflag[1]==0 ){
	    	dskflag[1]=1;
		//ret= 1;
	} 
	else if (dskflag[1]==1 && ! input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN) ){
	   	dskflag[1]=0;
		ret= 1; 
	}//dskflag[1]=0;
		
	if ( input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT) && dskflag[4]==0 )
	    	dskflag[4]=1;
	else if (dskflag[4]==1 && ! input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT) ){
	   	dskflag[4]=0;
		ret= -10; 
	}

	if ( input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT) && dskflag[5]==0 )
	    	dskflag[5]=1;
	else if (dskflag[5]==1 && ! input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT) ){
	   	dskflag[5]=0;
		ret= 10; 
	}

	if ( ( input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A) || \
		input_state_cb(0, RETRO_DEVICE_KEYBOARD, 0, RETROK_LCTRL) ) && dskflag[2]==0 ){
	    	dskflag[2]=1;
		
	}
	else if (dskflag[2]==1 && ! input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A)\
		&& !input_state_cb(0, RETRO_DEVICE_KEYBOARD, 0, RETROK_LCTRL) ){

	   	dskflag[2]=0;
		ret= 2;
	}

	if ( ( input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B) || \
		input_state_cb(0, RETRO_DEVICE_KEYBOARD, 0, RETROK_LALT) ) && dskflag[3]==0 ){
	    	dskflag[3]=1;
	}
	else if (dskflag[3]==1 && ! input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B) &&\
		!input_state_cb(0, RETRO_DEVICE_KEYBOARD, 0, RETROK_LALT) ){

	   	dskflag[3]=0;
		ret= 3;
	}

	if ( input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_X) && dskflag[6]==0 )
	    	dskflag[6]=1;
	else if (dskflag[6]==1 && ! input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_X) ){
	   	dskflag[6]=0;
		ret= 4; 
	}

	return ret;

}

