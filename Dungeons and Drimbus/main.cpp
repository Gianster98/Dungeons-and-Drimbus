//
//  main.cpp
//  Dungeons and Drimbus
//
//  Created by Giancarlo Herrera on 9/30/17.
//  Copyright © 2017 Giancarlo Herrera. All rights reserved.
//

#include <iostream>
#include <iomanip>
#include <ctime>
#include <cstdlib>
using namespace std;

//rolls a die with entered amount of sides
int d(int die);

class Character {
public:
    string name;
    double hp;
    double ac;
    int level;
    int init;
    //attackDie
    double attack;
    int attackBonus;
    bool hasCompanion = false;
    string companion = "nobody";
    
    
    //sets up Character's stats
    void set(){
        
        //stores classType
        char classType;
        
        //sets character name
        cout << "What is your name? ";
        cin >> name;
        
        //sets class
        cout << "What class are you? Fighter (A), Sorcerer (B), or Thief (C)? ";
        cin >> classType;
        switch (classType){
            case 'a':
            case 'A':
                setFighter();
                break;
            case 'b':
            case 'B':
                setSorcerer();
                break;
            case 'c':
            case 'C':
                setThief();
                break;
        }
        
        //adjusts stats for level
        cout << "What level are you? ";
        cin >> level;
        
        hp = hp + (level * d(hp));
        ac = ac + (level);
        init = init + (level/2);
        attackBonus = attackBonus + level/2;
        
    }
    
    void setNPC(string NPCname, int NPChp, int NPCac, int NPClevel, int NPCinit, int NPCattack, int NPCattackBonus){
        
        name = NPCname;
        level = NPClevel;
        hp = NPChp + (level * d(NPChp));
        ac = NPCac + (level);
        init = NPCinit + (level/2);
        attackBonus = NPCattackBonus + level/2;
    }
    
    //sets default fighter stats
    void setFighter(){
        hp = 10;
        ac = 15;
        init = 2;
        attack = 8;
        attackBonus = 2;
    }
    
    //sets default sorcerer stats
    void setSorcerer(){
        hp = 5;
        ac = 9;
        init = 4;
        attack = 4;
        attackBonus = 0;
    }
    
    //sets default thief stats
    void setThief(){
        hp = 8;
        ac = 10;
        init = 8;
        attack = 6;
        attackBonus = 2;
    }
};

class Item {
public:
    
    Character nobody;
    
    string name;
    bool isHeld;
    int attackBonus;
    int defenseBonus;
    int hpRestore;
    Character &heldBy = nobody;
    
    void set(string itemName, int dmgBonus = 0, int acBonus = 0, int hpBonus = 0, bool held = false){
        name = itemName;
        attackBonus = dmgBonus;
        defenseBonus = acBonus;
        hpRestore = hpBonus;
        isHeld = held;
    }
    
    void pickUp(Character &player){
        isHeld = true;
        heldBy = player;
        player.attackBonus += attackBonus;
        player.ac += defenseBonus;
        player.hp += hpRestore;
    }
    
    void drop(){
        isHeld = false;
        heldBy = nobody;
        heldBy.attackBonus -= attackBonus;
        heldBy.ac -= defenseBonus;
    }
};


void Fight(Character &a, Character &b);
int attack(Character a, Character b);
int initiative (Character a, Character b);
void Input(char &userChoice);
bool check(Character player, bool check);
void choice1 (Character &player, Character &Jorgun, bool &check1);
void choice2 (Character &player, Character &Jorgun, bool &check2);
void choice3 (Character &player, Character &Jorgun, bool &check3);
void choice4 (Character &player, Character &Jorgun, Character &realJorgun, bool &check4);
void choice5 (Character &player, Character &Jorgun, bool &check5);
void Puzzle (Character &player, Character &Jorgun, bool &puzzle);
void End (Character &player, Character &Jorgun);
void campaign(Character &player);

int main() {
    
    //welcomes user
    cout << "Welcome to Dungeons 'n' Drimbus" << endl << endl;
    
    //creates and initializes player's character
    Character player;
    player.set();
   /*
    //sets stats for a dragon
    Character dragon;
    dragon.hp = 10;
    dragon.ac = 5;
    dragon.level = 5;
    dragon.init = -2;
    dragon.attack = 3;
    dragon.attackBonus = 1;
    dragon.name = "Dragon";
    
    //starts fight between user and dragon
    Fight(player, dragon);
   */
    campaign(player);
    
    return 0;
}

