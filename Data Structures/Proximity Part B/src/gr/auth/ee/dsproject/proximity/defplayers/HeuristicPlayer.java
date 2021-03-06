//Τσιάκας Κοσμάς - ΑΕΜ 8255 - 6987921920 - ktsiakas@ece.auth.gr
//Παπάζογλου Αθανάσιος - AEM 8324 - 6976939155 - acpapazo@ece.auth.gr

package gr.auth.ee.dsproject.proximity.defplayers;

import java.util.ArrayList;
//import java.util.Arrays;
//import java.util.Collections;
//import java.util.Comparator;
import java.util.HashMap;

import gr.auth.ee.dsproject.proximity.board.Board;
import gr.auth.ee.dsproject.proximity.board.ProximityUtilities;
import gr.auth.ee.dsproject.proximity.board.Tile;

public class HeuristicPlayer implements AbstractPlayer {

	int score;
	int id;
	String name;
	int numOfTiles;

	public HeuristicPlayer (Integer pid)
	{
		id = pid;
		name = "Heuristic";
	}

	public String getName ()
	{
		return name;
	}

	public int getNumOfTiles(){
		return numOfTiles;
	}

	public void setNumOfTiles(int tiles){
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

	//Η συνάρτηση getNextMove(..) υλοποιεί την επιλογή της τελικής μας κίνησης
	//Αρχικά, δημιουργεί μια Array List για την αποθήκευση της αξιολόγησης
	//κάθε πιθανής θέσης στην οποία μπορεί να τοποθετηθεί το επόμενο πλακίδιο
	//Κάθε πιθανή θέση που είναι αποθηκευμένη στο Array List αξιολογείται
	//και επιλέγεται το καλύτερο με βάση το μεγαλύτερο evaluation που 
	//λαμβάνει μέσω της συνάρτησης μας getEvaluation(..).
	//Η συνάρτηση επιστρέφει ένα πίνακα 1x3 που περιέχει τις συντεταγμένες x,y 
	//της καλύτερης θέσης και τον τυχαίο αριθμό που θα τοποθετηθεί στη θέση αυτή.
	
	public int[] getNextMove (Board board , int randomNumber)
	{
		//Δημιουργία δυναμικής δομής για την αποθήκευση όλων των πιθανών κινήσεων
		//possibleMoves
		ArrayList<Double[]> posMoves = new ArrayList<Double[]>();
		//δημιουργία πίνακα 1x3 για την εύρεση της θέσης με το μεγαλύτερο evaluation 
		Double[] maxEval = new Double[3];
		//σκανάρουμε όλο το board για να βρούμε ποιές θέσεις είναι διαθέσιμες 
		//για την τοποθέτηση του επόμενου πλακιδίου και τις τοποθετούμε στο ArrayList posMoves
		for (int i=0;i<ProximityUtilities.NUMBER_OF_COLUMNS;i++){
			for (int j=0;j<ProximityUtilities.NUMBER_OF_ROWS;j++){
				//πίνακας με [x,y,evaluation]. Δημιουργία νέου αντικειμένου σε κάθε επανάληψη
				//για την τοποθέτηση νέων στοιχείων στο Array List
				Double[] xye = new Double[3];//τύπου Double αφου το evaluation είναι double
				if (board.getTile(i, j).getPlayerId()==0){//έλεγχος διαθεσιμότητας
					xye[0]=(double)i;//αποθήκευση της πρώτης συντεταγμένης
					xye[1]=(double)j;//αποθήκευση της δεύτερης συντεταγμένης
					//αποθήκευση του evaluation της θέσης (i,j)
					xye[2]=getEvaluation(board, randomNumber, board.getTile(i, j));
					posMoves.add(xye);//προσθήκη στο Array List
				}
			}
		}
		//Υπολογισμός του μέγιστου στοιχείου της Array List, με βάση το evaluation
		maxEval=posMoves.get(0);
		for (int i=1;i<posMoves.size();i++){
			//στην 3η θέση των πινάκων maxEval και xye βρίσκεται το evaluation τα οποία συγκρίνω
			if (posMoves.get(i)[2] > maxEval[2]){
				maxEval=posMoves.get(i);
			}
		}
		//Δημιουργία του πίνακα που θέλουμε να επιστρέφει η συνάρτηση μας
		//πίνακα 1x3 που περιέχει τις συντεταγμένες της καλύτερης θέσης 
		//και τον τυχαίο αριθμό που θα τοποθετηθεί στη θέση αυτή.
		int[] bestEval=new int[3];
		bestEval[0]=maxEval[0].intValue();//x
		bestEval[1]=maxEval[1].intValue();//y
		bestEval[2]=randomNumber;
		return bestEval;	
	}

	//Η συνάρτηση getEvaluation(..) υλοποιεί την αξιολόγηση κάθε διαθέσιμης θέσης για
	//την τοποθέτηση ενός πλακιδίου. Η υλοποίηση μας βασίζει της αξιολόγηση πάνω σε 
	//σε 3 στοιχεία. 
	//Το πρώτο (50%) είναι η δυνατότητα απόκτησης του μεγαλύτερου αντίπαλου tile
	//Το δεύτερο (25%) είναι ο αριθμός των γειτόνων, δηλαδή η κεντρικότητα του πλακιδίου
	//και αν οι γείτονες είναι δικοί μας ή όχι
	//Το τρίτο (25%) είναι οι διαθέσιμες τιμές που υπάρχουν για τα tiles μας
	//συναρτήσει των τιμών των πλακιδίων των γειτόνων
	double getEvaluation(Board board, int randomNumber, Tile tile){
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
				else if (neighbors[i].getPlayerId()!=this.id && neighbors[i].getPlayerId()!=0){//αν ο γείτονας i είναι αντίπαλος
					yourNeighbors += 1;
				}
				//Για να κάνουμε επιθετικό τον παίκτη μας,δίνουμε το 50% του συνολικού evaluation
				if (neighbors[i].getPlayerId()!=this.id && neighbors[i].getPlayerId()!=0){//είναι αντίπαλος
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
		
		//ανάλογα με τις διαθέσιμες τιμές που είναι πιθανόν να έρθουν για εμας
		//και την τιμή των γειτονικών πλακιδίων του αντιπάλου επηρεάζουμε την τιμή
		//του συνολικού evaluation κατα 25%
		//στο HashMap myPool υπάρχουν όλες οι διαθέσιμες τιμές για το πλακίδιο μου με 
		//τη σχέση (key-value)=(τιμή πλακιδίου-διαθεσιμότητα)
		HashMap <Integer,Integer> myPool = board.getMyPool();
		//στον πίνακα avTiles αποθηκεύω πόσα Tiles μου απομένουν για τέσσερις ομάδες αριθμών
		//στην 1η θέση αποθηκεύω για τις τιμές 1 εώς 5, στην 2η 6 εώς 10
		//στην 3η 11 εώς 15 και στην 4η 16 εώς 20
		int[] avTiles= new int[4];
		//νέος πίνακας για υπολογισμό του ξεχωριστού evaluation για κάθε ομάδα τιμών 
		//ανάλογα με το pool διαθέσιμων κινήσεων
		//κάθε θέση είναι σε αντιστοιχία με την ομάδα τιμών του avTiles
		double[] poolEval= new double[4];
		//αρχικοποιηση των θεσεων του πινακα με 0
		for (int i=0;i<4;i++){
			poolEval[i]=0;
		}
		//το συνολικό evaluation που θα προκύψει για το 4ο κριτήριο και θα επηρεάζεται 
		//κατά ποσοστά από το poolEval κάθε ομάδας τιμών
		//αυτό θα προστεθεί στο συνολικό evaluation της θέσης (x,y)
		double poolEvaluation=0;
		//υπολογισμός του αριθμού διαθέσιμων Tile
		//το max του avTiles[i] είναι 15 (3 υπολειπόμενα Tile για κάθε μία από τις 5 τιμές)
		
		for (int i=1;i<6;i++){//tiles απο 1 εως 5
			avTiles[0] += myPool.get(i);
		}
		for (int i=6;i<11;i++){//tiles απο 6 εως 10
			avTiles[1] += myPool.get(i);
		}
		for (int i=11;i<16;i++){//tiles απο 11 εως 15
			avTiles[2] += myPool.get(i);
		}
		for (int i=16;i<21;i++){//tiles απο 16 εως 20
			avTiles[3] += myPool.get(i);
		}
		//θέλω να ελέγχω και τις 4 κατηγορίες διαθέσιμων τιμών
		//και για κάθε μία τον αριθμό των γειτόνων
		//αν έχω γενικώς μεγάλα Tile θα είναι καλύτερο να έχω πολλούς γείτονες καθώς
		//κλείνω ένα δυνατό μπλοκ από Tile και μπορώ συνήθως να πάρω περισσότερα tile
		//του αντιπάλου
		//αντίθετα, αν έχω μικρά Tiles με συμφέρει να το βάλω κάπου απομονωμένα
		//γιατί δεν θα μπορώ να "κλέψω" tiles και κάνω πιο ευάλωτα τα υπόλοιπα tiles
		//μου στο board
		//οι τιμές που προστίθενται στο poolEval για κάθε κατηγορία πάλι
		//προέκυψαν με δική μας εκτίμηση ανάλογα με το ποιά περίπτωση μας συμφέρει
		if (avTiles[0]>8){//τιμες απο 1 εως 5 - πολλά tiles με μικρές τιμές 
			if (yourNeighbors>=4){//4 ή 5 ή 6 αντίπαλους γείτονες
				poolEval[0]+=15;
			}
			else if(yourNeighbors>=2){//2 ή 3 αντίπαλους γείτονες
				poolEval[0]+=20;
			}
			else{                  //0 η 1 αντίπαλο γείτονα
				poolEval[0]+=25;
			}
		}
		if(avTiles[1]>8){    //πολλά tiles με τιμές 6 εως 10
			if (yourNeighbors>=4){
				poolEval[1]+=15;
			}
			else if(yourNeighbors>=2){
				poolEval[1]+=20;
			}
			else{
				poolEval[1]+=25;
			}
		}
		if(avTiles[2]>8){    //πολλά tiles με τιμές 11 εως 15
			if (yourNeighbors>=4){
				poolEval[2]+=15;
			}
			else if(yourNeighbors>=2){
				poolEval[2]+=20;
			}
			else{
				poolEval[2]+=25; 
			}
		}
		if(avTiles[3]>8){    //πολλά tiles με τιμές 16 εως 20
			if (yourNeighbors>=4){
				poolEval[3]+=25;
			}
			else if(yourNeighbors>=2){
				poolEval[3]+=20;
			}
			else{
				poolEval[3]+=15;
			}
		}
		//δίνουμε την ανάλογη βαρύτητα σε κάθε ομάδα τιμών
		//μεγαλυτερη βαρύτητα παίρνουν τα μέγαλα tiles καθώς μπορούν να κάνουν
		//περισσότερα πράγματα που βοηθούν την ομάδα μας, λόγω των τιμών τους
		//ενώ χαμηλότερη τα tiles μικρών τιμών, που δεν μπορούν να επηρεάσουν σημαντικά
		//άμεσα το τελικό σκορ
		poolEvaluation=0.15*poolEval[0] + 0.20*poolEval[1] + 0.25*poolEval[2] + 0.4*poolEval[3];		
		//το προσθέτουμε στο συνολικό evaluation το οποίο επιστρέφεται
		evaluation += poolEvaluation;				
		//System.out.println(evaluation);
		return evaluation;
	}
}
