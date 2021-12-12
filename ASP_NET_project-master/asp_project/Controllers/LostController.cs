using System;
using System.Collections.Generic;
using System.Xml;
using Microsoft.AspNetCore.Mvc;
using PagedList;
using WebApplication3.Models;
using WebApplication3.DataContext;

// For more information on enabling MVC for empty projects, visit https://go.microsoft.com/fwlink/?LinkID=397860

namespace WebApplication3.Controllers
{
    public class LostController : Controller
    {
        public IActionResult Seoul_xmlToDB()
        {
            string url = "http://openapi.seoul.go.kr:8088/76676170526a7268313032484a736978/xml/lostArticleInfo/" +
                1 + "/" +
                1000 + "/";

            XmlDocument xdoc = new XmlDocument();

            xdoc.Load(url);
            XmlNodeList xnodeList = xdoc.DocumentElement.SelectNodes("/lostArticleInfo/row");

            foreach (XmlNode xnode in xnodeList)
            {
                if(xnode["STATUS"].InnerText != "수령")
                {
                    if(DateTime.Compare(Convert.ToDateTime("2020-01-01"),
                        Convert.ToDateTime(xnode["REG_DATE"].InnerText)) <= 0)
                    {
                        Lost_seoul lost_Seoul = new Lost_seoul();

                        lost_Seoul.Lost_seoul_DateTime = xnode["REG_DATE"].InnerText;
                        lost_Seoul.Lost_seoul_Cate = xnode["CATE"].InnerText;
                        lost_Seoul.Lost_seoul_Name = xnode["GET_NAME"].InnerText;
                        lost_Seoul.Lost_seoul_GetArea = xnode["GET_AREA"].InnerText;
                        lost_Seoul.Lost_Seoul_GetPosition = xnode["GET_POSITION"].InnerText;

                        using (var db = new NoteDBContext())
                        {
                            db.Lost_seoul.Add(lost_Seoul);
                            db.SaveChanges();
                        }
                    }
                    
                }
            }

            return View();
        }

        public IActionResult Index(int page = 1)
        {
            string url = "http://apis.data.go.kr/1320000/LostGoodsInfoInqireService/getLostGoodsInfoAccToClAreaPd?serviceKey=NWOxLHFs7YrxdPu2caTYxawT1IP1aY%2B2aL7UKm2IuwSuoUYaMLvPxScxP4CeFl%2Fs5vMLnVzKnjAVqqkqNVznMw%3D%3D" +
        "&START_YMD=20200601" +
        "&END_YMD=20200607" +
        "&LST_LCT_CD=LCA000" +
        "&pageNo=1" +
        "&numOfRows=100";
            XmlDocument xdoc = new XmlDocument();
            List<Lost> losts = new List<Lost>();
            xdoc.Load(url);
            XmlNodeList xnodeList = xdoc.DocumentElement.SelectNodes("/response/body/items/item");

            foreach (XmlNode xnode in xnodeList)
            {
                losts.Add(new Lost
                {
                    location = xnode["lstPlace"].InnerText,
                    goods = xnode["lstPrdtNm"].InnerText,
                    description = xnode["lstSbjt"].InnerText,
                    date = xnode["lstYmd"].InnerText,
                    
                });
            }
           
            list_sort(losts);
            return View(losts.ToPagedList(page, 5));
        }
        public static void list_sort(List<Lost> a)
        {
            string next;
            if (a.Count == 0) return;
            for (int i = 0; i < a.Count; i++)
            {
              a[i].date=  a[i].date.Replace("-","");
            }
            
            
            for (int i = 1; i < a.Count; i++)
            {
                next = a[i].date;
                int j = i;
                while (j > 0 && Int32.Parse(next) < Int32.Parse(a[j - 1].date))
                {
                    a[j].date = a[j - 1].date;
                    
                    j--;
                }
                a[j].date = next;
            }
            for (int i = 0; i < a.Count; i++)
            {
                a[i].date = a[i].date.Insert(4,"-");
                a[i].date = a[i].date.Insert(7, "-");
            }

            a.Reverse();
        }

    }
}