void Input (char &userChoice, char limit = 'b'){
    cin >> userChoice;
    limit = toupper(limit);
    userChoice = toupper(userChoice);
    while (userChoice > limit){
        cout << "Incorrect choice: Try again ";
        cin >> userChoice;
        userChoice = toupper(userChoice);
    }
}

bool check(Character player, bool check){
    if (player.hp > 0 && check == true){
        return true;
    }
    else {
        return false;
    }
}


//handles fights until someone is dead or player disengages
void Fight(Character &player, Character &opponent) {
    //Beginning prompt
    char userEngage;
    bool engage;
    cout << "Would you like to fight a " << opponent.name << "? (Y or N)";
    cin >> userEngage;
    cout << endl << endl;
    
    //checks if player engages fight
    while (userEngage != 'y' && userEngage != 'Y' && userEngage != 'n' && userEngage != 'N'){
        cout << "Incorrect input. Try again: ";
        cin >> userEngage;
    }
    if (userEngage == 'y' || userEngage == 'Y'){
        engage = true;
    }
    else {
        engage = false;
    }
    
    //checks initiative order
    int fightOrder = initiative(player, opponent);
    
    //makes sure player is still fighting and both are still alive
    while (player.hp > 0 && opponent.hp > 0 && engage == true){
        
        //executes each player's attack
        switch(fightOrder){
        case 1:
                opponent.hp -= attack(player, opponent);
                player.hp -= attack(opponent, player);
            break;
        case 2:
                player.hp -= attack(player, opponent);
                opponent.hp -= attack(opponent, player);
            break;
        }
        
        //if both are alive, asks if player wants to continue fighting
        if (opponent.hp > 0 && player.hp > 0){
            cout << "You have " << player.hp << " hp left" << endl << opponent.name << " has " << opponent.hp << " hp left " << endl << endl << "Continue fighting? (Y or N)";
            cin >> userEngage;
            
            while (userEngage != 'y' && userEngage != 'Y' && userEngage != 'n' && userEngage != 'N'){
                cout << "Incorrect input. Try again: ";
                cin >> userEngage;
            }
            if (userEngage == 'y' || userEngage == 'Y'){
                engage = true;
            }
            else {
                engage = false;
            }
        }
        //outputs player victory
        else if (player.hp > 0) {
            cout << "You killed " << opponent.name << "!" << endl;
        }
        //outputs player loss
        else {
            cout << "You died. :(" << endl;
        }
        
    }

}

//rolls a die with x sides
int d(int die) {
    int roll;
    srand((unsigned int)(time(0)));
    roll = ((rand()%die)+1);
    return roll;
}

//returns an amount equal to damage dealt
int attack(Character attack, Character defend){
    int attackRoll = (d(20)+attack.attackBonus);
    int defendRoll = defend.ac;
    int damage;
    
    if (attackRoll >= defendRoll){
        damage = d(attack.attack);
        cout << attack.name << " deals " << damage << " points of damage" << endl;
    }
    else {
        damage = 0;
        cout << attack.name << " deals no damage" << endl;
    }
    
    return damage;
}

//returns 1 or 2 for which player goes first
int initiative(Character player, Character opponent){
    int playerInit = d(20)+player.init;
    int opponentInit = d(20)+opponent.init;
    
    if (playerInit > opponentInit){
        return 1;
    }
    else {
        return 2;
    }
}

void campaign (Character &player){
    
    bool check1, check2, check3, check4, check5, puzzle;
    
    Character Jorgun;
    Jorgun.setNPC("Jorgun", 20, 15, 2, 1, 6, 1);
    
    Character spider;
    spider.setNPC("Spider", 5, 7, 1, 2, 3, 1);
    
    Character skeleton;
    skeleton.setNPC("Skeleton", 10, 5, 1, 0, 2, 1);
    
    Character realJorgun;
    realJorgun.setNPC("Other Jorgun", 10, 10, 1, 1, 4, 0);
    
    Item healthPotion;
    healthPotion.set("Health Potion", 0, 0, d(8));
    
    Item sword;
    sword.set("Sword", 2);
    
    Item cloak;
    sword.set("Cloak", 0, 2);
    
    cout << "You awaken at the Tarnished Tack to the sound of a great commotion downstairs. What do you do?" << endl << endl << "Go Downstairs (A)" << endl << "Continue Sleeping (B) ";
    
    choice1(player, Jorgun, check1);
    if (check(player, check1) == true){
        choice2(player, Jorgun, check2);
        if (check(player, check2) == true){
            choice3(player, Jorgun, check3);
            if (check(player, check3) == true){
                choice4(player, Jorgun, realJorgun, check4);
                if (check(player, check4) == true){
                    choice5(player, Jorgun, check5);
                    if(check(player, check5) == true){
                        Puzzle(player, Jorgun, puzzle);
                        if(check(player, puzzle) == true){
                            End(player, Jorgun);
                        }
                    }
                }
            }
        }
    }
    

}

