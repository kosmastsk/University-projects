//Τσιάκας Κοσμάς - ΑΕΜ 8255 - 6987921920 - ktsiakas@ece.auth.gr
//Παπάζογλου Αθανάσιος - AEM 8324 - 6976939155 - acpapazo@ece.auth.gr

package gr.auth.ee.dsproject.proximity.defplayers;

import java.util.ArrayList;
import java.util.HashMap;

import gr.auth.ee.dsproject.proximity.board.Board;
import gr.auth.ee.dsproject.proximity.board.ProximityUtilities;
import gr.auth.ee.dsproject.proximity.board.Tile;

//Η κλάση Node82558324 υλοποιεί ένα δέντρο διαθέσιμων κινήσεων το οποίο θα χρησιμοποιηθεί
//για την επιλογή της βέλτιστης για εμάς κίνησης μέσω του αλγορίθμου MinMax
public class Node82558324
{
	Node82558324 parent;//ο πατέρας του κόμβου που δημιουργήσαμε
	ArrayList<Node82558324> children;//δυναμικός πίνακας για την αποθήκευση των παιδιών του κόμβου
	int nodeDepth;//το βάθος του κόμβου στο δέντρο του MinMax αλγορίθμου
	int[] nodeMove;//την κίνηση που αντιπροσωπεύει το Node. (x, y, randomNumber)
	Board nodeBoard;//το board μετά την τη συγκεκριμένη κίνηση-κόμβο
	double nodeEvaluation;//η αξιολόγηση για την κίνηση αυτή που δείχνει ο κόμβος
	
	//Constructors της κλάσης
	//χωρίς ορίσματα
	public Node82558324()
	{
		parent=null;
		children=null;
		nodeDepth=0;
		nodeMove=null;
		nodeBoard=null;
		nodeEvaluation=0;
	}
	//με ορίσματα	
	public Node82558324(Node82558324 parent, Board board)
	{
		this.parent=parent;
		nodeBoard=board;
		children=new ArrayList<Node82558324>();
	}
	
	public Node82558324(Board board)
	{
		nodeBoard=board;
		children=new ArrayList<Node82558324>();
	}
	
	//συναρτήσεις get και set
	public Node82558324 getParent()
	{
		return this.parent;
	}
	
	public ArrayList<Node82558324> getChildren()
	{
		return this.children;
	}
	
	public int getNodeDepth()
	{
		return this.nodeDepth;
	}
	
	public int[] getNodeMove()
	{
		return this.nodeMove;
	}
	
	public Board getNodeBoard()
	{
		return this.nodeBoard;
	}
	
	public double getNodeEvaluation()
	{
		return this.nodeEvaluation;
	}
	
	public void setParent(Node82558324 parent)
	{
		this.parent=parent;
	}
	
	public void setNodeDepth(int NodeDepth)
	{
		this.nodeDepth=NodeDepth;
	}
	
	public void setNodeEvaluation(double evaluation)
	{
		nodeEvaluation=evaluation;
	}
	
	public void setNodeMove(int x, int y, int randomNumber)
	{
		nodeMove = new int[3];
		nodeMove[0]=x;
		nodeMove[1]=y;
		nodeMove[2]=randomNumber;
	}
	
	public void setNodeMove(int[] nodeMove)
	{
		this.nodeMove=nodeMove;
	}
	
	public void setChildren(Node82558324 child)
	{
		children.add(child);
	}
	
