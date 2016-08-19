/*
 * 
 CREATED IN MY FRESHMAN YEAR at UCSC
 
 Program 2 - Adventure Game
 
 This program is a text based adventure game that takes place in a cave.
 The player has the ability to move about the cave, use objects in the caves rooms, as well as
 pick up or drop any item on the ground in the room.
 
 Base code provided by: Charlie McDowell
 
 Final program created by: Peter Greer and Jim Brower
 
 */

import java.util.*;

/*
 * creates the player object and builds the world before starting the game
 */

class Adventure {
  public static void main(String[] args) {
    Player player = new Player();
    Room entryWay = buildWorld(player);
    player.moveTo(entryWay);
    play(player);   
  }
  
  /*
   * prints out instructions to play at the start
   * the game continues until the player is killed or he types quit, which make gameOn become false
   * requests an action of the player
   * will look at commands the player types in and react accordingly
   * will only let the player access a special room if he says the magic word in the correct room
   * Will check what the player is trying to use and will check if the player is in the same room as that object
   * initiates a special event where the player is chased by a goblin when they enter room 4
   */
  
  static void play(Player player) {
    String playerInput;
    boolean gameOn = true;
    boolean start = true;
    boolean goblinAlive = true;
    playerInput = "help";
    while(gameOn == true){
      Scanner input = new Scanner(System.in);
      if (!start) {
        playerInput = input.next();
      }
      start = false;
      switch (playerInput.toLowerCase()){
        case "north": player.moveTo(player.getLocation().north());
        break;
        case "south": player.moveTo(player.getLocation().south());
        break;
        case "east": player.moveTo(player.getLocation().east());
        break;
        case "west": player.moveTo(player.getLocation().west());
        break;
        case "look": player.look();
        break;
        case "drop": player.drop(input.next());
        break;
        case "pickup": player.pickup(input.next());
        break;
        case "inventory": player.inventory();
        break;
        case "quit": gameOn = false;
        break;
        case "xyzzy":if(player.getLocation().getName().equals("Room 5")){
          System.out.println("A red portal opens in front of you, and you walk into it without thinking. \nYou are still inside the cave. You see a chest in the center of the room.");
          player.moveTo(player.getLocation().xyzzy());
        }else{
          System.out.println("Nothing happens.");
        }
        break;
        case "help": printInstructions();
        break;
        case "use": switch (input.next()){
          case "man": if (player.getLocation().getName().equals("Room 1")) {
            player.use();
          }
          case "well": if (player.getLocation().getName().equals("Room 7")) {
            player.use();
          }
          case "skeleton": if (player.getLocation().getName().equals("Room 2")) {
            player.use();
          }
          case "writing": if (player.getLocation().getName().equals("Room 8")) {
            player.use();
          }
          case "ears": if (player.getLocation().getName().equals("Room 3")) {
            player.use();
          }
          case "chest": if (player.getLocation().getName().equals("Room 6")) {
            player.use();
          }
        }
        break;
        default: System.out.println("What?");
      }
      if(player.getLocation().getName().equals("Room 4") && goblinAlive){
        gameOn = goblinChase(player);
        goblinAlive = false;
      }
    }
    System.out.println("Game Over");
  }
  
  /*
   * a special method that is used when the player is being chased by a goblin
   * gives the player 5 moves to act before they are caught and killed by the goblin
   * allows the player to act normally with the added ability to turn and fight the goblin
   * does a series of checks to see what happens when the player fights the goblin
   * the player may kill the goblin if he is in the right room or has a sword in his inventory
   * if the player fights the goblin unprepared he dies
   * tells the player the status of the goblin after each move the player makes. If the player makes 5 actions without killing the goblin, the goblin catches them and kills them 
   */
  
