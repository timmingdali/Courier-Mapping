/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   interface.h
 * Author: lixin39
 *
 * Created on February 21, 2017, 1:39 AM
 */

#ifndef INTERFACE_H
#define INTERFACE_H
#include "load_map.h"
#include "graphics.h"
#include "graphics_types.h"
#include "Help_Print_Funcs.h"
#include "m1.h"
#include "m2.h"
#include "load_map.h"
#include "FuzzySearch.h"
#include "FuzzySearch_POI.h"
#include "API_module.h"
#include "Scale_dict.h"
#include "easygl/Surface.h"
#include "routeInstruction.h"
#include "m3.h"
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <thread>
#include <chrono>

#define MAP_SIZE_REFERENCE 0.43998
#define stVec (*getPtrToStSet())
#define segVec (*getPtrToStSegSet())
#define itsVec (*getPtrToIntersectSet())
#define segLvlookUp (*getPtrToLookUpTable())

struct zoomAnimeConfi;

enum styles{
    WHITE_50A = 0,              /* White, 50 % transparency : style for inactive search bar */
    WHITE_70A,                  /* White, 70 % transparency : style for inactive search bar */
    WHITE_100A,                 /* White, 100% transparency : style for active search bar */
    BLACK_50A,                  /* Black, 50% transparency : style for text in inactive search bar */
    BLACK_100A,                 /* Black, 100% transparency : style for text in active search bar */
    HIGHLIGHT,
    LOCKED,
    REALLYRED,
    NULLCOLOR
};

typedef zoomAnimeConfi zoomConi;

//two useful 
extern int xworld_to_scrn(float worldx);
extern int yworld_to_scrn(float worldy);

// defined in m2
extern float screen_ratio_width;
extern float screen_ratio_height;

extern zoomConi world;
struct searchBarStruct;

struct POI_Click {
    float x;
    float y;
    bool draw;
    string info;
};

enum pathClickType {
    firstClick = 0,
    secondClick
};

struct PathInterface{
   
    int numClick = firstClick;
    bool enable = false;
    unsigned intx_1 = 0;
    unsigned intx_2 = 0;
    
};

//a collection of all loaded png files 
struct Loaded_Surface {
    Surface uni = load_png_from_file ("libstreetmap/src/png/university.png");
    Surface hospital = load_png_from_file ("libstreetmap/src/png/hospital.png"); 
    Surface restaurant = load_png_from_file ("libstreetmap/src/png/restaurant.png");
    Surface fast_food = load_png_from_file ("libstreetmap/src/png/fast_food.png");
    Surface bank = load_png_from_file ("libstreetmap/src/png/bank.png");
    Surface fuel = load_png_from_file ("libstreetmap/src/png/fuel.png");
    Surface parking = load_png_from_file ("libstreetmap/src/png/parking.png");
    Surface poi = load_png_from_file ("libstreetmap/src/png/poi.png");
    Surface intersectionPin = load_png_from_file("libstreetmap/src/png/small_pin.png");
    Surface allpoi = load_png_from_file("libstreetmap/src/png/allpoi.png");
    Surface res = load_png_from_file("libstreetmap/src/png/restricted.png");
    Surface route = load_png_from_file("libstreetmap/src/png/route.png");
    Surface cross = load_png_from_file("libstreetmap/src/png/cross.png");
    Surface user_manual = load_png_from_file("libstreetmap/src/png/user_manual.png");
};

struct candidateRect {
    
    float ori_hegiht_bar;
    
    std::string text;
    t_bound_box rect;
    t_color text_style;
    t_color rect_style;
    unsigned index;
    bool selected;

    candidateRect(std::string _text, unsigned order, searchBarStruct* motherBar);
    
    void update(float x, float y);
    
    void switcher(bool active);
};

struct instructionRect {
    
    float ori_height_bar;
    std::string text;
    t_bound_box backgroud;
    t_color backgroundColor;
    
};

struct Buttons {
    
    bool uniON=false;
    bool hospitalOn=false;
    bool bankOn=false;
    bool foodOn=false;
    bool petroOn=false;
    bool parkOn=false;
    bool allOn=false;
    
};

struct intersectionPin {
    float x, y;
    
    intersectionPin(float x, float y);
};

struct POIPin {
    float x, y;
    
    POIPin(float x, float y);
};

struct zoomAnimeConfi{
    float left;
    float right;
    float top;
    float bottom;
};

struct searchBarStruct{
    std::string inTimeInput;
    
    std::string firstStreet, secondStreet, linker, POI_prefix;
    
    /* Graphics configurations */
    float search_bar_x;
    float search_bar_y;
    float search_bar_width;
    float search_bar_height;
    unsigned transparency;
    float text_x_center;
    float text_y_center;
    t_color font_color;
    t_color bar_color;
    bool activated;
    float orginalWidth;
    float candidateBoxHeight, widthExtension;
    
