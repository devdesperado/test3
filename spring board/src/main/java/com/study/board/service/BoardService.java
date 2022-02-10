package com.study.board.service;

import com.study.board.entity.Board;
import com.study.board.Repository.BoardRepository;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.List;

@Service
public class BoardService {

    @Autowired
    private BoardRepository boardRepository;

    //글 작성 처리
    public void write(Board board) {

        boardRepository.save(board);
    }

    //게시글 리스트 처리
   public List<Board> boardList() {
        return boardRepository.findAll();
    }

    // 특정 게시물 불러오기
    public Board boardView(Integer num) {
        return boardRepository.findById(num).get();
    }

    //특정 게시글 삭제
    public void boardDelete(Integer num) {
        boardRepository.deleteById(num);
    }
}
