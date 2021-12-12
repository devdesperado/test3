using System;
using System.Web.Mvc;
using System.Linq;
using System.Collections.Generic;
using Microsoft.AspNetCore.Mvc;
using PagedList;
using WebApplication3.Models;
using WebApplication3.DataContext;

namespace WebApplication3.Controllers
{
    public class FoundController : Microsoft.AspNetCore.Mvc.Controller
    {

        GetDBtoAPI getDB = new GetDBtoAPI();

        // GET: /<controller>/
        public IActionResult Index()
        {
            getDB.GetSeoul(DateTime.Now);
            getDB.GetPolice(DateTime.Now, DateTime.Now, 100);

            List<string> cateList = new List<string>();
            using (NoteDBContext db = new NoteDBContext())
            {
                var dv = db.Found;
                foreach (var a in dv)
                    cateList.Add(a.Found_BigCate);
            }
            cateList = cateList.Distinct().ToList();
            List<SelectListItem> selectlist = new List<SelectListItem>();
            foreach (string s in cateList)
            {
                selectlist.Add(new SelectListItem
                {
                    Value = s,
                    Text = s
                });
            }
            SearchValue sv = new SearchValue();

            sv.cateList = selectlist;

            return View(sv);
        }

        public IActionResult Search(SearchValue model, int page = 1)
        {
            List<Found> founds = new List<Found>();
            using (NoteDBContext db = new NoteDBContext())
            {
                var dbList = db.Found;
                var searchList = from a in dbList
                                 where a.Found_DateTime >= model.Start_date
                                 where a.Found_DateTime <= model.End_date
                                 where a.Found_BigCate == model.Value
                                 select a;
                searchList = searchList.OrderByDescending(data => data.Found_DateTime);
                foreach (var found in searchList)
                {
                    founds.Add(new Found
                    {
                        Found_id = found.Found_id,
                        Found_data = found.Found_data,
                        Found_BigCate = found.Found_BigCate,
                        Found_DateTime = found.Found_DateTime,
                        Found_Description = found.Found_Description,
                        Found_GetPosition = found.Found_GetPosition,
                        Found_ImageURL = found.Found_ImageURL,
                        Found_Name = found.Found_Name,
                        Found_SmallCate = found.Found_SmallCate
                    });
                }
            }
            ViewBag.sDate = model.Start_date;
            ViewBag.eDate = model.End_date;
            ViewBag.cateValue = model.Value;
            
            return View(founds.ToPagedList(page, 10));
        }


        public IActionResult Detail(int found_id)
        {
            using (var db = new NoteDBContext())
            {
                var note = db.Found.FirstOrDefault(n => n.Found_id.Equals(found_id));

                return View(note);
            }
            return View();
        }
    }
}