    // FSM value, representing what job the search bar is doing right now
    unsigned mode;
    
    FuzzySearch* activeSearch;
    FuzzySearch* anotherActiveSearch;
    
    FuzzySearch_POI* activeSearch_POI;
    
    // Used to Store boxes on screen which display all search candidates
    std::vector<candidateRect> candidatesBox_street;

    std::vector<candidateRect> candidatesBox_intersection;
    
    std::vector<candidateRect> candidatesBox_POI;
    
    // Used to store intersection IDs found from two streets
    std::set<IntersectionIndex> intersectIdClt;
    
    // Used to store boxes on screen which display all intersection candidates
    // Default constructor
    searchBarStruct(double, double);
    
    // Update properties on displaying of search bar
    void update();
    
    // update vector of candidates
    void updateCandidates_street();
    
    // update vector of candidates of intersections
    void updateCandidates_intersection();
    
    // update vector of candidates of POIs
    void updateCandidates_POI();
    
    // No parameter overload for updateCandidatesStyle
    void updateCandidatesStyle();
    
    // Update candidates displaying depending on position of the mouse
    void updateCandidatesStyle(float x, float y);
    
    // Check if user has checked on the search bar
    bool checkActivation(float x, float y);
    
    // Whenever during the input the user pressed "backspace" on keyboard, the
    // method will be invoked
    void backspace();
    
    // Whenever during the input, the user pressed any none functional key, the
    // method will be invoked
    void input(char key_pressed);
    
    // Initialize the FuzzySearch object
    void activateSearch(bool searchPOI = false);
    
    // Initialize another street search, then provide with intersection candidates
    void activateIntersectionSearch();
    
    // When the "enter" is pressed, one of three methods below will be invoked 
    // and present the result 
    void searchStreet();
    void searchIntersection();
    void searchPOIs();
    
    // In the case where mouse click is detected, check if mouse has clicked on one 
    // of the candidate boxes
    bool checkOnSearchSection();
    
    // Clear out highlights put on the screen
    void clearHighlights();
    
    // Clear out pins put on the screen for intersections
    void clearPins();
    
    void clearCandidatesFlag();
    
    // When tab is pressed, the program automatically complete the next word for user
    void autoComplete();
    
    // Detecting whether or not the user is entering another street
    bool isSecondStreetInput();
    
    // 
    void streetsHandler();
    
    string POIHandler();
    
    // 
    string getSecondStreetFromInTimeInput();
    
    unsigned findSelectedAndUpdateInput();
    
    bool checkForPOIPrefix();
    
    void shiftUpSelection();
    
    void shiftDownSelection();
    
    void drawself();
};

typedef searchBarStruct searchBar;

class animationManager {
public:
    
    animationManager(searchBar*);
    
    void invoke_animation (unsigned ani_no, coord obj = coord(0,0), t_bound_box vision = t_bound_box(0,0,0,0));
    
    bool isInAnimation();
    
    void find_button_in();
    
    void find_button_out();
    
private:
    
    searchBar* anime_obj;
    
    // Whether or not an animation is playing
    bool in_animation;
    
    unsigned current_anime;
    
    unsigned current_frame;
    
    unsigned total_frame;
    
    // The position of zooming point
    float obj_x, obj_y;
    
    // Used for zooming in and out animation
    zoomConi zoom_out_confi;
    zoomConi objConfi;
    
    t_bound_box objVision;
    
    void zooming_value_detector();
    
    void setObjConfiguration();
    
    void search_bar_in();
    
    void search_bar_out();
    
    void zoom_out_and_in();
    
    void play();
    
    void delay(long milliseconds);
};

class searchBarManager {
public:
    
    // constructor
    searchBarManager(searchBar* _main , searchBar* _sub);
    
    // Drawing required search bar
    void drawSearchBar();
    
    void drawHighlights(float zooming_level);
    
    void mouseMoveReact(float x, float y);
    
    bool mouseClickReact(float x, float y, t_event_buttonPressed button_info);
    
    bool isFindingRoute();
    
    searchBar* inputAcceptor();
    
    void lockSearchBar(searchBar* acceptor);
    
    void clearUpSearchBars();
    
    void searchRoute();
    
    bool isBothBarLocked();
    
    void showUpFindButton();
    
    void resetSearchBar(searchBar* acceptor);
    
    // Helping update main search bar or sub search bar
    void updateSearchBarsInput(searchBar* acceptor, string text, 
                               unsigned font_color_no = NULLCOLOR, unsigned bar_color_no = NULLCOLOR);
    
    // A overloaded version of update search bar
    void updateSearchBarsInput(searchBar* acceptor, string text, 
                               t_color font_color, t_color bar_color);
    
    friend void (button_func_query) (void (drawscreen) (void));
    
    friend void animationManager::find_button_in();
    
    friend void animationManager::find_button_out();
    
    PathInterface clickPath;
    
private:
    
    bool findRoute, inFindRoute;
    
    searchBar* main;
    
