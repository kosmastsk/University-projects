package gr.auth.ee.dsproject.proximity.defplayers;

import gr.auth.ee.dsproject.proximity.board.Board;

public interface AbstractPlayer
{
  public void setId (int id);

  public int getId ();

  public void setName (String name);

  public String getName ();

  public void setScore (int score);

  public int getScore ();
  
  public void setNumOfTiles(int tiles);
  
  public int getNumOfTiles();
  
  public int[] getNextMove (Board board);
  

}