void choice1 (Character &player, Character &Jorgun, bool &check1){
    char userChoice;
    Input(userChoice, 'b');
//    while (userChoice != 'A' || userChoice != 'a' || userChoice != 'B' || userChoice != 'b'){
    //Go Downstairs
    if (userChoice == 'A' || userChoice == 'a'){
        check1 = true;
    }
    //Stay Sleeping (1B)
    else if (userChoice == 'B'|| userChoice == 'b'){
        cout << "You go back to bed and are murdered in your sleep. Good job, milk drinker." << endl;
        check1 = false;
    }
}

void choice2 (Character &player, Character &Jorgun, bool &check2){
    char userChoice;
    cout << "You head downstairs and are greeted by a frazzled man who introduces himself as Jorgun." << endl << "He has been pleading for help, claiming that someone has taken his children from him and has burned his house down." << endl << endl << "Do you help him? (A) " << endl << "Or tell him to go fuck himself? (B) ";
    Input(userChoice, 'b');
    //Offer Help (2A)
    if (userChoice == 'A' || userChoice == 'a'){
        check2 = true;
    }
    //Don't Help Jorgun (2B)
    else if (userChoice == 'B' || userChoice == 'b'){
        Fight(player, Jorgun);
        cout << "The gods strike you down." << endl;
        check2 = false;
    }
}

void choice3 (Character &player, Character &Jorgun, bool &check3){
    char userChoice;
    cout << "He hands you a map of his home and offers to travel with you to take down the demon that stole his family." << endl << endl << "Do you allow him to come with you? (A)" << endl << "Or do you go alone (B) ";
    Input(userChoice, 'b');
    //Go with Jorgun (3A)
    if (userChoice == 'A' || userChoice == 'a'){
        player.companion = Jorgun.name;
        player.hasCompanion = true;
    }
    //Go alone (3B)
    else if (userChoice == 'B' || userChoice == 'b'){
        player.hasCompanion = false;
    }
    check3 = true;
}

