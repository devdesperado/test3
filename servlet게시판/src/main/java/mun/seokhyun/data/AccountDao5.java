package mun.seokhyun.data;

import java.sql.*;

public class AccountDao5 {

    private static String RDB_DRIVE = "com.mysql.cj.jdbc.Driver";
    private static String URL = "jdbc:mysql://desperado-db.cky2c0usf9no.ap-northeast-2.rds.amazonaws.com:3306/test";
    private static String USER = "admin";
    private static String PASS = "23786080";

    public static Connection getConnection() {
        try {
            Class.forName(RDB_DRIVE);
            Connection con = DriverManager.getConnection(URL, USER,
                    PASS);
            return con;
        } catch (Exception e) {
            throw new IllegalStateException(e);
        }
    }

    public int delete(String id) {
        Connection con = null;
        Statement smt = null;

        int count = 0;

        String sql = "DELETE FROM account WHERE id='"+ id +"'";
        try {
            con = getConnection();
            smt = con.createStatement();
            count = smt.executeUpdate(sql);
        } catch (Exception e) {
            throw new IllegalStateException(e);
        } finally {
            if (smt != null) {
                try {
                    smt.close();
                } catch (SQLException ignore) {
                }
            }
            if (con != null) {
                try {
                    con.close();
                } catch (SQLException ignore) {
                }
            }

        }
        return count;
    }

}
