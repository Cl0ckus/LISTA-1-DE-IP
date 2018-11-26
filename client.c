//==============================================
#include "client.h"
#include "GameRules.h"
#include "AllegroCore.h"
#include "ACore.h"
//============================================
//=========================

void game_singleplayer();
void game_multiplayer();

typedef struct{
    int x, y, position_switch;
}CAR;

CAR myCar;
CAR adCar;
char server_ip[30] = {"172.20.4.4"};
int id;
GAME useless;
GAME PLEASE_WORK = {{false,false,false,false,false,false},0,0,0,0,0,{false,false}, false, false, 0, TIE};
PLAYER_SEND packageR; //package que o jogador irá receber do server
PLAYER_RECEIVE packageS; //package que o jogador irá enviar ao server

//FUNÇÃO QUE CONECTA O JOGADOR NO SERVER
void connect_to_server(){

    enum conn_ret_t answer;

    do{
        answer = connectToServer(server_ip);
        if(answer == SERVER_DOWN){
            puts("Server is Down. \n");
        }
        if(answer == SERVER_FULL){
            puts("Server is full. \n");
        }
        if(answer == SERVER_CLOSED){
            puts("Server is Down. \n");
        }
        if(answer == SERVER_TIMEOUT){
            puts("Server time out. \n");
        }
    }while(answer!=SERVER_UP);


    recvMsgFromServer(&id,WAIT_FOR_IT);
}

//FUNÇÃO QUE LIMPA O LIXO DE MEMPÓRIA DE UMA ESTRUTURA ANTES DE USÁ-LA
void clear_struct_trash(){
    int i;


    packageR.x = 0;
    packageR.y = 0;
    packageR.position_switch = 0;
    packageR.speed = 0;
    packageR.laps = 0;
    packageR.ID = 0;
    packageR.victory = TIE;

    packageS.x = 0;
    packageS.y = 0;
    packageS.position_switch = 0;
    packageS.ID = 0;
    for(i = 0; i< 6; i++){
        packageS.keys[i] = false;
    }
    packageS.timer_flag = false;

    
}

//FUNÇÃO QUE SETA A POSIÇÃO INICIAL DO JOGADOR
void set_position_id(){
    if(id == 0){
        packageR.x = 940;
        packageR.y = 650;
        packageR.position_switch =1;
        packageR.ID =0;

        myCar.x = packageR.x;
        myCar.y = packageR.y;
        myCar.position_switch = packageR.position_switch;

        adCar.x = 940;
        adCar.y = 700;
        adCar.position_switch = packageR.position_switch;

        packageS.x = 940;
        packageS.y = 650;
        packageS.ID =0;
        packageS.position_switch =1;




    }
    else if(id == 1){

        packageR.x = 940;
        packageR.y = 650;
        packageR.position_switch =1;
        packageR.ID =1;

        myCar.x = packageR.x;
        myCar.y = packageR.y;
        myCar.position_switch = packageR.position_switch;

        adCar.x = 940;
        adCar.y = 700;
        adCar.position_switch = packageR.position_switch;

        packageS.x = 940;
        packageS.y = 700;
        packageS.ID =1;
        packageS.position_switch =1     ;  
    }
}

void set_position_id_big_struct(){

     if(id == 0){
        PLEASE_WORK.x = 940;
        PLEASE_WORK.y = 650;
        PLEASE_WORK.position_switch =1;
        PLEASE_WORK.ID =0;

        myCar.x = PLEASE_WORK.x;
        myCar.y = PLEASE_WORK.y;
        myCar.position_switch = PLEASE_WORK.position_switch;

        adCar.x = 940;
        adCar.y = 700;
        adCar.position_switch = PLEASE_WORK.position_switch;


    }
    else if(id == 1){

        PLEASE_WORK.x = 940;
        PLEASE_WORK.y = 650;
        PLEASE_WORK.position_switch =1;
        PLEASE_WORK.ID =1;

        myCar.x = PLEASE_WORK.x;
        myCar.y = PLEASE_WORK.y;
        myCar.position_switch = PLEASE_WORK.position_switch;

        adCar.x = 940;
        adCar.y = 700;
        adCar.position_switch = PLEASE_WORK.position_switch;
 
    }


    
}

