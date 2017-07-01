/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "m2.h"
#include "graphics.h"
#include "load_map.h"
#include "interface.h"

zoomConi world;

float screen_ratio_width;
float screen_ratio_height;

//draws all spatial data based on the current state of the database
void draw_map (){
    
    t_color t_Color(238, 236, 230, 255);
  // t_color t_Color(219,214,210,255);
    init_graphics ("BEST MAP IN THE WORLD", t_Color);
     
    double xmin = ((*getPtrToIntersectSet())[getNumberOfIntersections()+2].cartesian_p.first);
    double ymin = (*getPtrToIntersectSet())[getNumberOfIntersections()].location.lat();
    double xmax = ((*getPtrToIntersectSet())[getNumberOfIntersections()+3].cartesian_p.first);
    double ymax = (*getPtrToIntersectSet())[getNumberOfIntersections()+1].location.lat();
    
    (*getPtrToIntersectSet()).pop_back();   // f
    (*getPtrToIntersectSet()).pop_back();   // u
    (*getPtrToIntersectSet()).pop_back();   // c
    (*getPtrToIntersectSet()).pop_back();   // k
    
    // Use bottom and right values to calculate screen width height ratio for
    // use of animation
    world.bottom = get_visible_screen().bottom();
    world.right = get_visible_screen().right();
    
    screen_ratio_width = (world.right)/(world.right + world.bottom);
    screen_ratio_height = (world.bottom)/(world.right + world.bottom);
    
    // Record initial coordinates of the world
    world.left = xmin-VISIBLE_WORLD_TOLERANCE;
    world.bottom = ymin-VISIBLE_WORLD_TOLERANCE;
    world.right = xmax+VISIBLE_WORLD_TOLERANCE;
    world.top = ymax+VISIBLE_WORLD_TOLERANCE;
    
    set_visible_world (world.left, world.bottom, 
                       world.right, world.top);
    
    setcolor(238, 236, 230, 255);
    fillrect(world.left, world.bottom , world.right, world.top);
    
    update_message("Yo this is our first window");
    
    set_mouse_move_input(true);
    
    set_keypress_input(true);
    
    //creates the buttons to turn on and off poi display
    enable_or_disable_button (0, true);
    create_button("Zoom Fit","University On", button_func_uni_off);
    enable_or_disable_button (1, true);
    create_button("University On","Hospital On", button_func_hospital_off);
      enable_or_disable_button (2, true);
    create_button("Hospital On","Bank On", button_func_bank_off);
    enable_or_disable_button (3, true);
    create_button("Bank On","Food On", button_func_food_off);
    enable_or_disable_button (4, true);
    create_button("Food On","Petro On", button_func_gas_off);
    enable_or_disable_button (5, true);
    create_button("Petro On","Parking On", button_func_parking_off);
    enable_or_disable_button (6, true);
    create_button("Parking On","All POI On", button_func_all_off);
    enable_or_disable_button (6, true);
    create_button("All POI On","Query Path", button_func_query);

    enable_or_disable_button (7, true);
    create_button("Query Path","Help", button_func_help);
    event_loop (act_on_mousebutton, act_on_mousemove,  act_on_keypress, drawscreen);
  //  close_graphics();
    std::cout<<"graphics closed \n";
    destroy_button("University Off");
    destroy_button("Hospital Off");
    destroy_button("Bank Off");
    destroy_button("Food Off");
    destroy_button("Petro Off");
    destroy_button("Parking Off");
    destroy_button("All POI Off");
    
    destroy_button("University On");
    destroy_button("Hospital On");
    destroy_button("Food On");
    destroy_button("Petro On");
    destroy_button("Parking On");
    destroy_button("University On");
    destroy_button("University On");
    destroy_button("Bank On");
    destroy_button("Bank On");
    destroy_button("Query Path");
    destroy_button("Help");
}

