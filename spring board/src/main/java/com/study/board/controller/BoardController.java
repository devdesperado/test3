package com.study.board.controller;


import com.study.board.Repository.BoardRepository;
import com.study.board.entity.Board;
import com.study.board.service.BoardService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.stereotype.Repository;
import org.springframework.ui.Model;
import org.springframework.validation.annotation.Validated;
import org.springframework.web.bind.annotation.*;
import org.springframework.web.servlet.ModelAndView;
import com.study.board.form.InsertForm;

@Controller
public class BoardController {

    @Autowired
    private BoardService boardService;

    @Autowired
    private BoardRepository repository;


    @GetMapping("/board/write") //localhost:8080/board/write 페이지로 접속하면 boardwrite를 보여주겠다
    public String boardWriteForm() {

        return "boardwrite";
    }

    @PostMapping("/board/writepro")
    public String boardWritePro(Board board) {

        boardService.write(board);

        return "";
    }

    @GetMapping("/board/list")
    public String BoardList(Model model) {

        model.addAttribute("list", boardService.boardList());
        return "boardlist";
    }

    @GetMapping("/board/view") //localhost:8080/board/view?id=1
    public String boardView(Model model, Integer num) {

        model.addAttribute("board", boardService.boardView(num));

        return "boardview";
    }

    @GetMapping("/board/delete")
    public String boardDelete(Integer num) {
        boardService.boardDelete(num);

        return "redirect:/board/list";

    }


    @GetMapping("/board/modify/{num}")
    public String boardModify(@PathVariable("num") Integer num, Model model){

        model.addAttribute("board", boardService.boardView(num));

        return "boardmodify";
    }

    @PostMapping("/board/update/{num}")
    public String boardUpdate(@PathVariable("num") Integer num, Board board) {

        Board boardTemp = boardService.boardView(num);
        boardTemp.setTitle(board.getTitle());
        boardTemp.setContent(board.getContent());

        boardService.write(boardTemp);

        return "redirect:/board/list";
    }

    @RequestMapping("/board/insert")
    public ModelAndView insert(@ModelAttribute InsertForm insertForm, ModelAndView mav) {
        Board board = new Board();
        board.setNum(insertForm.getNum());
        board.setTitle(insertForm.getTitle());
        board.setContent(insertForm.getContent());
        board.setUsers(insertForm.getUsers());
        board.setWriteDate(insertForm.getWriteDate());


        mav.addObject("insertForm", insertForm);

        mav.setViewName("boardinsert");
        return mav;
    }

    @RequestMapping(value = "/board/insert", method = RequestMethod.POST)
    public ModelAndView insertPost(@ModelAttribute @Validated Board board, ModelAndView mav) {
        repository.saveAndFlush(board);
        mav = new ModelAndView("redirect:/list");
        return mav;
    }

}