void draw_victory(){
    if(packageR.victory == VICTORY && packageR.laps>=MAX_LAPS){
        al_clear_to_color(al_map_rgb(255,255,255));
        al_draw_bitmap(you_won,0,0,0);
    }
    else if(packageR.victory == DEFEAT && packageR.laps<=MAX_LAPS){
         al_clear_to_color(al_map_rgb(255,255,255));
         al_draw_bitmap(you_lost,0,0,0);

    }
}

void set_actual_position(){

    packageS.x = packageR.x;
    packageS.y = packageR.y;
    packageS.position_switch =  packageR.position_switch;
}

void printConnectScreen(char str[])
{
    al_draw_bitmap(initial,0,0,0);

    al_draw_text(nfs, al_map_rgb(255, 255, 255), DISPLAY_WIDTH / 2, 30, ALLEGRO_ALIGN_CENTRE, "Digite o IP do server:");

    if (strlen(str) > 0)
    {
        al_draw_text(nfs, al_map_rgb(255, 255, 255), DISPLAY_WIDTH / 2,
                     (DISPLAY_HEIGHT - al_get_font_ascent(nfs)) / 2,
                     ALLEGRO_ALIGN_CENTRE, str);
    }
    else
    {
        al_draw_text(nfs, al_map_rgb(255, 255, 255), DISPLAY_WIDTH / 2,
                     (DISPLAY_HEIGHT - al_get_font_ascent(nfs)) / 2,
                     ALLEGRO_ALIGN_CENTRE, "0.0.0.0");
    }
}
void read_ip(){

    bool login = false;
    ALLEGRO_EVENT event;

    while(login == false){
        while(!al_event_queue_is_empty(event_queue)){
        al_wait_for_event(event_queue, &event);


        if (event.type == ALLEGRO_EVENT_KEY_CHAR){
        if (strlen(server_ip) <= 100){
            char temp[100] = {event.keyboard.unichar, '\0'};
            if (event.keyboard.unichar == ' '){
                strcat(server_ip, temp);
            }
            else if (event.keyboard.unichar >= '!' &&
                     event.keyboard.unichar <= '?'){
                strcat(server_ip, temp);
            }
            else if (event.keyboard.unichar >= 'A' &&
                     event.keyboard.unichar <= 'Z')
            {
                strcat(server_ip, temp);
            }
            else if (event.keyboard.unichar >= 'a' &&
                     event.keyboard.unichar <= 'z')
            {
                strcat(server_ip, temp);
            }
        }

        if (event.keyboard.keycode == ALLEGRO_KEY_BACKSPACE && strlen(server_ip) != 0)
        {
            server_ip[strlen(server_ip) - 1] = '\0';
        }
    }




        if (event.type == ALLEGRO_EVENT_KEY_DOWN){
            switch(event.keyboard.keycode){
                    case ALLEGRO_KEY_ENTER:
                        login = true;
                        break;
                }
            }

            if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
                login = true;
            }

        printConnectScreen(server_ip);
        al_flip_display();
        al_clear_to_color(al_map_rgb(0, 0, 0));
         
        }


    }
   
}


