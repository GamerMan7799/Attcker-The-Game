#ifndef __MY_SCREEN_HEADER__
#define __MY_SCREEN_HEADER__
/*****************************************************************************/
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <cstdio>
/*****************************************************************************/
#include "../global.h"
/*****************************************************************************/
/** @def DEFINED_DEFAULT_IMAGE_PATH
    All images are stored in a folder called "Images" however, because Linux,
    and Windows handle file paths differently this macro will be set based on
    the OS. */
/** @def DEFINED_MESSAGE_FONT
    Much like DEFINED_DEFAULT_IMAGE_PATH we have to set this differently depending
    on if the program is being run on a Windows computer or Linux. I dislike Apple
    so we don't care about that. */

#if defined(_WIN32) || defined(_WIN64)
    #define DEFINED_DEFAULT_IMAGE_PATH ".\\Images\\"
    #define DEFINED_MESSAGE_FONT "C:\\Windows\\Fonts\\timesbd.ttf"
#elif defined(__unix__) || defined(__linux__)
    #define DEFINED_DEFAULT_IMAGE_PATH "./Images/"
    #define DEFINED_MESSAGE_FONT "/usr/share/fonts/truetype/freefont/FreeMono.ttf"
#elif defined(__CYGWIN__)
    #define DEFINED_DEFAULT_IMAGE_PATH "./Images/"
    #define DEFINED_MESSAGE_FONT "C:/Windows/Fonts/timesbd.ttf"
#else
    #define DEFINED_DEFAULT_IMAGE_PATH "OS NOT SUPPORTED!"
    #define DEFINED_MESSAGE_FONT "OS NOT SUPPORTED!"
#endif // defined OS
/*****************************************************************************/
/** @struct stcLoaded
    Holds booleans for if SDL stuff is loaded or not. */

struct stcLoaded {
  bool blnWindow;     /**< A boolean for the window. */
  bool blnRenderer;   /**< A boolean for the renderer. */
  bool blnMapTiles;   /**< A boolean for the map tiles. */
  bool blnErrortex;   /**< A boolean for the error texture. */
  bool blnMessage;    /**< A boolean for the message texture. */
  bool blnMessageFont;/**< A boolean for the Message font. */
};

/** @struct stcColors
    SDL colors for the messages. */
struct stcColors {
  SDL_Color Black; /**< The color black. */
  SDL_Color White; /**< The color white. */
};

/** @struct stcTextures
    All the textures loaded; held in a structure for easier reference */
struct stcTextures {
  SDL_Texture *maptiles; /**< Pointer to "tiles.png" in memory. */
  SDL_Texture *errortex; /**< Pointer to the error texture (its embedded) in memory. */
  SDL_Texture *texmessage; /**< Pointer to the texture for the messages in memory. */
  //SDL_Texture *splash; /**< Pointer to the texture for the splash screen. */
};

struct stcWinAtt {
  SDL_Window *win;    /**< Pointer to the window in memory. */
  SDL_Renderer *ren;  /**< Pointer to the renderer in memory. */
  uint width;         /**< Screen width in pixels. */
  uint height;        /**< Screen height in pixels. */
  TTF_Font *font;     /**< Screen height in pixels. */
};

typedef struct stcLoaded Loaded;
typedef struct stcColors clrs;
typedef struct stcTextures TEX;
/*****************************************************************************/
class clsScreen {
  public:
    clsScreen();
    ~clsScreen();

    //void destory(void);
    void start(void);
    void DrawMap(void);
    void cleanup(void);
    void update(void);
    void clearRen(void);
    void showErrors(void);

    /** If SDL is currently running without issue. */
    static bool bln_SDL_started;

    void ShowStartUp(void);
    stcWinAtt* getWinAtt(void);
    TEX getTextures(void);


  private:

    static stcWinAtt window;

    void loadTextures(void);

    /** SDL Rectangles for the location of each of the tiles in tiles.png */
    static SDL_Rect clips[DEFINED_NUM_MAP_TILES];

    void set_clips(void);
    void error(void);

    /** Colors. */
    static clrs colors;

    static uint pic_size; /**< Picture size in pixels */

    static TEX textures; /**< All the textures. */

    static Loaded blnloaded; /**< All the booleans. */

    void wait(ulong);
};
/*****************************************************************************/
/////////////////////////////////////////////////
/// @class clsScreen screen.h "src/ui/screen.h"
/// @brief This class will hold all of the values and functions related to the
///        the SDL screen..
/////////////////////////////////////////////////
/*****************************************************************************/
#endif // __MY_SCREEN_HEADER__
