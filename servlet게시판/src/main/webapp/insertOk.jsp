<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<%
    Integer count = (Integer) request.getAttribute("count");
    String err = (String)request.getAttribute("err");
%>
<html>
<head>
    <title>데이터입력 완료</title>
</head>
<body>
<div style="text-align:center">
    <h2 style="text-align:center">데이터 등록</h2>
    <hr style="height:3; background-color:#02a1e9" />
    <br>
    <%= err %>
    <% if(count != null){ %>
    <%= count%>개의 데이터를 등록하였습니다.
    <% } %>
    <br>
    <br><br>
    <form action="<%=request.getContextPath()%>/searchForm.jsp">
        <input type="submit" value="검색페이지로">
    </form>
</div>
</body>
</html>
</html>
