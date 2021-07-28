import java.util.ArrayList;

public class s10centralizer {
	static ArrayList<String> s10() {
		int n = 10;
		ArrayList<String> res = new ArrayList<String>();
		for (int a = 0; a < n; a++) {
			for (int b = 0; b < n; b++) {
				if (a == b) continue;
				for (int c = 0; c < n; c++) {
					if (a == c || b == c) continue;
					for (int d = 0; d < n; d++) {
						if (a == d || b == d || c == d) continue;
						for (int e = 0; e < n; e++) {
							if (a == e || b == e || c == e || d == e) continue;
							for (int f = 0; f < n; f++) {
								if (a == f || b == f || c == f || d == f || e == f) continue;
								for (int g = 0; g < n; g++) {
									if (a == g || b == g || c == g || d == g || e == g || f == g) continue;
									for (int h = 0; h < n; h++) {
										if (a == h || b == h || c == h || d == h || e == h || f == h || g == h) continue;
										for (int i = 0; i < n; i++) {
											if (a == i || b == i || c == i || d == i || e == i || f == i || g == i || h == i) continue;
											for (int j = 0; j < n; j++) {
												if (a == j || b == j || c == j || d == j || e == j || f == j || g == j || h == j || i == j) continue;
												String s = "" + a + b + c + d + e + f + g + h + i + j;
												s = s.substring(0, n);
												res.add(s);
												if (res.size() % 1000000 == 0) System.out.println(res.size());
											//	System.out.println(s);
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
		return res;
	}
	
	static String perm(String a, String p) {
		String s = "";
		for (int i = 0; i < p.length(); i++) {
			int n = p.charAt(i) - '0';
			s = "" + s + "" + a.charAt(n);
		}
		return s;
	}
	
	static String perm(String p) {
		if (p.length() > 10) return p;
		String s = "";
		for (int i = 0; i < p.length(); i++) s = s + "" + i;
		return perm(s, p);
	}
	
	public static void main(String[] args) {
		long start = System.nanoTime();
		ArrayList<String> perms = s10();
		System.out.println(perms.size());
		
		String p = "1204537896"; // REPLACE ME WITH A 10 DIGIT NUMBER OF DISTINCT DIGITS
		long count = 0;
		for (String s : perms) {
			if (perm(s, p).equals(perm(p, s))) {
				count++;
				if (count % 10 == 0) System.out.println(count);
				System.out.println(s + " " + p + " = " + perm(p, s) + " + " + perm(s, p));
			//	System.out.println(s);
			}
		}
		
		System.out.println("THE CENTRALIZER HAS ORDER " + count);
		
		System.out.println("ELAPSED " + (System.nanoTime() - start) / 1000000);
	}
}
