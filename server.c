#include "server.h"
#include "GameRules.h"
#define MAX_PLAYERS 1
#define MIN_PLAYERS 1

//ESTE SERVER SERÁ CRIADO ORIGINALMENTE PARA 2 PLAYERS ASSIM COMO O CÓDIGO DO CLIENT




int main(){

    int i =0, sercon =0;
    bool begin = true;

    player_0 =clear_struct(player_0);
    player_1 =clear_struct(player_1);

    serverInit(MAX_PLAYERS);
    puts("The Server is running");

    while(!false == true){

        while(i<MIN_PLAYERS){

            sercon = acceptConnection();
            if(sercon!=NO_CONNECTION){
                printf("IF NO CONNECTION\n");
                printf("%d", sercon);

                if(i==0){
                    player_0.ID = sercon;
                    sendMsgToClient(&i,sizeof(int),0);

                }
                else if(i==1){
                    player_1.ID = sercon;
                    sendMsgToClient(&i,sizeof(int),1);

                }
                i++;
                //printf("%d",i);
                
            }
            //puts("1");
        }
        //puts("it ist here");
            if(i>=MAX_PLAYERS){
                rejectConnection();
            }

            if(begin == true){
                puts("it works");
                recvMsgFromClient(&player_0,0, WAIT_FOR_IT);
                //player_0 = expand_struct_to_use(playerReceive);
                //recvMsgFromClient(&playerReceive,1,WAIT_FOR_IT);
                //player_1 = expand_struct_to_use(playerReceive);
                begin = false;
                puts("MSG RECEIVED");
            }

            recvMsg(&player_0);
            //printf("%d\n", player_0.x);

            if(playerReceive.ID == 0){
                player_0 = key_actions(player_0);
                player_0 = lap_counter(player_0);
                player_0 = no_trespassing(player_0);
                //puts("1");
                //player_0 = expand_struct_to_use(playerReceive);
                //player_0 = PSEUDO_SERVER(player_0);
                //playerSend = compress_struct_to_send(player_0);
                //player_0.x -=5;
                broadcast(&player_0, sizeof(GAME));
            }
            else if(playerReceive.ID == 1){
                player_1 = expand_struct_to_use(playerReceive);

                playerSend = compress_struct_to_send(player_1);
                broadcast(&player_0, sizeof(GAME));
            }


        if(player_0.victory == true || player_1.victory ==true){

            define_winner(&player_0,&player_1);

            playerSend = compress_struct_to_send(player_0);
            sendMsgToClient(&playerSend,sizeof(PLAYER_SEND),0);
            playerSend = compress_struct_to_send(player_1);
            sendMsgToClient(&playerSend,sizeof(PLAYER_SEND),1);
            break;
            
        }           


    }

}