void choice4 (Character &player, Character &Jorgun, Character &realJorgun, bool &check4){
    char userChoice;
    //They go together
    if (player.hasCompanion == true){
        cout << "You two head down the path together as he fills you in on as much detail as possible. After a while, as you travel through some tall fields, you hear an intense rustling. A man bursts out of the grass pleading for help and he falls on you." << endl << endl;
    }
    else {
        cout << "You trek down the path on your own, trying to follow the map. As you cross through some tall fields, you hear an intense rustling. A man bursts out of the grass pleading for help and falls on you." << endl << endl;
    }
    
    cout << "Push him off (A) " << endl << "Reassure him (B) ";
    Input(userChoice, 'b');
    //Push (4A)
    if (userChoice == 'A' || userChoice == 'a'){
        cout << "You push him off and he falls to the ground." << endl;
    }
    //Help Up (4B)
    else if (userChoice == 'B' || userChoice == 'b'){
        cout << "You help him back to his feet and tell him its ok." << endl;
    }
    
    cout << "All of a sudden, you notice something odd. His face..." << endl;
    cout << "That's Jorgun." << endl << endl;
    
    if (player.hasCompanion == true){
        cout << "You shout: WHO ARE YOU?" << endl;
        cout << "The new Jorgun looks up and catches a glimpse of the Jorgun travelling with you and shouts: THAT'S HIM! KILL HIM! and jumps to his feet." << endl << endl;
        cout << "Stop Him (A)" << endl << "Do Nothing (B) ";
        Input(userChoice, 'b');
        
        //Stop Him (5A)
        if (userChoice == 'A' || userChoice == 'a'){
            cout << "You pry Jorgun off Jorgun." << endl << "The new Jorgun rushes you and says, I wont let him take my family!" << endl;
            Fight(player, realJorgun);
        }
        //Do Nothing (5B)
        else if (userChoice == 'B' || userChoice == 'b'){
            cout << "The new Jorgun rushes your Jorgun and begins to pummel him. Your Jorgun pulls out a dagger and slashes at him. He cries for help." << endl << endl << "Help Your Jorgun (A)" << endl << "Help New Jorgun (B) ";
            Input(userChoice, 'b');
            
            //Help Bad Jorgun (6A)
            if (userChoice == 'A' || userChoice == 'a'){
                Fight(player, realJorgun);
            }
            //Help Real Jorgun
            else if (userChoice == 'B' || userChoice == 'b'){
                cout << "Something about this new, frazzled Jorgun strikes you right. You take his side and draw your sword as you stare down the man who put you on this trail. His face turns sour as he realizes what is happening. The new Jorgun looks ar you and nods. You both rush in toward the fake Jorgun, weapons drawn as he takes off the amulet around his neck and transforms into a red skinned, winged demon. This is it." << endl;
                Fight(player, Jorgun);
            }
        }
    }
     else {
         cout << "You ask Jorgun what happened and he looks at you, confused. He claims to have never met you in his life. You explain that you are the man he asked for help at the Tarnished Tack and he replies that he was just running there now to ask for help. He quickly explains that his house was torn down, his child and wife taking by a shape shifting demon that took his form. He begs that you help him get his family back." << endl << endl;
         cout << "Do you believe him?" << endl << "Yes, help. (A) " << endl << "No, he must be the imposter! (B) ";
         Input(userChoice, 'b');
                             
         //Help Real Jorgun
         if (userChoice == 'A' || userChoice == 'a'){
             player.companion = realJorgun.name;
         }
         //Continue Alone
         else if (userChoice == 'B' || userChoice == 'b'){
             cout << "You stare the new Jorgun down. His crazed eyes unsettle you. You draw your sword" << endl;
             Fight(player, realJorgun);
         }
     }

                         
     //Sets appropriate variables true or false
     if (player.hp > 0 && Jorgun.hp > 0){
         check4 = true;
     }
     else {
         if (player.hp <= 0){
             cout << "You died.";
         }
         else {
             cout << "With the demon defeated, the real Jorgun thanks you. You head out with him towards his home. You encounter a burnt down barn. In one of the stalls, you discover a passage leading undergrounf. You carefully head down to find some sort of confusing chamber. In a room on the Northern wall, Jorgun rushes in to find his infant child in the center of a pentagram. He is relieved until he looks up. His wife lies dead at the tip. He seems overcome with grief. You try to help him up and you thanks you for your help but says he needs a moment.";

         }
         check4 = false;
     }
}
 
