package algorithm;

import java.util.Scanner;

public class algorithm3 {
    public static void main(String[] args) {

        Scanner sc = new Scanner(System.in);
        int[] arr = new int[10];
        int cnt = 0;
        boolean bl;
        for(int i=0; i<arr.length; i++){

            arr[i] = sc.nextInt() % 42;
        }
        
        
        for(int i=0; i<arr.length; i++) {
            bl = false;

            for(int j=i+1; j<arr.length; j++){
                if(arr[i]==arr[j]) {
                    bl =true;
                    break;
                }

            }
            if(bl == false) {
                cnt++;
            }
        }
        System.out.println(cnt);
       
    }
}
