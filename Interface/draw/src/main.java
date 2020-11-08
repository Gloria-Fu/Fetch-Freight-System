public class main {
    public static void main(String[] args) {

        Map map = new Map();



        map.start();


    }
}


/*
import java.util.*;
import java.util.regex.*;
import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

public class main {
    public static void main(String[] args) throws FileNotFoundException {
        String path = "F:/ML/map1.txt";
        Scanner in = new Scanner(new File(path));
        List data = new ArrayList();
        while (in.hasNextLine()) {
            int num = in.nextInt();

            int[] arrs = {num};

            data.add(arrs);
        }
        for (int i = 0; i < data.size(); i++) {
            int[] arr = (int[]) data.get(i);
            System.out.println("第" + (i + 1) + "行是:" + arr[0]);

        }



    }
}
*/





/*
public class main {
    public static void main(String[] args) throws FileNotFoundException {
        String path = "F:/ML/map1.txt";
        Scanner in = new Scanner(new File(path));
        List data = new ArrayList();
        while (in.hasNextLine()) {
// 取第一行
         //   String s = in.nextLine();
         //   int idx = s.indexOf("\t");
         //   System.out.println("s的长度是"+ s.length());
         //   String s0 = s.substring(0,s.length());
            */
/*String s1 = s.substring(idx+1, idx+4);
            String s2 = s.substring(idx+5, idx+8);
            String s3 = s.substring(idx+9, idx+12);
            String s4 = s.substring(idx+13, idx+6);
            String s5 = s.substring(idx+17, idx+20);*//*


            int num = in.nextInt();

            int[] arrs = {num};
            //String[] arrs = { num};
            data.add(arrs);
        }
        for (int i = 0; i < data.size(); i++) {
            int[] arr = (int[]) data.get(i);
//            System.out.println("第" + (i + 1) + "行是:" + arr[0] + "\t" + arr[1] + "\t" + arr[2] + "\t" + arr[3] + "\t" + arr[4] + "\t" + arr[5]);

        */
/*    Pattern p = Pattern.compile("   ");

            Matcher m = p.matcher(arr);

            //保存结果数组
            List<String> ret = new ArrayList<String>();
            //临时变量
            String temp = null;
            int index = 0;
            while(m.find())
            {
                int start = m.start();

                temp = str.substring(index, start);
                ret.add(temp);

                index = m.end();

            }
            System.out.println(ret);*//*



            System.out.println("第" + (i + 1) + "行是:" + arr[0]);

        }





    }
}
*/



/*

import java.util.*;
import java.util.regex.*;


public class main {
    public static void main(String[] args)
    {
        String str = "  a   b   c   d   ";
        Pattern p = Pattern.compile("   ");

        Matcher m = p.matcher(str);

        //保存结果数组
        List<String> ret = new ArrayList<String>();
        //临时变量
        String temp = null;
        int index = 0;
        while(m.find())
        {
            int start = m.start();

            temp = str.substring(index, start);
            ret.add(temp);

            index = m.end();

        }

        System.out.println(ret);
    }
}

*/
