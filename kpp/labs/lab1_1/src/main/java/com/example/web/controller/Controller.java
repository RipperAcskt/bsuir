package com.example.web.controller;

import com.example.web.polinom.Polinom;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.RestController;

@RestController
public class Controller {
    @GetMapping("/polinom")
    Polinom len(@RequestParam(value = "word", defaultValue = "default") String word){
        boolean cheak = true;
        for(int i = 0; i < word.length()/2; i++){
            if(word.charAt(i) != word.charAt(word.length()-i-1)){
                cheak = false;
                break;
            }
        }
        return new Polinom(word.length(), cheak);
    }
}
