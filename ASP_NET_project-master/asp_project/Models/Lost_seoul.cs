using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;

namespace WebApplication3.Models
{
    public class Lost_seoul
    {
        [Key]
        public int Lost_seoul_No { get; set; }

        public string Lost_seoul_DateTime { get; set; }
        public string Lost_seoul_Cate { get; set; }
        public string Lost_seoul_Name { get; set; }
        public string Lost_seoul_GetArea { get; set; }
        public string Lost_Seoul_GetPosition { get; set; }
    }
}