	//Η συνάρτηση evaluate(..) υλοποιεί την αξιολόγηση κάθε διαθέσιμης θέσης για
	//την τοποθέτηση ενός πλακιδίου. Η υλοποίηση μας βασίζει της αξιολόγηση πάνω σε 
	//σε 3 στοιχεία. 
	//Το πρώτο (50%) είναι η δυνατότητα απόκτησης του μεγαλύτερου αντίπαλου tile
	//Το δεύτερο (25%) είναι ο αριθμός των γειτόνων, δηλαδή η κεντρικότητα του πλακιδίου
	//και αν οι γείτονες είναι δικοί μας ή όχι
	//Το τρίτο (25%) είναι οι διαθέσιμες τιμές που υπάρχουν για τα tiles μας
	//συναρτήσει των τιμών των πλακιδίων των γειτόνων
	public static double evaluate(Board board, int randomNumber, Tile tile)
	{
		double evaluation = 0;//η τελική τιμή που θα επιστραφεί
		//οι συντεταγμένες της θέσης που θα αξιολογήσουμε
		int x=tile.getX();
		int y=tile.getY();
		//πίνακας για την αποθήκευση επιμέρους αξιολόγησης για κάθε γείτονα
		//στις θέσεις του πίνακα υπάρχουν με τη σειρά οι αξιολογήσεις για τους
		//γείτονες: EAST - SE - SW - WEST - NW - NE
		double[] evaluations = new double[6];
		for (int i=0;i<6;i++){
			evaluations[i]=0;
		}
		int myNeighbors=0;//πόσοι γείτονες είναι στη δική μου ομάδα
		int yourNeighbors=0;//πόσοι γείτονες είναι αντίπαλοι
		int totalNeighbors=0;//πόσοι είναι οι συνολικοί γείτονες
		//στον πίνακα Neighbors αποθηκεύουμε τους γείτονες της θέσης (x,y)
		//με τη σειρά EAST - SE - SW - WEST - NW - NE
		Tile[] neighbors = new Tile[6];
		neighbors = ProximityUtilities.getNeighbors(x,y,board);
		for (int i=0;i<6;i++){
			if (neighbors[i] != null){//έλεγχος αν υπάρχει γείτονας
				if (neighbors[i].getPlayerId()==tile.getPlayerId()){//αν ο γείτονας i ανήκει στην ομάδα μας
					myNeighbors += 1;
				}
				else if (neighbors[i].getPlayerId()!=tile.getPlayerId() && neighbors[i].getPlayerId()!=0){//αν ο γείτονας i είναι αντίπαλος
					yourNeighbors += 1;
				}
				//Για να κάνουμε επιθετικό τον παίκτη μας,δίνουμε το 50% του συνολικού evaluation
				if (neighbors[i].getPlayerId()!=tile.getPlayerId() && neighbors[i].getPlayerId()!=0){//είναι αντίπαλος
					if (randomNumber>neighbors[i].getScore()){//μπορούμε να πάρουμε το tile του
						evaluation+=50;
					}
				}
			}
		}
		totalNeighbors=myNeighbors+yourNeighbors;
		evaluation /= totalNeighbors;
		
		//ανάλογα με τον αριθμό των γειτόνων(οποιασδήποτε ομάδας) αυξάνουμε το συνολικό
		//evaluation. Αν και οι γείτονες της ομάδας μας είναι πιο πολλοί απ'ότι του αντιπάλου
		//αυξάνουμε περισσότερο το evaluation, καθώς δημιουργείται ένα block από δικά μας 
		//tiles που κάνουν πιο δύσκολο το κλέψιμο του δικού μας tile που θα τοποθετηθεί εκεί
		//οι τιμές που προστίθενται στο evaluation προέκυψαν από δική μας εκτίμηση
		//σχετικά με την καλύτερη και τη χειρότερη περίπτωση
		if (randomNumber<8){
			if (totalNeighbors<=2){//0 ή 1 ή 2 γειτονες
				evaluation+=10;
				if (myNeighbors>yourNeighbors){
					evaluation +=15;
				}
			}
			else if(totalNeighbors<=4){//3 ή 4 γειτονες
				evaluation+=5;
				if (myNeighbors>yourNeighbors){
					evaluation += 8;
				}
			}
			else{//5 ή 6 γειτονες
				evaluation+=0;
				if (myNeighbors>yourNeighbors){
					evaluation += 5;//η χειροτερη περίπτωση
				}
			}
		}
		else if(randomNumber<=20){
			if (totalNeighbors<=2){//0 ή 1 ή 2 γειτονες
				evaluation+=0;
				if (myNeighbors>yourNeighbors){
					evaluation += 2;
				}
			}
			else if(totalNeighbors<=4){//3 ή 4 γειτονες
				evaluation+=7.5;
				if (myNeighbors>yourNeighbors){
					evaluation += 5;
				}
			}
			else{//5 ή 6 γειτονες
				evaluation+=15;
				if (myNeighbors>yourNeighbors){
					evaluation += 10;//η καλύτερη περίπτωση
				}
			}
		}
		//Αν ο μέσος όρος των διαθέσιμων τιμών μας είναι μεγάλος μπορούμε να 
		//κάνουμε τον παίχτη μας πιο επιθετικό, ενώ αν είναι μικρός
		//παίζει πιο συντηρητικά
		//ελέγχουμε παράλληλα και τα διαθέσιμα tiles του αντιπάλου και 
		//αν έχει αρκετά μεγάλα tiles παίζουμε πιο κλειστά ενώ αν δεν έχει πολλά μεγάλα
		//χτυπάμε για να πάρουμε όσα περισσότερα
		//στο HashMap myPool υπάρχουν όλες οι διαθέσιμες τιμές για το πλακίδιο μου με 
		//τη σχέση (key-value)=(τιμή πλακιδίου-διαθεσιμότητα)
		HashMap<Integer, Integer> myPool = new HashMap<Integer, Integer>();
		myPool = board.getMyPool();
		double myMean = ProximityUtilities.calculateMeanNumberForPool(myPool);
		HashMap<Integer, Integer> yourPool = new HashMap<Integer, Integer>();
		yourPool=board.getOpponentsPool();
		double yourMean=ProximityUtilities.calculateMeanNumberForPool(yourPool);
		if (myMean<6){//μικρό μέσο όρο τιμών
			//αν έχει πολλούς δικούς μου γειτονες για να κλείσω μπλοκ
			//ή λίγους γείτονες γενικά
			if((totalNeighbors>4 && myNeighbors==totalNeighbors)|(totalNeighbors<2)){
				evaluation+=12.5;
			}
			if (yourMean>14){//μεγάλα tiles διαθέσιμα στον αντίπαλο
				if(totalNeighbors>4 && myNeighbors==totalNeighbors){
					evaluation+=12.5;
				}
				else if(myNeighbors>yourNeighbors){
					evaluation+=7.5;
				}
			}
		}else if(myMean<15){//μέτρια tiles
			if (yourMean<6){
				if(totalNeighbors>3){
					evaluation+=12.5;
				}
			}
			else if(yourMean>5 && yourMean<15){//έχουμε ίδιο μέσο όρο τιμών περίπου
				if (myNeighbors>yourNeighbors){
					evaluation+=7.5;
				}
			}
			if(myNeighbors>yourNeighbors){
				evaluation+=12.5;
			}
		}
		else{//μεγάλο μέσο όρο τιμών
			if (totalNeighbors>3){
				evaluation+=12.5;
			}
			if (yourMean<6){
				if(totalNeighbors>3){
					evaluation+=12.5;
				}
			}
			else if(yourMean>14){
				if(myNeighbors>yourNeighbors){
					evaluation+=12.5;
				}
			}
		}
		System.out.println(evaluation);
		return evaluation;
	}
}