void menu(){

    ALLEGRO_EVENT event;
    ALLEGRO_TIMEOUT timeout;
    bool EXIT = false;
    bool BACK = false;
    int key = 0, image = 0, sub_menu =0, menu_play =0;
    bool begin = true;

    al_init_timeout(&timeout,1/MY_FPS);

    while(EXIT==false){

        start_timer();

        /*if(image == 0 && begin == true){
            al_draw_bitmap(def,0,0,0);
            al_flip_display();
            al_rest(1.5);
            al_draw_textf(nfs, al_map_rgb(255,255,255),DISPLAY_WIDTH/2,DISPLAY_HEIGHT/2,ALLEGRO_ALIGN_CENTRE, "Press any key to start.");
            al_rest(1.5);
            al_flip_display();
            al_clear_to_color(al_map_rgb(255,255,255));
       }
       
       al_rest(0.1);

       if(event.type == ALLEGRO_EVENT_KEY_DOWN && begin == true){
                   al_clear_to_color(al_map_rgb(255,255,255));
                   al_draw_bitmap(menu1,0,0,0);
                   al_rest(0.5);
                   al_flip_display();
                   begin = false;              
        }
        */

        al_wait_for_event_until(menu_queue,&event,&timeout);
                
           

            if(event.type == ALLEGRO_EVENT_KEY_DOWN){
                
                switch(event.keyboard.keycode){

                    case ALLEGRO_KEY_UP:
                        key =1;
                        break;
                    case ALLEGRO_KEY_DOWN:
                        key = 2;
                        break;
                    case ALLEGRO_KEY_ENTER:
                        key = 3;
                        break;
                    default:
                        break;
                }
            }
            
            if(sub_menu ==0){
            if(key == 2 ){
            if(image<=4){
                    
                    image++;
            }
            if(image > 4){
                    image = 1;
            }
                key = 0;
            }
            else if(key == 1 ){

                image --;
                
                if(image <= 0){
                    image = 4;
                }
                key = 0;

            }

            if(key == 3){


            if(image == 1){

                sub_menu = 1;
                

            }
            else if(image ==2){
                sub_menu =2;
                
            }
            else if(image == 3){
                sub_menu =3;     
            }
            else if(image ==4){
                EXIT = true;
                        
            }

            }

            if(image == 1){
                al_draw_bitmap(play,0,0,0);
                al_rest(0.1);
                al_flip_display();

            }
            else if(image ==2){
                 al_draw_bitmap(help,0,0,0);
                 al_rest(0.1);
                 al_flip_display();

            }
            else if(image == 3){
                al_draw_bitmap(credits,0,0,0);
                al_rest(0.1);
                al_flip_display();

            }
            else if(image ==4){
                al_draw_bitmap(exit1,0,0,0);
                al_rest(0.1);
                al_flip_display();
                        
            }
            }

            

            if(sub_menu == 1){
                
                if(key == 2 ){
                if(image<=3){
                        
                        menu_play ++;
                }
                    if(image > 4){
                        menu_play = 1;
                }
                    key = 0;
                }
                else if(key == 1 ){

                    menu_play --;
                    
                    if(image <= 0){
                        menu_play = 3;
                    }
                    key = 0;

                }

                else if(key == 3){

                    if(menu_play == 1){

                        //game_singleplayer();

                    }
                    else if(menu_play == 2){

                        //read_ip();
                        connectToServer(server_ip);
                        game_multiplayer();   
                    }

                    else if(menu_play == 3){
                        image = 0;
                        sub_menu =0;
                        menu_play = 0;
                        key = 0;
                        
                    }

                }

            if(menu_play == 0 ){

                al_draw_bitmap(game_mode,0,0,0);
                al_rest(0.1);
                al_flip_display();
                        

            }
            else if(menu_play == 1){
                al_draw_bitmap(game_single,0,0,0);
                al_rest(0.1);
                al_flip_display();
                        

            }
            else if(menu_play == 2){
                al_draw_bitmap(game_multi,0,0,0);
                al_rest(0.1);
                al_flip_display();
                        
                        
            }
            else if(menu_play == 3){

                al_draw_bitmap(game_back,0,0,0);
                al_rest(0.1);
                al_flip_display();
                                   
            }

            }

            

            


            
            if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
                EXIT = true;
            }
            al_rest(0.1);
            al_flip_display();


        if(get_time() < 1.0/MY_FPS){
            al_rest((1.0/MY_FPS) - get_time());
        }
            
    

                           
        }
        
  


}


//====================================
//Header of the functions;
//===================================

