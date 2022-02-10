package com.study.board.entity;


import lombok.Data;

import javax.persistence.*;
import java.time.LocalDateTime;

@Entity
@Data
public class Board {

    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    @Column(name = "board_num")
    private Integer num;

    private String title;

    private String content;
    @ManyToOne
    @JoinColumn(name = "users_num")
    private Users users;
    private LocalDateTime writeDate;

}
