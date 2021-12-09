import java.util.Scanner;

public class problem1 {


    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);

        while(true) {
            int a = sc.nextInt();
            int b = sc.nextInt();

            System.out.println("x값: " +a);
            System.out.println("y값: " +b);
            System.out.println("x*y " +(a*b));
            System.out.println("x/y: " +(a/b));
        }
    }
}