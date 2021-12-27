<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<html>
<head>
    <title>등록 입력양식</title>
</head>
<body>
<div style="text-align:center">
    <h2 style="text-align:center">데이터 등록 입력양식</h2>
    <hr style="height:3; background-color:#02a1e9" />
    <br>
    <form action="<%=request.getContextPath() %>/InsertServlet">
        <table style="margin:0 auto">
            <tr>
                <td style="width:60">ID</td>
                <td ><input type="text" size="30" name="id"></td>
            </tr>
            <tr>
                <td style="width:60">이름</td>
                <td ><input type="text" size="30" name="name"></td>
            </tr>
            <tr>
                <td style="width:70">가격</td>
                <td ><input type="text" size="30" name="price"></td>
            </tr>
            <tr>
                <td style="width:70">작성자</td>
                <td ><input type="text" size="30" name="comment"></td>
            </tr>
            <tr>
                <td style="width:70">댓글</td>
                <td ><input type="text" size="30" name="comment"></td>
            </tr>
        </table>
    </form>
    <br>
</div>
</body>
</html>
