using System;
using System.ComponentModel.DataAnnotations;
using System.Web.Mvc;
using System.ComponentModel.DataAnnotations.Schema;
using System.Linq;
using System.Collections.Generic;

namespace WebApplication3.Models
{
    public class Found
    {
        [Key]
        public int Found_id { get; set; }           //id
        public string Found_data { get; set; }      //서울시 or 경찰청
        public DateTime Found_DateTime { get; set; }  //습득 날짜
        public string Found_BigCate { get; set; }   //대분류
        public string Found_SmallCate { get; set; } //소분류 
        public string Found_Name { get; set; }      //이름
        public string Found_GetPosition { get; set; }//보관장소
        public string Found_ImageURL { get; set; }   //이미지URL   
        public string Found_Description { get; set; } //설명
    }

    public class SearchValue
    {
        public List<SelectListItem> cateList { get; set; } //DB에서 분류만 가져옴
        public DateTime Start_date { get; set; }           //검색 시작 날짜
        public DateTime End_date { get; set; }             //검색 끝  날짜
        public string Value { get; set; }                  //분류 선택 값
        //public string name { get; set; }                
    }

}