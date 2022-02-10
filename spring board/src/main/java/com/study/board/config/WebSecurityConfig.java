package com.study.board.config;


import com.study.board.service.UserService;
import lombok.RequiredArgsConstructor;
import org.springframework.context.annotation.Configuration;
import org.springframework.security.config.annotation.authentication.builders.AuthenticationManagerBuilder;
import org.springframework.security.config.annotation.web.builders.HttpSecurity;
import org.springframework.security.config.annotation.web.configuration.EnableWebSecurity;
import org.springframework.security.config.annotation.web.configuration.WebSecurityConfigurerAdapter;

@Configuration
@RequiredArgsConstructor
@EnableWebSecurity

public class WebSecurityConfig  extends WebSecurityConfigurerAdapter {
    private final UserService service;

    @Override
    protected void configure(HttpSecurity http) throws Exception {
//        http
//                .authorizeRequests()
//                .antMatchers("/","/post","/css/*","/account/*","/api/*").permitAll()
//                .anyRequest().authenticated()
//                .and()
//                .formLogin()
//                .loginPage("/login")
//                .defaultSuccessUrl("/list")
//                .permitAll()
//                .and()`
//                .logout()
//                .permitAll();

        http.authorizeRequests()
                .antMatchers("/").permitAll()
                .antMatchers("/board/list").hasRole("USER")
                .antMatchers("/board/list").hasRole("ADMIN").anyRequest().authenticated()
                .and().formLogin()
                .loginPage("/login").permitAll()
                .defaultSuccessUrl("/board/list")
                .and().csrf().ignoringAntMatchers("/console/**")
                .and().headers().frameOptions().sameOrigin()
                .and().logout().permitAll();
    }

    @Override
    protected void configure(AuthenticationManagerBuilder auth) throws Exception {
        auth.userDetailsService(service);
    }

}
