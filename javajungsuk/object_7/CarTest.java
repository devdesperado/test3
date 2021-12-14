package javajungsuk.object_7;

class Car{
    String color;
    String gearType;
    int door;

    Car() {}

    Car(String c, String g, int d) {
        color = c;
        gearType = g;
        door = d;
    }
}

class carTest  {
    public static void main(String[] args) {
        Car c1 = new Car();
        c1.color = "white";
        c1.gearType = "auto";
        c1.door = 4;

        Car c2 = new Car("White" , "auto", 4);

        System.out.println("C1과 color=" + c1.color + " , gearType="
        +c1.gearType + " , door = " +c1.door);

        System.out.println("C2와 color=" + c2.color + " , gearType="
        +c2.gearType + " , door = " +c2.door);
    }
}
