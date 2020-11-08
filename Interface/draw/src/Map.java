import javax.swing.*;
import java.awt.*;
import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;


public class Map extends Thread
{
    public static int start = 0, gap = 10;
    public static int gridSize = 9;
    public static int width = 147, height = 89;

    private JFrame jFrame = new JFrame();

    @Override
    public void run() {
        jFrame.getContentPane().setBackground(Color.white);
        jFrame.setSize(1900, 1020);

        JPanel jp1 = new JPanel(){
            public void paint(Graphics g){
                super.paint(g);

                try {
                    drawRobot(g);
                } catch (FileNotFoundException e) {
                    e.printStackTrace();
                }

                drawTarget(g);

                try {
                    drawRobots(g);
                } catch (FileNotFoundException e) {
                    e.printStackTrace();
                }

                try {
                    showInfo(g);
                } catch (FileNotFoundException e) {
                    e.printStackTrace();
                }

                try {
                    showInfo2(g);
                } catch (FileNotFoundException e) {
                    e.printStackTrace();
                }

            }
        };
        jp1.setBackground(null);
        jp1.setOpaque(false);
        jFrame.add(jp1);
        jFrame.setVisible(true);
        while (true) {
            try {
                Thread.sleep(200);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }

            jp1 = new JPanel() {
                public void paint(Graphics g) {
                    super.paint(g);
                }
            };
            jp1.setBackground(null);
            jp1.setOpaque(false);
            jFrame.add(jp1);
            jFrame.setVisible(true);
            jFrame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        }
    }



    public static int drawRobot(Graphics g) throws FileNotFoundException {

        String path = "E:\\project\\3X3V6\\video\\V0\\map.txt";

        Scanner in = new Scanner(new File(path));
        List data = new ArrayList();
        while (in.hasNextLine()) {
            int num = in.nextInt();

            if(num == -1){
                break;
            }

            int[] arrs = {num};

            data.add(arrs);
        }


        int[] mapNode = new int[5];
        int index = 0;
        for (int i = 0; i < data.size(); i++) {
            int[] arr = (int[]) data.get(i);

            mapNode[index] = arr[0];
            index++;
            if(index == 5) {
                if(mapNode[3] == 0){
                    g.setColor(Color.BLACK);

                }else if(mapNode[3] == 1){
                    g.setColor(Color.white);
                }else if(mapNode[3] == 2){
                    g.setColor(Color.ORANGE);
                }else if(mapNode[3] == 3){
                    g.setColor(Color.red);
                }
                g.fillRect(mapNode[1]*gap, (mapNode[2]+2)*gap, gridSize, gridSize);
                index = 0;
            }

        }

        return 0;
    }


