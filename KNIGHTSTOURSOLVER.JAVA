import java.util.ArrayList;
import java.util.HashSet;
import java.util.Scanner;
 
public class KNIGHTSTOURSOLVER { // DISCLAIMER - This is an old CSES solution from 2020 (i.e. really really old) that is repurposed for hamiltonian circuit problem.
	static int a, b;
	static boolean good;
	static int width, height;
	static int[][] board;
	static int[][] deg;
	
	static int[] dx = {-1, 01, 02, 02, 01, -1, -2, -2};
	static int[] dy = {02, 02, 01, -1, -2, -2, -1, 01};
	
	public static boolean ok(int x, int y) {
		if (x < 0 || x >= width) return false;
		if (y < 0 || y >= height) return false;
		if (board[x][y] != 0) return false;
		return true;
	}
	
	public static boolean in(int x, int y) {
		if (x < 0 || x >= width) return false;
		if (y < 0 || y >= height) return false;
		return true;
	}
	
	
	public static void init() { // initialize the crap algorithm
		for (int i = 0; i < width; i++) {
			for (int j = 0; j < height; j++) {
				int count = 0;
				for (int k = 0; k < 8; k++) {
					int xp = i + dx[k];
					int yp = j + dy[k];
					if (in(xp, yp)) count++;
				}
				deg[i][j] = count;
			}
		}
	}
	
	public static void solve(int x, int y, int length) {
		if (good) return;
		board[x][y] = length;
		if (length >= width * height) {
		    boolean circuit = false;
		    for (int i = 0; i < 8; i++) {
		        int xp = x + dx[i];
			    int yp = y + dy[i];
			    if (!in(xp, yp)) continue;
			    if (xp == a && yp == b) circuit = true;
		    }
		    if (!circuit) return;
			printarr();
			good = true;
			return;
		}
		
		ArrayList<Integer> xc = new ArrayList<Integer>();
		ArrayList<Integer> yc = new ArrayList<Integer>();
		ArrayList<Integer> d = new ArrayList<Integer>();
		
		for (int i = 0; i < 8; i++) {
			int xp = x + dx[i];
			int yp = y + dy[i];
			if (ok(xp, yp)) {
				xc.add(xp);
				yc.add(yp);
				d.add(deg[xp][yp]);
			}
		}
		
		boolean sorted = false; // Bubble sort the things by degree (need to put in the triple class for this)
		while (!sorted) {
			sorted = true;
			for (int i = 0; i < d.size() - 1; i++) {
				if (d.get(i) > d.get(i + 1)) {
					sorted = false;
					
					int temp = d.get(i);
					d.set(i, d.get(i + 1));
					d.set(i + 1, temp);
					
					temp = xc.get(i);
					xc.set(i, xc.get(i + 1));
					xc.set(i + 1, temp);
					
					temp = yc.get(i);
					yc.set(i, yc.get(i + 1));
					yc.set(i + 1, temp);
				}
			}
		}
		
		
		
		for (int i = 0; i < xc.size(); i++) {
			int xp = xc.get(i);
			int yp = yc.get(i);
			if (ok(xp, yp)) {
				solve(xp, yp, length + 1);
				board[xp][yp] = 0;
			}
			
		}
		
		/*
		for (int i = 0; i < 8; i++) {
			int xp = x + dx[i];
			int yp = y + dy[i];
			if (ok(xp, yp)) {
				solve(xp, yp, length + 1);
				board[xp][yp] = 0;
			}
			
		}
		*/
	}
	
	public static void printarr() {
		for (int i = 0; i < width; i++) {
			for (int j = 0; j < height; j++) {
			    if (board[i][j] < 10) System.out.print("0");
			    System.out.print(board[i][j] + " ");
			}
			System.out.println();
		}
	}
	
	public static void main(String[] args) {
	    a = 0; // Starting positions [a][b]
	    b = 0;
	    width = 6; // Board size
	    height = 5;
	    board = new int[width][height];
	    deg = new int[width][height];
		init(); // Setup degree matrix for Warnsdorff's Rule
		good = false;
		solve(a, b, 1);
	}
}
