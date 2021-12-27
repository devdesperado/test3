<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<%@ page import="mun.seokhyun.data.PracticeInfo" %>
<%
    PracticeInfo info = (PracticeInfo) request.getAttribute("info");
    String err = (String)request.getAttribute("err");
%>
<html>
<head>
    <title>데이터 세부 사항</title>
</head>
<body>
<div style="text-align:center">
    <h2 style="text-align:center">상세보기</h2>
    <hr style="height:3; background-color:#02a1e9" />
    <br>

    <table style="margin:0 auto">
        <%
            if(info != null){
        %>
        <tr>
            <th style="text-align:center; width:150; height: 3; background-color: #02a1e9">ID</th>
            <td style=" width:300"><%= info.getId()%></td>
        </tr>
        <tr>
            <th style="text-align:center;width:60; height: 3; background-color: #02a1e9">name</th>
            <td style=" width:300"><%= info.getName()%></td>
        </tr>
        <tr>
            <th style="text-align:center; width:60; height: 3; background-color: #02a1e9">가격</th>
            <td style=" width:300"><%= info.getPrice()%></td>
        </tr>
        <tr>
            <th style="text-align:center; width:60; height: 3; background-color: #02a1e9">작성자</th>
            <td style=" width:300"><%= info.getAuthor()%></td>
        </tr>
        <tr>
            <th style="text-align:center; width:60; height: 5; background-color: #02a1e9">댓글</th>
            <td style=" width:300"><%= info.getComment()%></td>
        </tr>

        <%
          }
        %>
    </table>

    <br>
</div>
</body>
</html>
