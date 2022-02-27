package com.example.polinom.controller;

import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.ResponseBody;


@Controller
public class Controll {
    @GetMapping("/cheak")
    String len(@RequestParam(value = "str", defaultValue = "default") String word, Model model){
        boolean cheak = true;
        String str = "Да";
        for(int i = 0; i < word.length()/2; i++){
            if(word.charAt(i) != word.charAt(word.length()-i-1)){
                cheak = false;
                break;
            }
        }
        if(!cheak) str = "Нет";
        model.addAttribute("len", word.length());
        model.addAttribute("isPolinom", str);
        return "cheak";
    }
}
