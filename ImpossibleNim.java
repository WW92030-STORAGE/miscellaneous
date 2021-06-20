import java.util.ArrayList;
import java.util.Scanner;

public class ImpossibleNim {
	public static void error() {
		System.out.println("YOU IDIOT");
	}
	
	static final int MAX = 32;
	static int n;
	static boolean p1 = false;
	static ArrayList<Integer> arr = new ArrayList<Integer>();
	
	public static int xor(ArrayList<Integer> arr) {
		int sum = 0;
		for (int i : arr) sum = sum ^ i;
		return sum;
	}
	
	public static void game() {
		Scanner sc = new Scanner(System.in);
		int x = 0;
		int y = 0;
		
		System.out.println("X Y = REMOVE Y FROM PILE X");
		
		while (arr.size() > 0) {
			System.out.println(arr);
			if (p1) {
				while (true) {
					try {
						String[] stuff = sc.nextLine().split(" ");
						x = Integer.parseInt(stuff[0]);
						y = Integer.parseInt(stuff[1]);
						if (x < 0 || x >= arr.size() || y <= 0) error();
						else break;
					}
					catch (Exception e) {
						error();
						continue;
					}
				}
				System.out.println("REMOVED " + Math.min(y, arr.get(x)) + " FROM PILE " + x);
				arr.set(x, arr.get(x) - y);
				if (arr.get(x) <= 0) arr.remove(x);
			}
			else {
				boolean found = false;
				for (x = 0; x < arr.size(); x++) {
					int num = arr.get(x);
					for (y = 1; y <= num; y++) {
						arr.set(x, num - y);
						int sum = xor(arr);
						if (sum == 0) {
							found = true;
							arr.set(x, num);
							break;
						}
					}
					arr.set(x, num);
					if (found) break;
				}
				System.out.println(x + " " + y);
				if (found) {
					System.out.println("COMP'ED " + Math.min(y, arr.get(x)) + " FROM PILE " + x);
					arr.set(x, arr.get(x) - y);
					if (arr.get(x) <= 0) arr.remove(x);
				}
				else {
					x = (int)(Math.random() * arr.size());
					y = (int)(Math.random() * (arr.get(x) - 1)) + 1;
					System.out.println("COMP'ED " + Math.min(y, arr.get(x)) + " FROM PILE " + x);
					arr.set(x, arr.get(x) - y);
					if (arr.get(x) <= 0) arr.remove(x);
				}
			}
			p1 = !p1;
		}
		
		// Player is swapped after each move
		if (!p1) System.out.println("YOU WIN");
		else System.out.println("YOU LOSE");
	}
	
	public static void main(String[] args) {
		Scanner x = new Scanner(System.in);
		
		System.out.println("16 ROWS MAX");
		System.out.println("THE VICTOR EMPTIES THE STASH");
		
		while (x.hasNextInt()) {
			try {
				n = x.nextInt();
				if (n <= 0 || n > 16) {
					error();
					continue;
				}
				arr.clear();
				for (int i = 0; i < n; i++) arr.add(1 + (int)(Math.random() * (MAX - 1)));
				System.out.println(arr);
				System.out.println("FIRST MOVE - 0 HUMAN 1 COMPUTER");
				p1 = (x.nextInt() == 0);
				game();
			}
			catch (Exception e) {
				error();
				System.out.println(e);
				break;
			}
		}
	}
}
