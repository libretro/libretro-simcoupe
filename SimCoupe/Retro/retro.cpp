#include "SimCoupe.h"
#include "retro.h"

#include "Frame.h"
#include "GUI.h"
#include "Options.h"
#include "UI.h"

#define FULLSCREEN_DEPTH    16

static DWORD aulPalette[N_PALETTE_COLOURS];
static DWORD aulScanline[N_PALETTE_COLOURS];


RetroVideo::RetroVideo ()
	: pFront(NULL), pBack(NULL), pIcon(NULL), nDesktopWidth(0), nDesktopHeight(0)
{
	m_rTarget.x = m_rTarget.y = 0;
	m_rTarget.w = Frame::GetWidth();
	m_rTarget.h = Frame::GetHeight();
}

RetroVideo::~RetroVideo ()
{
    if (pBack) free(pBack), pBack = NULL;    
}


int RetroVideo::GetCaps () const
{
	return 0;
}

bool RetroVideo::Init (bool fFirstInit_)
{
    TRACE("-> Video::Init(%s)\n", fFirstInit_ ? "first" : "");

   
    if (fFirstInit_)
    {
        
        nDesktopWidth = 640;
        nDesktopHeight = 480;
        TRACE("Desktop resolution: %dx%d\n", nDesktopWidth, nDesktopHeight);
    }

	UpdateSize();
    return UI::Init(fFirstInit_);
}


void RetroVideo::Update (CScreen* pScreen_, bool *pafDirty_)
{
	// Draw any changed lines to the back buffer
	if (!DrawChanges(pScreen_, pafDirty_))
		return;
}

#define RGB565(r, g, b)  (((r) << (5+6)) | ((g) << 6) | (b))

// Create whatever's needed for actually displaying the SAM image
void RetroVideo::UpdatePalette ()
{
    // Determine the scanline brightness level adjustment, in the range -100 to +100
    int nScanAdjust = GetOption(scanlines) ? (GetOption(scanlevel) - 100) : 0;
    if (nScanAdjust < -100) nScanAdjust = -100;

    const COLOUR *pSAM = IO::GetPalette();

    // Build the full palette from SAM and GUI colours
    for (int i = 0; i < N_PALETTE_COLOURS ; i++)
    {
        // Look up the colour in the SAM palette
        const COLOUR *p = &pSAM[i];
        BYTE r = p->bRed, g = p->bGreen, b = p->bBlue;

        aulPalette[i] = RGB565(r>>3, g>>3, b>>3);//SDL_MapRGB(pBack->format, r,g,b);
        AdjustBrightness(r,g,b, nScanAdjust);
        aulScanline[i] = RGB565(r>>3, g>>3, b>>3);//SDL_MapRGB(pBack->format, r,g,b);
    }

    // Ensure the display is redrawn to reflect the changes
    Video::SetDirty();
}

extern unsigned short int bmp[640 * 480];

