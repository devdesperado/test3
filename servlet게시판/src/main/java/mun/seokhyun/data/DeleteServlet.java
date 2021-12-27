package mun.seokhyun.data;

import javax.servlet.*;
import javax.servlet.http.*;
import javax.servlet.annotation.*;
import java.io.IOException;

@WebServlet(name = "DeleteServlet", value = "/DeleteServlet")
public class DeleteServlet extends HttpServlet {
    @Override
    protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        String err= "";

        try {
            request.setCharacterEncoding("UTF-8");
            String id = request.getParameter("id");

            AccountDao5 adao5 = new AccountDao5();
            int count = adao5.delete(id);

            request.setAttribute("count", count);
        } catch (IllegalStateException e) {
            err = "DB 연결 오류: <br>" + e;
        } catch (Exception e) {
            err = "알수 없는 오류: <br>" +e;
        } finally {
            request.setAttribute("err", err);

            request.getRequestDispatcher("/deleteOk.jsp").forward(request, response);

        }
    }

    @Override
    protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {

    }
}
