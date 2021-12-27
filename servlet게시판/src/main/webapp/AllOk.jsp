<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<%@page import="mun.seokhyun.data.PracticeInfo"%>
<%@ page import="java.util.ArrayList" %>
<%
    ArrayList<PracticeInfo> list = (ArrayList<PracticeInfo>) request.getAttribute("list");
    String err = (String)request.getAttribute("err");

%>
<html>
<head>
    <title>데이터 전부 표시 후 1개 선택</title>
</head>
<body>
<div style="text-align:center">
    <h2 style="text-align:center">모든 데이터 표시 </h2>
    <hr style="height:3; background-color:#02a1e9" />
    <br>
    <h3> 검색결과 </h3>
    <%=err %>

    <table style="margin:0 auto">
        <tr>
        <tr>
            <th style="background-color:#02a1e9; width:100">ID</th>
            <th style="background-color:#02a1e9; width:200">이름</th>

        </tr>
        <%
            if(list != null) {
                for(int i=0; i<list.size(); i++){

        %>
        <tr>
            <td style="text-align:center; width:100"><a
                    href="<%=request.getContextPath()%>/OneServlet?id=<%=list.get(i).getId() %>">
                <%= list.get(i).getId() %></td>

            <td style="text-align:center; width:200"><%= list.get(i).getName()%></td>
        </tr>
            <%
                }
            }
            %>
    </table>
    <br>


</div>
</body>
</html>