//=================================================================================================
//IGNOREM ESSA PARTE, ESTA SÃO APENAS COISAS DA ALLEGRO
int main(){

    if(!initialize()){
        fprintf(stderr, "ERROR, Program not initialized correct.\n");
        return -1;
    }
    if(!loadFonts()){
        fprintf(stderr, "ERROR, Fonts not initialized correct.\n");
        return -1;
    }
    if(!loadBitmaps()){
        fprintf(stderr, "ERROR, Bitmaps not initialized correct.\n");
       return -1;
    }
    if(!loadAudios()){
        fprintf(stderr, "ERROR, Audios not initialized correct.\n");
        return -1;
    }
    registerEvents();
    //game();
    menu();
    ALLEGRO_EVENT event;
    if(false){
  
        read_ip();

    }
    destroy();
    return 0;
}

void choose_car(){
    if(id == 0){
        my_car = blue_car;
        ad_car = red_car;
    }
    else if(id == 1){
        my_car = red_car;
        ad_car = blue_car;
    }
}
void draw_car(){


    const int CAR_WIDTH = al_get_bitmap_width(blue_car), CAR_HEIGHT = al_get_bitmap_height(blue_car);

        if(PLEASE_WORK.ID == id){

            myCar.x = PLEASE_WORK.x;
            myCar.y = PLEASE_WORK.y;
            myCar.position_switch = PLEASE_WORK.position_switch;

            if(PLEASE_WORK.position_switch){
                switch(PLEASE_WORK.position_switch){
                    case 1:
                        al_draw_rotated_bitmap(my_car,CAR_WIDTH/2,CAR_HEIGHT/2, PLEASE_WORK.x, PLEASE_WORK.y,DEGREE_270, 0);
                        break;
                    case 2:
                        al_draw_rotated_bitmap(my_car,CAR_WIDTH/2,CAR_HEIGHT/2, PLEASE_WORK.x, PLEASE_WORK.y,DEGREE_360, 0);                        
                        break;
                    case 3:
                        al_draw_rotated_bitmap(my_car,CAR_WIDTH/2,CAR_HEIGHT/2, PLEASE_WORK.x, PLEASE_WORK.y,DEGREE_45, 0);
                        break;
                    case 4:
                        al_draw_rotated_bitmap(my_car,CAR_WIDTH/2,CAR_HEIGHT/2, PLEASE_WORK.x, PLEASE_WORK.y,DEGREE_180, 0);                        
                        break;
                    default:
                         break;
                }
            }
            if(adCar.position_switch){

                switch(adCar.position_switch){
                 case 1:
                        al_draw_rotated_bitmap(ad_car,CAR_WIDTH/2,CAR_HEIGHT/2, adCar.x, adCar.y,DEGREE_270, 0);
                        break;
                    case 2:
                        al_draw_rotated_bitmap(ad_car,CAR_WIDTH/2,CAR_HEIGHT/2, adCar.x, adCar.y,DEGREE_360, 0);                        
                        break;
                    case 3:
                        al_draw_rotated_bitmap(ad_car,CAR_WIDTH/2,CAR_HEIGHT/2, adCar.x, adCar.y,DEGREE_45, 0);
                        break;
                    case 4:
                        al_draw_rotated_bitmap(ad_car,CAR_WIDTH/2,CAR_HEIGHT/2, adCar.x, adCar.y,DEGREE_180, 0);                        
                        break;
                    default:
                         break;
                }

        }
        }
        else if(packageR.ID != id){

            adCar.x = PLEASE_WORK.x;
            adCar.y = PLEASE_WORK.y;
            adCar.position_switch = packageR.position_switch;

            if(packageR.position_switch){
            switch(packageR.position_switch){
                case 1:
                    al_draw_rotated_bitmap(ad_car,CAR_WIDTH/2,CAR_HEIGHT/2, PLEASE_WORK.x, PLEASE_WORK.y,DEGREE_270, 0);
                    break;
                case 2:
                    al_draw_rotated_bitmap(ad_car,CAR_WIDTH/2,CAR_HEIGHT/2, PLEASE_WORK.x, PLEASE_WORK.y,DEGREE_360, 0);                        
                    break;
                case 3:
                    al_draw_rotated_bitmap(ad_car,CAR_WIDTH/2,CAR_HEIGHT/2, PLEASE_WORK.x, PLEASE_WORK.y,DEGREE_45, 0);
                    break;
                case 4:
                    al_draw_rotated_bitmap(ad_car,CAR_WIDTH/2,CAR_HEIGHT/2, PLEASE_WORK.x, PLEASE_WORK.y,DEGREE_180, 0);                        
                    break;
                default:
                    break;
                }
            }
            if(myCar.position_switch){

                switch(myCar.position_switch){
                    case 1:
                        al_draw_rotated_bitmap(my_car,CAR_WIDTH/2,CAR_HEIGHT/2,myCar.x, myCar.y,DEGREE_270, 0);
                        break;
                    case 2:
                        al_draw_rotated_bitmap(my_car,CAR_WIDTH/2,CAR_HEIGHT/2, myCar.x, myCar.y,DEGREE_360, 0);                        
                        break;
                    case 3:
                        al_draw_rotated_bitmap(my_car,CAR_WIDTH/2,CAR_HEIGHT/2,myCar.x, myCar.y,DEGREE_45, 0);
                        break;
                    case 4:
                        al_draw_rotated_bitmap(my_car,CAR_WIDTH/2,CAR_HEIGHT/2, myCar.x, myCar.y,DEGREE_180, 0);                        
                        break;
                    default:
                         break;
                }
        }
    }       

}