    public static int showInfo(Graphics g) throws FileNotFoundException {

        String path = "E:\\project\\3X3V6\\video\\V0\\news.txt";

        Scanner in = new Scanner(new File(path));
        List data = new ArrayList();

        while (in.hasNextLine()) {
            int num = in.nextInt();
            if (num == -1) {
                break;
            }
            int[] arrs = {num};
            data.add(arrs);
        }

        g.setColor(Color.black);
        Font font = new Font("Times New Roman", Font.PLAIN, 18);
        g.setFont(font);
        int airsX = (width / 10) * gap;
        int airsY = 15;
        g.drawString(" Shenzhen Institute of Artificial Intelligence and Robotics\n" +
                " for Society  ----   Fetch & Freight Robots Cooperatively Scheduling Simulation Platform", airsX, airsY);

        g.setColor(Color.BLACK);
        font = new Font("Times New Roman", Font.BOLD, 15);
        g.setFont(font);

        int textX = (width + 1) * gap;
        int textY = 4 * gap;
        int gapp = 22;

        int[] a = new int[3];
        for (int i = 0; i < data.size(); i++) {
            int[] arr = (int[]) data.get(i);
            a[i] = arr[0];
        }
        g.drawString("Fetch Robot Number: 110", textX, textY);
        g.drawString("Freight Robot Number: 220", textX, textY += gapp);
        g.drawString("Goods Area Number: 36", textX, textY += gapp);
        g.drawString("Goods Type Number: 40", textX, textY += gapp);
        g.drawString("The Current Working Time Step: " + String.valueOf(a[0]), textX, textY += gapp);
        g.drawString("The Total Task Number: " + String.valueOf(a[1]), textX, textY += gapp);
        g.drawString("The Finished Task Number: " + String.valueOf(a[2]), textX, textY += gapp);

        font = new Font("Times New Roman", Font.BOLD, 13);
        g.setColor(Color.red);
        g.fillRect(100, 920, gridSize + 2, gridSize + 2);
        g.setColor(Color.BLACK);
        g.drawString("Goods Shelf", 135, 930);

        g.setColor(Color.ORANGE);
        g.fillRect(100 + gridSize * 20, 920, gridSize + 2, gridSize + 2);
        g.setColor(Color.BLACK);
        g.drawString("Goods Area", 135 + gridSize * 20, 930);

        g.setColor(Color.LIGHT_GRAY);
        g.fillRect(100 + gridSize * 40, 920, gridSize + 2, gridSize + 2);
        g.setColor(Color.BLACK);
        g.drawString("Conveyor", 135 + gridSize * 40, 930);

        g.setColor(Color.BLACK);
        g.fillRect(100 + gridSize * 60, 920, gridSize + 4, gridSize + 4);
        g.setColor(Color.white);
        g.fillRect(100 + gridSize * 60 + 1, 921, gridSize + 2, gridSize + 2);
        g.setColor(Color.BLACK);
        g.drawString("Public Road", 135 + gridSize * 60, 930);

        g.setColor(Color.MAGENTA);
        g.fillOval(100 + gridSize * 80, 925, 3, 4);
        g.fillOval(100 + gridSize * 80+6, 925, 3, 4);
        g.fillRect(100 + gridSize * 80, 922, gridSize + 2, gridSize / 2);
        g.setColor(Color.BLACK);
        g.drawString("The Freight Robot", 135 + gridSize * 80, 930);

        g.setColor(Color.blue);
        g.fillRect(100 + gridSize * 100,920 + gridSize - 5, gridSize, 5);
        g.fillRect(100 + gridSize * 100 + gridSize/2 - 2, 920, 2, gridSize - 5);
        int [] xPoints = new int [] {100 + gridSize * 100 + gridSize/2, 100 + gridSize * 100 + gridSize/2 + 3, 100 + gridSize * 100 + gridSize/2 + 5};
        int [] yPoints = new int [] {920, 920 + 2, 920 + 2};
        g.drawPolyline(xPoints, yPoints, 3);
        g.drawLine(100 + gridSize * 100 + gridSize/2 + 3, 920 + 3, 100 + gridSize * 100 + gridSize/2 + 5, 920 + 1);
        g.drawLine(100 + gridSize * 100 + gridSize/2 + 3, 920 + 3, 100 + gridSize * 100 + gridSize/2 + 5, 920 + 5);
        g.setColor(Color.BLACK);
        g.drawString("The Fetch Robot", 135 + gridSize * 100, 930);

        return 0;
    }



    public static int showInfo2(Graphics g) throws FileNotFoundException {

        String path = "E:\\project\\3X3V6\\video\\V0\\news2.txt";

        Scanner in = new Scanner(new File(path));
        List data = new ArrayList();

        while (in.hasNextLine()) {
            int num = in.nextInt();
            if(num == -1){
                break;
            }
            int[] arrs = {num};
            data.add(arrs);
        }

        g.setColor(Color.BLACK);
        Font font = new Font("Times New Roman", Font.BOLD, 14);
        g.setFont(font);

        int textX = (width+1)*gap;
        int textY = 22*gap;
        int gapp = 18;

        int[] a = new int[5];
        int j = 0;
        for (int i = 0; i < data.size(); i++) {
            int[] arr = (int[]) data.get(i);
            a[j] = arr[0];
            j++;
            if(j >= 5){
                j = 0;
                if(a[0] == 1){
                    g.drawString("Fetch Robot "+String.valueOf(a[2])+" and Freight Robot "+String.valueOf((a[3]))+" handover successfully.",textX,textY+=gapp);
                }else if(a[0] == 2){
                    g.drawString("Task "+String.valueOf(a[1])+" has been finished successfully.",textX,textY+=gapp);
                }
            }
        }

        return 0;
    }


