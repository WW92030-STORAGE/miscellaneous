import java.awt.Canvas;
import java.awt.Color;
import java.awt.Graphics;
import java.util.TreeMap;

import javax.swing.JFrame;

public class Mandelbrot extends JFrame{
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
	static int width = 1000;
	static int height = 1000;
	@SuppressWarnings("deprecation")
	public MANDELBROT (TreeMap<pair, Float> points, int iterations) throws Exception {
		int margins = 100;
		Canvas c = new Canvas () {
			public void paint(Graphics g) {
				for (pair p : points.keySet()) {
					float col = 1 - points.get(p);
					Color c = new Color(col, col, col);
					g.setColor(c);
					g.drawLine(p.x, p.y, p.x , p.y);
				}
			}
		}; 
		
		c.setBackground(Color.WHITE);
		setSize(width, height);
		  
        add(c);  
		show();
	}
	
	public MANDELBROT (TreeMap<pair, Float> points, int iterations, double x0, double y0, int zoom) throws Exception {
		int margins = 100;
		Canvas c = new Canvas () {
			public void paint(Graphics g) {
				for (pair p : points.keySet()) {
					float col = 1 - points.get(p);
					Color c = new Color(col, col, col);
					g.setColor(c);
					g.drawLine(p.x, p.y, p.x , p.y);
				}
				
				g.setColor(Color.RED);
				double px, py;
				double x = x0;
				double y = y0;
				for (int i = 0; i < iterations; i++) {
					px = x;
					py = y;
					x = px * px - py * py + x0;
					y = 2 * px * py + y0;
					g.drawLine(xn(px, zoom), yn(py, zoom), xn(x, zoom), yn(y, zoom));
				}
			}
		}; 
		
		c.setBackground(Color.WHITE);
		setSize(width, height);
		  
        add(c);  
		show();
	}
	
	static int xn(double x, double zoom) {
		return (int)(x * zoom + width / 2);
	}
	
	static int yn(double y, double zoom) {
		return (int)(y * zoom + height / 2);
	}
	
	static double nx(int x, double zoom) {
		return (x - width / 2) / zoom;
	}
	
	static double ny(int y, double zoom) {
		return (y - height / 2) / zoom;
	}
	
	static int calc(double x0, double y0, int max) {
		double x = 0;
		double y = 0;
		double px = 0;
		double py = 0;
		for (int i = 0; i <= max; i++) {
			if (x * x + y * y >= 4) return i;
			px = x;
			py = y;
			x = px * px - py * py + x0;
			y = 2 * px * py + y0;
		}
		return max;
	}
	
	static String x(double d) {
		String s = "" + d;
		if (s.length() > 10) s = s.substring(0, 10);
		while (s.length() < 10) s = s + "0";
		return s;
	}
	
	static TreeMap<pair, Float> gen(int zoom, int iterations, double ox, double oy) {
		TreeMap<pair, Float> res = new TreeMap<pair, Float>();
		for (int i = 0; i < width; i++) {
			if (i % 200 == 0) System.out.println(i + " / " + width);
			for (int j = 0; j < height; j++) {
				double nx = nx(i, zoom) + ox;
				double ny = ny(j, zoom) + oy;
			//	System.out.println(x(nx) + " " + x(ny));
				int num = calc(nx, ny, iterations);
				res.put(new pair(i, j), (float)num / iterations);
			}
		}
		
		return res;
	}
	
	static pair x(int x, int y) {
		return new pair(x, y);
	}
	
	public static void main(String[] args) {
		System.out.println("MANDELBROT SET");
		width = height = 1000;
		int zoom = 240;
		int max = 256;
		
		double x0 = 0;
		double y0 = 0;
		
		long start = System.nanoTime();
		try {
			TreeMap list = gen(zoom, max, 0.0, 0.0);
			MANDELBROT res = new MANDELBROT(list, max, x0, y0, zoom);
		}
		catch (Exception e) {
			System.out.println(e);
		}
		System.out.println("ELAPSED " + (System.nanoTime() - start) / 1000000);
	}
}
