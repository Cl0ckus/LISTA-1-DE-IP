#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <ctype.h>
#define MAX_LAPS 1
#define MAX_SPEED 7
#define VICTORY 'V'
#define TIE 'T'
#define DEFEAT 'D'

int pps = 1;

typedef struct{
    int position_switch;
    unsigned int x, y;
    int speed;
    int laps;
    int ID;
    char victory;
}PLAYER_SEND;

typedef struct{
    bool keys[6];
    bool timer_flag;
    unsigned int x, y;
    int position_switch;
    int ID;
}PLAYER_RECEIVE;

typedef struct{
    bool keys[6];
    int position_switch;
    unsigned int x, y;
    int speed;
    int laps;
    bool anti_cheat[2];
    bool timer_flag;
    bool red_flag;
    int ID;
    char victory;
}GAME;


typedef struct{

    int MAX_X, MIN_X, MAX_Y, MIN_Y;    

}LIMITED_POINTS;

GAME player_0 = {{false,false,false,false,false,false},0,0,0,0,0,{false,false},false,false,0,TIE};
GAME player_1;
PLAYER_SEND playerSend;
PLAYER_RECEIVE playerReceive;


//A PARTIR DAQUI COMEÇA A LÓGICA PARA O SERVER
//ESSES SÃO OS LIMITES QUE O CARRO NÃO PODE ULTRAPASSAR
//A VELOCIDADE ZERA QUANDO O CARRO TENTA ULTRAPASSAR 
//SE O CARRO TENTAR ULTRAPASSAR ALGUMA DAS BORDAS DA TELA, ELE VOLTA PARA A POSIÇÃO ANTERIOR E ATIVA UMA FLAG CHAMADA RED ZONE
//SE O CARRO TENTAR IR PARA O MEIO ELE ATIVA A FLAG RED ZONE
//RED ZONE É UMA FLAG QUE SERÁ USADA POR OUTRA FUNÇÃO PARA DIMINUIR A VELOCIDADE DO CARRO PARA NIVEIS ABSURDOS

GAME no_trespassing(GAME Rules){


    //ESTA É UMA ESTRUTURA QUE SALVA OS PONTOS MÁXIMOS E MINIMOS QUE O CARRO NÃO PODE ULTRAPASSAR
    //ESTÁ NA SEGUINTE ORDEM MAX_X, MIN_X, MAX_Y; MIN_Y;
    LIMITED_POINTS center[1] = {{1160,245,595,210}};
    LIMITED_POINTS board[1] = {{1340,0,740,70}};

    


        if(Rules.x<center[0].MAX_X && Rules.x>center[0].MIN_X && Rules.y<center[0].MAX_Y && Rules.y>center[0].MIN_Y){
            Rules.speed = 0;
            Rules.red_flag = true;

            
    
        }
        else{
            Rules.red_flag = false;
        }

        //VERIFICA SE O CARRO TENTOU SAIR DA TELA E IMPEDE ES AÇÃO
        if(Rules.x>=board[0].MAX_X){
            Rules.speed = 0;
            Rules.x = board[0].MAX_X;
            Rules.red_flag = true;
        }
        else if(Rules.x<=board[0].MIN_X){
            Rules.speed = 0;
            Rules.x = board[0].MIN_X;
            Rules.red_flag = true;
        }
        if(Rules.y>=board[0].MAX_Y){
            Rules.speed = 0;
            Rules.y = board[0].MAX_Y;
            Rules.red_flag = true;
        }
        else if(Rules.y<=board[0].MIN_Y){
            Rules.speed = 0;
            Rules.y = board[0].MIN_Y;
            Rules.red_flag = true;
        }

       
}