void choice5(Character &player, Character &Jorgun, bool &check5){
                         
     Character skeleton;
     skeleton.setNPC("Skeleton", 10, 5, 1, 0, 2, 1);
                         
     Item healthPotion;
     healthPotion.set("Health Potion", 0, 0, d(8));
                         
     Item sword;
     sword.set("Sword", 2);
                         
     Item cloak;
     cloak.set("Cloak", 0, 2);
                         
     char userChoice;
                         
     if (player.hasCompanion == true){
         cout << "Jorgun thanks you for handling the situation but seems a bit shaken. Nevertheless, you trek onwards." << endl << endl;
         cout << "After an hour or so you begin to crest a hill. Jorgun tells you the house is not far off. However, you also spot what looks like a fallen tower a bit off to your left. Jorgun says time is imminent and suggests focusing on the house." << endl << "Go to the Tower (A)" << endl << "Continue towards the House (B) ";
     }
     else {
         cout << "After an hour or so you begin to crest a hill. The house is not far off. However, you also spot what looks like a fallen tower a bit off to your left. The sun seems to be setting." << endl << endl << "Go to the Tower (A)" << endl << "Continue towards the House (B) ";
     }
                         
     Input(userChoice, 'b');
                         
     //Go to Tower (8A)
     if (userChoice == 'A' || userChoice == 'a'){
         cout << endl << endl << "After about half an hour, you walk up towards the fallen stone tower. Creeping carefully around fallen bricks and gravestones. A breeze hits your back as you notice a chest in what was once an entrance hall for the tower." << endl << "Check the Chest (A)" << endl << "Turn Back (B) ";
         Input(userChoice, 'b');
         //Check Chest
         if (userChoice == 'A' || userChoice == 'a'){
             cout << "You walk up towards the chest and find a potion, glistening sword, and a cloak." << endl;
             cout << "Grab the Sword (A)" << endl << "Grab the Potion (B)" << endl << "Grab the Cloak (C)" << endl << "Leave (D) ";
             Input(userChoice, 'd');
             //Grab Sword
             if (userChoice == 'A' || userChoice == 'a') {
                 sword.pickUp(player);
                 cout << endl << "You pick up a hefty sword and your attacks feel stronger. The other items disappear." << endl << endl;
             }
             //Grab Potion
             else if (userChoice == 'B' || userChoice == 'b'){
                 healthPotion.pickUp(player);
                 cout << endl << "You drink the red potion and suddenly feel healthier. You now have " << player.hp << " hp. The other items disappear." << endl << endl;
             }
             //Grab Cloak
             else if (userChoice == 'C' || userChoice == 'c'){
                 cloak.pickUp(player);
                 int damage = d(2);
                 cout << endl << "The cloak is thick and offers protection but a thin puff of yellow gas poisons you and you take " << damage << " points of damage. The other items disappear." << endl << endl;
                 player.hp -= damage;
                                     
             }
             cout << "As you turn, you are confronted by a skeleton, freshly out of the grave. Clawing at you. " << endl;
             if (player.hasCompanion == true){
                 cout << "Jorgun draws a dagger and stabs at a second skeleton clawing at your back, as the one running at you draws near." << endl;
             }
             Fight(player, skeleton);
             //Player survives
             if (player.hp > 0) {
                 if (player.hasCompanion == true){
                     cout << "The skeleton falls apart, still clawing and becomes lifeless. Jorgun is shaken, and urges that you move onward" << endl << endl;
                 }
                 else {
                     cout << "The skeleton falls apart, still clawing and becomes lifeless. You are a bit shaken but move onward" << endl << endl;
                 }
                 check5 = true;
             }
             else {
                 check5 = false;
             }
         }
                             
     }
     //Continue to House (8B)
     else if (userChoice == 'B' || userChoice == 'b'){
         cout << "You continue on towards the house." << endl;
         check5 = true;
     }
     /*else {
         error(userChoice);
     }*/
                         
}
                     

