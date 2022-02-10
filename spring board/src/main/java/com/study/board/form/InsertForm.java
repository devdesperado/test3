package com.study.board.form;


import com.study.board.entity.Users;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

import javax.persistence.*;
import java.time.LocalDateTime;


@Data
@NoArgsConstructor
@AllArgsConstructor
public class InsertForm {

    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    @Column(name="board_num")
    private Integer num;

    private String Title;

    private String content;

    @ManyToOne
    @JoinColumn(name="users_num")
    private Users users;

    private LocalDateTime writeDate;
}
