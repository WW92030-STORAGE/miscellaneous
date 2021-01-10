import java.math.BigInteger;

public class nightmare { // BE WARNED THAT THIS TAKES A LOT OF TIME
	// b and e refer to two independent events
	
	// probabilities
	static int p_e = 20; // 20 / 423
	static int p_b = 1; // 1 / 2
	static int tot_e = 423;
	static int tot_b = 2;
	// total number of iterations
	static BigInteger n = BigInteger.ZERO; 
	// total number of successful rolls
	static BigInteger e_tot = BigInteger.ZERO; 
	static BigInteger b_tot = BigInteger.ZERO;
	// number of trials per iteration
	static int n_e = 262; 
	static int n_b = 305;
	// average over all iterations
	static double e_avg = 0.0; 
	static double b_avg = 0.0;
	// current iteration
	static int b = 0; 
	static int e = 0;
	// record high for b (this is one run)
	static int rbb = 0; 
	static int reb = 0;
	// record high for e
	static int rbe = 0; 
	static int ree = 0;
	// your goals
	static int de = 42; 
	static int db = 211;
	
	static BigInteger x(int n) {
		return BigInteger.valueOf(n);
	}
	
	static boolean iterate() {
		n = n.add(BigInteger.ONE);
		b = 0;
		e = 0;
		for (int i = 0; i < n_e; i++) {
			double rand = (int)(Math.random() * tot_e);
			if (rand < p_e) e++;
		}
		for (int i = 0; i < n_b; i++) {
			double rand = (int)(Math.random() * tot_b);
			if (rand < p_b) b++;
		}
		
		e_tot = e_tot.add(x(e));
		b_tot = b_tot.add(x(b));
		e_avg = e_tot.doubleValue() / n.doubleValue();
		b_avg = b_tot.doubleValue() / n.doubleValue();
		if (e > rbe) {
			rbe = b;
			ree = e;
		}
		if (b > rbb) {
			rbb = b;
			reb = e;
		}
		return (e >= de && b >= db);
	}
	
	static void print() {
		String s = "TRIALS : " + n + "\nTOTAL : " + e_tot + " | " + b_tot + "\nAVERAGE : " + e_avg + " | " + b_avg + 
				"\nRECORD B : " + reb + " | " + rbb + "\nRECORD E : " + ree + " | " + rbe;
		System.out.println(s);
	}
	
	public static void main(String[] args) {
		System.out.println("PATIENCE IS A VIRTUE\nEEEE | BBBB");
		while (true) {
			boolean finished = iterate();
			if (finished) break;
			if (n.remainder(x(1000000)).equals(x(0))) print();
		}
		print();
		System.out.println("GAME FINISHED! YOUR SCORE IS : " + n);
	}
}
