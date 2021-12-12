package javajungsuk.object_7;

public class Tvtest4 {
    public static void main(String args[]) {
        Tv[] tvarr = new Tv[3];

        for(int i=0; i < tvarr.length; i++) {
            tvarr[i] = new Tv();
            tvarr[i].channel = i+10;
            
        }
        for(int i=0; i < tvarr.length; i++) {
            tvarr[i].channelUp();
            System.out.printf("tvarr[%d].channel=%d%n", i, tvarr[i].channel);

        }
 
    }
}

class Tv {
    String color;
    boolean power;
    int channel;
    void power() { power = !power; }
    void channelUp() { ++channel; }
    void channelDown() { --channel; }

}