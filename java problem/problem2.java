import java.util.InputMismatchException;
import java.util.Scanner;


public class problem2 {
    
    static int mul(int x, int y){
        return x*y;
    }

    static int div(int x, int y) {
        return x/y;
    }

    static void muldiv(int x, int y) {
        System.out.println("x + y = " + mul(x, y) );
        System.out.println("x / y = " + div(x, y) );
    }

    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        try {
           System.out.print("x값: " ); int x = sc.nextInt();
           System.out.print("y값: "); int y = sc.nextInt();
           muldiv( x,  y);

        } catch(InputMismatchException e) {
            System.out.println("입력 오류 발생: " +e);
        } catch(ArithmeticException e) {
            System.out.println("입력 오류 발생: " +e);
            e.printStackTrace();
        } finally {
            System.out.println("프로그램 종료");
        }
    }



    } 

