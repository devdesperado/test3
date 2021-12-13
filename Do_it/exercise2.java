package Do_it;

public class exercise2 {
    static int min3(int a, int b, int c) {
        int min = a;
        if (b < min)
            min = b;
        if (c < min)
            min = c;
        
            
        return min;
    }

    public static void main(String[] args) {
        System.out.println("max4(3,4,2) = " + min3(3, 4, 2));
    }
}
