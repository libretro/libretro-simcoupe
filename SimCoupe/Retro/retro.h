#ifndef RETRO_H
#define RETRO_H

#include "Video.h"

typedef struct{
     signed short x, y;
     unsigned short w, h;
} SDL_Rect;

static __inline__ unsigned short SDL_Swap16(unsigned short x){
	unsigned short result= ((x<<8)|(x>>8)); 
return result;
}
static __inline__ unsigned SDL_Swap32(unsigned x){
	unsigned result= ((x<<24)|((x<<8)&0x00FF0000)|((x>>8)&0x0000FF00)|(x>>24));
 return result;
}

typedef struct
{
    int w, h;
    int pitch;    
    unsigned char * pixels;
} SDL_Surface;

#ifdef LSB_FIRST

#define SDL_SwapLE16(X)	(X)
#define SDL_SwapLE32(X) (X)

#define SDL_SwapBE16(X) SDL_Swap16(X)
#define SDL_SwapBE32(X) SDL_Swap32(X)

#else

#define SDL_SwapLE16(X)	SDL_Swap16(X)
#define SDL_SwapLE32(X) SDL_Swap32(X)

#define SDL_SwapBE16(X) (X)
#define SDL_SwapBE32(X) (X)

#endif


class RetroVideo : public VideoBase
{
	public:
		RetroVideo ();
		~RetroVideo ();

	public:
		int GetCaps () const;
		bool Init (bool fFirstInit_);

		void Update (CScreen* pScreen_, bool *pafDirty_);
		void UpdateSize ();
		void UpdatePalette ();

		void DisplayToSamSize (int* pnX_, int* pnY_);
		void DisplayToSamPoint (int* pnX_, int* pnY_);

	protected:
		bool DrawChanges (CScreen* pScreen_, bool *pafDirty_);

	private:
		SDL_Surface *pFront, *pBack, *pIcon;
		int nDesktopWidth, nDesktopHeight;

		SDL_Rect m_rTarget;
};

#endif // RETRO_H
