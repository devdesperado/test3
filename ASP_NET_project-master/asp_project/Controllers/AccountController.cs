using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Http;
using Microsoft.AspNetCore.Identity;
using Microsoft.AspNetCore.Mvc;
using Microsoft.AspNetCore.Mvc.TagHelpers;
using WebApplication3.DataContext;
using WebApplication3.Models;
using WebApplication3.ViewModel;

// For more information on enabling MVC for empty projects, visit https://go.microsoft.com/fwlink/?LinkID=397860

namespace WebApplication3.Controllers
{
    public class AccountController : Controller
    {
        // GET: /<controller>/

        // 로그인
        public IActionResult Login()
        {
            return View();
        }

        [HttpPost]
        public IActionResult Login(LoginViewModel model)
        {
            if (ModelState.IsValid)
            {
                using (var db = new NoteDBContext())
                {
                    // Linq - method chaining
                    //var usre = db.Users
                    //    .FirstOrDefault(u=>u.UserID == model.UserID && u.UserPassword == model.UserPassword);
                    var user = db.Users
                        .FirstOrDefault(u => u.UserID.Equals(model.UserID) && 
                        u.UserPassword.Equals(model.UserPassword));

                    if(user != null)
                    {
                        // HttpContext.Session.SetInt32(key, value);
                        HttpContext.Session.SetInt32("USER_LOGIN_KEY", user.UserNo);



                        return RedirectToAction("LoginSuccess", "Home");
                        
                    }
                }
                ModelState.AddModelError(string.Empty, "ID 혹은 비밀번호가 올바르지 않습니다.");
            }
            return View(model);
        }
        // 회원가입


        public IActionResult Logout()
        {
            HttpContext.Session.Remove("USER_LOGIN_KEY");

            return RedirectToAction("Index", "Home");
        }
        public IActionResult Register()
        {
            return View();
        }

        [HttpPost]
        public IActionResult Register(User model)
        {
            if (ModelState.IsValid)
            {
                // Java try(sqlsession) {} catch() {}

                // C#
                using(var db = new NoteDBContext())
                {
                    db.Users.Add(model);
                    db.SaveChanges();
                }

                return RedirectToAction("Index", "Home");
            }
            return View();
        }
    }
}
