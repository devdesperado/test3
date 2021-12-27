package mun.seokhyun.data;

import java.sql.*;
import java.util.*;

public class PracticeDao {
    private static String RDB_DRIVE = "com.mysql.cj.jdbc.Driver";
    private static String URL = "jdbc:mysql://desperado-db.cky2c0usf9no.ap-northeast-2.rds.amazonaws.com:3306/test";
    private static String USER = "admin";
    private static String PASS = "23786080";

    public static Connection getConnection() {
        try {
            Class.forName(RDB_DRIVE);
            Connection con = DriverManager.getConnection(URL, USER, PASS);

            return con;
        } catch (Exception e) {
            throw new IllegalStateException(e);
        }
    }

    public ArrayList<PracticeInfo> selectAll() {
        Connection con = null;
        Statement smt = null;

        ArrayList<PracticeInfo> list = new ArrayList<PracticeInfo>();

        String sql = "SELECT * FROM practice_tb";

        try {
            con = getConnection();
            smt = con.createStatement();
            ResultSet rs = smt.executeQuery(sql);
            while (rs.next()) {
                PracticeInfo pi = new PracticeInfo();
                pi.setId(rs.getString("id"));
                pi.setName(rs.getString("name"));
                pi.setPrice(rs.getString("price"));
                pi.setAuthor(rs.getString("author"));
                pi.setComment(rs.getString("comment"));
                list.add(pi);
            }
        } catch (Exception e) {
            throw new IllegalStateException(e);
        } finally {
            // 사용한 리소스 폐기
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
        return list;
    }
    public PracticeInfo selectById(String id) {
        Connection con = null;
        Statement smt = null;

        PracticeInfo pinfo = new PracticeInfo();

        String sql = "SELECT * FROM practice_tb WHERE id = '"+ id +"'";


        try {
            con = getConnection();
            smt = con.createStatement();

            ResultSet rs = smt.executeQuery(sql);

            while (rs.next()) {
                pinfo.setId(rs.getString("id"));
                pinfo.setName(rs.getString("name"));
                pinfo.setPrice(rs.getString("price"));
                pinfo.setAuthor(rs.getString("author"));
                pinfo.setComment(rs.getString("comment"));
            }
        } catch (Exception e) {
            throw new IllegalStateException(e);
        } finally {
            // 사용한 리소스 폐기
            if(smt != null) {
                try {
                    smt.close();
                } catch (SQLException ignore){}
            }
            if(con != null) {
                try {
                    con.close();
                } catch (SQLException ignore){}
            }
        }
        return pinfo;
        }

    }