//=========================================





void game_multiplayer(){

    bool EXIT = false;
    unsigned long long int  game_timer_seconds =0, game_timer_minutes = 0;
    int x= 940, y=650,h=650,w=940;
    const int CAR_WIDTH = al_get_bitmap_width(blue_car), CAR_HEIGHT = al_get_bitmap_height(blue_car);
    ALLEGRO_EVENT event;
    set_position_id_big_struct();
    choose_car();
    
    al_start_timer(key_timer); 
    while(EXIT==false){

       start_timer();

        while(!al_is_event_queue_empty(event_queue)){
            
            sendMsgToServer(&PLEASE_WORK, sizeof(GAME));
            recvMsgFromServer(&PLEASE_WORK, DONT_WAIT);
            al_wait_for_event(event_queue, &event);

        
            if(event.type == ALLEGRO_EVENT_KEY_DOWN){

                

                switch(event.keyboard.keycode){

                    case ALLEGRO_KEY_UP:
                        PLEASE_WORK.keys[0] = true;
                        break;
                    case ALLEGRO_KEY_DOWN:
                        PLEASE_WORK.keys[1] = true;
                        break;
                    case ALLEGRO_KEY_LEFT:
                        PLEASE_WORK.keys[2] = true;
                        break;
                    case ALLEGRO_KEY_RIGHT:
                        packageS.keys[3] = true;
                        break;
                    case ALLEGRO_KEY_ESCAPE:
                        EXIT = true;
                        break;
                    case ALLEGRO_KEY_X:
                        PLEASE_WORK.keys[4] = true;
                        break;
                    case ALLEGRO_KEY_Z:
                        PLEASE_WORK.keys[5] = true;
                        break;   
                    case ALLEGRO_KEY_W:
                        PLEASE_WORK.keys[0] = true;
                        break;
                    case ALLEGRO_KEY_S:
                        PLEASE_WORK.keys[1] = true;
                        break;
                    case ALLEGRO_KEY_A:
                        PLEASE_WORK.keys[2] = true;
                        break;
                    case ALLEGRO_KEY_D:
                        PLEASE_WORK.keys[3] = true;
                        break;

                }
            }

            if(event.type == ALLEGRO_EVENT_KEY_UP){

                

                switch(event.keyboard.keycode){

                    case ALLEGRO_KEY_UP:
                        PLEASE_WORK.keys[0] = false;
                        break;
                    case ALLEGRO_KEY_DOWN:
                        PLEASE_WORK.keys[1] = false;
                        break;
                    case ALLEGRO_KEY_LEFT:
                        PLEASE_WORK.keys[2] = false;
                        break;
                    case ALLEGRO_KEY_RIGHT:
                        PLEASE_WORK.keys[3] = false;
                        break;
                    case ALLEGRO_KEY_X:
                        PLEASE_WORK.keys[4] = false;
                        break;
                    case ALLEGRO_KEY_Z:
                        PLEASE_WORK.keys[5] = false;
                        break;    
                    case ALLEGRO_KEY_W:
                        PLEASE_WORK.keys[0] = false;
                        break;
                    case ALLEGRO_KEY_S:
                        PLEASE_WORK.keys[1] = false;
                        break;
                    case ALLEGRO_KEY_A:
                        PLEASE_WORK.keys[2] = false;
                        break;
                    case ALLEGRO_KEY_D:
                        PLEASE_WORK.keys[3] = false;
                        break;

                }
            }
           
       
            if(event.type == ALLEGRO_EVENT_TIMER){
                packageS.timer_flag = true;
                game_timer_seconds ++;

                if(game_timer_seconds >= 60){
                    game_timer_seconds = 0;
                    game_timer_minutes ++;
                }
            } 



  
    al_draw_bitmap(bar,0,0,0);
    al_draw_textf(nfs, al_map_rgb(255,255,255),DISPLAY_WIDTH/2,2,ALLEGRO_ALIGN_CENTRE, "TIME : 0%lld : %lld   LAPS : %d SPEED: %d km/h ",game_timer_minutes, game_timer_seconds, PLEASE_WORK.laps, abs(PLEASE_WORK.speed)*17);

                
                al_draw_textf(nfs, al_map_rgb(255,255,255),DISPLAY_WIDTH/2,DISPLAY_HEIGHT/2,ALLEGRO_ALIGN_CENTRE, "x = %d, y= %d, %d",PLEASE_WORK.x, PLEASE_WORK.y, PLEASE_WORK.position_switch);
                al_flip_display();                        
                al_draw_bitmap(race_track_2,0,0,0); 
                draw_car();
                draw_victory();
                
     
        }

        
        if(event.type ==ALLEGRO_EVENT_DISPLAY_CLOSE){
            EXIT = true;
        }
            

        

        if(get_time() < 1.0/MY_FPS){
            al_rest((1.0/MY_FPS) - get_time());
        }

        //PLEASE_WORK.x = 3;
    }
}

