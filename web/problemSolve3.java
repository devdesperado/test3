import java.util.Scanner;
import java.lang.Math;



public class problemSolve3 {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        float a = sc.nextFloat();
        float b = Math.abs(a);
        double c = Math.sqrt(a);
        final double PI = 3.14159;
        System.out.println("실수: " +a);
        System.out.println("절대값: " +b);
        System.out.println("제곱값: " +c);
        System.out.println("넓이: " +(a*a*PI));
    }
}