    searchBar* sub;
    
    float button_x;
    
    float button_y;
    
    // Three attributes for route finding
    int from;
    int to;
    unsigned searchFor;
    // attribute reserved for search for intersection to poi
    string to_POI;
    
    t_color backgroundStyle;
    
    t_bound_box background;
    
    t_color crossBackgroundStyle;
    
    t_bound_box crossBackground;
    
    // Below attributes are used in displaying of find route button
    bool findButtonOut;
    t_bound_box findRouteButton;
    t_color findRouteButtonStyle;
    float findRouteButtonTransparency;
    
    vector<unsigned> routeFound;
    
    // Instruction panel configuration
    t_bound_box instructionPanel;
    routeInstruction instructions;
    unsigned first;
    unsigned second;
    unsigned currentHighlight;
    
    void buttonMouseMoveReact(float x, float y);
    
    void buttonMouseClickReact(float x, float y);
    
    void findButtonMouseMoveReact(float x, float y);
    
    void findButtonMouserClickReact(float x, float y);
    
    void drawInstructions();
    
    void pinPlacer(unsigned interxId);
    
};

enum animations{
    PRESENT_SEARCH_BAR = 0,
    WARP_UP_SEARCH_BAR,
    AUTO_ZOOM_OUT,
    PRESENT_FIND_BUTTON,
    WARP_UP_FIND_BUTTON
};

enum infoType {
    POI =0,
    Intersection,
    Feature,
    Restriction,
    Do_not_show
};

enum search_state {
    inactive = 0,
    streetInSearch,
    highlightStreet,
    waitForInput,
    intersectionInSearch,
    highlightIntersection,
    waitForInput_POI,
    POIInSearch,
    highlightPOI,
    managerControlled
};

enum routeFindingSate {
    notInFind = 0,
    interToInter,
    interToPOI
};

/************************** Interface globals *********************************/
// search bar exists as a global in interface.h
static std::vector<t_color> predefined_styles = {  
    t_color(255, 255, 255, 122),        /* White, 50 % transparency : style for inactive search bar */
    t_color(255, 255, 255, 178),        /* White, 70 % transparency : style for inactive search bar */
    t_color(255, 255, 255, 255),        /* White, 100% transparency : style for active search bar */
    t_color(  0,   0,   0, 122),        /* Black, 50% transparency : style for text in inactive search bar */
    t_color(  0,   0,   0, 255),        /* Black, 100% transparency : style for text in active search bar */
    
    // Highlight style
    t_color( 41, 128, 185, 122),        /* Highlight color, 50% transparency */
    // Locked style
    t_color(137, 208, 212, 255),        /* Color used for locked search bar*/
    // The color you can always see
    t_color(255,   0,   0, 255),        /* Really red */
    // A NULL representing color
    t_color(  0,   0,   0,   0)         /* Nothing */
    };

static searchBarStruct search_bar(10., 10.);

static searchBarStruct search_bar_route(search_bar.search_bar_width + 130., 10.);

static searchBarManager search_bar_manager(&search_bar, &search_bar_route);

static animationManager animation_manager(&search_bar_route);

static std::vector<StreetSegmentIndex> streetResult;

static std::vector<intersectionPin> intersectionResult;

static std::vector<POIPin> POIResult;

static std::pair <infoType, std::string> information_bar = make_pair (Do_not_show, "");

static POI_Click landing_poi;

static std::vector<std::string> streetLinker = {
    "and", "&", "at"
    };

/************************ Call back functions for event loop *******************/
void act_on_mousebutton (float x, float y, t_event_buttonPressed button_info);

void act_on_mousemove (float x, float y);

void act_on_keypress (char key_pressed, int keysym);

void drawscreen();

/************************ helper functions for drawing *************************/
bool react_to_find_closest (double x, double y);

void react_to_highlight (float x, float y);

void react_to_landing_poi(float x, float y);

void draw_street(double zooming_level);

void drawCandidates(std::vector<candidateRect> candiVec);

void draw_segment_names (unsigned zooming_level);

void draw_features(float x, float y);
 
void draw_poi();

void draw_information_bar();

void highlight_intersection ();

void react_to_restriction(float x, float y);

void highlightStreets(const vector<unsigned> &streetIdxClt, 
                        float zooming_level, unsigned color = HIGHLIGHT);

/************************* Button call back functions **************************/
void (button_func_uni_off) (void (drawscreen) (void));

void (button_func_hospital_off) (void (drawscreen) (void));

void (button_func_food_off) (void (drawscreen) (void));

void (button_func_bank_off) (void (drawscreen) (void));

void (button_func_gas_off) (void (drawscreen) (void));

void (button_func_parking_off) (void (drawscreen) (void));

void (button_func_all_off) (void (drawscreen) (void));

void (button_func_query) (void (drawscreen) (void));

void (button_func_help) (void (drawscreen) (void));

#endif /* INTERFACE_H */

