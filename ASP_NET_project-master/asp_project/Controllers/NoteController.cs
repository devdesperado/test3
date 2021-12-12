using System;
using System.Linq;
using Microsoft.AspNetCore.Http;
using Microsoft.AspNetCore.Mvc;
using PagedList;
using WebApplication3.DataContext;
using WebApplication3.Models;



// For more information on enabling MVC for empty projects, visit https://go.microsoft.com/fwlink/?LinkID=397860

namespace WebApplication3.Controllers
{
    public class NoteController : Controller
    {


        /// <summary>
        ///  게시판 리스트
        /// </summary>
        /// <returns></returns>


        // GET: /<controller>/
        
        public IActionResult Index(int page = 1)
        {
            if (HttpContext.Session.GetInt32("USER_LOGIN_KEY") == null)
            {
                return RedirectToAction("Login", "Account");
            }

            using (var db = new NoteDBContext())
            {
                var list = db.Notes.ToList().OrderByDescending(p=>p.NoteNo);
                
                return View(list.ToPagedList(page, 5));
            }

            return View();
        }
        /// <summary>
        /// 게시판 상세
        /// </summary>
        /// <param name="noteNo"></param>
        /// <returns></returns>
        public IActionResult Detail(int noteNo)
        {
            if (HttpContext.Session.GetInt32("USER_LOGIN_KEY") == null)
            {
                return RedirectToAction("Login", "Account");
            }
            using (var db = new NoteDBContext())
            {
                var note = db.Notes.FirstOrDefault(n => n.NoteNo.Equals(noteNo));

                return View(note);
            }
            return View();
        }

        /// <summary>
        /// 게시물 추가
        /// </summary>
        /// <returns></returns>
        public IActionResult Add()
        {
            if (HttpContext.Session.GetInt32("USER_LOGIN_KEY") == null)
            {
                return RedirectToAction("Login", "Account");
            }


            return View();
        }

        [HttpPost]
        public IActionResult Add(Note model)
        {
            if (HttpContext.Session.GetInt32("USER_LOGIN_KEY") == null)
            {
                return RedirectToAction("Login", "Account");
            }
            model.UserNo = Int32.Parse(HttpContext.Session.GetInt32("USER_LOGIN_KEY").ToString());

            if (ModelState.IsValid)
            {
                using (var db = new NoteDBContext())
                {
                    db.Notes.Add(model);
                    if (db.SaveChanges() > 0)
                    {
                        db.SaveChanges();
                        return Redirect("Index");
                    }
                }
                ModelState.AddModelError(string.Empty, "게시물을 저장할 수 없습니다.");
            }

            return View(model);
        }


        /// <summary>
        ///  게시물 수정
        /// </summary>
        /// <returns></returns>
        public IActionResult Edit(int noteNo)
        {
            if (HttpContext.Session.GetInt32("USER_LOGIN_KEY") == null)
            {
                return RedirectToAction("Login", "Account");
            }
            using (var db = new NoteDBContext())
            {
                var note = db.Notes.Where(n => n.NoteNo.Equals(noteNo)).FirstOrDefault();
                return View(note);
            }

            return View(noteNo);
        }

        [HttpPost]
        public IActionResult Edit(Note model)
        {
            if (HttpContext.Session.GetInt32("USER_LOGIN_KEY") == null)
            {
                return RedirectToAction("Login", "Account");
            }
            model.UserNo = Int32.Parse(HttpContext.Session.GetInt32("USER_LOGIN_KEY").ToString());
            using (var db = new NoteDBContext())
            {
                
                db.Entry(model).CurrentValues.SetValues(model);
                db.Update(model);
                db.SaveChanges();
                return Redirect("Index");
            }
            ModelState.AddModelError(string.Empty, "게시물을 저장할 수 없습니다.");


            return View(model);
        }
        /// <summary>
        /// 게시물 삭제
        /// </summary>
        /// <returns></returns
        /// 


        public IActionResult Delete(Note model)
        {
            using (var db = new NoteDBContext())
            {
                var note = db.Notes.Where(n => n.NoteNo.Equals(model.NoteNo)).FirstOrDefault();
                return View(note);
            }
        }

        [HttpPost]

        public IActionResult Delete(int noteNo)
        {
            if (HttpContext.Session.GetInt32("USER_LOGIN_KEY") == null)
            {
                return RedirectToAction("Login", "Account");
            }

            using (var db = new NoteDBContext())
            {
                var note = db.Notes.FirstOrDefault(n => n.NoteNo.Equals(noteNo));

                db.Notes.Remove(note);
                
                db.SaveChanges();

                return Redirect("Index");

            }
            ModelState.AddModelError(string.Empty, "게시물을 삭제할 수 없습니다.");


            return View();
        }

    }
}

