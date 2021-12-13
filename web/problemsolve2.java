import java.util.Scanner;


public class problemsolve2 {

   

    public static int sub(int a, int b) {
        return a * b;
    }


    public static int remainder(int a, int b) {
        return a / b;
    }

  
    public static void main(String[] args) 
    {
        Scanner sc = new Scanner(System.in);
        int a = sc.nextInt();
        int b = sc.nextInt();
        int c = -1;
        try {
        System.out.println(sub(a, b));
        System.out.println(remainder(a, b));
        } catch(ArithmeticException e) {
            c = -1;
        }

    }
   
    
    
}
