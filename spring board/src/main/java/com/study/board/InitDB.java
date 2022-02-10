package com.study.board;

import com.study.board.Repository.BoardRepository;
import com.study.board.Repository.RoleRepository;
import com.study.board.Repository.UserRepository;
import com.study.board.entity.Board;
import com.study.board.entity.Role;
import com.study.board.entity.Users;
import lombok.RequiredArgsConstructor;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.security.crypto.password.PasswordEncoder;
import org.springframework.stereotype.Component;

import javax.annotation.PostConstruct;
import javax.transaction.Transactional;
import java.time.LocalDateTime;

@RequiredArgsConstructor
@Component
public class InitDB {
    private final InitService initService;

    @PostConstruct
    public void init() {

        initService.doInit();
    }

    @Component
    @Transactional
    @RequiredArgsConstructor
    static class InitService {

        private final UserRepository userRepository;
        private final BoardRepository boardRepository;
        private final RoleRepository roleRepository;

        @Autowired
        private PasswordEncoder passwordEncoder;

        public void doInit() {
            Role role = new Role();

            role.setRoleName("ROLE_ADMIN");

            roleRepository.save(role);
            Role role1 = new Role();
            role1.setRoleName("ROLE_USER");
            roleRepository.save(role1);

            Users users = new Users();

            users.setUsername("june");
            users.setPassword(passwordEncoder.encode("june"));
            users.addRole(role);
            users.addRole(role1);
            userRepository.save(users);


            Board board = new Board();

            board.setTitle("인사");
            board.setContent("안녕하세요");
            board.setWriteDate(LocalDateTime.now());
            board.setUsers(users);
            boardRepository.save(board);

            Users users1 = new Users();

            users1.setUsername("june2");
            users1.setPassword(passwordEncoder.encode("june2"));
            users1.addRole(role1);
            userRepository.save(users1);

            Board board1 = new Board();

            board1.setTitle("인사2");
            board1.setContent("안녕하세요2");
            board1.setWriteDate(LocalDateTime.now());
            board1.setUsers(users1);
            boardRepository.save(board1);
        }
    }
}