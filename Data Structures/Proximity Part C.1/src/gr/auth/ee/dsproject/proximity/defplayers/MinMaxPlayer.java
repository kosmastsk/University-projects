//Τσιάκας Κοσμάς - ΑΕΜ 8255 - 6987921920 - ktsiakas@ece.auth.gr
//Παπάζογλου Αθανάσιος - AEM 8324 - 6976939155 - acpapazo@ece.auth.gr

package gr.auth.ee.dsproject.proximity.defplayers;

import java.util.ArrayList;

import gr.auth.ee.dsproject.proximity.board.Board;
import gr.auth.ee.dsproject.proximity.board.ProximityUtilities;
import gr.auth.ee.dsproject.proximity.board.Tile;

//Η κλάση MinMaxPlayer υλοποιεί έναν παίκτη ο οποίος χρησιμοποιεί
//τις συναρτήσεις του για τη δημιουργία ενός δέντρου διαθέσιμων κινήσεων
//και με τον αλγόριθμο Minmax προσπαθεί να βρεί την βέλτιστη κίνηση σε βάθος
//2 κινήσεων. Τα χαρακτηριστικά του παίκτη είναι το σκορ του, το όνομα του,
//to id του και ο αριθμός των πλακιδίων που του ανήκουν.
public class MinMaxPlayer implements AbstractPlayer
{

  int score;
  int id;
  String name;
  int numOfTiles;

  public MinMaxPlayer (Integer pid)
  {
    id = pid;
    name = "MinMaxPlayer";
  }

  public String getName ()
  {
    return name;
  }

  public int getNumOfTiles ()
  {
    return numOfTiles;
  }

  public void setNumOfTiles (int tiles)
  {
    numOfTiles = tiles;
  }

  public int getId ()
  {
    return id;
  }

  public void setScore (int score)
  {
    this.score = score;
  }

  public int getScore ()
  {
    return score;
  }

  public void setId (int id)
  {
    this.id = id;
  }

  public void setName (String name)
  {
    this.name = name;
  }
  
  //Η συνάρτηση getNextMove(..) υλοποιεί την επιλογή της βέλτιστης για εμάς κίνησης
  //Χρησιμοποιεί το υπάρχων board για να δημιουργήσει τη ρίζα του δέντρου στο οποίο θα αποθηκεύονται
  //όλες οι διαθέσιμες κινήσεις. Αρχικά, προσωμοιώνει μια δική μας κίνηση
  //μέσω της συνάρτησης createMySubTree(..), η οποία στο εσωτερικό της προσωμοιώνει και 
  //μία κίνηση του αντιπάλου. Στο τέλος της συνάρτησης μας, μέσω της chooseMinMaxMove(..)
  //επιλέγεται η καλύτερη δυνατή κίνηση και επιστρέφεται μέσω ενός πίνακα 1x3 οι συντεταγμένες
  //της καλύτερης θέσης και ο αριθμός που θα τοποθετηθεί σε αυτήν.
  public int[] getNextMove (Board board, int number)
  {
	  int[] nextMove=new int [3];//[x,y,randomNumber]
	  int[] index=new int[2];
	  //Δημιουργία ενός κόμβου που θα είναι η ρίζα του δέντρου απο το υπάρχων board
	  Node82558324 root=new Node82558324(board); 
	  createMySubtree(root,1,Board.getNextTenNumbersToBePlayed()[0]);//το δέντρο έχει τελειώσει,
	  //καθώς αυτή η συνάρτηση προσωμοιώνει τόσο τη δική μας κίνηση όσο και του αντιπάλου
	  index=chooseMinMaxMove(root);//επιλογή της καλύτερης δυνατής κίνησης
	  nextMove[0]=index[0];
	  nextMove[1]=index[1];
	  nextMove[2]=number;
	  //επιστροφή των συντεταγμένων της βέλτιστης θέσης
	  //και του αριθμού που θα τοποθετηθεί στη θέση αυτή
	  return nextMove;
  }
  
