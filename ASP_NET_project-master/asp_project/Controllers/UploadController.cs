using System;
using System.IO;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Hosting;
using Microsoft.AspNetCore.Http;
using Microsoft.AspNetCore.Mvc;

// For more information on enabling MVC for empty projects, visit https://go.microsoft.com/fwlink/?LinkID=397860

namespace WebApplication3.Controllers
{


    public class UploadController : Controller
    {
        IWebHostEnvironment _environment;

        public UploadController(IWebHostEnvironment environment)
        {
            _environment = environment;
        }

        [HttpPost, Route("api/upload")]
        public async Task<IActionResult> ImageUpload(IFormFile file)    
        {
            // # 이미지나 파일을 업로드할 때 필요한 구성
            // 1. Path (저장할 위치)
            
            var path = Path.Combine(_environment.WebRootPath, "upload");
            
            // 2. Name (파일 이름) - DateTime + GUID(전역 고유 식별자) 
            // 3. Extension (확장자) 
            var fileFullName = file.FileName.Split('.');
            var fileName = $"{Guid.NewGuid()}.{fileFullName[1]}";
            using (var filestream = new FileStream(Path.Combine(path, fileName), FileMode.Create))
            {
                await file.CopyToAsync(filestream);
            }
            return Ok(new { file = "/upload/" + fileName, success = true });
        }
    }
}
