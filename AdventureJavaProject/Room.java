import java.util.*;

public class Room {
  private Room north, south, east, west, xyzzy;
  private String name;
  private static Player player;
  private ArrayList<String> loot = new ArrayList<String>();
  private String firstInteract; 
  private String triggeredInteract;
  private String look;
  private boolean trigger = false;
  private String gift;
  
  /*
   * sends room objects the needed variables to use the interactions class
   */
  
  Room(String name, Player player,String look, String firstInteract, String triggeredInteract, String gift) {
    this.name = name;
    this.player = player;
    this.look = look;
    this.firstInteract  = firstInteract; 
    this.triggeredInteract = triggeredInteract;
    this.gift = gift;
    
  }
  
  void setTrigger() {
    trigger = true;
  }
  
  /*
   *multiple methods that allow other classes to get various private variables from room objects
   */
  
  boolean getTrigger() {
    return trigger;
  }
  
  String getGift() {
    return gift;
  }
  
  String getInteract() {
    if (trigger == false) {
      return firstInteract;
    }
    return triggeredInteract;
  }
  
  String getName() {
    return name;
  }
  
  String getLook() {
    return look;
  }
  
  ArrayList<String> getLoot(){
    return loot;
  }
  
  /*
   *will print out what is on the floor of the room
   */
  
  void loot() {
    for(int i = 0; i < loot.size(); i++)
      System.out.println(loot.get(i));
  }
  
  /*
   *will add an item to the floor of a room if it is dropped
   */
  
  void drop(String item) {
    loot.add(item);
  }
  
  /*
   *will remove an item from the floor of a room if it is picked up
   */
  
  void pickup(String item) {
    loot.remove(item);
  }
  
  /*
   *will either return the room that is in the sepcified direction or will say that that path is blocked and return the room the player is currently in
   */
  Room north() {
    if (north != null){
      return north;
    }
    else {
      System.out.println("That passage is blocked by fallen rocks.");
      return player.getLocation();
      
    }
  }
  
  Room south() {
    if (south != null){
      return south;
    }
    else {
      System.out.println("That passage is blocked by fallen rocks.");
      return player.getLocation();
    }
  }
  
  Room west() {
    if (west != null){
      return west;
    }
    else {
      System.out.println("That passage is blocked by fallen rocks.");
      return player.getLocation();
    }
  }
  
  Room east() {
    if (east != null){
      return east;
    }
    else {
      System.out.println("That passage is blocked by fallen rocks.");
      return player.getLocation();
    }
  }
  
  Room xyzzy() {
    return xyzzy;
  }
  
  /*
   *sets the rooms that are in a specific direction of this room
   */
  void connectNorth(Room room) {
    north = room;
  }
  
  void connectSouth(Room room) {
    south = room;
  }
  
  void connectEast(Room room) {
    east = room;
  }
  
  void connectWest(Room room) {
    west = room;
  }
  
  void connectXyzzy(Room room) {
    xyzzy = room;
  }
}