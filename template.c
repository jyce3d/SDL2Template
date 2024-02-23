/**** TEMPLATE pour jeu en C */
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include "stdio.h"
#include <time.h>

/* Initialization des composants */
SDL_Window* pWindow = NULL;
TTF_Font *pFont =NULL;
SDL_Renderer* pRenderer = NULL;
typedef struct key_tag {
    int status;
} KeyStatus;
int status_score=0;

//TODO:adapt according the keys you retain
KeyStatus a_ks[5];

#define KS_RIGHT  0
#define KS_LEFT  1
#define KS_UP 2
#define KS_SPACE 8
#define KS_DOWN 4

#define KS_KEYDOWN 1
#define KS_KEYUP 2

// Définir la taille écran:
#define SIV_WND_WIDTH 1024
#define SIV_WND_HEIGHT 768
// Lié à l'affichage de texte
#define SIV_TXT_CENTER 1
#define SIV_TXT_LEFT 2
#define SIV_TEXT_FILLED 4

#define SIV_GAME 1
#define SIV_GAME_OVER 2


/* SPACE INVUDERS - is a SHMUP inspired from space invaders (Taito 1978) */
/* jyce3d - 2024 */


int status = SIV_GAME; // TODO:definir le status du jeu.
int g_cur_level = 0;



int q=0;
int play_x ;
int play_y ;

/* Initialization des éléments graphiques */
int sfc_loaded=0;
SDL_Surface* sprite_sfc = NULL;

SDL_Texture* pScene = NULL;

SDL_Texture* pSprite = NULL;

// Paramètres
int score =0;
int high_score =0;
int vie =3;

// Definition des mosaiques
// if applicable Pas de mosaique dans cet exemple
// Ex :

/* Définition des sprites */
// Perso:
// TODO: change the value
SDL_Rect spr_player[1] = { 
    {24,268, 90, 120 } };
// TODO: change the value
SDL_Rect spr_player_dead[1] = { {40,416, (90-40),(498-416) }};

// TODO: dest rectangle
SDL_Rect spr_player_dest = {0,0,90,120};


// Enemies

// SDL_Rect spr_sirena[2]= { {0,0,125,115 }, {132,0,125,115} }; // Enemy 1

// TODO: dest rectangle
//SDL_Rect spr_enemy_dest = {0, 0, 75,115};



// Permet de retenir l'état des touches enfoncées
// à customiser.
void Initialize_ks() {
    
    a_ks[KS_RIGHT].status = KS_KEYUP;
    a_ks[KS_LEFT].status = KS_KEYUP;
    a_ks[KS_UP].status = KS_KEYUP;
    a_ks[KS_SPACE].status = KS_KEYUP;
    a_ks[KS_DOWN].status = KS_KEYUP;
}
void initialize_level() {
    // Initialize le joueur

    play_x = 20;
    play_y = 650;


    // TODO : put your initialization during the game session
    Initialize_ks();

    choose_leve();

}

void display_enemy(int idx) {
// TODO:
}

void deplace_enemy() {
//TODO:        display_enemy(i);
}

/* Définition des fonctions */

int Create() {
    if (SDL_Init(SDL_INIT_VIDEO) !=0) {
        printf("Erreur d'initialisation de la SDL : %s\n", SDL_GetError() );
        return -1;
    }
    //TODO: Customize
    pWindow = SDL_CreateWindow("SPACE INVUDERS", 
                                SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                SIV_WND_WIDTH, SIV_WND_HEIGHT,SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI );
    //TODO: Uncomment this to have the game in full screen
//    SDL_SetWindowFullscreen(pWindow, SDL_WINDOW_FULLSCREEN);
                            
    if (pWindow) {
            if (!TTF_Init()) {
                if ( !(pFont = TTF_OpenFont("freefont/FreeMono.ttf", 16)) ) {
                    printf("Erreur lors du chargement de la font %s \n", SDL_GetError());
                    return -1;
                } else {
                    if ((pRenderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC) )==NULL) {
                        printf("Erreur lors de la création du Renderer %s \n", SDL_GetError());
                        return -1;
                    } else {
                    //TODO : Charger les sprites et ou les mosaique
                    // EX:    sprite_sfc = IMG_Load("sprites.bmp");
                    //TODO: Si chromakey
                      // if req,  SDL_SetColorKey(player_sfc, SDL_TRUE, SDL_MapRGB(player_sfc->format, 0xFF, 0xFF, 0xFF));
                        pSprite = SDL_CreateTextureFromSurface(pRenderer, sprite_sfc);
                        SDL_ShowCursor(SDL_DISABLE );

                        return 1;
                    }
                }
            } else {
                printf("Erreur lors de la création de TTF_Init %s \n", SDL_GetError());
                return -1;

            }
    } else {
        printf("Erreur lors de la création de la fenêtre %s \n", SDL_GetError());
        return -1;
    }
}

