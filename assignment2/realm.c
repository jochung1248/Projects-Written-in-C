// Assignment 2 20T2 COMP1511: Castle Defense
//
// This program was written by JOSEPH CHUNG (z5308483)

//
// Version 1.0.0 (2020-07-20): Assignment released.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "realm.h"
///////////////////////////////////////////////////////////////////////
// struct realm represents a realm, which represents the state of the
// entire program. It is mainly used to point to a linked list of 
// locations, though you may want to add other fields to it.
//
// You may need to add fields to struct realm.
struct realm {
    struct location *castle;
    struct location *lair;
};
// struct location represents a location, which could be a land,
// a tower, or a castle. Each of those places has a different type
// of information that this struct will need to store.


// In the location struct, I had to add all the necessary fields for lands, 
// towers and castles.
struct location {
    char name[MAX_NAME_LENGTH];
    int power;
    int uses;
    int type;
    int effect;
    int defense;
    struct enemy *enemies;
    struct location *next;
};
// struct enemy represents an enemy, which will move through the
// realm (to each location). Towers can deal damage to it, and
// it can deal damage to the Castle.

// I added the declarations for enemy name, max_hp and cur_hp for current hp
// Have to have both max and current as enemies can be damaged
struct enemy {
    char name[MAX_NAME_LENGTH];
    int max_hp;
    int cur_hp;
    struct enemy *next;
};
// Add any other structs you define here.
///////////////////////////////////////////////////////////////////////
// Function prototypes for helper functions
static Location new_location(char *name);
static void print_tower(char *name, int power, int uses, Effect effect);
static void print_land(char *name);
static void print_castle(char *name, int defense);
static void print_enemy(char *name, int cur_hp, int max_hp);
// Add prototypes for any extra functions you create here.
///////////////////////////////////////////////////////////////////////
// You need to implement the following 9 functions.
// In other words, write code to make the function work as described 
// in realm.h.
// Create a new realm, and return a pointer to it.
// You may need to change this function in later stages.
Realm new_realm(void) {
    struct realm *realm = malloc(sizeof(struct realm));
    realm->castle = new_location("Castle");
    realm->lair = new_location("Lair");
    if (realm->castle != NULL && realm->lair != NULL) {
        realm->castle->next = realm->lair;
    }
    realm->castle->type = TYPE_CASTLE;
    return realm;
}