void Puzzle(Character &player, Character &Jorgun, bool &puzzle){
     char userChoice;
     cout << "After a bit of walking, you and Jorgun come up on a burnt down barn. Jorgun looks at the rubble and weeps. You walk through the ashes and find several empty stables." << endl << "Further down the line you find an odd shaped hole in the ground." << endl << "As you descend you find an odd chamber. At the center there are red, blue, yellow, and black colored containers and four doors (one along each wall)." << endl << endl;
                         
     //Puzzle
     bool yellowRoom = false;
     bool redRoom = false;
     bool blueRoom = false;
     bool blackRoom = false;
                         
     while (blackRoom == false){
         cout << "Check Containers (A)" << endl << "Check North Room (B) " << endl << "Check East Room (C)" << endl << "Check West Room (D) " << endl << "Check South Room (E) ";
         cin >> userChoice;
                             
         //Player finds key if all other rooms are completed
         if ((userChoice == 'A' || userChoice == 'a') && yellowRoom == true && blueRoom == true && redRoom == true){
             cout << "Inside the black container you find a black key that seems to match the Northern Door. " << endl;
             blackRoom = true;
         }
         //Checking containers before all rooms are completed
         else if ((userChoice == 'A' || userChoice == 'a') && (yellowRoom == false || redRoom == false || blueRoom == false)){
             if (yellowRoom == true){
                 cout << "The yellow box has closed over and become a solid cube." << endl;
             }
             if (redRoom == true){
                 cout << "The red box has closed over and become a solid cube." << endl;
             }
             if (blueRoom == true){
                 cout << "The blue box has closed over and become a solid cube." << endl;
             }
             if (redRoom == false && redRoom == false && blueRoom == false) {
                 cout << "You stare intently at the containers and all of a sudden..." << endl << "Nothing." << endl;
             }
         }
         //Black door is locked
         else if (userChoice == 'B' || userChoice == 'b'){
             cout << "The northern door is locked but you notice a small key hole in the door handle." << endl;
         }
         //First entrance into east/red room
         else if ((userChoice == 'C' || userChoice == 'c') && redRoom == false){
             cout << "You enter a room with a long dining table and several different plates arranged. They are full of the most delicious food you have ever smelt. The door you entered through locks behind you. What do you do?" << endl << endl << "Eat Food (A)" << endl << "Cut the Food Open (B) " << endl << "Flip the Bowls (C) ";
             Input(userChoice, 'c');
             while (userChoice != 'A' && userChoice != 'a'){
             cout << "Mmmm... Delicious." << endl << endl << "Eat Food (A)" << endl << "Cut the Food Open (B) " << endl << "Flip the Bowls (C) ";
             Input(userChoice, 'c');
             }
             cout << "You ingest this heavenly food and your stomach begins to bloat. Ouch. That hurts." << endl << endl << "What do you do?" << endl;
             cout << "Eat More Food (A)" << endl << "Cut the Food Open (B) " << endl << "Flip the Bowls (C) ";
             while (userChoice != 'C' && userChoice != 'c'){
                 player.hp--;
                 cout << "The smells of the food intoxicate you and you can't help but to eat a little more. You take a point of damage. You have " << player.hp << " hp points left." << endl << endl;
                 cout << "Eat More Food (A)" << endl << "Cut the Food Open (B) " << endl << "Flip the Bowls (C) ";
                 Input(userChoice, 'c');
             }
             cout << "As you flip the bowls the food in them begins to disappear. The smell dissipates and your hunger subsides. You finish flipping them all and the door you entered through opens again. You are left with a slight lingering hunger. " << endl << endl;
             redRoom = true;
         }
         //Repeated entrance into east/red room
         else if ((userChoice == 'C' || userChoice == 'c') && redRoom == true){
             cout << "The room is as you left it." << endl << endl;
         }
         //First entrance into west/blue room
         else if ((userChoice == 'D' || userChoice == 'd') && blueRoom == false){
             cout << "You enter the room and are immediately hit by a haze. Your eyes feel heavy as the door shuts behind you." << endl << "In front of you, the floor is littered with plush pillows and velvet throws. You feel the weight of your body on your knees. You are very tired." << endl << "What do you do?" << endl << endl << "Lay Down (A)" << endl << "Slap Yourself to Stay Awake (B) " << endl << "Flip the mattresses over (C) ";
             Input(userChoice, 'c');
             while (userChoice != 'A' && userChoice != 'a'){
                 if (userChoice == 'B' || userChoice == 'b'){
                     cout << "Ouch. You take a point of damage." << endl;
                     player.hp--;
                 }
                 else if (userChoice == 'C' || userChoice == 'c'){
                     cout << "The other side looks even softer. You yawn." << endl;
                 }
                 else {
                     cout << "Incorrect choice. Try again" << endl;
                 }
                 cout << "What do you do?" << endl << endl << "Lay Down (A)" << endl << "Slap Yourself to Stay Awake (B) " << endl << "Flip the mattresses over (C) ";
                 Input(userChoice, 'c');
             }
             int restore = d(4);
             cout << "As you lay down you feel yourself encompassed by the warm, warm sheets. You feel pleasant. You awaken what feels like hours later, very rested. You gain " << restore << " hp." << endl << "The door is wide open and all of the items in the room but the mattress you are on, are gone." << endl << "You feel someone has been here." << endl << "You feel violated" << endl;
             if (player.hasCompanion == true){
                 cout << "Jorgun is gone too..." << endl;
             }
             cout << endl;
             player.hp += restore;
             blueRoom = true;
                                 
         }
         //Repeated entrance into west/blue room
         else if ((userChoice == 'D' || userChoice == 'd') && blueRoom == true){
             cout << "The room is as you left it." << endl << endl;
         }
         //First entrance into south/yellow room
         else if ((userChoice == 'E' || userChoice == 'e') && yellowRoom == false){
             cout << "As you open the door you are immediately confronted with a curtain. You cross through, hearing the door shut behind you." << endl << "Directly in front of you is what looks like a watery curtain." << endl << "You peer to the other side and notice there is a gorgeous, completely naked woman on the other side. You are entranced and she beckons you over seductively." << endl << endl;
             cout << "What do you do?" << endl << "Cross over to the other side (A)" << endl << "Tear off all your fucking clothes (B)" << endl << "Look away (C) ";
             Input(userChoice, 'c');
             while (userChoice != 'B' && userChoice != 'b'){
                 if (userChoice == 'A' || userChoice == 'a'){
                     cout << "You excitedly cross over to the other side. Perhaps a bit too eagerly, as you are confronted with nothing. You turn around and the water curtain remains with nothing on the other side. You cross back and the woman is there again, beckoning." << endl << endl;
                 }
                 else if (userChoice == 'C' || userChoice == 'c'){
                     cout << "You turn around only to hear her cry: " << player.name << " did I upset you?" << endl << "Her voice enthralls you and you turn around to face her." << endl << endl;
                 }
                 else {
                     cout << "Incorrect choice. Try again." << endl << endl;
                 }
                 cout << "What do you do?" << endl << "Cross over to the other side (A)" << endl << "Tear off all your fucking clothes (B)" << endl << "Look away (C) ";
                 Input(userChoice, 'c');
             }
             cout << "As you quickly toss your clothes aside, you see the woman on the other side begin to get more and more giddy, awaiting your arrival." << endl << "You burst through the watery curtain and find only a studded leather chest plate. You put it on and - although a bit disappointed - feel more secure. You cross back over, put on your clothes, and exit through the now open door." << endl << endl;
             player.ac++;
             yellowRoom = true;
         }
         //Repeated entrance into south/yellow room
         else if ((userChoice == 'E' || userChoice == 'e') && yellowRoom == true){
             cout << "The room is as you left it." << endl << endl;
         }
         //error
         else {
             cout << "Incorrect choice. Try again." << endl;
         }
     }
                         
     if (blackRoom == true && player.hp > 0){
         puzzle = true;
     }
    
}


