package com.study.board.service;


import com.study.board.Repository.UserRepository;
import com.study.board.entity.Users;
import lombok.AllArgsConstructor;
import lombok.RequiredArgsConstructor;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.security.core.userdetails.User;
import org.springframework.security.core.userdetails.UserDetails;
import org.springframework.security.core.userdetails.UserDetailsService;
import org.springframework.security.core.userdetails.UsernameNotFoundException;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import java.util.Optional;

@Service
@RequiredArgsConstructor
@Transactional
public class UserService implements UserDetailsService {

    private final UserRepository repository;


    @Override
    public UserDetails loadUserByUsername(String username) throws UsernameNotFoundException {
        Optional<Users> opUser = repository.findByUsername(username);
        if(opUser.isPresent()) {
            Users users = opUser.get();

            User user = (User) User.builder()
                    .username(users.getUsername()).password(users.getPassword()).authorities(users.getRoles()).build();
        return user;
        }
        throw new UsernameNotFoundException("message:유저를 찾지 못했습니다.");
    }
}
