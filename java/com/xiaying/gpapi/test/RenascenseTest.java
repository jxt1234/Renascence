package com.xiaying.gpapi.test;

import com.xiaying.gpapi.Renascence;

/**
 * Created by jiangxiaotang on 16/5/19.
 */
public class RenascenseTest {
    public static void main(String[] args) {
        Renascence.setLibBasicPath("/Users/jiangxiaotang/Documents/Renascence/");
        Renascence.setStreamBasicPath("/Users/jiangxiaotang/Documents/Renascence/");

        Renascence renascence = new Renascence(new String[]{"func.xml"});

        Renascence.Content content = renascence.createContent(new String[]{"input.jpg"}, "TrBmp");

        Renascence.AutoFunction function = renascence.createFunction("F(F(F(x0)))", "TrBmp");

        Renascence.Content outputContent = function.run(content);

        System.out.println(outputContent.size());
        outputContent.save("output/output_java.jpg", 0);

        function.recycle();
        outputContent.recycle();
        content.recycle();
        renascence.recycle();
    }
}