// Return a new location created with malloc.
static Location new_location(char *name) {
    // Using malloc to allocate memory for new location
    struct location *newLocation = malloc(sizeof(struct location));
    
    // Looping through name array to copy input name into location name
    int i = 0;
    while (name[i] != '\0') {
        newLocation->name[i] = name[i];
        i++;
    }
    newLocation->name[i] = '\0';
    newLocation->power = 0;
    newLocation->uses = 0;
    newLocation->type = TYPE_LAND;
    newLocation->effect = 0;
    newLocation->defense = STARTING_CASTLE_HP;
    newLocation->enemies = NULL;
    newLocation->next = NULL;
    return newLocation;
}
////////////////////////////////////////////////////////////////////////
//                         Stage 1 Functions                          //
////////////////////////////////////////////////////////////////////////
// Add a new location to the realm, and return the total number of
// locations in the realm.
int add_location(Realm realm, char *name) {
    struct location *addLocation = new_location(name);
    
    // Set the head of the list as castle and then set current as head
    struct location *head = realm->castle;
    struct location *curr = head;
    
    // Go through list to find the end
    while (curr->next->next != NULL) {
        curr = curr->next;
    }
    // Add the location at the end of the list
    curr->next = addLocation;
    addLocation->next = realm->lair;
    
    // Set current back to head to go through list again
    // This time, I'm looping to count how many locations there are
    curr = realm->castle;
    int i = 0;
    while (curr != NULL) {
        curr = curr->next;
        i++;
    }
    return i;
}
// Print out the realm.
void print_realm(Realm realm) {
    struct location *head = realm->castle;
    struct location *curr = head;
    curr->type = TYPE_CASTLE;
    
    while (curr != NULL) {
        struct enemy *enHead = curr->enemies;
        struct enemy *enCurr = enHead;
        // if the location is castle, print castle
        if (curr->type == TYPE_CASTLE) {
            print_castle(realm->castle->name, realm->castle->defense);
        // if the location is land, print land    
        } else if (curr->type == TYPE_LAND) {
            print_land(curr->name);
        // if the location is tower, print tower    
        } else if (curr->type == TYPE_TOWER) {
            print_tower(curr->name, curr->power, curr->uses, curr->effect);
        }
        // if an enemy exists, print out in current location
        if (enCurr != NULL) {
            while (enCurr != NULL) {
                print_enemy(enCurr->name, enCurr->cur_hp, enCurr->max_hp);
                enCurr = enCurr->next;
            }
        }
        curr = curr->next;
    }
}

    
////////////////////////////////////////////////////////////////////////
//                         Stage 2 Functions                          //
////////////////////////////////////////////////////////////////////////
// Add an enemy to the realm.
int new_enemy(Realm realm, char *location_name, char *name, int hp) {
    struct enemy *createEnemy = malloc(sizeof(struct enemy));
    struct location *head = realm->castle;
    struct location *curr = head;
    
    int i = 0;
    int j = 0;
    
    // Go through list and compare the input location name with already
    // existing locations to check if it's valid 
    while (curr != NULL) {
        // If they don't match, strcmp will return a value other than 0, 
        // which means that +1 will be added to i 
        if (strcmp(location_name, curr->name) != 0) {
            i++; 
        }
        // Count how many times the while loop has been looped using j
        j++;
        curr = curr->next;
    }
    // Compare i and j counters. 
    // If they're equal, that means no location name matched
    // If they're not equal, that means there was at least 1 location that 
    // matched.
    if (i == j) {
        return ERROR_NO_LOCATION;
    } //Check if input hp is greater than 0
    if (hp <= 0) {
        return ERROR_INVALID_STAT;
    } // Set current back to head to go through list again
    curr = head;
    // This time, when we strcmp, for all the valid location inputs,
    // break from while loop
    while (curr != NULL) {        
        if (strcmp(location_name, curr->name) == 0) {
            break;
        }
        curr = curr->next;
    }
    // Insert all input info into enemy details
    strcpy(createEnemy->name, name);
    createEnemy->max_hp = hp;
    createEnemy->cur_hp = hp;
    createEnemy->next = NULL;
    // If no previous enemy exists, create enemy
    if (curr->enemies == NULL) {
        curr->enemies = createEnemy;    
    } else { // If previous enemies exist, go to end of list and create enemy
        while (curr->enemies->next != NULL) {
            curr->enemies = curr->enemies->next;
        }
        curr->enemies->next = createEnemy;       
    }                      
    return SUCCESS;
}
// Add a new tower to the realm.
int new_tower(Realm realm, char *prev_name, char *name, int power, int uses) {
    struct location *createTower = malloc(sizeof(struct location));
    struct location *head = realm->castle;
    struct location *curr = head;
    struct location *after = NULL;
    int i = 0;
    int j = 0;
    // Similar to enemy function, strcmp location name arrays to check validity
    while (curr != NULL) {
        if (strcmp(prev_name, curr->name) != 0) {
            i++; 
        }
        j++;
        curr = curr->next;
    } // If i and j counters are equal, no matching location name
    if (i == j) {
        return ERROR_NO_LOCATION;
    } // If power or uses are not greater than 0, invalid input
    if (power <= 0 || uses <= 0) {
        return ERROR_INVALID_STAT;
    }
    curr = head;
    while (curr != NULL) {        
        if (strcmp(prev_name, curr->name) == 0) {
            break;
        }
        curr = curr->next;
    }
    after = curr->next;
    strcpy(createTower->name, name);
    createTower->power = power;
    createTower->uses = uses;
    createTower->effect = EFFECT_NONE;
    createTower->type = TYPE_TOWER;
    createTower->enemies = NULL;
    createTower->next = after;
    curr->next = createTower;

    return SUCCESS;
}
////////////////////////////////////////////////////////////////////////
//                         Stage 3 Functions                          //
////////////////////////////////////////////////////////////////////////
// Destroy the realm, and free any associated memory.
void destroy_realm(Realm realm) {
    //struct location *head = realm->castle;
    //struct location *curr = head;
    
    free(realm);
    
    /*while (curr != NULL) {
        struct enemy *enHead = curr->enemies;
        struct enemy *enCurr = head;
        
        if (enHead != NULL) {
            free(enHead);
        }
    }*/
        
}
// Advance enemies towards the castle.
int advance_enemies(Realm realm) {
    struct location *head = realm->castle;
    struct location *curr = head;
    
    int counter = 0;
    // If there are enemies at the castle, turn them into NULL
    // And count how many have gone past    
    if (head->enemies != NULL && curr == head) {
        while (curr->enemies != NULL) {
            counter++;
            curr->enemies = curr->enemies->next;
        }    
        head->enemies = NULL;
    } // Loop through list starting from first location
    while (curr != NULL) {
        // If the location after current has an enemy,
        // Move that enemy to current location
        if (curr->next != NULL) {
            curr->enemies = curr->next->enemies;
        }
        // Once the loop reaches Lair, turn the enemy into NULL at Lair 
        // To avoid duplicate
        if (curr->next == realm->lair) {
            curr->next->enemies = NULL;
        }    
        curr = curr->next;    
    }
        
            
    return counter;
}
// Apply damage from the enemies at each tower to that tower.
int apply_damage(Realm realm) {
    struct location *head = realm->castle;
    struct location *curr = head;
    struct enemy *enHead = curr->enemies;
    struct enemy *enCurr = enHead;
    
    int counter = 0;
    while (curr != NULL) {
        if (curr->type == TYPE_TOWER) {          
            while (curr->enemies != NULL) {    
                // Apply damage to enemies
                curr->enemies->cur_hp = curr->enemies->cur_hp - curr->power;
                // Then +1 to counter
                counter++;
                // If the enemy has no more hp, turn into NULL
                if (curr->enemies->cur_hp <= 0) {
                     curr->enemies = NULL;
                }
                // Then -1 uses of tower
                curr->uses--;
                // If there are no more uses, turn into land
                if (curr->uses <= 0) {
                    curr->type = TYPE_LAND;
                }        
                curr = curr->next;
            } // if there are enemies at castle, subtract from castle hp
        } else if (curr->type == TYPE_CASTLE) {
            int damage_sum = 0;
            enHead = curr->enemies;
            enCurr = enHead;
            while (enCurr != NULL) {
                damage_sum = damage_sum + enCurr->cur_hp;
                enCurr = enCurr->next;
            }
            realm->castle->defense = realm->castle->defense - damage_sum;
        }        
        curr = curr->next; 
    }               
    
    return counter;
}
////////////////////////////////////////////////////////////////////////
//                         Stage 4 Functions                          //
////////////////////////////////////////////////////////////////////////
// Apply the specified buff to the relevant towers or enemies.
int apply_buff(Realm realm, char *search_term, Buff buff, int amount){
    return 0;
}
////////////////////////////////////////////////////////////////////////
//                         Stage 5 Functions                          //
////////////////////////////////////////////////////////////////////////
// Apply the specified effect to each tower whose name matches the
// specified search term.
int apply_effect(Realm realm, char *search_term, Effect effect) {
    return 0;
}
////////////////////////////////////////////////////////////////////////
// Add definitions for your own functions below.
// Make them static to limit their scope to this file.



////////////////////////////////////////////////////////////////////////
//                    Provided print functions                        //
//         NOTE: YOU SHOULD NOT MODIFY THE FOLLOWING FUNCTIONS        //
////////////////////////////////////////////////////////////////////////
void print_tower(char *name, int power, int uses, int effect) {
    printf(" ^ %32s [pow: %3d | uses: %3d]", name, power, uses);
    if (effect == EFFECT_NONE) printf(" {%c}", EFFECT_NONE_CHAR);
    if (effect == EFFECT_PORTAL) printf(" {%c}", EFFECT_PORTAL_CHAR);
    if (effect == EFFECT_ICE) printf(" {%c}", EFFECT_ICE_CHAR);
    printf("\n");
}
void print_land(char *name) {
    printf(" ^ %32s [____________________]\n", name);
}
void print_castle(char *name, int defense) {
    printf(" ^ %32s [Castle Defenses: %3d]\n", name, defense);
}
void print_enemy(char *name, int cur_hp, int max_hp) {
    printf(" ^ %40s [hp: %d/%d]\n", name, cur_hp, max_hp);
}
////////////////////////////////////////////////////////////////////////
//               End of provided print functions                      //
////////////////////////////////////////////////////////////////////////
