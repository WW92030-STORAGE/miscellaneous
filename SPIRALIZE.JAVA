import java.util.*;
public class SPIRALIZE
{
    // SPIRALIZE operates on a square matrix of objects.
    // The matrix is partitioned into concentric square rings, where the outermost rings are the edge elements and each ring inwards directly borders another.
    // A ring's index is defined as follows: the outermost ring has index 0, and any ring that borders ring i internally has index i + 1.
    // The elements in the rings are rotated. If a ring's index is even then its elements are rotated counterclockwise. If the index is odd the elements are rotated clockwise.
    public static Object[][] spiralize(Object[][] args) {
        int n = args.length;
        for (Object[] row : args) {
            if (row.length != n) return args;
        }
        
        
        for (int i = 0; i < n / 2; i++) {
            Object topleft = args[i][i];
            if (i % 2 == 0) {
                for (int j = i + 1; j < n - i; j++) args[i][j - 1] = args[i][j];
                for (int j = i + 1; j < n - i; j++) args[j - 1][n - i - 1] = args[j][n - i - 1];
                for (int j = n - i - 2; j >= i; j--) args[n - i - 1][j + 1] = args[n - i - 1][j];
                for (int j = n - i - 2; j >= i; j--) args[j + 1][i] = args[j][i];
                args[i + 1][i] = topleft;
            }
            else {
                for (int j = i + 1; j < n - i; j++) args[j - 1][i] = args[j][i];
                for (int j = i + 1; j < n - i; j++) args[n - i - 1][j - 1] = args[n - i - 1][j];
                for (int j = n - i - 2; j >= i; j--) args[j + 1][n - i - 1] = args[j][n - i - 1];
                for (int j = n - i - 2; j >= i; j--) args[i][j + 1] = args[i][j];
                args[i][i + 1] = topleft;
            }
        }
        return args;
    }
    
    public static void disp(Object[][] arr) {
        for (int i = 0; i < arr.length; i++) System.out.print("-");
        System.out.println();
        int length = 1;
        for (Object[] i : arr) {
            for (Object o : i) length = Math.max(length, o.toString().length());
        }
        
        for (Object[] row : arr) {
            for (Object i : row) {
                String s = i.toString();
                while (s.length() < length) s = s + " ";
                System.out.print(s + " ");
            }
            System.out.println();
        }
    }
    
	public static void main(String[] args) { // Examples
		Integer[][] arr = {{0, 1, 2, 3}, {4, 5, 6, 7}, {8, 9, 10, 11}, {12, 13, 14, 15}};
		disp(arr);
		disp(spiralize(arr));
		
		Integer[][] arr2 = new Integer[8][8];
		for (int i = 0; i < 8; i++) {
		    for (int j = 0; j < 8; j++) arr2[i][j] = 8 * i + j;
		}
		
		disp(arr2);
		disp(spiralize(arr2));
		
		Character[][] arr3 = new Character[8][8];
		for (int i = 0; i < 8; i++) {
		    for (int j = 0; j < 8; j++) arr3[i][j] = (i == j) ? '#' : '.';
		}
		
		disp(arr3);
		disp(spiralize(arr3));
		
		Integer[][] arr4 = new Integer[7][7];
		for (int i = 0; i < 7; i++) {
		    for (int j = 0; j < 7; j++) arr4[i][j] = 7 * i + j;
		}
		
		disp(arr4);
		disp(spiralize(arr4));
	}
}
