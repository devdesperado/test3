package javajungsuk;

class java1 {
    public static void main(String[] args) {
        String str = "3";

        System.out.println(str.charAt(0) - '0');
        System.out.println('3' - '0' + 1);
        System.out.println(Integer.parseInt("3") + 1);
        System.out.println("3" + 1); // 문자 + 숫자를 하게 되면 문자로 변환
        System.out.println(3 + '0'); // '0'은 숫자로 48
    }
}