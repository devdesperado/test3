package algorithm;

import java.util.Scanner;


public class algorthm4 {
    public static void main(String[] args) {

        Scanner sc = new Scanner(System.in);

        int N = sc.nextInt();

        double max;

        double sum=0;

        double result = 0;

         // 입력받은 배열의 값을 저장

        for(int i=0; i<N; i++) {
            
            double[] arr = new double[N]
            arr[N] =sc.nextDouble();

            for(int j=1; j<N-1; j++){
                max = arr[0]; // max값 초기화
                if(arr[i] <arr[j])
                    max = arr[j]; // 입력받은 수에서 최대값을 구하는 알고리즘
                
            }


        }
        for(int k=0; k<arr.length; k++) {
            if (arr[k] = max) {
                return max;
            }

            else 
                arr[k] = arr[k] / max * 100; 
        } // 최대값 이외의 나머지 수를 정수 / M * 100으로 변환

        for(int l=0; i<arr.length; l++) {
            sum += arr[l];
            result = sum / N;
            return result;
        }

        System.out.print(result);    
    }    
}