//ESTAS SÃO AS AÇÕES QUE SERÃO EXECUTADAS DE ACORDO COM CADA TECLA PRESSIONADA
// O CARRO SEMPRE IRÁ PARA FRENTE QUANDO A TECLA UP É PRESSIONADA E AUMENTARÁ A VELOCIDADE NUNCA ULTRAPASSANDO O MAX_SPEED
//E A VELOCIDADE DIMINUIRÁ ATÉ ZERO SE A TECLA DOWN É PRESSIONADA
//e depois o carro vai para trás
//mas frente e trás e lados mudam conforme a posição que o carro estiver , para isso serve a variavel position switch
//QUE BAZICAMENTE É MODIFICADA QUANDO A TECLA Z É PRESSIONADA
//MAS NÃO SE PREOUCUPEM COM ISSO FOQUEM APENAS NO ARRAY DE BOOLEANS CHAMDO KEYS; E VEJA O QUE ELA FAZE QUANDO CERTA POSIÇÃO
//DESSE ARRAY == TRUE
//PARA USAR BOOL INCLUAM A BIBLIOTECA stdbool.h no serve;
//BOOL É BASICAMENTE UMA VARIÁVEL QUE PODE SER VERDADEIRA OU FALSA
GAME acceleration(GAME Rules){    

    return Rules;
}
GAME key_actions(GAME Rules){
  
        int i;

        //if(Rules.red_flag == true){
        //    Rules.speed = 0;
        //    pps = 1;
        //}
        if(Rules.keys[0] == true && Rules.speed<=MAX_SPEED){    
            Rules.speed += pps;   
            printf("%d", Rules.speed); 
        }
        else{
            pps =1;
        }


        if(Rules.position_switch ==1){
           // printf(" position_switch = 1");
                
            if(Rules.keys[0] == true){    
                Rules.x-=Rules.speed;
                printf("KEYS 0 = TRUE\n");
            }
            else if(Rules.keys[1] == true){
                if(Rules.speed< 0){
                    Rules.speed+= pps;
                    printf("KEYS 1 = TRUE\n");
                }
                else if(Rules.speed>=0){
                    Rules.speed = 0;
                    Rules.x+=pps;
                }
                
            } 
            else if(Rules.keys[2] == true){
                Rules.y-= pps;
                printf("KEYS 2 = TRUE\n");
            }  
            else if(Rules.keys[3] == true){
                Rules.y+=pps;
                printf("KEYS 3 = TRUE\n");
            }       
                                        

        }
        else if(Rules.position_switch ==2){
                
            if(Rules.keys[0] == true){    
                Rules.y-= Rules.speed;
            }
            else if(Rules.keys[1] == true){
                if(Rules.speed>0){
                Rules.speed -=pps;
                }
                if(Rules.speed<= 0){
                    Rules.speed=0;
                    Rules.y+= pps;
                }
            } 
            else if(Rules.keys[2] == true){
                Rules.x-= pps;
            }  
            else if(Rules.keys[3] == true){
                Rules.x+=pps;
            }       
                                        

        }
        else if(Rules.position_switch ==3){
                
            if(Rules.keys[0] == true){    
                Rules.x+=Rules.speed;
            }
            else if(Rules.keys[1] == true){
                if(Rules.speed>0){
                Rules.speed -=pps;
                }
                if(Rules.speed<= 0){
                    Rules.speed =0;
                    Rules.x-= pps;
                }
            } 
            else if(Rules.keys[2] == true){
                Rules.y -= pps;
            }  
            else if(Rules.keys[3] == true){
                Rules.y += pps;
            }       
                                        
        }

        else if(Rules.position_switch ==4){
                
            if(Rules.keys[0] == true){    
                Rules.y+=Rules.speed;
            }
            else if(Rules.keys[1] == true){
                if(Rules.speed>0){
                Rules.speed -=pps;
                }
                if(Rules.speed<= 0){
                    Rules.speed = 0;
                    Rules.y-= pps;
                }
            } 
            else if(Rules.keys[2] == true){
                Rules.x -= pps;
            }  
            else if(Rules.keys[3] == true){
                Rules.x += pps;
            }       
                                        
        } 

            
        if(Rules.keys[4] == true){
                
                Rules.position_switch +=1;
               
                if(Rules.position_switch > 4){
                        Rules.position_switch = 1;
                }
                    Rules.keys[4] = false;
        }

        if(Rules.keys[5] == true){
                
                Rules.position_switch -=1;
               
                if(Rules.position_switch <= 0){
                        Rules.position_switch = 4;
                }
                    Rules.keys[5] = false;
        }          

        Rules.timer_flag = false;
        for(i=0;i<6;i++){
            Rules.keys[i] = false;
        }

        return Rules;
                             
}