//TODO: destroy all global structures
void Destroy() {
    SDL_FreeSurface(sprite_sfc);
    SDL_DestroyTexture(pSprite);
    SDL_DestroyTexture(pScene);
    TTF_CloseFont(pFont);
    SDL_DestroyWindow(pWindow);
    SDL_Quit();
}

void Display_Text(const char* pszTitle, int attrb, SDL_Color *pColor, int x, int y, int filled) {
    SDL_Surface* pSurface = TTF_RenderText_Solid(pFont, pszTitle, *pColor );
    SDL_Texture* pTexture = SDL_CreateTextureFromSurface(pRenderer, pSurface);
    SDL_FreeSurface(pSurface);
    int texW, texH;
    SDL_QueryTexture(pTexture, NULL, NULL, &texW, &texH);
    SDL_Rect dstrect = {0,0, texW, texH};
    switch(attrb) {
        case SIV_TXT_CENTER:
            dstrect.x = (SIV_WND_WIDTH - texW) /2;
            dstrect.y = y;
        break;
        case SIV_TXT_LEFT:
            dstrect.x = x;
            dstrect.y = y;
        break;
        default:
        break;
    }
    if (filled == 1) {
        SDL_SetRenderDrawColor( pRenderer, 0, 0, 0, 0 );
        SDL_RenderFillRect( pRenderer, &dstrect );
    }
    SDL_RenderCopy(pRenderer, pTexture, NULL, &dstrect );
    SDL_DestroyTexture(pTexture);
}

void Update_GameOver() {
        SDL_Color color = {255,0,0};
        SDL_SetRenderDrawColor(pRenderer, 0,0,0, 255);
        SDL_RenderClear(pRenderer);
        Display_Text("Fin du Jeu", SIV_TXT_CENTER, &color, 0,96,0);
        char s_params[255];
        sprintf(s_params,"Score :%d", score);
        Display_Text(s_params, SIV_TXT_CENTER, &color, 0,104,0);
        if (score>high_score) {
            high_score = score;
            Display_Text("Nouveau High Score", SIV_TXT_CENTER, &color, 0,112,0);
        }
        vie =3;
        score =0;
        SDL_RenderPresent(pRenderer);
        SDL_Delay(2500);
        sfc_loaded = 0;
        g_cur_level=0;

}
// Choose_level:
// TODO:

void choose_level() {
}

void Update_NextLevel() {
        SDL_Delay(2500);
        g_cur_level ++;
        initialize_level();
        choose_level();
        // Modify the score bonus
//        score = score +1000;  
}


void Init_Scene() {
    SDL_Surface* scene_sfc = SDL_CreateRGBSurface(0,SIV_WND_WIDTH,SIV_WND_HEIGHT,32,0,0,0,0);
  
    // Write the maps or the sprite at the begining of the level here:
    // TODO:

    if (pScene !=NULL) SDL_DestroyTexture(pScene);
    pScene = SDL_CreateTextureFromSurface(pRenderer, scene_sfc);
    SDL_FreeSurface(scene_sfc);


}


void Move_Player() {
                if (a_ks[KS_RIGHT].status == KS_KEYDOWN) {
                    if ( (play_x+=16)>=950 )
                        play_x = 950;
                }
                if (a_ks[KS_LEFT].status == KS_KEYDOWN) {
                    if ( (play_x-=2)<=0)
                        play_x = 0;
                }

}

// Return 1 if collided 0 if not

int is_collided() {
    // missile colides missile
    // missile colides bunker
    // missile colides vessel
    // missile colides alien

    return 0;
}