void game_singleplayer(){

    bool EXIT = false;
    unsigned long long int  game_timer_seconds =0, game_timer_minutes = 0;
    int x= 940, y=650,h=650,w=940;
    const int CAR_WIDTH = al_get_bitmap_width(blue_car), CAR_HEIGHT = al_get_bitmap_height(blue_car);
    ALLEGRO_EVENT event;
    int i;
    id = 1;
    clear_struct_trash();
    set_position_id();
    

    al_start_timer(key_timer); 
    while(EXIT==false){

       start_timer();

        while(!al_is_event_queue_empty(event_queue)){
       
           
            al_wait_for_event(event_queue, &event);
        
            if(event.type == ALLEGRO_EVENT_KEY_DOWN){

                

                switch(event.keyboard.keycode){

                    case ALLEGRO_KEY_UP:
                        useless.keys[0] = true;
                        break;
                    case ALLEGRO_KEY_DOWN:
                        useless.keys[1] = true;
                        break;
                    case ALLEGRO_KEY_LEFT:
                        useless.keys[2] = true;
                        break;
                    case ALLEGRO_KEY_RIGHT:
                        useless.keys[3] = true;
                        break;
                    case ALLEGRO_KEY_ESCAPE:
                        EXIT = true;
                        break;
                    case ALLEGRO_KEY_X:
                        useless.keys[4] = true;
                        break;
                    case ALLEGRO_KEY_Z:
                        useless.keys[5] = true;
                        break;   
                    case ALLEGRO_KEY_W:
                        useless.keys[0] = true;
                        break;
                    case ALLEGRO_KEY_S:
                        useless.keys[1] = true;
                        break;
                    case ALLEGRO_KEY_A:
                        useless.keys[2] = true;
                        break;
                    case ALLEGRO_KEY_D:
                        useless.keys[3] = true;
                        break;

                }
            }

            /*if(event.type == ALLEGRO_EVENT_KEY_UP){

                

                switch(event.keyboard.keycode){

                    case ALLEGRO_KEY_UP:
                        useless.keys[0] = false;
                        break;
                    case ALLEGRO_KEY_DOWN:
                        useless.keys[1] = false;
                        break;
                    case ALLEGRO_KEY_LEFT:
                        useless.keys[2] = false;
                        break;
                    case ALLEGRO_KEY_RIGHT:
                        useless.keys[3] = false;
                        break;
                    case ALLEGRO_KEY_X:
                        useless.keys[4] = false;
                        break;
                    case ALLEGRO_KEY_Z:
                        useless.keys[5] = false;
                        break;    
                    case ALLEGRO_KEY_W:
                        useless.keys[0] = false;
                        break;
                    case ALLEGRO_KEY_S:
                        useless.keys[1] = false;
                        break;
                    case ALLEGRO_KEY_A:
                        useless.keys[2] = false;
                        break;
                    case ALLEGRO_KEY_D:
                        useless.keys[3] = false;
                        break;

                }
            }*/
           
       
            if(event.type == ALLEGRO_EVENT_TIMER){
                useless.timer_flag = true;
                game_timer_seconds ++;

                if(game_timer_seconds >= 60){
                    game_timer_seconds = 0;
                    game_timer_minutes ++;
                }
            } 




    
    
    al_draw_bitmap(bar,0,0,0);
    al_draw_textf(nfs, al_map_rgb(255,255,255),DISPLAY_WIDTH/2,2,ALLEGRO_ALIGN_CENTRE, "TIME : 0%lld : %lld   LAPS : %d SPEED: %d km/h ",game_timer_minutes, game_timer_seconds, useless.laps, abs(useless.speed)*17);
    useless = PSEUDO_SERVER(useless);
                
                al_draw_textf(nfs, al_map_rgb(255,255,255),DISPLAY_WIDTH/2,DISPLAY_HEIGHT/2,ALLEGRO_ALIGN_CENTRE, "x = %d, y= %d, %d",useless.x, useless.y, useless.position_switch);
                al_flip_display();                        
                al_draw_bitmap(race_track_2,0,0,0);
          
            if(useless.position_switch){
                switch(useless.position_switch){

                    case 1:
                        al_draw_rotated_bitmap(blue_car,CAR_WIDTH/2,CAR_HEIGHT/2, useless.x, useless.y,DEGREE_270, 0);
                        break;
                    case 2:
                        al_draw_rotated_bitmap(blue_car,CAR_WIDTH/2,CAR_HEIGHT/2, useless.x, useless.y,DEGREE_360, 0);                        
                        break;
                    case 3:
                        al_draw_rotated_bitmap(blue_car,CAR_WIDTH/2,CAR_HEIGHT/2, useless.x, useless.y,DEGREE_45, 0);
                        break;
                    case 4:
                        al_draw_rotated_bitmap(blue_car,CAR_WIDTH/2,CAR_HEIGHT/2, useless.x, useless.y,DEGREE_180, 0);                        
                        break;
                    default:
                        al_draw_textf(nfs, al_map_rgb(255,255,255),DISPLAY_WIDTH/2,DISPLAY_HEIGHT/2,ALLEGRO_ALIGN_CENTRE, "NMA x = %d, y= %d",useless.x, useless.y);
                        break;
                }
            }

            

            
        }


        
        if(event.type ==ALLEGRO_EVENT_DISPLAY_CLOSE){
            EXIT = true;
        }
        
     


        if(get_time() < 1.0/MY_FPS){
            al_rest((1.0/MY_FPS) - get_time());
        }

        
    }
}