void End (Character &player, Character &Jorgun){
    char userChoice;
    cout << "You take the solid obsidian key from the container and place it into the slot on the northern door. It slides in smoothly and the door creaks open. Inside, the room is completely square and empty." << endl << endl << "What do you do?" << endl;
    cout << "Investigate the Room (A)" << endl << "Shut the Door (B)" << endl << "Kill Yourself (C) ";
    Input(userChoice, 'c');
    
    
    while (userChoice == 'A' || userChoice == 'a'){
        //Investigate
        cout << "You scour the walls, floor, and everything inbetween but find abolutely nothing." << endl << endl;
        cout << "What do you do?" << endl << "Investigate the Room (A)" << endl << "Shut the Door (B)" << endl << "Kill Yourself (C) ";
        Input(userChoice, 'c');
    }
    
    //Begin Fight Sequence
    if (userChoice == 'B' || userChoice == 'b'){
        cout << "You shut the door and it takes your eyes a moment to adjust to the darkness." << endl << "Then you ntoice the faint glowing light of candles. You look up, and there are candles at the points of a red pentagram drawn on the floor." << endl << "The illusion spell has worn off and Jorgun stands before you. Not as Jorgun, but as his true, demonic form." << endl << "He says: Thank you. You have no idea how long I have waited for this moment. The black sacrament is not easy to complete." << endl << endl << "He lunges at you, his leathery red wings gliding through the air; fangs glaring. You brace for the fight." << endl << endl;
        Fight(player, Jorgun);
        if (player.hp > 0 && player.hp < 10){
            cout << "With the last bit of your strength, you plunge your sword into the demon's neck. He falls back, gurgling blood. Triumphant, you toss his body aside. You leave in one piece." << endl;
        }
        else if (player.hp >= 10){
            cout << "That was easy. Too easy..." << endl << "You leave basically completely unscathed. Congrats." << endl << endl << endl << endl << "Cheater." << endl;
        }
        else {
            cout << "The demon claws at you, knocking you down and sinks his fangs into your neck. You bleed out as he drags you into the pentagram and completes his sacrament." << endl << "Your soul gets dragged to hell." << endl;
        }
    }
    //Suicide
    else if (userChoice == 'C' || userChoice == 'c'){
        player.hp = 0;
        cout << "This room..." << endl << "It makes no sense... This enigma is..." << endl << "it's too much." << endl << endl << "You snap your neck and fall limp on the ground." << endl << endl;
    }
}

                     
