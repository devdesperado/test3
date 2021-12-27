<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<%
    Integer count = (Integer)request.getAttribute("count");
    String err = (String)request.getAttribute("err");
%>

<html>
<head>
    <title>데이터삭제 완료</title>
</head>
<body>
<div style="text-align:center">
    <h2 style="text-align:center">데이터 삭제</h2>
    <hr style="height:3; background-color:#02ale9" />
    <br>
    <%= err %>
    <% if(count != null){ %>
    <%= count%>개의 데이터가 삭제되었습니다.
    <% } %>
    <br>
    <br><br>
    <form action="<%=request.getContextPath()%>/index.jsp">
        <input type="submit" value="index">
    </form>
</div>
</body>
</html>