  //Η συνάρτηση createMySubtree(..) εντοπίζει τις κενές θέσεις του board
  //και για κάθε μία από αυτές δημιουργεί κλώνους του board, όπως θα ήταν εάν έπαιζα
  //στην θέση αυτή το πλακίδιο με την τιμή number. Το board που προκύπτει
  //προστίθεται ως child του parent που παίρνει ως όρισμα η συνάρτηση.
  //Για να ολοκληρωθεί το δέντρο στο τέλος καλείται η συνάρτηση createOpponentSubtree(..)
  //η οποία θα το συμπληρώσει με τους κλάδους για τις κινήσεις του αντιπάλου
  public void createMySubtree (Node82558324 parent, int depth, int number)
  {
	//βρίσκω τα ελεύθερα tiles στο board
	for (int i=0;i<ProximityUtilities.NUMBER_OF_COLUMNS;i++){//σκανάρω το board
		for (int j=0;j<ProximityUtilities.NUMBER_OF_ROWS;j++){//για διαθέσιμα tiles
			if (parent.getNodeBoard().getTile(i,j).getPlayerId()==0){//αν υπάρχει διαθέσιμο tile
				//δημιουργώ έναν κλώνο του board του parent προσωμοιώνοντας μια κίνηση
				//δική μου βάζοντας το tile με τιμή number στη θέση (i,j)
				Board cloneBoard=ProximityUtilities.boardAfterMove(this.getId(),parent.getNodeBoard(),i,j,number);
				//δημιουργώ έναν νέο κόμβο με το νέο board (μετά την κίνηση)
				//για να γίνει αυτός ο κόμβος το child του parent
				//να προστεθεί δηλαδή στο arrayList children
				Node82558324 child=new Node82558324(parent, cloneBoard);
				child.setNodeMove(i,j,number);
				child.setNodeDepth(depth);
				child.setNodeEvaluation(0);//το evaluation του κόμβου αυτόυ θα προκύψει στη συνέχεια
				parent.setChildren(child);
				//καλώ τη συνάρτηση για τη συμπλήρωση του δέντρου με την κίνηση του αντιπάλου
				createOpponentSubtree(child,depth+1,Board.getNextTenNumbersToBePlayed()[1]);
			 }
		 }
	 }
  }

  //Η συνάρτηση createOpponentSubtree(..) βρίσκει τις θέσεις που είναι πλέον διαθέσιμες στο board
  //και για κάθε μία από αυτές δημιουργεί έναν κλώνο του board προσωμοιώνοντας
  //μία κίνηση του αντιπάλου στη θέση αυτή. Μετά, θα αξιολογηθεί η κίνηση αυτή που υποδεικνύει το φύλλο
  //και το board που προκύπτει θα προστεθεί ως child του parent
  public void createOpponentSubtree (Node82558324 child, int depth, int number)
  {
	//πρέπει να βρω το id του αντιπάλου μου, εφόσον ξέρω μόνο το δικό μου
	int opponentId=1;
	if (this.getId()==1){
		opponentId=2;
	}
	//βρίσκω τα ελεύθερα tiles στο board
	for (int i=0;i<ProximityUtilities.NUMBER_OF_COLUMNS;i++){//σκανάρω το board
		for (int j=0;j<ProximityUtilities.NUMBER_OF_ROWS;j++){//για διαθέσιμα tiles
			if (child.getNodeBoard().getTile(i,j).getPlayerId()==0){//αν υπάρχει διαθέσιμο tile
				//δημιουργώ έναν κλώνο του board, όπως θα ήταν εάν έπαιζε ο αντίπαλος
				//στη θέση (i,j) ένα πλακίδιο με τιμή randomNumber
				Board cloneBoard = ProximityUtilities.boardAfterMove(opponentId,child.getNodeBoard(),i,j,number);
				//δημιουργώ ένα αντικείμενο τύπου Node που περιέχει το νέο board
				//και τα υπόλοιπα στοιχεία
				Node82558324 leaf=new Node82558324(child, cloneBoard);
				//αξιολογώ τη θέση (i,j)
				int opponentScore=0;
				for (int k=0;k<ProximityUtilities.NUMBER_OF_COLUMNS;k++){
					for (int l=0;l<ProximityUtilities.NUMBER_OF_ROWS;l++){
						if (leaf.getNodeBoard().getTile(k,l).getPlayerId()==opponentId){
							opponentScore=opponentScore+leaf.getNodeBoard().getTile(k,l).getScore();
						}
					}
				}
				int evaluation=this.getScore()-opponentScore;
				leaf.setNodeEvaluation(evaluation);
				leaf.setNodeMove(i,j,number);
				leaf.setNodeDepth(depth);
				//προσθέτω το νέο στοιχείο στο arrayList του parent της συνάρτησης
				//που είναι το child που δημιούργησε η συνάρτηση createMySubtree(..)
				child.setChildren(leaf);
			}
		}
	}
  }
  
