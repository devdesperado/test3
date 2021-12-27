package mun.seokhyun.data;

import javax.servlet.*;
import javax.servlet.http.*;
import javax.servlet.annotation.*;
import java.io.IOException;


@WebServlet(name = "OneServlet", value = "/OneServlet")
public class OneServlet extends HttpServlet {
    @Override
    protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        String err = "";

        try {
            String id = request.getParameter("id");

            PracticeInfo pinfo = new PracticeInfo();

            PracticeDao pdao3 = new PracticeDao();

            pinfo = pdao3.selectById(id);

            request.setAttribute("info", pinfo);
        } catch (IllegalStateException e) {
            err = "DB 연결 오류: <br>" + e;
        } catch (Exception e) {
            err = "알수없는 오류: <br>" + e;
        } finally {
            request.setAttribute("err", err);
            request.getRequestDispatcher("/detail.jsp").forward(request, response);
        }
    }


    @Override
    protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {

    }
}
