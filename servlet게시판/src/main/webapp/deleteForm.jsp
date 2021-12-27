<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<html>
<head>
    <title>등록내용 삭제 입력양식</title>
</head>
<body>
<div style="text-align:center">
    <h2 style="text-align:center">등록내용 삭제 입력양식</h2>
    <hr style="height:3; background-color: #02a1e9" />
    <br>
    <form action="<%=request.getContextPath() %>/DeleteServlet">
        <table style="margin:0 auto"></table>
        <tr>
            <td style="width:60">삭제할 ID</td>
            <td ><input type="text" size="30" name="id"></td>
        </tr>

        <tr>
            <td colspan=2 style="text-align:center">
                <input type="submit" value="삭제">
            </td>
        </tr>
        </table>
    </form>
    <br>
</div>

</body>
</html>