  //Η συνάρτηση chooseMinMaxMove(..) περιλαμβάνει έναν αλγόριθμo Minmax για να βρεί
  //την καλύτερη διαθέσιμη κίνηση. Για να βρεί τη θέση αυτή,αρχικά αποθηκεύει τα παιδιά
  //της ρίζας, και για κάθε παιδί αποθηκεύει τα φύλλα του. Σε βάθος 2, κρατάει την
  //μικρότερη τιμή που βρίσκει από κάθε φύλλο και η τιμή αυτή περνάει στα παιδιά.
  //Σε βάθος 1, κρατάει την μεγαλύτερη τιμή από τα παιδιά και αυτή περνάει στη ρίζα 
  //που είναι και η τελική τιμή που θα επιλέξουμε.
  //Στο τέλος επιστρέφουμε τις συντεταγμένες της καλύτερης θέσης 
  public int[] chooseMinMaxMove(Node82558324 root)
  {
	int[] index=new int[2];//ο πίνακας που θα περιέχει την καλύτερη θέση
	//αποθηκεύω τα παιδιά της ρίζας σε μια ArrayList-ήμαστε σε βάθος 1
	ArrayList<Node82558324> children= root.getChildren();
	//ο αριμός των παιδιών της ρίζας
	int numberOfChildren=0;
	numberOfChildren=children.size();
	//στη μεταβλητή maxEval θα αποθηκεύσω το μεγαλύτερο από τα evaluation στο επίπεδο 1
	double maxEval=children.get(0).getNodeEvaluation();
	//κρατάω τις συντεταγμένες για την καλύτερη θέση στο maxNodeMove (x,y,number)
	int[] maxNodeMove=children.get(0).getNodeMove();
	//σε κάθε επανάληψη σκανάρω ένα παιδί της ρίζας
	for (int i=0;i<numberOfChildren;i++){
		//έχω τα φύλλα του κόμβου-βάθος 2
		ArrayList<Node82558324> leaves=children.get(i).getChildren();
		//υπολογίζω πόσα φύλλα έχει ο κάθε κόμβος
		int numberOfLeaves = leaves.size();
		//στη μεταβλητή minEval θα αποθηκεύσω το μικρότερο από τα evaluation σε βάθος 2
		double minEval=leaves.get(0).getNodeEvaluation();
		//(x, y , randomNumber) για την θέση που επιλέγεται
		int[] minNodeMove=leaves.get(0).getNodeMove();
		//σκανάρω κάθε φύλλο του παιδιού i
		for (int j=1;j<numberOfLeaves;j++){
			//βρίσκω ποιο είναι το μικρότερο evaluation
			if (leaves.get(j).getNodeEvaluation()<minEval){
				minEval=leaves.get(j).getNodeEvaluation();
				minNodeMove=leaves.get(j).getNodeMove();
			}
		}
		//ανεβάζω σε κάθε κόμβο την μικρότερη από τις τιμές των φύλλων
		children.get(i).setNodeEvaluation(minEval);
		children.get(i).setNodeMove(minNodeMove);
		//αν ήμαστε στην 1η επανάληψη η σύγκριση είναι περιττή
		if (i==0) continue;
		//εντοπίζω το μεγαλύτερο evaluation, αποθηκεύω παράλληλα και τις συντεταγμένες
		if(children.get(i).getNodeEvaluation()>maxEval){
			maxEval=children.get(i).getNodeEvaluation();
			maxNodeMove=children.get(i).getNodeMove();
		}
	}
	//αφου σκανάρω όλα τα παιδιά και φύλλα, στη ρίζα βάζω την τιμή της καλύτερης συνολικής κίνησης και τις συντεταγμένες  
	root.setNodeEvaluation(maxEval);
	root.setNodeMove(maxNodeMove);
	//επιστρέφω τις συντεταγμένες της καλυτερης κίνησης
	index[0]=root.getNodeMove()[0];
	index[1]=root.getNodeMove()[1];
	return index;
  }
}
