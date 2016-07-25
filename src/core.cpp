/*****************************************************************************/
#include "core.h"
#include "chest.h"
#include "door.h"
#include "save.h"
/*****************************************************************************/
//General Todos
/** \todo (Gamerman7799#9#): Finish Doxygen support */
/** \todo (Gamerman7799#9#): Add music */
/*****************************************************************************/
clsCore::clsCore() {
    //ctor
}
/*****************************************************************************/
clsCore::~clsCore() {
    //dtor
}
/*****************************************************************************/
void clsCore::start() {
    /////////////////////////////////////////////////
    /// @brief The "main" function. Handles all of the starting and most of the
    ///        core functions.
    /////////////////////////////////////////////////
    char menuselection;
    m_quit = false;
    SDL_Event event;
    m_level = 1;

    m_screen.start();
    //Show start up splash
    //m_screen.ShowStartUp();

    while (!m_quit) {
        menuselection = m_menu.MainMenu();

        switch (menuselection){
        case menuAbout:
            m_menu.AboutMenu();
            if (Global::blnDebugMode) {printf("About shown.\n");}
            break;
        case menuLoad :
            if (Global::blnDebugMode) {printf("Loading Save\n");}
            m_save.doLoad();
            m_Map.playerfind();
            doGame();
            break;
        case menuNew:
            if (Global::blnDebugMode) {printf("New Game.\n");}
            //New game
            m_player.initialize();
            //new dungeon
            m_Map.cmain();
            doGame();
            break;
        case menuOptions:
            m_menu.OptionsMenu();
            if (Global::blnDebugMode) {printf("Options shown.\n");}
            break;
        case menuError:
            if (Global::blnDebugMode) {printf("Menu Error returned.\n");}
        default:
            printf("ERROR!!! Now closing everything!\n");
            m_screen.showErrors();
        case menuQuit:
            m_quit = true;
            break;
        } //end switch
    } //end loop

    m_screen.~clsScreen();
	printf("\nDone\n");
} //end Core Start
/*****************************************************************************/
void clsCore::ShowConsole() {
    /////////////////////////////////////////////////
    /// @brief Shows the debug console that will allow commands to be entered.
    ///        Currently unimplemented, possibly will never be implemented. I
    ///        have an idea on how to do this, but I'm not sure it will work,
    ///        and since other stuff is more important I haven't tested it yet.
    /////////////////////////////////////////////////
    /// @todo (GamerMan7799#9#) Add debugging console
    return;
}
/*****************************************************************************/
void clsCore::ShowInventory() {
    /////////////////////////////////////////////////
    /// @brief Shows the inventory system. Currently unimplemented.
    /////////////////////////////////////////////////
    /// @todo (GamerMan7799#8#) Add inventory support
    return;
}
/*****************************************************************************/
void clsCore::MovePlayer(SDL_Event dirpress ) {
    /////////////////////////////////////////////////
    /// @brief Handles moving the player, making sure they can move to that spot
    ///        and other related functions.
    ///
    /// @param dirpress = The event that holds the direction that was pressed
    /////////////////////////////////////////////////
    char direction;
    static char lasttile = tileUpStairs;
    LOC temploc, playerloc;
    temploc = playerloc = m_player.getLocation();

    switch ( dirpress.key.keysym.sym ) {
    case SDLK_UP:
    case SDLK_w:
        //if (Global::blnDebugMode) {printf("Going up!\n");}
        direction = dirUp;
        temploc.y--;
        break;
    case SDLK_DOWN :
    case SDLK_s:
        //if (Global::blnDebugMode) {printf("Going down!\n");}
        direction = dirDown;
        temploc.y++;
        break;
    case SDLK_LEFT:
    case SDLK_a :
        //if (Global::blnDebugMode) {printf("Going left!\n");}
        direction = dirLeft;
        temploc.x--;
        break;
    case SDLK_RIGHT:
    case SDLK_d:
        //if (Global::blnDebugMode) {printf("Going right!\n");}
        direction = dirRight;
        temploc.x++;
        break;
    default:
        if (Global::blnDebugMode) {printf("Going nowhere!\n");}
        direction = dirNone;
        m_audio.playSound(soundBump,1);
        return;
        break;
    } //end case

    //check if player stepping in valid place
    clsDoor door;
    clsChest chest;

    switch ( m_Map.getCell(temploc) ) {
        //Any of the floor tiles that can be walked on
    case tileDirtFloor:
    case tileCorridor:
    case tileDoor:
    case tileUpStairs:
        m_audio.playSound(soundStep,1);
        m_Map.setCell(playerloc, lasttile);
        lasttile = m_Map.getCell(temploc);
        m_Map.setCell(temploc, tilePlayer);
        m_player.setLocation(temploc);

        //check if run into monster
        if (rand() % 101 <= DEFINED_MONSTER_SPAWN_CHANCE) {
            BattleScene();
            if ( m_quit ) {return;} //if player dies in battle
        } //end if monster
        break;

        //special tiles
    case tileChest:
        m_audio.playSound(soundChing,1);

        chest.activate();

        m_audio.playSound(soundStep,1);
        m_Map.setCell(temploc, tilePlayer);
        m_Map.setCell(playerloc, tileDirtFloor);
        m_player.setLocation(temploc);

        break;
    case tileLockedDoor:
        door.activate();

        m_audio.playSound(soundStep,1);
        m_Map.setCell(temploc, tilePlayer);
        m_Map.setCell(playerloc, tileDirtFloor);
        m_player.setLocation(temploc);
        break;

    case tileDownStairs :
        m_audio.playSound(soundLevelUp,1);
        doLevelUp();
        return;
        break;

    //Tiles you can't walk through
    case tileDirtWall:
    case tileStoneWall:
    case tileUnused:
        m_audio.playSound(soundBump,2);
        break;
    } //end switch tile

    door.~clsDoor();
    chest.~clsChest();
    return;
}
/*****************************************************************************/
void clsCore::BattleScene() {
    /////////////////////////////////////////////////
    /// @brief Will handle everything related to battling
    /////////////////////////////////////////////////
    /// @todo (GamerMan7799#1#) Add battle scene
    return;
}
/*****************************************************************************/
void clsCore::HandleEvent(SDL_Event event) {
    /////////////////////////////////////////////////
    /// @brief Handles all events to determine what to do.
    ///
    /// @param event = Event that occurred
    /////////////////////////////////////////////////
    if (event.type == SDL_QUIT) {m_quit = true;}
    else if (event.type == SDL_KEYDOWN) {
        //Key has been pressed figure out what to do
        switch (event.key.keysym.sym) {
        case SDLK_UP:
        case SDLK_w:
        case SDLK_DOWN:
        case SDLK_s:
        case SDLK_RIGHT:
        case SDLK_d:
        case SDLK_LEFT:
        case SDLK_a:
            MovePlayer(event);
            break;
        case SDLK_q:
            m_quit = true;
            break;
        case SDLK_i:
            ShowInventory();
            break;
        case SDLK_m:
        case SDLK_ESCAPE:
            m_menu.GameMenu();
            break;
        case SDLK_v:
            m_save.doSave();
            break;
        case SDLK_TAB:
            ShowConsole();
            break;
        case SDLK_h:
            m_player.doHeal();
            break;
        case SDLK_l:
            if(Global::blnDebugMode) {doLevelUp();}
            break;
        case SDLK_n:
            m_player.showStatus();
            break;
        } //end switch
    } //end if event
}
/*****************************************************************************/
void clsCore::doLevelUp() {
    /////////////////////////////////////////////////
    /// @brief Handles leveling up, ensures the right functions are run together.
    /////////////////////////////////////////////////
    m_screen.clearRen();
    m_player.doLevelup();
    m_Map.cmain();
    m_screen.DrawMap();
    m_screen.update();
    return;
}
/*****************************************************************************/
void clsCore::doGame() {
    /////////////////////////////////////////////////
    /// @brief The main game loop. Draws Map, Updates SDL Screen, and
    ///        polls for events
    /////////////////////////////////////////////////
    SDL_Event event;
    if(Global::blnDebugMode) {printf("Game started.\n");}

    while (!m_quit) {
        m_screen.DrawMap();
        m_screen.update();
        if (SDL_PollEvent( &event ) ) { HandleEvent(event); }
    } //end while not quit
}
/*****************************************************************************/