  static boolean goblinChase(Player player){
    String playerInput = "";
    System.out.println("As you enter the room a goblin jumps out of the darkness and runs at you.");
    System.out.println("You feel the urge to either fight or run.");
    for(int i = 5; i > 0; i--){
      Scanner input = new Scanner(System.in);
      playerInput = input.next();
      switch (playerInput.toLowerCase()){
        case "north": player.moveTo(player.getLocation().north());
        break;
        case "south": player.moveTo(player.getLocation().south());
        break;
        case "east": player.moveTo(player.getLocation().east());
        break;
        case "west": player.moveTo(player.getLocation().west());
        break;
        case "look": player.look();
        break;
        case "drop": player.drop(input.next());
        break;
        case "pickup": player.pickup(input.next());
        break;
        case "inventory": player.inventory();
        break;
        case "quit": return false;
        case "xyzzy":if(player.getLocation().getName().equals("Room 5")){
          System.out.println("A red portal opens in front of you, and you walk into it without thinking. \n You are still inside the cave. You see a chest in the center of the room.");
          player.moveTo(player.getLocation().xyzzy());
        }else{
          System.out.println("Nothing happens.");
        }
        break;
        case "help": printInstructions();
        break;
        case "use": switch (input.next()){
          case "man": if (player.getLocation().getName().equals("Room 1")) {
            player.use();
          }
          case "well": if (player.getLocation().getName().equals("Room 7")) {
            player.use();
          }
          case "skeleton": if (player.getLocation().getName().equals("Room 2")) {
            player.use();
          }
          case "writing": if (player.getLocation().getName().equals("Room 8")) {
            player.use();
          }
          case "ears": if (player.getLocation().getName().equals("Room 3")) {
            player.use();
          }
          case "chest": if (player.getLocation().getName().equals("Room 6")) {
            player.use();
          }
        }
        case "fight": if (player.getInventory().contains("sword")) {
          System.out.println("You decide to turn and fight.");
          System.out.println("You pull out the sword you picked up earlier, the only weapon you have.");
          System.out.println("As the goblin charges, you stand your ground and take a fighting stance.");
          System.out.println("You plunge your sword into the goblin's chest with one decisive thrust.");
          System.out.println("The goblin falls to the ground in front of you as you pull out the bloody sword.");
          System.out.println("You examine the body. The goblin is dead.");
          player.getLocation().drop("goblin");
          return true;
        } else {
          if (player.getLocation().getName().equals("Room 7")) {
            System.out.println("You decide to turn and fight.");
            System.out.println("As the goblin charges, you swing at him with all of your might.");
            System.out.println("The goblin is knocked back slightly and trips over a rock.");
            System.out.println("The goblin falls into the well situated in the center of the room.");
            System.out.println("As he falls you hear him screech, and eventually a loud THUNK!");
            System.out.println("You approach the well and look inside it, but you can't see anything, even with the lamp.");
            return true;
          } else {
            if (1 == 1){
              System.out.println("The goblin grabs you and tears your arm off.");
              System.out.println("Wounded, you slowly bleed out as you watch the goblin eat your severed arm.");
              return false;
            }
          }
        }
        break;
        default: System.out.println("What?");
      }
      switch (i){
        case 5: System.out.println("The goblin seems dazed by the light of the lantern and trips over a rock.");
        break;
        case 4: System.out.println("The goblin gets up and persues you.");
        break;
        case 3: System.out.println("You try to escape the goblin's pursuit, but he is slowly getting closer.");;
        break;
        case 2: System.out.println("The goblin has nearly caught up to you.");
        break;
        case 1: System.out.println("The goblin is right behind you, there is no hope for escape.");
        break;
      }
    }
    System.out.println("The goblin grabs you and tears your arm off.");
    System.out.println("Wounded, you slowly bleed out as you watch the goblin eat your severed arm.");
    return false;
  }
  
  /*
   * creates the nine room objects used in the game, as well as sends those rooms a reference to the player object and some String variables
   * connects the rooms in a planned out fashion
   * starts the player off in the entryway of the cave
   */
  
  static Room buildWorld(Player player) {
    
    Room entryWay = new Room("Entry Chamber", player, "nothing" , "" , "", "");
    Room room1 = new Room("Room 1", player, "an old man sitting near a fire",  "Oh, hello adventurer, you won't get far without a lantern. Take this one.",
                          "I'm sorry, I have nothing else to offer.", "lantern");
    Room room2 = new Room("Room 2", player, "a skeleton in the corner of the room" , "You inspect the skeleton's belongings and find that it possesses a sword!\nYou don't mean to disrespect the dead, but a weapon might be useful in this\ntreacherous place." ,
                          "The poor sod has nothing else of value.", "sword");
    Room room3 = new Room("Room 3", player, "nothing of interest, but you can hear the faint sound of running water" , "You close your eyes and stand in silence. The sound of flowing water becomes\nmore prominent as you focus your mind. Your enhanced state of focus gives you\nnewfound resolve to continue your adventure." , "You meditate to the sound of the water for a little longer.", "");
    Room room4 = new Room("Room 4", player, "nothing of interest" , "" , "", "");
    Room room5 = new Room("Room 5", player, "You feel a strange air about this room. You feel as if you could go anywhere\nfrom here." , "" , "", "");
    Room room6 = new Room("Room 6", player, "a chest" , "You open the chest and get gold." , "There is no treasure left.", "gold");
    Room room7 = new Room("Room 7", player, "a well" , "You can't see anything inside the well." , "You can't see anything inside the well.", "");
    Room room8 = new Room("Room 8", player, "writing scribbled across the cave wall" , "You walk up to the wall for a closer look. As you approach, the message becomes clear. When you see it, a shiver runs up your spine. You decide it would be\nbest to get away. The message: XYZZY." ,
                          "You try to approach the writing, but your legs won't move out of fear.", "");
    
    entryWay.connectNorth(room1);
    
    room1.connectNorth(room5);
    room1.connectWest(room7);
    
    room2.connectWest(room4);
    
    room3.connectSouth(room2);
    room3.connectEast(entryWay);
    
    room4.connectWest(room7);
    room4.connectSouth(room1);
    
    room5.connectWest(room7);
    room5.connectNorth(room8);
    room5.connectXyzzy(room6);
    
    room6.connectWest(room3);
    
    room7.connectNorth(room8);
    room7.connectWest(room1);
    room7.connectEast(room5);
    
    room8.connectNorth(room7);
    room8.connectWest(room4);
    room8.connectSouth(room3);
    
    return entryWay;
  }
  
  /*
   * prints out the instructions to play at the start of the game or if the player types "help"
   */
  static void printInstructions() {
    System.out.println("You've entered the Cavern of the Fallen Hero.");
    System.out.println("Say a direction (north, south, east, west) to move from room to room.");
    System.out.println("Say 'look' to look around and 'inventory' to see what you're carrying.");
    System.out.println("Say 'use thing' (where thing is the person/object you want to interact with).");
    System.out.println("Say 'pickup item' (where item is the name of the item you want to pickup).");
    System.out.println("Say 'drop item' (where item is the name of the item you want to drop).");
    System.out.println("Say 'fight' to engage an enemy.");
    System.out.println("Say 'help' to repeat this message.");
  }
}
