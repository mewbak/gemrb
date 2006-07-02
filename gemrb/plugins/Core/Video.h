/* GemRB - Infinity Engine Emulator
 * Copyright (C) 2003 The GemRB Project
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * $Header: /data/gemrb/cvs2svn/gemrb/gemrb/gemrb/plugins/Core/Video.h,v 1.64 2006/07/02 11:46:58 wjpalenstijn Exp $
 *
 */

/**
 * @file Video.h
 * Declares Video, base class for video output plugins.
 * @author The GemRB Project
 */

#ifndef VIDEO_H
#define VIDEO_H

#include "../../includes/globals.h"
#include "Plugin.h"
#include "EventMgr.h"
#include "Animation.h"
#include "Polygon.h"
#include "ScriptedAnimation.h"

class SpriteCover;
class Palette;

#ifdef WIN32

#ifdef GEM_BUILD_DLL
#define GEM_EXPORT __declspec(dllexport)
#else
#define GEM_EXPORT __declspec(dllimport)
#endif

#else
#define GEM_EXPORT
#endif

// Note: not all these flags make sense together. Specifically:
// NOSHADOW overrides TRANSSHADOW
enum SpriteBlitFlags {
	BLIT_HALFTRANS = IE_VVC_TRANSPARENT, // 2
	BLIT_BLENDED = IE_VVC_BLENDED, // 8; not implemented in SDLVideo yet
	BLIT_MIRRORX = IE_VVC_MIRRORX, // 0x10
	BLIT_MIRRORY = IE_VVC_MIRRORY, // 0x20
	BLIT_NOSHADOW = 0x1000,
	BLIT_TRANSSHADOW = 0x2000,
	BLIT_TINTED = 0x00010000, // IE_VVC_TINT = 0x00030000
	BLIT_GREY = IE_VVC_GREYSCALE, // 0x80000
	BLIT_RED = IE_VVC_RED_TINT, // 0x02000000
	BLIT_DARK = IE_VVC_DARKEN, // 0x00100000; not implemented in SDLVideo yet
	BLIT_GLOW = IE_VVC_GLOWING // 0x00200000; not implemented in SDLVideo yet
	// Note: bits 29,30,31 are used by SDLVideo internally
};

/**
 * @class Video
 * Base class for video output plugins.
 */

class GEM_EXPORT Video : public Plugin {
public:
	Video(void);
	virtual ~Video(void);
	virtual int Init(void) = 0;
	virtual int CreateDisplay(int width, int height, int bpp, bool fullscreen) = 0;
	/** Sets window title of GemRB window */
	virtual void SetDisplayTitle(char* title, char* icon) = 0;
	virtual VideoModes GetVideoModes(bool fullscreen = false) = 0;
	virtual bool TestVideoMode(VideoMode& vm) = 0;
	/** Toggles GemRB between fullscreen and windowed mode */
	virtual bool ToggleFullscreenMode() = 0;
	/** Swaps displayed and back buffers */
	virtual int SwapBuffers(void) = 0;
	/** Grabs and releases mouse cursor within GemRB window */
	virtual bool ToggleGrabInput() = 0;
	virtual short GetWidth() = 0;
	virtual short GetHeight() = 0;

	virtual void InitSpriteCover(SpriteCover* sc) = 0;
	virtual void AddPolygonToSpriteCover(SpriteCover* sc, Wall_Polygon* poly,
										 int flags) = 0;
	virtual void DestroySpriteCover(SpriteCover* sc) = 0;

	virtual Sprite2D* CreateSprite(int w, int h, int bpp, ieDword rMask,
		ieDword gMask, ieDword bMask, ieDword aMask, void* pixels, bool cK = false,
		int index = 0) = 0;
	virtual Sprite2D* CreateSprite8(int w, int h, int bpp, void* pixels,
		void* palette, bool cK = false, int index = 0) = 0;
	virtual Sprite2D* CreateSpriteBAM8(int /*w*/, int /*h*/, bool /* RLE */,
									   void* /*pixeldata*/,
									   unsigned int /*datasize*/,
									   Palette* /*palette*/,
									   int /*transindex*/) { return 0; }
	virtual void FreeSprite(Sprite2D* spr) = 0;
	virtual void BlitSprite(Sprite2D* spr, int x, int y, bool anchor = false,
		Region* clip = NULL) = 0;
	virtual void BlitSpriteHalfTrans(Sprite2D* spr, int x, int y,
									 bool anchor = false,
									 Region* clip = NULL) = 0;

	// Note: BlitSpriteRegion's clip region is shifted by Viewport.x/y if
	// anchor is false. This is different from the other BlitSprite functions.
	virtual void BlitSpriteRegion(Sprite2D* spr, Region& size, int x, int y,
		bool anchor = true, Region* clip = NULL) = 0;

