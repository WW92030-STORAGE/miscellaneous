import java.util.*;
import java.lang.reflect.*;

public class METHODTEST // Test of passing method as parameter
{
    public double function(Double x) {
        return Math.pow(Math.E, -1 * x * x);
    }

    public double integral(Object object, Method method, double a, double b, int steps) throws Exception { // Simpson's rule for numerical integration
        double res = 0;
        double dx = (b - a) / steps;
        
        for (int i = 0; i < steps; i++) {
            Object[] params = new Object[1];
            params[0] = a + dx * i;
            Double left = (Double)method.invoke(object, params);
            params[0] = a + dx * (i + 0.5);
            Double mid = (Double)method.invoke(object, params);
            params[0] = a + dx * (i + 1);
            Double right = (Double)method.invoke(object, params);
            
            res += (dx / 6) * (left + 4 * mid + right);
        }
        return res;
    }
    
	public static void main(String[] args) throws Exception {
	    Class[] p = new Class[1];
	    p[0] = Double.class;
		Method function = METHODTEST.class.getMethod("function", p[0]);
		METHODTEST main = new METHODTEST();
		double res = main.integral(main, function, -256, 256, 1<<16); // Should be sqrt(pi)
		System.out.println(res * res);
	}
}