    public static int drawTarget(Graphics g) {

        g.setColor(Color.LIGHT_GRAY);

        g.fillRect((width - 6)*gap,(start + 4)*gap,5*gap,2*gap);
        g.fillRect((width - 6)*gap, (start + 6)*gap,2*gap, (height - 8)*gap);
        g.fillRect((width - 6)*gap,(height - 2)*gap,5*gap,2*gap);

        g.fillRect(1*gap,(start + 4)*gap,5*gap,2*gap);
        g.fillRect(4*gap, (start + 6)*gap,2*gap, (height - 8)*gap);
        g.fillRect(1*gap,(height - 2)*gap,5*gap,2*gap);

        return 0;
    }



    public static void drawRobots(Graphics g) throws FileNotFoundException {
        boolean isShow = false;

        Font font = new Font("Times New Roman", Font.PLAIN, 9);
        g.setColor(Color.BLACK);
        g.setFont(font);

        String path = "E:\\project\\3X3V6\\video\\V0\\robots.txt";
        Scanner in = new Scanner(new File(path));
        List data = new ArrayList();
        int num = 0;
        while (in.hasNextLine()) {
            num = in.nextInt();

            if(num == -1){
                break;
            }

            int[] arrs = {num};

            data.add(arrs);
        }

        int[] robotNode = new int[5];
        int index = 0;
        for (int i = 0; i < data.size(); i++) {
            int[] arr = (int[]) data.get(i);

            robotNode[index] = arr[0];
            index++;
            if(index == 5) {
                index = 0;
                robotNode[2]+=2;
                if(robotNode[3] > 0){
                    g.setColor(Color.MAGENTA);
                    g.fillOval(robotNode[1]*gap + gridSize/4 - 2, robotNode[2]*gap + (gridSize/4)*3 - 1, 3,3);
                    g.fillOval(robotNode[1]*gap + (gridSize/4)*3 - 2, robotNode[2]*gap + (gridSize/4)*3 - 1, 3,3);
                    g.fillRect(robotNode[1]*gap-1,robotNode[2]*gap  + gridSize/2, gridSize+1, gridSize/4);
                    if(isShow == true) {
                        g.drawString(String.valueOf(robotNode[0]), robotNode[1] * gap + gridSize + 1, robotNode[2] * gap + gridSize);
                    }

                    if(robotNode[4] > 0) {
                        int X;
                        if(robotNode[4] < 10){
                            X = robotNode[1] * gap + gridSize / 4 - 2;
                        }else{
                            X = robotNode[1] * gap + gridSize / 4 - 4;
                        }
                        g.setColor(Color.BLACK);
                        g.drawString(String.valueOf(robotNode[4]), X + 2, robotNode[2] * gap + gridSize / 2 );
                    }
                }else{
                    g.setColor(Color.blue);
                    g.fillRect(robotNode[1]*gap,robotNode[2]*gap + gridSize - 5, gridSize, 5);
                    g.fillRect(robotNode[1]*gap + gridSize/2 - 2, robotNode[2]*gap, 2, gridSize - 5);
                    int [] xPoints = new int [] {robotNode[1]*gap + gridSize/2, robotNode[1]*gap + gridSize/2 + 3, robotNode[1]*gap + gridSize/2 + 5};
                    int [] yPoints = new int [] {robotNode[2]*gap, robotNode[2]*gap + 2, robotNode[2]*gap + 2};
                    g.drawPolyline(xPoints, yPoints, 3);
                    g.drawLine(robotNode[1]*gap + gridSize/2 + 3, robotNode[2]*gap + 3, robotNode[1]*gap + gridSize/2 + 5, robotNode[2]*gap + 1);
                    g.drawLine(robotNode[1]*gap + gridSize/2 + 3, robotNode[2]*gap + 3, robotNode[1]*gap + gridSize/2 + 5, robotNode[2]*gap + 5);
                    if(isShow == true) {
                        g.drawString(String.valueOf(robotNode[0]), robotNode[1] * gap + gridSize + 1, robotNode[2] * gap + gridSize);
                    }
                }
            }
        }
    }

}