//ESTA FUNÇÃO CONTA O NÚMERO DE VOLTAS BASEANDO-SE NO FATO QUE O CARRO PASSOU PREVIAMENTE POR 2 ÁREAS DO MAPA
//Os valores de x e y são implementados segundo a mesma regra das funões anteriores
GAME lap_counter(GAME Rules){


    const int MIN_START_X =790, MAX_START_X = 900,  MIN_START_Y = 605, MAX_START_Y = 740;
    LIMITED_POINTS corners[2] = {{1100,850,220,60},{430,232,740,605}};


    if(Rules.x<=corners[0].MAX_X && Rules.x>=corners[0].MIN_X && Rules.y<=corners[0].MAX_Y && Rules.y>=corners[0].MIN_Y){

        Rules.anti_cheat[0] = true;
   
    }

    if(Rules.x<=corners[1].MAX_X && Rules.x>=corners[1].MIN_X && Rules.y<=corners[1].MAX_Y && Rules.y>=corners[1].MIN_Y){

        Rules.anti_cheat[1] = true;
    }

    


    if(Rules.anti_cheat[0] == true && Rules.anti_cheat[1]==true && Rules.x >= MIN_START_X && Rules.x <= MAX_START_X && Rules.y >= MIN_START_Y && Rules.y<=MAX_START_Y && Rules.laps<=MAX_LAPS){
            

            Rules.laps +=1 ;
            Rules.anti_cheat[0] = false; 
            Rules.anti_cheat[1] = false; 
              
    }

    return Rules;

}


GAME player_victory(GAME Rules){

    if(Rules.laps >= MAX_LAPS){
       Rules.victory = VICTORY;
    }

    return Rules;

}

PLAYER_SEND compress_struct_to_send(GAME Rules){

    PLAYER_SEND player_send;

    player_send.x = Rules.x;
    player_send.y = Rules.y;
    player_send.position_switch = Rules.position_switch;
    player_send.ID = Rules.ID;
    player_send.laps = Rules.laps;
    player_send.speed = Rules.speed;

    return player_send;
}

GAME expand_struct_to_use(PLAYER_RECEIVE player_receive){

    GAME Rules;
    
    Rules.x = player_receive.x;
    Rules.y = player_receive.y;
    Rules.position_switch = player_receive.position_switch;
    Rules.timer_flag = player_receive.timer_flag;

    return Rules;
}


GAME PSEUDO_SERVER(GAME Rules){

    Rules = acceleration(Rules);
    Rules = key_actions(Rules);
    Rules = lap_counter(Rules);
    Rules = no_trespassing(Rules);
    Rules = player_victory(Rules);

    return Rules;
}

GAME clear_struct(GAME Rules){

    int i;
    for(i=0; i<6; i++){
        Rules.keys[i] = false;
    }

    Rules.position_switch = 0;
    Rules.x =0; 
    Rules.y =0;
    Rules.speed =0;
    Rules.laps = 0;
    for(i=0;i<2;i++){
        Rules.anti_cheat[i] = false;
    }
    Rules.timer_flag = false;
    Rules.red_flag = false;
    Rules.ID =0;
    Rules.victory =TIE;



    return Rules;
}

void define_winner(GAME *first_player,GAME *second_player){
    if((*first_player).laps >= MAX_LAPS){
        (*first_player).victory = VICTORY;
        (*second_player).victory =DEFEAT;
    }
    if((*second_player).laps >= MAX_LAPS){
        (*second_player).victory = VICTORY;
        (*first_player).victory =DEFEAT;
    }
    else{
        (*second_player).victory = TIE;
        (*first_player).victory = TIE;
    }
}

GAME clear_struct_game(GAME Rules){







}