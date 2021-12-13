package id;

import java.sql.Date;

import id.DateId;

public class DateTester {
    public static void main(String[] args) {
        DateId a = new DateId();
        DateId b = new DateId();
        DateId c = new DateId();

        System.out.println("a의 식별번호: " +a.getid());
        System.out.println("b의 식별번호: " +b.getid());
        System.out.println("c의 식별번호: " +c.getid());


    }
}
