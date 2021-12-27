package mun.seokhyun.data;

import javax.servlet.*;
import javax.servlet.http.*;
import javax.servlet.annotation.*;
import java.io.IOException;
import java.util.ArrayList;

@WebServlet(name = "AllServlet", value = "/AllServlet")
public class AllServlet extends HttpServlet {
    @Override
    protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        String err = "";

        try {
            ArrayList<PracticeInfo> list = new ArrayList<>();

            PracticeDao pdao = new PracticeDao();

            list = pdao.selectAll();

            request.setAttribute("list", list);
        } catch (IllegalStateException e) {
            err = "DB 연결에 오류가 발생했습니다." + e;
        } catch (Exception e) {
            err = "오류 발생: <br>" + e;
        } finally {
            request.setAttribute("err", err);
            request.getRequestDispatcher("/AllOk.jsp").forward(request, response);
        }
    }

    @Override
    protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {

    }
}
