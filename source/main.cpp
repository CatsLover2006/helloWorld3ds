// A Hello World for 3DS
// By Half-Qilin AKA Hail

#include <citro2d.h>
#include <iostream>
#include <time.h>
#include <cmath>

#define SCREEN_WIDTH  400
#define BOTTOM_SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

static C2D_Font fontMain;
static C2D_Font fontJP;

u8 consoleModel = 3;
u8 lang = CFG_LANGUAGE_EN;
u8 serial = 0;
u8 wideModifier = 2;

int main(int argc, char* argv[]) {
    // Init Libs
    romfsInit();
    gfxInitDefault();
    C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
    C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
    C2D_Prepare();
    srand(time(NULL));
	
	// Vars
	double animTimer = 0, botmTimer = 0;
	u32 kDown;
	
	// Load Fonts
	fontMain = C2D_FontLoad("romfs:/gfx/Figtree-SemiBold.bcfnt");
	fontJP = C2D_FontLoad("romfs:/gfx/Kosugi-Regular.bcfnt");
	
	// Font Buffers
	C2D_TextBuf hiWorld = C2D_TextBufNew(50);
	
	// Wide mode
    Result res = cfguInit();
    if (R_SUCCEEDED(res)) {
		CFGU_GetSystemLanguage(&lang);
        CFGU_GetSystemModel(&consoleModel);
		CFGI_SecureInfoGetSerialNumber(&serial);
        cfguExit();
    }
    if (consoleModel == 3) wideModifier = 1;
	if (serial == 0) wideModifier = 1;
    gfxSetWide(wideModifier == 2);
	
	// Prepare Text
	C2D_Text hiText;
	switch(lang) {
		case CFG_LANGUAGE_JP:
			C2D_TextFontParse(&hiText, fontJP, hiWorld, "こんにちは");
			break;
		case CFG_LANGUAGE_ES:
			C2D_TextFontParse(&hiText, fontMain, hiWorld, "Hola Mundo!");
			break;
		default:
			C2D_TextFontParse(&hiText, fontMain, hiWorld, "Hello World!");
			break;
	}
	C2D_TextOptimize(&hiText);
	
    // Create colors
    u32 clrClear = C2D_Color32(230, 234, 235, 0xFF);
    u32 clrWhite = C2D_Color32(255, 255, 255, 0xFF);
    u32 clrTop = C2D_Color32(rand()%256, rand()%256, rand()%256, 0xFF);
    u32 clrBottom = C2D_Color32(rand()%256, rand()%256, rand()%256, 0xFF);
	
	// Create Screens
    C3D_RenderTarget * top_main = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
    C3D_RenderTarget * bottom = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);
	
	// Booleans
	bool lFromLeft = true;
	bool lFromBottom = true;
	bool tFromLeft = false;
	bool tFromBottom = false;
	
	// Main Loop
	while (aptMainLoop())
    {
        animTimer += 1 / 2.0;
        botmTimer += 1 / 2.0;
		if (animTimer > 220) {
			clrTop = C2D_Color32(rand()%256, rand()%256, rand()%256, 0xFF);
			tFromLeft = rand()%2;
			tFromBottom = rand()%2;
			animTimer = -1;
		}
		if (botmTimer > 200) {
			clrBottom = C2D_Color32(rand()%256, rand()%256, rand()%256, 0xFF);
			lFromLeft = rand()%2;
			lFromBottom = rand()%2;
			botmTimer = -1;
		}
		
		// Input
		hidScanInput();
		kDown = hidKeysDown();
		
		// Exit
		if (kDown & KEY_START) break;
		
		// Start Render
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		C2D_TargetClear(top_main, clrClear);
		C2D_TargetClear(bottom, clrClear);
		
		// Top Screen
		C2D_SceneBegin(top_main);
		
		// Draw two triangles
		C2D_DrawTriangle(tFromLeft?SCREEN_WIDTH:0, tFromBottom?SCREEN_HEIGHT:0, clrTop, 
						 tFromLeft?SCREEN_WIDTH:0, tFromBottom?(SCREEN_HEIGHT-animTimer*6.0f):animTimer*6.0f, clrWhite, 
						 tFromLeft?(SCREEN_WIDTH-animTimer*8.0f):animTimer*8.0f, tFromBottom?SCREEN_HEIGHT:0, clrWhite, 0.1f);
		C2D_DrawTriangle(tFromLeft?SCREEN_WIDTH:0, tFromBottom?SCREEN_HEIGHT:0, clrClear, 
						 tFromLeft?SCREEN_WIDTH:0, tFromBottom?(SCREEN_HEIGHT-animTimer*2.5f):animTimer*2.5f, clrClear, 
						 tFromLeft?(SCREEN_WIDTH-animTimer*3.333333333333333333f):animTimer*3.333333333333333333f, tFromBottom?SCREEN_HEIGHT:0, clrClear, 0.2f);
		
		// Draw text
		for (int i = -2; i < 3; i++) for (int j = -2; j < 3; j++) C2D_DrawText(&hiText, C2D_AlignCenter | C2D_WithColor | C2D_AtBaseline, SCREEN_WIDTH/2 - i, SCREEN_HEIGHT/2 + 20 - j, 0.25f, 1.6f, 1.6f, clrWhite);
		C2D_DrawText(&hiText, C2D_AlignCenter | C2D_AtBaseline, SCREEN_WIDTH/2, SCREEN_HEIGHT/2 + 20, 0.3f, 1.6f, 1.6f);
		
		// Bottom Scene
        C2D_SceneBegin(bottom);
		
		// Draw two triangles
		C2D_DrawTriangle(lFromLeft?BOTTOM_SCREEN_WIDTH:0, lFromBottom?SCREEN_HEIGHT:0, clrBottom,
						 lFromLeft?BOTTOM_SCREEN_WIDTH:0, lFromBottom?(SCREEN_HEIGHT - botmTimer*6.0f):botmTimer*6.0f, clrWhite,
						 lFromLeft?(BOTTOM_SCREEN_WIDTH - botmTimer*8.0f):botmTimer*8.0f, lFromBottom?SCREEN_HEIGHT:0, clrWhite, 0.5f);
		C2D_DrawTriangle(lFromLeft?BOTTOM_SCREEN_WIDTH:0, lFromBottom?SCREEN_HEIGHT:0, clrClear, 
						 lFromLeft?BOTTOM_SCREEN_WIDTH:0, lFromBottom?(SCREEN_HEIGHT - botmTimer*2.5f):botmTimer*2.5f, clrClear, 
						 lFromLeft?(BOTTOM_SCREEN_WIDTH - botmTimer*3.333333333333333333f):botmTimer*3.333333333333333333f, lFromBottom?SCREEN_HEIGHT:0, clrClear, 0.6f);
		
		// Done Rendering!
		C3D_FrameEnd(0);
	}
	
	// Free Fonts
	C2D_FontFree(fontJP);
	C2D_FontFree(fontMain);

    // Deinit Graphics
    C2D_Fini();
    C3D_Fini();
    gfxExit();
	
	// Exit ROM
    romfsExit();
	
	return 0;
}