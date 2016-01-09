import java.util.*;

class Player {
  private Room location;
  private ArrayList<String> inventory = new ArrayList<String>();
  private static boolean start = true;
  
  /*
   * allows other classes to get the inventory of the player
   */
  
  ArrayList<String> getInventory(){
    return inventory;
  }
  
  /*
   * tells the player what room they are standing in
   */
  
  void moveTo(Room room) {
    location = room;
    if (!start){ 
      System.out.println("You are standing in the " + getLocation().getName() + ".");
    }
    start = false;
  }
  
  /*
   * allows other classes to get the location of the player
   */
  
  public Room getLocation(){
    return location;
  }
  
  boolean getStart(){
    return start;
  }
  
  /*
   * allows the player to interact with the unique object in a room
   */
  
  void use() {
    System.out.println(location.getInteract());
    if(location.getTrigger() == false && !(location.getGift().equals(""))) {
      location.drop(location.getGift());
      pickup(location.getGift());
    }
    location.setTrigger();
  }
  
  /*
   * tells the player what they can see in the cave
   */
  
  public void look(){
    System.out.println("You see:");
    if(inventory.contains("lantern") || location.getName().equals("Room 1")){
      System.out.println(location.getLook());
    } else {
      System.out.println("nothing but darkness");
    }
    location.loot();
  }
  
  /*
   * lets the player pick up something on the ground
   */
  
  public void pickup(String item){
    int indexOfItem;
    ArrayList<String> loot = location.getLoot();
    if(loot.contains(item)){
      indexOfItem = loot.indexOf(item);
      inventory.add(loot.get(indexOfItem));
      location.pickup(item);
      System.out.println("You picked up " + item);
    } else {
      System.out.println("There is no " + item + " in this room");
    }
  }
  
  /*
   * lets the player drop something from their inventory
   */
  
  public void drop(String item){
    if(inventory.contains(item)){
      location.drop(item);
      inventory.remove(item);
      System.out.println("You dropped " + item);
    } else { 
      System.out.println("You don't have " + item + " in your inventory");
    }
  }
  
  /*
   * lets the player check their inventory
   */
  public void inventory(){
    System.out.println("Inventory \n---------");
    for(int i = 0; i < inventory.size(); i++)
      System.out.println(inventory.get(i));
  }
  
}