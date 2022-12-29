import java.util.*;

// Is the 16-bit integer (15 bits only) representative of a complete red-black tree? (0 = black, highest digit = root)
// The integer is the level-order of the red-black tree. For example the binary starting 0110000... has a black root, two red children, and four black grandchildren.
// This program hardcodes the red-black tree to exactly 15 nodes to make a complete binary tree. Integers of less than 15 bits are padded to the left with leading 0s (black nodes).

public class Main 
{
    public static String bin15(int n) { // Integer to a 15-bit binary string (modulus is taken)
        n = n % (1<<15);
        String s = Integer.toString(n, 2);
        while (s.length() < 15) s = "0" + s;
        return s;
    }
    
    public static boolean blackHeightConsistent(String s, int n) { // Given a binary color string, does node n have a consistent black height?
        int bh = -1;
        for (int index = 0; index < (1<<10); index++) {
            String binary = bin15(index).substring(7);
            int count = 0;
            int node = n;
            for (int i = 0; i < binary.length(); i++) {
                count += (s.charAt(node) == '0') ? 1 : 0;
                node *= 2;
                node += (binary.charAt(i) == '1') ? 1 : 0;
                if (node >= s.length()) break;
            }
            if (bh == -1) bh = count;
            else if (count != bh) return false;
        }
        return true;
    }
    
    public static boolean twoReds(String s, int n) { // Does node n in string S satisfy the no consecutive red nodes rule? (node + children)
        if (s.charAt(n) != '1') return true;
        if (2 * n < s.length() && s.charAt(2 * n) == '1') return false;
        if (2 * n + 1 < s.length() && s.charAt(2 * n + 1) == '1') return false;
        return true;
    }
    
    public static boolean solve(int n) { // Solve the integer binary string
        n = n % (1<<15);
        String s = "0" + bin15(n);
        if (s.charAt(1) != '0') return false;
        for (int i = 1; i <= 15; i++) {
            if (!twoReds(s, i)) return false;
        }
        
        for (int i = 1; i <= 15; i++) {
            if (!blackHeightConsistent(s, i)) return false;
        }
        
        return true;
    }
    
    public static int inOrder(int x) { // Get the inorder number of a level order (i.e. bits in inorder)
        String s = bin15(x);
        int[] arr = {7, 3, 8, 1, 9, 4, 10, 0, 11, 5, 12, 2, 13, 6, 14};
        String res = "";
        for (int i : arr) res = res + s.charAt(i);
        return Integer.parseInt(res, 2);
    }
    
	public static void main(String[] args) { // List of all possible red complete red black trees in 15 bits
		for (int i = 1; i < (1<<15); i++) {
		    if (solve(i)) System.out.println(i + " " + bin15(i) + " = " + inOrder(i));
		}
	}
}

