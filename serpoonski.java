import java.awt.Canvas;
import java.awt.Color;
import java.awt.Graphics;
import java.util.TreeSet;

import javax.swing.JFrame;

public class serpoonski extends JFrame {
	static class pair implements Comparable<pair>{
		private int x;
		private int y;
		public pair(int a, int b) {
			x = a;
			y = b;
		}
		
		public void set(pair p) {
			x = p.x;
			y = p.y;
		}
		
		public pair clone() {
			return new pair(x + 0, y + 0);
		}
		
		public int compareTo(pair other) {
			Integer px = this.x;
			Integer py = this.y;
			Integer ox = other.x;
			Integer oy = other.y;
			if (!px.equals(ox)) return px.compareTo(ox);
			if (!py.equals(oy)) return py.compareTo(oy);
			return 0;
		}
		
		public boolean equals(pair other) {
			return this.compareTo(other) == 0;
		}
		
		public String toString() {
			return "[" + x + " " + y + "]";
		}
	}
	
	@SuppressWarnings("deprecation")
	public serpoonski (TreeSet<pair> points) throws Exception {
		int margins = 100;
		int xmax = -1;
		int ymax = -1;
		Canvas c = new Canvas () {
			public void paint(Graphics g) {
				g.setColor(Color.BLACK);
				for (pair p : points) {
					g.drawLine(p.x + margins, p.y + margins, p.x + margins, p.y + margins);
				}
			}
		}; 
		
		for (pair p : points) {
			xmax = Math.max(p.x, xmax);
			ymax = Math.max(p.y, ymax);
		}
		
		c.setBackground(Color.WHITE);
		setSize(xmax + 2 * margins, ymax + 2 * margins);
		  
        add(c);  
		show();
	}
	
	static TreeSet<pair> gen(pair[] vertices, int n, double p, boolean diff, int diffset) {
		int x = vertices[0].x;
		int y = vertices[0].y;
		int length = vertices.length;
		int prev = 0;
		int corner = 0;
		double q = 1 - p;
		
		TreeSet<pair> res = new TreeSet<pair>();
		for (pair point : vertices) res.add(point.clone());
		for (int i = 0; i < n; i++) {
			res.add(new pair(x, y));
			prev = corner;
			corner = (int)(Math.random() * length);
			if (diff) {
				while ((corner - prev + 10 * length) % length == diffset) corner = (int)(Math.random() * length);
			}
			x = (int)Math.round(p * x + q * vertices[corner].x);
			y = (int)Math.round(p * y + q * vertices[corner].y);
		}
			
		return res;
	}
	
	static TreeSet<pair> gen(pair[] vertices, int n, double p) {
		return gen(vertices, n, p, false, 0);
	}
	
	static pair x(int x, int y) {
		return new pair(x, y);
	}
	
	public static void main(String[] args) {
		System.out.println("FRACTALS");
		int n = 600;
		pair[] triangle = {x(0, 0), x(n, 0), x(n / 2, n)};
		pair[] square = {x(0, 0), x(0, n), x(n, n), x(n, 0)};
		pair[] midsquare = {x(0, 0), x(0, n), x(n, n), x(n, 0), x(0, n / 2), x(n / 2, 0), x(n, n / 2), x(n / 2, n)};
		
		try {
			TreeSet list = gen(midsquare, 1000000, 1.0 / 3);
			serpoonski res = new serpoonski(list);
		}
		catch (Exception e) {
			System.out.println(e);
		}
	}
}
