package com.study.board.controller;


import com.study.board.form.SignupForm;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.validation.BindingResult;
import org.springframework.validation.annotation.Validated;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.ModelAttribute;
import org.springframework.web.bind.annotation.PostMapping;

import java.util.LinkedHashMap;
import java.util.Map;

@Controller
public class SignupController {

    private Map<String, String> radioMarriage;

    private Map<String, String> initRadioMarriage() {
        Map<String, String> radio = new LinkedHashMap<>();

        radio.put("기혼", "true");
        radio.put("미혼", "false");

        return radio;
    }

    @GetMapping("/signup")
    public String getSignup(@ModelAttribute SignupForm form, Model model) {

        model.addAttribute("SignupForm", form);
        radioMarriage = initRadioMarriage();
        model.addAttribute("radioMarriage", radioMarriage);

        return "login/signup";
    }

    @PostMapping("/signup")
    public String postSignup(@ModelAttribute @Validated SignupForm form, BindingResult bindingResult,
                             Model model) {
        if(bindingResult.hasErrors()) {
            return getSignup(form, model);
        }
        System.out.println(form);

        return "redirect:/login";
    }
}