	virtual void BlitGameSprite(Sprite2D* spr, int x, int y,
								unsigned int flags, Color tint,
								SpriteCover* cover, Palette *palette = NULL,
								Region* clip = NULL) = 0;
	virtual void SetCursor(Sprite2D* up, Sprite2D* down) = 0;
	/** Sets a temporary cursor when dragging an Item from Inventory */
	virtual void SetDragCursor(Sprite2D* drag) = 0;
	/** Return GemRB window screenshot.
	 * It's generated from the momentary back buffer */
	virtual Sprite2D* GetScreenshot( Region r ) = 0;
	virtual Region GetViewport(void) = 0;
	virtual void SetViewport(int x, int y, unsigned int w, unsigned int h) = 0;
	virtual void MoveViewportTo(int x, int y, bool center) = 0;
	virtual void ConvertToVideoFormat(Sprite2D* sprite) = 0;
	/** No descriptions */
	virtual void SetPalette(Sprite2D* spr, Palette* pal) = 0;
	/** This function Draws the Border of a Rectangle as described by the Region parameter. The Color used to draw the rectangle is passes via the Color parameter. */
	virtual void DrawRect(Region& rgn, Color& color, bool fill = true, bool clipped = false) = 0;
	virtual void DrawRectSprite(Region& rgn, Color& color, Sprite2D* sprite) = 0;
	/** Draws a circle */
	virtual void DrawCircle(short cx, short cy, unsigned short r, Color& color) = 0;
	/** Draws an ellipse */
	virtual void DrawEllipse(short cx, short cy, unsigned short xr,
		unsigned short yr, Color& color, bool clipped = true) = 0;
	/** Draws a polygon on the screen */
	virtual void DrawPolyline(Gem_Polygon* poly, Color& color,
		bool fill = false) = 0;
	/** Draws a line segment */
	virtual void DrawLine(short x1, short y1, short x2, short y2,
		Color& color) = 0;
	/** Blits a Sprite filling the Region */
	virtual void BlitTiled(Region rgn, Sprite2D* img, bool anchor = false) = 0;
	/** Set Event Manager */
	void SetEventMgr(EventMgr* evnt);
	/** Send a Quit Signal to the Event Queue */
	virtual bool Quit(void) = 0;
	/** Get the Palette of a Sprite */
	virtual Palette* GetPalette(Sprite2D* spr) = 0;
	/** Flips sprite vertically, returns new sprite */
	virtual Sprite2D *MirrorSpriteVertical(Sprite2D *sprite, bool MirrorAnchor) = 0;
	/** Flips sprite horizontally, returns new sprite */
	virtual Sprite2D *MirrorSpriteHorizontal(Sprite2D *sprite, bool MirrorAnchor) = 0;
	/** Transforms sprite to have an alpha channel */
	virtual void CreateAlpha(Sprite2D *sprite) = 0;

	/** Convers a Screen Coordinate to a Game Coordinate */
	virtual void ConvertToGame(short& x, short& y) = 0;
	/** Sets the Fading Color */
	virtual void SetFadeColor(int r, int g, int b) = 0;
	/** Sets the Fading to Color Percentage */
	virtual void SetFadePercent(int percent) = 0;
	/** Set Clip Rectangle */
	virtual void SetClipRect(Region* clip) = 0;
	/** move the mouse forcibly */
	virtual void MoveMouse(unsigned int x, unsigned int y) = 0;
	/** initialize the screen for movie */
	virtual void InitMovieScreen(int &w, int &h) = 0;
	/** set the font and color of the movie subtitles */
	virtual void SetMovieFont(Font *stfont, Palette *pal) = 0;
	/** draw a movie frame */
	virtual void showFrame(unsigned char* buf, unsigned int bufw,
		unsigned int bufh, unsigned int sx, unsigned int sy,
		unsigned int w, unsigned int h, unsigned int dstx,
		unsigned int dsty, int truecolor, unsigned char *palette,
		ieDword titleref) = 0;
	/** handle events during movie */
	virtual int PollMovieEvents() = 0;
public:
	/** Event Manager Pointer */
	EventMgr* Evnt;
	short moveX, moveY;
	bool DisableMouse, DisableScroll;
	short xCorr, yCorr;
public:
	/** Returns true if a pixel on a given position in the sprite 
	 * is transparent.
	 * It is used to mask clicks to non-rectangular shaped controls */
	virtual bool IsSpritePixelTransparent(Sprite2D* sprite, unsigned short x, unsigned short y) = 0;
	virtual Sprite2D* SpriteScaleDown( Sprite2D* sprite, unsigned int ratio ) = 0;
};

#endif
