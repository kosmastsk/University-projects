//Τσιάκας Κοσμάς - ΑΕΜ 8255 - 6987921920 - ktsiakas@ece.auth.gr
//Παπάζογλου Αθανάσιος - AEM 8324 - 6976939155 - acpapazo@ece.auth.gr

package gr.auth.ee.dsproject.proximity.defplayers;

import gr.auth.ee.dsproject.proximity.board.Board;
import gr.auth.ee.dsproject.proximity.board.ProximityUtilities;
import gr.auth.ee.dsproject.proximity.board.Tile;

//Η κλάση RandomPlayer υλοποιεί τους δύο παίχτες, έχοντας ως μεταβλητές τα στοιχεία που
//χαρακτηρίζουν κάθε παίχτη, συναρτήσεις set/get για τις μεταβλητές και δύο συναρτήσεις
//που υλοποιούν τις ενέργειες των παιχτών για την επόμενη κίνηση
public class RandomPlayer implements AbstractPlayer
{
    int id; //1 ή 2 ανάλογα αν ο παίχτης είναι μπλέ ή κόκκινος
    String name;//το όνομα του παίχτη
    int score;//άθροισμα τιμών πλακιδίων του παίχτη
    int numOfTiles;//αριθμός πλακιδίων του παίχτη στο ταμπλό

    //1η συνάρτηση αρχικών συνθηκών που ορίζει μόνο το id
    public RandomPlayer(Integer pid){
        id=pid;
    }

    //2η συνάρτηση αρχικών συνθηκών που ορίζει όλες τις μεταβλητές
    public RandomPlayer(Integer pid, String pname, Integer pscore, Integer pnumOfTiles){
        id=pid;
        name=pname;
        score=pscore;
        numOfTiles=pnumOfTiles;
    }

    //συναρτήσεις set για κάθε μεταβλητή
    public void setId(int pid){
        id=pid;
    }

    public void setName(String pname){
        name=pname;
    }

    public void setScore(int pscore){
        score=pscore;
    }

    public void setNumOfTiles(int pnumOfTiles){
        numOfTiles=pnumOfTiles;
    }

    //συναρτήσεις get για κάθε μεταβλητή
    public int getId(){
        return id;
    }

    public String getName(){
        return name;
    }

    public int getScore(){
        return score;
    }

    public int getNumOfTiles(){
        return numOfTiles;
    }
    //Η συνάρτηση getNextMove(..)
    //επιστρέφει τη θέση [x,y] που θα τοποθετηθεί το επόμενο πλακίδιο
    //εφόσον η τυχαία θέση που επιλέγεται είναι εντός των ορίων του board
    //και δεν είναι ήδη κατειλημμένη η θέση αυτή
    public int[] getNextMove(Board board){
        int[] nextMove = new int[2];//ένας πίνακας 2 θέσεων [x,y],τύπου int για την αποθήκευση
                                    //των συντεταγμένων της επόμενης κίνησης
        do {
            //με την επόμενη εντολή προκύπτει αριθμός στο διάστημα [0,number_of_columns)
            nextMove[0]=(int)(Math.random()*ProximityUtilities.NUMBER_OF_COLUMNS);
            //με την επόμενη εντολή προκύπτει αριθμός στο διάστημα [0,number_of_rows)
            nextMove[1]=(int)(Math.random()*ProximityUtilities.NUMBER_OF_ROWS);
        //ελέγχω αν η θέση που επιλέχθηκε τυχαία είναι στα όρια του board
        //αλλιώς επιλέγω ξανά τυχαία ώσπου να ισχύουν οι συνθήκες μου
        }while (!(board.isInsideBoard(nextMove[0], nextMove[1])));
        //δημιουργώ το Tile για την επόμενη κίνηση με τις συντεταγμένες που επιλέχθηκαν πριν
        Tile nextTile = board.getTile(nextMove[0],nextMove[1]);
        if (nextTile.getPlayerId()==1 | nextTile.getPlayerId()==2){//ελέγχω αν η θέση είναι κατειλημμένη
            nextMove = getNextMove(board);//αν είναι, ξανακαλώ τη συνάρτηση
        }
        return nextMove;
    }
    //Η συνάρτηση getNeighborsCoordinates(..)
    //επιστρέφει τις συντεταγμένες των γειτόνων του στοιχείου που είναι στη θέση [x,y]
    //πρέπει να ελέγχουμε αν το στοιχείο είναι σε άρτια ή περιττή γραμμή
    //γιατί κάθε φορά διαφοροποιείται ο αλγόριθμος
    public static int[][] getNeighborsCoordinates(Board board,int x, int y){
        int[][] neighborsCoordinates = new int[6][2];//σε αυτόν τον πίνακα αποθηκεύονται οι συντεταγμένες των γειτόνων
        if (y%2==0){//άρτιες γραμμές
            neighborsCoordinates[0][0]=x+1;
            neighborsCoordinates[0][1]=y;
            neighborsCoordinates[1][0]=x;
            neighborsCoordinates[1][1]=y+1;
            neighborsCoordinates[2][0]=x-1;
            neighborsCoordinates[2][1]=y+1;
            neighborsCoordinates[3][0]=x-1;
            neighborsCoordinates[3][1]=y;
            neighborsCoordinates[4][0]=x-1;
            neighborsCoordinates[4][1]=y-1;
            neighborsCoordinates[5][0]=x;
            neighborsCoordinates[5][1]=y-1;
        }
        else{//περιττές γραμμές
            neighborsCoordinates[0][0]=x+1;
            neighborsCoordinates[0][1]=y;
            neighborsCoordinates[1][0]=x+1;
            neighborsCoordinates[1][1]=y+1;
            neighborsCoordinates[2][0]=x;
            neighborsCoordinates[2][1]=y+1;
            neighborsCoordinates[3][0]=x-1;
            neighborsCoordinates[3][1]=y;
            neighborsCoordinates[4][0]=x;
            neighborsCoordinates[4][1]=y-1;
            neighborsCoordinates[5][0]=x+1;
            neighborsCoordinates[5][1]=y-1;
        }
        //ελέγχω αν κάποιο από τα στοιχεία είναι στην άκρη του board
        //και άρα δεν έχει 6 γείτονες, οπότε στη θέση αυτή του πίνακα βάζω [-1,-1]
        for (int i=0; i<6; i++){
            if (neighborsCoordinates[i][0]==-1 | neighborsCoordinates[i][0]==ProximityUtilities.NUMBER_OF_COLUMNS){
                neighborsCoordinates[i][0]=-1;
                neighborsCoordinates[i][1]=-1;
            }

            if (neighborsCoordinates[i][1]==-1 | neighborsCoordinates[i][1]==ProximityUtilities.NUMBER_OF_ROWS){
                neighborsCoordinates[i][0]=-1;
                neighborsCoordinates[i][1]=-1;
            }
        }
        return neighborsCoordinates;
    }
}


