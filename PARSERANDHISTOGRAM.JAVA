/*
INPUT FORMAT - Each row is of the form (NAME) - ID (Misc. Information)
The histogram and frequency table is constructed using the ID.
*/

import java.lang.*;
import java.util.*;

public class Main {
    public static void main(String[] args) {
        Scanner x = new Scanner(System.in);
        
        int n = 0;
        
        TreeMap<String, String> ids = new TreeMap<String, String>();
        TreeMap<String, Integer> freq = new TreeMap<String, Integer>();
        TreeMap<Integer, Integer> histogram = new TreeMap<Integer, Integer>();
        
        while (x.hasNextLine()) {
            String[] arr = x.nextLine().split(" ");
            
            int dash;
            for (dash = 0; dash < arr.length; dash++) {
                if (arr[dash].equals("-")) break;
            }
            if (dash >= arr.length) {
                System.out.println("ERROR - " + Arrays.toString(arr));
                continue;
            }
            
            String name = arr[0];
            for (int i = 1; i < dash; i++) name = name + " " + arr[i];
            String id = arr[dash + 1];
            
            if (!freq.containsKey(id)) {
                freq.put(id, 0);
                ids.put(id, name);
            }
            
            freq.put(id, freq.get(id) + 1);
            
            // System.out.println(name + " / " + dash + " = " + id + " " + arr[dash + 2]);
            
            n++;
        }
        
        System.out.println("TOTAL " + n);
        
        for (String s : freq.keySet()) {
            System.out.println(ids.get(s) + " = " + freq.get(s));
            if (!histogram.containsKey(freq.get(s))) histogram.put(freq.get(s), 0);
            histogram.put(freq.get(s), histogram.get(freq.get(s)) + 1);
        }
        
        System.out.println("TOTAL " + n);
        
        System.out.println(histogram);
    }
}