void Update_Game() {
 
   
        // Affichage des paramètres:

        SDL_Delay(30); // 
        if (sfc_loaded==0) {
            Init_Scene(); // si on change de niveau et que la scene doit être recréé via une nouvelle surface.
            initialize_level(); // sprites coordinates;
            sfc_loaded=1;
        }

        // Create Screen

        SDL_Rect rct_scene = {0,0,SIV_WND_WIDTH,SIV_WND_HEIGHT};
        SDL_RenderCopy(pRenderer, pScene, &rct_scene, &rct_scene );
  
        // Collision
        if (is_collided()) {
            //status=CEP_STATUS_DEATH;
        } else {
        // Process Display
                // Move Player
                Move_Player();
                //TODO: Display Player
                spr_player_dest.x = play_x;
                spr_player_dest.y = play_y;
                SDL_RenderCopy(pRenderer, pSprite, &spr_player[0], &spr_player_dest);
 
 
            }
        // Process Enemy

            deplace_enemy();
        
    
    SDL_SetRenderDrawColor(pRenderer, 0,0,0, 255);
    SDL_Color color = {255,255,255};
    char s_params[255];
    sprintf(s_params, "Score=%d        HIGH Score= %d", score, high_score);
    Display_Text(s_params, SIV_TXT_LEFT , &color, 0,164,0);
    sprintf(s_params, "Vie(s)=%d", vie);
    Display_Text(s_params, SIV_TXT_LEFT , &color, 0,180, 0);
    

}

void update_death() {
        // Create Screen
    SDL_Rect rct_scene = {0,0,SIV_WND_WIDTH,SIV_WND_HEIGHT};
    SDL_RenderCopy(pRenderer, pScene, &rct_scene, &rct_scene );


    SDL_RenderCopy(pRenderer, pSprite, &spr_player_dead[0], &spr_player_dest);
    SDL_RenderPresent(pRenderer);

    SDL_Delay(2500);
    vie --;
    if (vie>0) {
        initialize_level();

      //  status = CEP_STATUS_GAME;
    } //TODO: else 
        //status = SIV_GAME_OVER;

}
// Gestion de l'automate à état fini du jeu
// TODO: process the key status according to the status in the game automata
void update_status() {
        SDL_SetRenderDrawColor(pRenderer, 0,0,0, 255);
        SDL_RenderClear(pRenderer);
            switch(status) {
                case SIV_GAME:
                    Update_Game();
                break;
                case SIV_GAME_OVER:
                    Update_GameOver();
                break;

                //TODO: manage other status
                /*case CEP_STATUS_DEATH:
                    update_death();
                break;
                case CEP_STATUS_NEXT:
                    Update_NextLevel();
                break;
                */
            }

            // display independant of the status
            SDL_RenderPresent(pRenderer);
}

// Boucle de jeu
// TODO: complete according to events 
int main(int argc, char *argv[]) {
    // Intialize le générateur congruentiel par rapport à l'horloge 
    time_t t;
    t = time(NULL);
    srand(t);
    // Initialisation de la première scene du jeu
    // ------------------------------------------

    SDL_Event event;
  
    Initialize_ks();
    if (Create()==1) {
        while (q==0) {    
            update_status();
              //  Process_Game();
            while (SDL_PollEvent(&event) != 0) {
                switch (event.type) {
                    case SDL_QUIT:
                        q=1;
                    break;
                    case SDL_KEYUP:
                        if (event.key.keysym.sym==SDLK_ESCAPE) q=1;                    
                        if (event.key.keysym.sym==SDLK_RIGHT) {
                            a_ks[KS_RIGHT].status = KS_KEYUP;
                        } else if (event.key.keysym.sym==SDLK_LEFT) {
                            a_ks[KS_LEFT].status = KS_KEYUP;
                        } else if (event.key.keysym.sym==SDLK_UP) {
                            a_ks[KS_UP].status = KS_KEYUP;
                        } else if (event.key.keysym.sym==SDLK_SPACE){
                            a_ks[KS_SPACE].status = KS_KEYUP;
                        } else if (event.key.keysym.sym==SDLK_DOWN){
                            a_ks[KS_DOWN].status = KS_KEYUP;
                        }
                    break;
                    case SDL_KEYDOWN:
                        if (event.key.keysym.sym==SDLK_RIGHT) {
                            a_ks[KS_RIGHT].status = KS_KEYDOWN;
                        } else if (event.key.keysym.sym==SDLK_LEFT) {
                            a_ks[KS_LEFT].status = KS_KEYDOWN;
                        } else if (event.key.keysym.sym==SDLK_UP) {
                            a_ks[KS_UP].status = KS_KEYDOWN;
                        } else if  (event.key.keysym.sym==SDLK_SPACE){
                            a_ks[KS_SPACE ].status = KS_KEYDOWN;
                        } else if  (event.key.keysym.sym==SDLK_DOWN){
                            a_ks[KS_DOWN].status = KS_KEYDOWN;
                        } 
                        
                    break;
                    default:
                    break;
                }
            }
        }

    }

    Destroy();
    exit(0);
    return 0;
}


                    