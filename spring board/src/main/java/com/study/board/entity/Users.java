package com.study.board.entity;

import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

import javax.persistence.*;
import java.util.ArrayList;
import java.util.List;

@Entity
@Data
@NoArgsConstructor
@AllArgsConstructor
public class Users {
    @Id
    @Column(name="board_num")
    @GeneratedValue
    private Integer num;

    private String username;

    private String password;

    @ManyToMany
    @JoinTable(joinColumns = @JoinColumn(name="users_num"),inverseJoinColumns = @JoinColumn(name="role_num"),name="users_role")
    private List<Role> roles = new ArrayList<>();


    public void addRole(Role role) {
        this.roles.add(role);
    }
}
