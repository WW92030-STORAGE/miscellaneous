import java.lang.*;
import java.util.*;

public class Main
{
    static Scanner input;
    static int[] cards;
    static int counter = 0;
    
    public static boolean isNumeric(String s) {
        try {
            int x = Integer.parseInt(s);
            return true;
        }
        catch (Exception e) {
            return false;
        }
    }
    
    public static int ip(String s) {
        if (isNumeric(s)) return Integer.parseInt(s);
        return Integer.MIN_VALUE;
    }
    
    static void reset() {
        System.out.println("CARD SHARKS HELPER\nENTER THE CARD SHOWN AND THE SYSTEM WILL TELL YOU WHAT TO GUESS");
        System.out.println("ACE = 1 JACK = 11 QUEEN = 12 KING = 13. ACES ARE HIGHEST.");
        System.out.println("/QUIT TO QUIT AND /RESET TO RESET");
        cards = new int[13];
        Arrays.fill(cards, 4);
        counter = 0;
    }
    
    static boolean run() {
        while (true) {
            System.out.print("TURN " + counter + " - INPUT VISIBLE CARD NUMBER - ");
            String line = input.nextLine().toUpperCase();
		    if (isNumeric(line)) {
		        int val = ip(line);
		        if (val <= 0 || val > 13) {
		            System.out.println("ERROR INVALID CARD");
		            continue;
		        }
		    }
		    else if (line.equals("/RESET") || line.equals("0")) {
		        reset();
		        return true;
		    }
		    else if (line.equals("/QUIT")) return false;
		    else if (!isNumeric(line)) {
		        System.out.println("ERROR INVALID COMMAND");
		            continue;
		    }
		    
		    int val = ip(line);
		    
		    val = (val + 11) % 13; // Normalized ... 2s at the bottom aces highest
		    while (val < 0) val = (val + 13) % 13;
		    
		    if (cards[val] <= 0) {
		        System.out.println("ERROR - THIS CARD SHOULD NOT EXIST. RESET?");
		        continue;
		    }
		    
		    cards[val]--;
		    int lower = 0;
		    int higher = 0;
		    for (int i = 0; i < 13; i++) {
		        if (i < val) lower += cards[i];
		        else if (i > val) higher += cards[i];
		    }
		    System.out.println("THERE ARE " + lower + " LOWER CARDS AND " + higher + " HIGHER CARDS.");
		    counter++;
        }
    }
    
	public static void main(String[] args) {
	    input = new Scanner(System.in);
	    cards = new int[13];
	    reset();
		while (true) {
		    if (!run()) return;
		}
	}
}