// OpenGL version of DisplayChanges
bool RetroVideo::DrawChanges (CScreen* pScreen_, bool *pafDirty_)
{
	
    int nWidth = Frame::GetWidth();
    int nHeight = Frame::GetHeight();

    int nRightHi = nWidth >> 3;
    int nRightLo = nRightHi >> 1;

    bool fInterlace = !GUI::IsActive();
    if (fInterlace) nHeight >>= 1;

    DWORD *pdwBack = reinterpret_cast<DWORD*>(bmp), *pdw = pdwBack;
    long lPitchDW = 1280 >> (fInterlace ? 1 : 2);
    bool *pfHiRes = pScreen_->GetHiRes();

    BYTE *pbSAM = pScreen_->GetLine(0), *pb = pbSAM;
    long lPitch = pScreen_->GetPitch();

    int nShift = fInterlace ? 1 : 0;
    int nDepth =16;

    // What colour depth is the target surface?
    switch (nDepth)
    {
        case 16:
        {
            nWidth <<= 1;

            for (int y = 0 ; y < nHeight ; pdw = pdwBack += lPitchDW, pb = pbSAM += lPitch, y++)
            {
                if (!pafDirty_[y])
                    continue;

                if (pfHiRes[y])
                {
                    for (int x = 0 ; x < nRightHi ; x++)
                    {
                        pdw[0] = SDL_SwapLE32((aulPalette[pb[1]] << 16) | aulPalette[pb[0]]);
                        pdw[1] = SDL_SwapLE32((aulPalette[pb[3]] << 16) | aulPalette[pb[2]]);
                        pdw[2] = SDL_SwapLE32((aulPalette[pb[5]] << 16) | aulPalette[pb[4]]);
                        pdw[3] = SDL_SwapLE32((aulPalette[pb[7]] << 16) | aulPalette[pb[6]]);

                        pdw += 4;
                        pb += 8;
                    }

                    if (fInterlace)
                    {
                        pb = pbSAM;
                        pdw = pdwBack + lPitchDW/2;

                        if (!GetOption(scanlevel))
                            memset(pdw, 0x00, nWidth);
                        else
                        {
                            for (int x = 0 ; x < nRightHi ; x++)
                            {
								pdw[0] = SDL_SwapLE32((aulScanline[pb[1]] << 16) | aulScanline[pb[0]]);
								pdw[1] = SDL_SwapLE32((aulScanline[pb[3]] << 16) | aulScanline[pb[2]]);
								pdw[2] = SDL_SwapLE32((aulScanline[pb[5]] << 16) | aulScanline[pb[4]]);
								pdw[3] = SDL_SwapLE32((aulScanline[pb[7]] << 16) | aulScanline[pb[6]]);

                                pdw += 4;
                                pb += 8;
                            }
                        }
                    }
                }
                else
                {
                    for (int x = 0 ; x < nRightLo ; x++)
                    {
                        pdw[0] = aulPalette[pb[0]] * 0x10001UL;
                        pdw[1] = aulPalette[pb[1]] * 0x10001UL;
                        pdw[2] = aulPalette[pb[2]] * 0x10001UL;
                        pdw[3] = aulPalette[pb[3]] * 0x10001UL;
                        pdw[4] = aulPalette[pb[4]] * 0x10001UL;
                        pdw[5] = aulPalette[pb[5]] * 0x10001UL;
                        pdw[6] = aulPalette[pb[6]] * 0x10001UL;
                        pdw[7] = aulPalette[pb[7]] * 0x10001UL;

                        pdw += 8;
                        pb += 8;
                    }

                    if (fInterlace)
                    {
                        pb = pbSAM;
                        pdw = pdwBack + lPitchDW/2;

                        if (!GetOption(scanlevel))
                            memset(pdw, 0x00, nWidth);
                        else
                        {
                            for (int x = 0 ; x < nRightLo ; x++)
                            {
                                pdw[0] = aulScanline[pb[0]] * 0x10001UL;
                                pdw[1] = aulScanline[pb[1]] * 0x10001UL;
                                pdw[2] = aulScanline[pb[2]] * 0x10001UL;
                                pdw[3] = aulScanline[pb[3]] * 0x10001UL;
                                pdw[4] = aulScanline[pb[4]] * 0x10001UL;
                                pdw[5] = aulScanline[pb[5]] * 0x10001UL;
                                pdw[6] = aulScanline[pb[6]] * 0x10001UL;
                                pdw[7] = aulScanline[pb[7]] * 0x10001UL;

                                pdw += 8;
                                pb += 8;
                            }
                        }
                    }
                }
            }
        }
        break;

        case 32:
        {
            nWidth <<= 2;

            for (int y = 0 ; y < nHeight ; pdw = pdwBack += lPitchDW, pb = pbSAM += lPitch, y++)
            {
                if (!pafDirty_[y])
                    continue;

                if (pfHiRes[y])
                {
                    for (int x = 0 ; x < nRightHi ; x++)
                    {
                        pdw[0] = aulPalette[pb[0]];
                        pdw[1] = aulPalette[pb[1]];
                        pdw[2] = aulPalette[pb[2]];
                        pdw[3] = aulPalette[pb[3]];
                        pdw[4] = aulPalette[pb[4]];
                        pdw[5] = aulPalette[pb[5]];
                        pdw[6] = aulPalette[pb[6]];
                        pdw[7] = aulPalette[pb[7]];

                        pdw += 8;
                        pb += 8;
                    }

                    if (fInterlace)
                    {
                        pb = pbSAM;
                        pdw = pdwBack + lPitchDW/2;

                        if (!GetOption(scanlevel))
                            memset(pdw, 0x00, nWidth);
                        else
                        {
                            for (int x = 0 ; x < nRightHi ; x++)
                            {
                                pdw[0] = aulScanline[pb[0]];
                                pdw[1] = aulScanline[pb[1]];
                                pdw[2] = aulScanline[pb[2]];
                                pdw[3] = aulScanline[pb[3]];
                                pdw[4] = aulScanline[pb[4]];
                                pdw[5] = aulScanline[pb[5]];
                                pdw[6] = aulScanline[pb[6]];
                                pdw[7] = aulScanline[pb[7]];

                                pdw += 8;
                                pb += 8;
                            }
                        }
                    }
                }
                else
                {
                    for (int x = 0 ; x < nRightLo ; x++)
                    {
                        pdw[0]  = pdw[1]  = aulPalette[pb[0]];
                        pdw[2]  = pdw[3]  = aulPalette[pb[1]];
                        pdw[4]  = pdw[5]  = aulPalette[pb[2]];
                        pdw[6]  = pdw[7]  = aulPalette[pb[3]];
                        pdw[8]  = pdw[9]  = aulPalette[pb[4]];
                        pdw[10] = pdw[11] = aulPalette[pb[5]];
                        pdw[12] = pdw[13] = aulPalette[pb[6]];
                        pdw[14] = pdw[15] = aulPalette[pb[7]];

                        pdw += 16;
                        pb += 8;
                    }

                    if (fInterlace)
                    {
                        pb = pbSAM;
                        pdw = pdwBack + lPitchDW/2;

                        if (!GetOption(scanlevel))
                            memset(pdw, 0x00, nWidth);
                        else
                        {
                            for (int x = 0 ; x < nRightLo ; x++)
                            {
                                pdw[0]  = pdw[1]  = aulScanline[pb[0]];
                                pdw[2]  = pdw[3]  = aulScanline[pb[1]];
                                pdw[4]  = pdw[5]  = aulScanline[pb[2]];
                                pdw[6]  = pdw[7]  = aulScanline[pb[3]];
                                pdw[8]  = pdw[9]  = aulScanline[pb[4]];
                                pdw[10] = pdw[11] = aulScanline[pb[5]];
                                pdw[12] = pdw[13] = aulScanline[pb[6]];
                                pdw[14] = pdw[15] = aulScanline[pb[7]];

                                pdw += 16;
                                pb += 8;
                            }
                        }
                    }
                }
            }
        }
        break;
    }
/*
    // Find the first changed display line
    int nChangeFrom = 0;
    for ( ; nChangeFrom < nHeight && !pafDirty_[nChangeFrom] ; nChangeFrom++);

    if (nChangeFrom < nHeight)
    {
        // Find the last change display line
        int nChangeTo = nHeight-1;
        for ( ; nChangeTo && !pafDirty_[nChangeTo] ; nChangeTo--);

        // Clear the dirty flags for the changed block
        for (int i = nChangeFrom ; i <= nChangeTo ; pafDirty_[i++] = false);

        // Calculate the dirty source and target areas - non-GUI displays require the height doubling
        SDL_Rect rect = { 0, nChangeFrom << nShift, pScreen_->GetPitch(), ((nChangeTo - nChangeFrom + 1) << nShift) };
        SDL_Rect rectFront = { (pFront->w - rect.w) >> 1, rect.y + ((pFront->h - (nHeight << nShift)) >> 1), rect.w, rect.h };
	        
    }
*/
    // Success
    return true;
}

