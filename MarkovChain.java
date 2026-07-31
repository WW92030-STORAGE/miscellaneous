import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Scanner;
import java.util.TreeMap;
import java.util.TreeSet;

public class MarkovChain{
	
	static int n;
	static int perline = 20;
	
	// Each set of characters bounded by whitespace is treated as a candidate. This includes punctuation excluding hyphens.
	// To "comment" out a line make sure to surround it by hyphens.
	// -Like this one-
	
	static ArrayList<String> data = new ArrayList<String>();
	static TreeMap<String, ArrayList<String>> prob = new TreeMap<String, ArrayList<String>>();
	
	static ArrayList<String> primers = new ArrayList<String>();
	
	static void push(String a, String b) { 
		if (!prob.containsKey(a)) prob.put(a, new ArrayList<String>());
		prob.get(a).add(b);
	}
	
	static ArrayList<String> run(String s, int n) {
		ArrayList<String> res = new ArrayList<String>();
		res.add(s);
		for (int i = 1; i < n; i++) {
			String now = res.get(res.size() - 1);
			int samplesize = prob.get(now).size();
			int choose = (int)(Math.random() * samplesize);
			res.add(prob.get(now).get(choose));
		}
		return res;
	}
	
	static String num(int n) {
		if (n == 21) return "9 + 10";
		if (n == 4) return n + " (minus 1 that's 3 quick maths)";
		if (n == 69) return n + " (nice)";
		return "" + n;
	}
	
	public static void main (String[] args) throws FileNotFoundException, IOException {
		Scanner fileReader = new Scanner(new File("Thing.in"));

		long start = System.nanoTime();
		
		data.clear();
		
		while (fileReader.hasNext()) {
			String line = fileReader.nextLine();
			if (line.length() == 0) continue;
			if (line.charAt(0) == '-' && line.charAt(line.length() - 1) == '-') continue;
		//	System.out.println(line);
			
			String[] s = line.split(" ");
		//	System.out.println(Arrays.toString(s));
			for (String h : s) data.add(h);
		}
		
		for (int i = 0; i < data.size() - 1; i++) {
			String s1 = data.get(i);
			String s2 = data.get(i + 1);
			push(s1, s2);
		}
		
		push(data.get(data.size() - 1), data.get(0)); // failsafe
		
		fileReader.close();
		
		TreeSet<String> set = new TreeSet<String>();
		for (String s : data) {
			char c = s.charAt(0);
			if (c >= 'A' && c <= 'Z') set.add(s);
		}
		for (String s : set) primers.add(s);
		
		System.out.println("ELAPSED " + (System.nanoTime() - start) / 1000000);
		System.out.println(data.size() + " STRINGS FOUND");
		System.out.println("1 - " + Short.MAX_VALUE);
		
		Scanner x = new Scanner(System.in);
		while (x.hasNextShort()) {
			int n = x.nextShort();
			if (n <= 0) {
				System.out.println("You really are an IDIOT");
				continue;
			}
			int p = (int)(Math.random() * primers.size());
			String primer = primers.get(p);
			System.out.println("MARKOV CHAIN USING " + num(n) + " TERMS AND STARTING WITH " + primer);
			ArrayList<String> res = run(primer, n);
			for (int i = 0; i < res.size(); i++) {
				String s = res.get(i);
				System.out.print(s + " " );
				if (i % perline == 0 && i != 0) System.out.print("\n");
			}
		}
	}
}
