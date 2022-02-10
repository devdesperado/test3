package com.study.board.entity;


import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;
import org.springframework.security.core.GrantedAuthority;

import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import javax.persistence.Id;

@Entity
@Data
@NoArgsConstructor
@AllArgsConstructor

public class Role implements GrantedAuthority {

    @Id
    @GeneratedValue
    @Column(name="role_num")
    private Integer num;

    private String roleName;


    @Override
    public String getAuthority() {
        return roleName;
    }
}
