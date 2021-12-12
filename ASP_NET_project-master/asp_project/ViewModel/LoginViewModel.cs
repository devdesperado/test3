using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.Linq;
using System.Threading.Tasks;

namespace WebApplication3.ViewModel
{
    public class LoginViewModel
    {
        [Required(ErrorMessage = "아이디를 입력하세요.")]
        public string UserID { get; set; }
        [Required(ErrorMessage ="비밀번호를 입력하세요.")]
        public string UserPassword{ get; set; }
    }
}
