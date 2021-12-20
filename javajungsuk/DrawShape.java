package javajungsuk;

public class DrawShape {
    public static void main(String[] args) {
        Point[] P = {  new Point(100, 100),
                       new Point(140, 50),
                       new Point(200, 100)

        };
        Triangle t = new Triangle(P);
        Circle c = new Circle(new Point(150, 150), 50 );
        t.draw();
        c.draw();
    }
}

class Shape {
    String color = "black";
    void draw() {
        System.out.printf("[color=%s]%n" , color);
    }
}

class Point {
    int x;
    int y;

    Point(int x, int y) {
        this.x = x;
        this.y = y;
    }

    Point() {
        this(0,0);
    }

    String getXY() {
        return "("+x+", "+y+")";
    }
}

class Cirlce extends Shape {
    Point center;
    int r;
    
    void Circle() {
        this(new Point(0, 0), 100);
    }

    void Circle(Point center, int r) {
        this.center = center;
        this.r = r;
    }

    void draw() {
        System.out.printf("[center=(%d, %d), r=%d, color=%s]%n",
        center.x, center.y, r, color);
    }
}

class Trinangle extends Shape {
    Point[] P = new Point[3];
    
    void Triangle(Point[] P) {
        this.P = P;
    }

    void draw() {
        System.out.printf("[p1=%s, p2=%s, p3=%s, color=%s%n",
                            P[0].getXY(),P[1].getXY(), P[2].getXY(),color)
    }
}