void RetroVideo::UpdateSize ()
{
    int nWidth = Frame::GetWidth();
    int nHeight = Frame::GetHeight();

    // Use 16-bit for fullscreen or the current desktop depth for windowed
    int nDepth = GetOption(fullscreen) ? FULLSCREEN_DEPTH : 0;

    
    {
        // Work out the best-fit mode for the visible frame area
        if (nWidth <= 640 && nHeight <= 480)
            nWidth = 640, nHeight = 480;
        else if (nWidth <= 800 && nHeight <= 600)
            nWidth = 800, nHeight = 600;
        else
            nWidth = 1024, nHeight = 768;
        
    }

    // Did we fail to create the front buffer?
    if (!pFront)
        TRACE("Failed to create front buffer!\n");

   	UpdatePalette();
}


// Map a native size/offset to SAM view port
void RetroVideo::DisplayToSamSize (int* pnX_, int* pnY_)
{
    int nHalfWidth = !GUI::IsActive();
    int nHalfHeight = nHalfWidth;

    *pnX_ = *pnX_ * Frame::GetWidth()  / (m_rTarget.w << nHalfWidth);
    *pnY_ = *pnY_ * Frame::GetHeight() / (m_rTarget.h << nHalfHeight);
}

// Map a native client point to SAM view port
void RetroVideo::DisplayToSamPoint (int* pnX_, int* pnY_)
{
    *pnX_ -= m_rTarget.x;
    *pnY_ -= m_rTarget.y;
    DisplayToSamSize(pnX_, pnY_);
}
